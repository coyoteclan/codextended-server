/*
	This file is part of CoDExtended.

    CoDExtended is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CoDExtended is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CoDExtended.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "server.h"

#define MAX_MSGLEN 32000

int* gvm = (int*)0x80E30C4;

cvar_t *sv_maxclients;
cvar_t *sv_privateClients;
cvar_t *g_gametype;
cvar_t *mapname;
cvar_t *sv_hostname;
cvar_t *sv_maxRate;
cvar_t *sv_maxPing;
cvar_t *sv_minPing;
cvar_t *sv_floodProtect;
cvar_t *sv_allowAnonymous;
cvar_t *sv_showCommands;
cvar_t *sv_pure;
cvar_t *sv_serverid;
cvar_t *rconPassword;
cvar_t *sv_privatePassword;
cvar_t *sv_fps;
cvar_t *sv_timeout;
cvar_t *sv_zombietime;
cvar_t *sv_allowDownload;
cvar_t *sv_master1;
cvar_t *sv_master2;
cvar_t *sv_master3;
cvar_t *sv_master4;
cvar_t *sv_master5;
cvar_t *sv_reconnectlimit;
cvar_t *sv_showloss;
cvar_t *sv_padPackets;
cvar_t *sv_killserver;
cvar_t *sv_onlyVisibleClients;
cvar_t *sv_showAverageBPS;
cvar_t *sv_mapRotation;
cvar_t *sv_mapRotationCurrent;
cvar_t *protocol;
cvar_t *shortversion;
cvar_t *dedicated;
cvar_t *sv_running;
cvar_t *sv_master[MAX_MASTER_SERVERS];

#if CODPATCH == 5
cvar_t *sv_disableClientConsole;
#endif

cvar_t *x_contents;
cvar_t *x_spectator_noclip;
cvar_t *x_authorize;
cvar_t *x_deadchat;

cvar_t *cl_allowDownload;
cvar_t *x_nodownload_paks;

typedef void (*Huff_Decompress_t)( msg_t *mbuf, int offset );
#if CODPATCH == 1
Huff_Decompress_t Huff_Decompress = (Huff_Decompress_t)0x8071F7C;
SV_GetClientScore_t SV_GetClientScore = (SV_GetClientScore_t)0x808D2DC;
#else
Huff_Decompress_t Huff_Decompress = (Huff_Decompress_t)0x8076C31;
SV_GetClientScore_t SV_GetClientScore = (SV_GetClientScore_t)0x8092421;
#endif

qboolean SV_VerifyChallenge(char *challenge)
{
	int i, j;
	if (!challenge)
	{
		return qfalse;
	}
	j = strlen(challenge);
	if (j > /*64*/128)
	{
		return qfalse;
	}
	
	for (i = 0; i < j; i++)
	{
		if (challenge[i] == '\\' ||
			challenge[i] == '/' ||
			challenge[i] == '%' ||
			challenge[i] == ';' ||
			challenge[i] == '"' ||
			challenge[i] < 32 || /*non-ascii*/
			challenge[i] > 126/*non-ascii*/)
		{
				return qfalse;
		}
	}
	return qtrue;
}

void SV_PacketEvent(netadr_t from, msg_t *msg)
{
	//cprintf(PRINT_UNDERLINE | PRINT_DEBUG, "##### Sys_GetPacket \n");

	int (*MSG_ReadShort)(msg_t*) = (int(*)(msg_t*))0x807f2c0;
	qboolean (*Netchan_Process)(netchan_t*, msg_t*) = (qboolean(*)(netchan_t*, msg_t*))0x80804f0;
	int (*MSG_ReadByte)(msg_t*) = (int(*)(msg_t*))0x807F294;
	void (*SV_Netchan_Decode)(client_t*, byte*, int) = (void(*)(client_t*, byte*, int))0x808de60;

	int i;
	client_t *cl;
	int qport;

	// check for connectionless packet (0xffffffff) first
	if ( msg->cursize >= 4 && *(int*)msg->data == -1 )
	{
		SV_ConnectionlessPacket(from, msg);
		return;
	}

	// read the qport out of the message so we can fix up
	// stupid address translating routers
	MSG_BeginReading( msg );
	MSG_ReadLong( msg ); // sequence number
	qport = (unsigned short)MSG_ReadShort(msg);

	// find which client the message is from
	for ( i = 0, cl = *clients ; i < sv_maxclients->integer ; i++,cl++ )
	{
		if ( cl->state == CS_FREE )
		{
			continue;
		}
		if ( !NET_CompareBaseAdr( from, cl->netchan.remoteAddress ) )
		{
			continue;
		}
		
		// it is possible to have multiple clients from a single IP
		// address, so they are differentiated by the qport variable
		if ( cl->netchan.qport != qport )
		{
			continue;
		}
		
		// the IP port can't be used to differentiate them, because
		// some address translating routers periodically change UDP
		// port assignments
		if ( cl->netchan.remoteAddress.port != from.port )
		{
			Com_Printf( "SV_PacketEvent: fixing up a translated port\n" );
			cl->netchan.remoteAddress.port = from.port;
		}
		
		// make sure it is a valid, in sequence packet
		if ( Netchan_Process(&cl->netchan, msg) )
		{
			int* cl_serverId = (int*)((int)cl + 370936);
			int* cl_messageAcknowledge = (int*)((int)cl + 67096);

			*cl_serverId = MSG_ReadByte(msg);
			*cl_messageAcknowledge = MSG_ReadLong(msg);

			if ( *cl_messageAcknowledge < 0 )
			{
				Com_Printf("Invalid reliableAcknowledge message from %s - reliableAcknowledge is %i\n", cl->name, cl->reliableAcknowledge);
				return;
			}

			cl->reliableAcknowledge = MSG_ReadLong(msg);

			//TODO: check exploit https://github.com/voron00/CoD2rev_Server/blob/f11de5cbb13f1eebb7e658a0f6782df4b3aa9e60/src/server/sv_main_mp.cpp#L1285C1-L1285C114

			SV_Netchan_Decode(cl, &msg->data[msg->readcount], msg->cursize - msg->readcount);

			// zombie clients still need to do the Netchan_Process
			// to make sure they don't need to retransmit the final
			// reliable message, but they don't do any other processing
			if ( cl->state != CS_ZOMBIE )
			{
				cl->lastPacketTime = svs_time;
				custom_SV_ExecuteClientMessage(cl, msg);
			}
		}
		return;

		// if we received a sequenced packet from an address we don't recognize,
		// send an out of band disconnect packet to it
		NET_OutOfBandPrint( NS_SERVER, from, "disconnect" );
	}
}

void SVC_Info(netadr_t* from)
{
	// Prevent using getinfo as an amplifier
	if (SVC_RateLimitAddress(*from, 10, 1000))
	{
		Com_DPrintf("SVC_Info: rate limit from %s exceeded, dropping request\n", NET_AdrToString(*from));
		return 0;
	}
	// Allow getinfo to be DoSed relatively easily, but prevent excess outbound bandwidth usage when being flooded inbound
	if (SVC_RateLimit(&outboundLeakyBucket, 10, 100))
	{
		Com_DPrintf("SVC_Info: rate limit exceeded, dropping request\n");
		return 0;
	}

	int i, count;
	char *gamedir;
	char infostring[MAX_INFO_STRING];
	char* g_password;
	
	if (!SV_VerifyChallenge(Cmd_Argv(1)))
	{
		return;
	}
	
	g_password = Cvar_VariableString("g_password");
	// count private clients too
	count = 0;
	for (i = 0; i < sv_maxclients->integer ; i++)
	{
		if (getclient(i)->state >= CS_CONNECTED)
		{
			count++;
		}
	}
	infostring[0] = 0;
	
	// echo back the parameter to status. so servers can use it as a challenge
	// to prevent timed spoofed reply packets that add ghost servers
	Info_SetValueForKey(infostring, "challenge", Cmd_Argv(1));
	Info_SetValueForKey(infostring, "protocol", va("%i", protocol->integer));
	Info_SetValueForKey(infostring, "hostname", sv_hostname->string);
	Info_SetValueForKey(infostring, "mapname", mapname->string);
	Info_SetValueForKey(infostring, "clients", va("%i", count));
	Info_SetValueForKey(infostring, "sv_maxclients", va("%i", sv_maxclients->integer - sv_privateClients->integer));
	Info_SetValueForKey(infostring, "gametype", g_gametype->string);
	Info_SetValueForKey(infostring, "pure", va("%i", sv_pure->integer));
	Info_SetValueForKey(infostring, "sv_allowAnonymous", va("%i", sv_allowAnonymous->integer));
	
	if (sv_minPing->integer)
	{
		Info_SetValueForKey(infostring, "minPing", va("%i", sv_minPing->integer));
	}
	if (sv_maxPing->integer)
	{
		Info_SetValueForKey(infostring, "maxPing", va("%i", sv_maxPing->integer));
	}

	gamedir = Cvar_VariableString("fs_game");
	if (*gamedir)
		Info_SetValueForKey(infostring, "game", gamedir);

	if (*g_password)
		Info_SetValueForKey(infostring, "pswrd", "1");
	else
	    Info_SetValueForKey(infostring, "pswrd", "0");

	Info_SetValueForKey(infostring, "codextended", va("%s", CODEXTENDED_VERSION));
	
	NET_OutOfBandPrint( NS_SERVER, *from, "infoResponse\n%s", infostring );
}

//extern challenge_t *challenges;

void SVC_Status(netadr_t* from)
{
	// Prevent using getstatus as an amplifier
	if (SVC_RateLimitAddress(*from, 10, 1000))
	{
		Com_DPrintf("SVC_Status: rate limit from %s exceeded, dropping request\n", NET_AdrToString(*from));
		return 0;
	}
	// Allow getstatus to be DoSed relatively easily, but prevent excess outbound bandwidth usage when being flooded inbound
	if (SVC_RateLimit(&outboundLeakyBucket, 10, 100))
	{
		Com_DPrintf("SVC_Status: rate limit exceeded, dropping request\n");
		return 0;
	}

	char player[1024];
	char status[MAX_MSGLEN];
	int i;
	client_t *cl;
	int/*playerState_t*/ *ps;
	int statusLength;
	int playerLength;
	char infostring[MAX_INFO_STRING];
	int custom_mod = 0;
	char *fs_game = Cvar_VariableString("fs_game");
	
	if (fs_game && *fs_game)
		custom_mod = 1;
	
	challenge_t* challenge;
	
	if (!SV_VerifyChallenge(Cmd_Argv(1)))
	{
		return;
	}

	strcpy(infostring, Cvar_InfoString(4)); //1.5 uses 8196

	Info_SetValueForKey(infostring, "challenge", Cmd_Argv(1));

	status[0] = 0;
	statusLength = 0;

	for (i = 0 ; i < sv_maxclients->integer ; i++)
	{
		cl = getclient(i);
		if (cl->state >= CS_CONNECTED)
		{
			Com_sprintf(player, sizeof(player), "%i %i \"%s\"\n", SV_GetClientScore(cl), cl->ping, cl->name);
			playerLength = strlen(player);
			if (statusLength + playerLength >= sizeof(status))
			{
				break; // can't hold any more
			}
			strcpy(status + statusLength, player);
			statusLength += playerLength;
		}
	}
	
	#if CODPATCH == 5
	if (sv_disableClientConsole->integer)
		Info_SetValueForKey(infostring, "con_disabled", va("%i", sv_disableClientConsole->integer));
	#endif
	
	char *g_password = Cvar_VariableString("g_password");
	Info_SetValueForKey(infostring, "pswrd", va("%i", (g_password && *g_password) ? 1 : 0));
	Info_SetValueForKey(infostring, "mod", va("%i", custom_mod));
	
	NET_OutOfBandPrint( NS_SERVER, *from, "statusResponse\n%s\n%s", infostring, status );
}

void SV_MasterHeartBeat(const char* hbname)
{
	static netadr_t adr[MAX_MASTER_SERVERS + 1];
	int i;

	cvar_t* x_heartbeattime = Cvar_Get("x_heartbeattime", "30000", 0);
	int HEARTBEAT_MSEC = x_heartbeattime->integer;
	if(HEARTBEAT_MSEC < 18000)
		HEARTBEAT_MSEC = 18000;
	//#define HEARTBEAT_MSEC  18000
	
	if (dedicated->integer != 2)
		return;
		
	int* nextHeartbeatTime = (int*)0x83B67F4;
	
	if (svs_time < *nextHeartbeatTime)
		return;
		
	*nextHeartbeatTime = svs_time + HEARTBEAT_MSEC;
	
	for (i = 0; i < MAX_MASTER_SERVERS; i++)
	{
		if (!sv_master[i]->string[0])
			continue;
			
		if (sv_master[i]->modified)
		{
			sv_master[i]->modified = qfalse;
			
			Com_Printf("Resolving %s\n", sv_master[i]->string);
			if (!NET_StringToAdr(sv_master[i]->string, &adr[i]))
			{
				// if the address failed to resolve, clear it
				// so we don't take repeated dns hits
				Com_Printf("Couldn't resolve address: %s\n", sv_master[i]->string);
				Cvar_Set(sv_master[i]->name, "");
				sv_master[i]->modified = qfalse;
				continue;
			}
			if (!strstr(":", sv_master[i]->string))
			{
				adr[i].port = BigShort(20510);
			}
			Com_Printf("%s resolved to %i.%i.%i.%i:%i\n", sv_master[i]->string,
						adr[i].ip[0], adr[i].ip[1], adr[i].ip[2], adr[i].ip[3],
						BigShort(adr[i].port));
		}
		
		Com_Printf("Sending heartbeat to %s\n", sv_master[i]->string);
		NET_OutOfBandPrint(NS_SERVER, adr[i], "heartbeat %s\n", hbname);
	}
}

void SVC_RemoteCommand(netadr_t *from, msg_t *msg)
{
	// Prevent using rcon as an amplifier and make dictionary attacks impractical
	if (SVC_RateLimitAddress(*from, 10, 1000))
	{
		Com_DPrintf("SVC_RemoteCommand: rate limit from %s exceeded, dropping request\n", NET_AdrToString(*from));
		return 0;
	}
	// Make DoS via rcon impractical
	if (!strlen(rconPassword->string) || strcmp(Cmd_Argv(1), rconPassword->string) != 0)
	{
		static leakyBucket_t bucket;
		if (SVC_RateLimit(&bucket, 10, 1000))
		{
			Com_DPrintf("SVC_RemoteCommand: rate limit exceeded, dropping request\n");
			return 0;
		}
	}

	static time_t lasttime = 0;
	time_t ttime;
	
	if (!strlen(rconPassword->string))
		return;
		
	if (strcmp(Cmd_Argv(1), rconPassword->string))
	{
		ttime = time(NULL);
		if (difftime(ttime, lasttime) < 1)
			return;
		else
			lasttime = ttime;
	}

    if (callbackRemoteCommand)
	{
        Scr_AddString((char *) msg->data);
        Scr_AddString(NET_AdrToString(*from));
        int result = Scr_ExecThread(callbackRemoteCommand, 2);
        Scr_FreeThread(result);
        if(!Scr_Continue())
            return;
    }
	
	((void(*)(netadr_t, msg_t*))0x808C404)(*from, msg);
}

void SV_ConnectionlessPacket(netadr_t from, msg_t *msg)
{
	char* s;
	char* c;
	
	MSG_BeginReading(msg);
	MSG_ReadLong(msg);
	
	#if CODPATCH == 5
	void (*SV_Netchan_AddOOBProfilePacket)(int);
	*(int*)&SV_Netchan_AddOOBProfilePacket = 0x8094928;
	SV_Netchan_AddOOBProfilePacket(msg->cursize);
	#endif
	
	if (!Q_strncmp("connect", (char*)&msg->data[4], 7))
		Huff_Decompress( msg, 12 );
	
	s = MSG_ReadStringLine(msg);
	Cmd_TokenizeString(s);
	c = Cmd_Argv(0);
	
	if (!Q_stricmp(c, "getstatus"))
	{
		SVC_Status(&from);
	}
	else if (!Q_stricmp(c, "getinfo"))
	{
		SVC_Info(&from);
	}
	else if (!Q_stricmp(c, "getchallenge"))
	{
		SV_GetChallenge(&from );
	}
	else if (!Q_stricmp(c, "connect"))
	{
		SV_DirectConnect(from );
	}
	else if (!Q_stricmp(c, "ipAuthorize"))
	{
		if (!x_authorize->integer)
		{
			((void(*)(netadr_t))0x808514c)(from); //SV_AuthorizeIpPacket
		}
	}
	else if (!Q_stricmp(c, "rcon"))
	{
		#if CODPATCH == 1
		SVC_RemoteCommand(&from, msg);
		#else if CODPATCH == 5
		((void(*)(netadr_t, msg_t*))0x80930D0)(from, msg);
		#endif
	}
	else if (!Q_stricmp(c, "disconnect"))
	{
		// if a client starts up a local server, we may see some spurious
		// server disconnect messages when their new server sees our final
		// sequenced messages to the old client
	}
	else
	{
		Com_DPrintf("bad connectionless packet '%s' from %s.\n", s, NET_AdrToString(from));
	}

	Com_DPrintf("SV packet %s : %s\n", NET_AdrToString(from), c);
}

// RATELIMITER (experimental)
unsigned long sys_timeBase = 0;
int Sys_Milliseconds(void)
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	if (!sys_timeBase)
	{
		sys_timeBase = tp.tv_sec;
		return tp.tv_usec / 1000;
	}
	curtime = (tp.tv_sec - sys_timeBase) * 1000 + tp.tv_usec / 1000;
	return curtime;
}
static long SVC_HashForAddress(netadr_t address)
{
	unsigned char *ip = address.ip;
	size_t size = 4;
	int i;
	long hash = 0;
	for (i = 0; i < size; i++)
	{
		hash += (long)(ip[i]) * (i + 119);
	}
	hash = (hash ^ (hash >> 10) ^ (hash >> 20));
	hash &= (MAX_HASHES - 1);
	return hash;
}

static leakyBucket_t *SVC_BucketForAddress(netadr_t address, int burst, int period)
{
	leakyBucket_t *bucket = NULL;
	int i;
	long hash = SVC_HashForAddress(address);
	int now = Sys_Milliseconds();

	for (bucket = bucketHashes[hash]; bucket; bucket = bucket->next)
	{
		if (memcmp(bucket->_4, address.ip, 4) == 0)
		{
			return bucket;
		}
	}

	for (i = 0; i < MAX_BUCKETS; i++)
	{
		int interval;
		bucket = &buckets[i];
		interval = now - bucket->lastTime;

		// Reclaim expired buckets
		if (bucket->lastTime > 0 && (interval > (burst * period) || interval < 0))
		{
			if (bucket->prev != NULL)
			{
				bucket->prev->next = bucket->next;
			}
			else
			{
				bucketHashes[bucket->hash] = bucket->next;
			}
			if (bucket->next != NULL)
			{
				bucket->next->prev = bucket->prev;
			}
			memset(bucket, 0, sizeof(leakyBucket_t));
		}

		if (bucket->type == 0)
		{
			bucket->type = address.type;
			memcpy(bucket->_4, address.ip, 4);
			bucket->lastTime = now;
			bucket->burst = 0;
			bucket->hash = hash;
			// Add to the head of the relevant hash chain
			bucket->next = bucketHashes[hash];
			if (bucketHashes[hash] != NULL)
			{
				bucketHashes[hash]->prev = bucket;
			}
			bucket->prev = NULL;
			bucketHashes[hash] = bucket;
			return bucket;
		}
	}
	// Couldn't allocate a bucket for this address
	return NULL;
}

bool SVC_RateLimit(leakyBucket_t *bucket, int burst, int period)
{
	if (bucket != NULL)
	{
		int now = Sys_Milliseconds();
		int interval = now - bucket->lastTime;
		int expired = interval / period;
		int expiredRemainder = interval % period;

		if (expired > bucket->burst || interval < 0)
		{
			bucket->burst = 0;
			bucket->lastTime = now;
		}
		else
		{
			bucket->burst -= expired;
			bucket->lastTime = now - expiredRemainder;
		}
		if (bucket->burst < burst)
		{
			bucket->burst++;
			return false;
		}
	}
	return true;
}

// Allows IPs to bypass ratelimiting
bool SVC_RateLimitAddress(netadr_t from, int burst, int period)
{
	/*
	const char *ignoredIPs[] = {
		"127.0.0.1",
		"69.69.69.69",
		NULL
	};
	int i;
	for(i = 0; ignoredIPs[i] != NULL; i++) {
		netadr_t adr;
		NET_StringToAdr(ignoredIPs[i], &adr);
		
		if(NET_CompareBaseAdr(from, adr))
			return false;
	}
	*/
	leakyBucket_t *bucket = SVC_BucketForAddress(from, burst, period);
	return SVC_RateLimit(bucket, burst, period);
}