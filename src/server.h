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

#ifndef SERVER_H
#define SERVER_H

/*
    Still not quite happy with the including of some files.
    //richard
*/

#include "shared.h"
#include "script.h"
#include <sys/time.h>

#if CODPATCH == 1
#define svsclients_ptr 0x83B67AC
#define clientsize 370940
#else
#define svsclients_ptr 0x83CCD90
#define clientsize 371124
#endif

#define SVF_NOCLIENT       		0x00000001
#define SVF_BROADCAST      		0x00000020
#define SVF_CAPSULE        		0x00000200  // use capsule for collision detection
#define SVF_SINGLECLIENT   		0x00000800 // only send to a single client (entityShared_t->singleClient)
#define SVF_NOTSINGLECLIENT		0x00002000 // send entity to everyone but one client

#if CODPATCH == 1
#define svs_time (*(int*)0x83B67A4)
#else if CODPATCH == 5
#define svs_time (*(int*)0x83CCD88)
#endif

typedef enum
{
	svc_bad,
	svc_nop,
	svc_gamestate,
	svc_configstring,		// [short] [string] only in gamestate messages
	svc_baseline,			// only in gamestate messages
	svc_serverCommand,		// [string] to be executed by client game module
	svc_download,			// [short] size [size bytes]
	svc_snapshot,
	svc_EOF
} svc_ops_e; //it's for client aswell
static char *svc_strings[256] =
{
	"svc_bad",
	"svc_nop",
	"svc_gamestate",
	"svc_configstring",
	"svc_baseline",
	"svc_serverCommand",
	"svc_download",
	"svc_snapshot",
	"svc_EOF"
};

typedef enum
{
	NA_BOT,
	NA_BAD, // an address lookup failed
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX
} netadrtype_t;
typedef enum
{
	NS_CLIENT,
	NS_SERVER
} netsrc_t;
typedef struct
{
	netadrtype_t type;
	union
	{
		byte ip[4];
		unsigned int _ip;
	};
	byte ipx[10];
	unsigned short port;
} netadr_t; //size = 0x14 (20)

#define MAX_CHALLENGES 1024
typedef struct
{
	netadr_t adr; //0
	int challenge; //20
	int time; //24
	int pingTime; //28
	int firstTime; //32
	int firstPing; //36
	int connected; //40
	#if CODPATCH == 5
	int guid; //44
	unsigned char __idk[36];
	#endif
} challenge_t;

typedef struct
{
	int idk33[4];
	int svFlags;
	int singleClient;
	char pad[256];
	int idk;
} entityShared_t;
typedef struct
{
	entityState_t s;
	entityShared_t r;
} sharedEntity_t;

typedef sharedEntity_t* (*SV_GentityNum_t)( int num );
extern SV_GentityNum_t SV_GentityNum;

typedef void (*SV_SetConfigstring_t)( int index, const char *val );
typedef void (*SV_GetConfigstring_t)( int index, char *buffer, int bufferSize );
extern SV_GetConfigstring_t SV_GetConfigstring;
extern SV_SetConfigstring_t SV_SetConfigstring;

void SV_Init(void);
void SV_GetChallenge(netadr_t* from);
void SV_AuthorizeIpPacket(netadr_t from);
void SV_DirectConnect(netadr_t from);
void SV_MasterHeartBeat(const char*);

extern cvar_t *sv_maxclients;
extern cvar_t *sv_privateClients;
extern cvar_t *g_gametype;
extern cvar_t *mapname;
extern cvar_t *sv_hostname;
extern cvar_t *sv_maxRate;
extern cvar_t *sv_maxPing;
extern cvar_t *sv_minPing;
extern cvar_t *sv_floodProtect;
extern cvar_t *sv_allowAnonymous;
extern cvar_t *sv_showCommands;
extern cvar_t *sv_pure;
extern cvar_t *sv_serverid;
extern cvar_t *rconPassword;
extern cvar_t *sv_privatePassword;
extern cvar_t *sv_fps;
extern cvar_t *sv_timeout;
extern cvar_t *sv_zombietime;
extern cvar_t *sv_allowDownload;
extern cvar_t *sv_master1;
extern cvar_t *sv_master2;
extern cvar_t *sv_master3;
extern cvar_t *sv_master4;
extern cvar_t *sv_master5;
extern cvar_t *sv_reconnectlimit;
extern cvar_t *sv_showloss;
extern cvar_t *sv_padPackets;
extern cvar_t *sv_killserver;
extern cvar_t *sv_onlyVisibleClients;
extern cvar_t *sv_showAverageBPS;
extern cvar_t *sv_mapRotation;
extern cvar_t *sv_mapRotationCurrent;
extern cvar_t *shortversion;
extern cvar_t *protocol;
extern cvar_t* dedicated;
extern cvar_t* sv_running;

#if CODPATCH == 5
extern cvar_t *sv_disableClientConsole;
#endif

extern cvar_t *x_contents;
extern cvar_t *x_deadchat;
extern cvar_t *x_authorize;
extern cvar_t *x_spectator_noclip;

extern cvar_t *cl_allowDownload; //the client will locally change any cvars to match the SYSTEMINFO cvars
extern cvar_t *x_nodownload_paks;

#define MAX_MASTER_SERVERS 5
extern cvar_t* sv_master[MAX_MASTER_SERVERS];

typedef void (QDECL *NET_OutOfBandPrint_t)( netsrc_t net_socket, netadr_t adr, const char *format, ... );
extern NET_OutOfBandPrint_t NET_OutOfBandPrint;

typedef qboolean (*NET_StringToAdr_t)( const char *s, netadr_t *a );
extern NET_StringToAdr_t NET_StringToAdr;

const char  *NET_BaseAdrToString (netadr_t a);
const char  *NET_AdrToString (netadr_t a);
qboolean    NET_CompareAdr( netadr_t a, netadr_t b );
qboolean    NET_CompareBaseAdr( netadr_t a, netadr_t b );

typedef void (*NET_SendPacket_t)( netsrc_t sock, int length, const void *data, netadr_t to );
extern NET_SendPacket_t NET_SendPacket;

extern netadr_t authorizeAddress;
extern netadr_t masterAddress;

typedef enum
{
	CS_FREE,        // can be reused for a new connection
	CS_ZOMBIE,      // client has been disconnected, but don't reuse connection for a couple seconds
	CS_CONNECTED,   // has been assigned to a client_t, but no gamestate yet
	CS_PRIMED,      // gamestate has been sent, but client hasn't sent a usercmd
	CS_ACTIVE       // client is fully in game
} clientState_t;

typedef struct //usercmd_s defined in server.h
{
	playerState_t *ps;
	usercmd_t cmd;
	//some remaining
} pmove_t;
extern pmove_t *pm;

#define iprintln(m) SV_SendServerCommand(NULL, 0, "e \"%s\"", m)

#define MAX_CONFIGSTRINGS 2048
#define MAX_MSGLEN 0x4000
typedef struct
{
	char command[MAX_STRING_CHARS];
	int cmdTime;
	int cmdType;
} reliableCommands_t; //FROM COD2REV
typedef struct
{
	netsrc_t	sock;
	int			dropped;			// between last packet and previous
	netadr_t	remoteAddress;
	int			qport;				// qport value to write when transmitting
	// sequencing variables
	int			incomingSequence;
	int			outgoingSequence;
	// incoming fragment assembly buffer
	int			fragmentSequence;
	int			fragmentLength;	
	byte		fragmentBuffer[MAX_MSGLEN];
	// outgoing fragment buffer
	// we need to space out the sending of large fragmented messages
	qboolean	unsentFragments;
	int			unsentFragmentStart;
	int			unsentLength;
	byte		unsentBuffer[MAX_MSGLEN];
} netchan_t; //FROM ID TECH3 GITHUB SOURCE

#define	MAX_RELIABLE_COMMANDS 64
typedef struct client_s
{
	int state;
	int unknown4;
	int unknown8;
	char userinfo[1024];
	reliableCommands_t	reliableCommands[MAX_RELIABLE_COMMANDS];
	int reliableSequence;
	int reliableAcknowledge;
	char gap_10614;
	char gap_10615[7];
	int gamestateMessageNum;
	int challenge;
	usercmd_t lastUsercmd;
	int lastClientCommand;
	char lastClientCommandString[1024];
	unsigned int gentity;
	char name[32];
	char downloadName[64];
	int download;
	int downloadSize;
	int downloadCount;
	int junk;
	int gap_10AB4;
	char gap_10AB8[84];
	int lastPacketTime;
	int lastConnectTime;
	int nextSnapshotTime;
	char gap_10B18[269704];
	int ping;
	int rate;
	int snapshotMsec;
	int pureAuthentic;
	netchan_t netchan;
	char unknown_rest[16];
} client_t;

typedef void (*Netchan_Setup_t)( netsrc_t sock, netchan_t* chan, netadr_t adr, int qport );
extern Netchan_Setup_t Netchan_Setup;

typedef struct animation_s
{
	char name[64]; //pb_combatwalk_left_loop_pistol
	int a; //4294967295
	int b; //31
	int c; //733
	int d; //426038
	int e; //144
	int f; //16
	int g; //0
} animation_t;

#define clearchallenge(i) \
memset(&challenges[i], 0, sizeof(challenges[i]))

extern client_t** clients;
extern cvar_t  *sv_maxclients;
extern challenge_t* challenges;

void Cmd_CallVote(unsigned);

typedef void (__cdecl *SV_StopDownload_f_t)(client_t*);
extern SV_StopDownload_f_t SV_StopDownload_f;
typedef void (*SV_BeginDownload_f_t)(client_t*);
extern SV_BeginDownload_f_t SV_BeginDownload_f;

typedef void (*SV_DropClient_t)(client_t*, const char* reason);
extern SV_DropClient_t SV_DropClient;

typedef void (*SV_FreeClientScriptId_t)(client_t*);
extern SV_FreeClientScriptId_t SV_FreeClientScriptId;

typedef int (*SV_GetClientScore_t)(client_t*);
extern SV_GetClientScore_t SV_GetClientScore;

void SV_Status_f();
void SV_UserinfoChanged( client_t* cl );

client_t* getclient(int);
char *ConcatArgs( int start);
typedef void (*SV_Trace_t)(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask);
typedef void (QDECL *SV_SendServerCommand_t)(client_t*, int, const char*, ...); //actually is gamesendservercommand which then checks and then calls sendservercommand
typedef void (*getuserinfo_t)( int index, char *buffer, int bufferSize);
typedef void (*setuserinfo_t)(int,const char*);
extern SV_Trace_t SV_Trace;
extern SV_SendServerCommand_t SV_SendServerCommand;
extern getuserinfo_t getuserinfo;
extern setuserinfo_t setuserinfo;

// RATELIMITER (experimental)
unsigned long sys_timeBase;
int curtime;
int Sys_Milliseconds(void);
typedef struct leakyBucket_s leakyBucket_t;
struct leakyBucket_s
{
	netadrtype_t type;
	unsigned char _4[4];
	int lastTime;
	signed char burst;
	long hash;
	leakyBucket_t *prev, *next;
};
#define MAX_BUCKETS 16384
#define MAX_HASHES 1024
static leakyBucket_t buckets[MAX_BUCKETS];
static leakyBucket_t *bucketHashes[MAX_HASHES];
leakyBucket_t outboundLeakyBucket;
static long SVC_HashForAddress(netadr_t address);
static leakyBucket_t *SVC_BucketForAddress(netadr_t address, int burst, int period);
bool SVC_RateLimit(leakyBucket_t *bucket, int burst, int period);
bool SVC_RateLimitAddress(netadr_t from, int burst, int period);
#endif // SERVER_H