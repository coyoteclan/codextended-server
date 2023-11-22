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
#include <sys/stat.h>

SV_SetConfigstring_t SV_SetConfigstring = (SV_SetConfigstring_t)0x8089BF0;
SV_GetConfigstring_t SV_GetConfigstring = (SV_GetConfigstring_t)0x808B05C;

netadr_t authorizeAddress;
netadr_t masterAddress;

cvar_t *developer;

void SV_Init( void )
{
	void (*init)( void );

	#if CODPATCH == 1
	*(int*)&init = 0x808A94C;
	#else if CODPATCH == 5
	*(int*)&init = 0x80913B3;
	#endif
	
	init();
	
	if (clientsize != sizeof(client_t))
	{
		cprintf(PRINT_UNDERLINE | PRINT_ERR, "ERROR: client_t size doesn't fit clientsize: %i != %i\n", sizeof(client_t), clientsize);
	}
	else
	{
		//cprintf(PRINT_UNDERLINE | PRINT_GOOD, "GOOD: SIZE IS SAME OF CLIENT\n");
	}
	
	developer = Cvar_Get("developer", "0", 256);
	sv_running = Cvar_Get("sv_running", "0", 64);
	sv_maxclients = Cvar_Get("sv_maxclients", "20", 36);
	sv_privateClients = Cvar_Get("sv_privateClients", "0", 4);
	g_gametype = Cvar_Get("g_gametype", "dm", 36);
	mapname = Cvar_Get("mapname", "nomap", 68);
	sv_hostname = Cvar_Get("sv_hostname", "CoDExtendedHost", 5);
	sv_maxRate = Cvar_Get("sv_maxRate", "0", 5);
	sv_maxPing = Cvar_Get("sv_maxPing", "0", 5);
	sv_minPing = Cvar_Get("sv_minPing", "0", 5);
	sv_floodProtect = Cvar_Get("sv_floodProtect", "1", 5);
	sv_allowAnonymous = Cvar_Get("sv_allowAnonymous", "", 4);
	sv_showCommands = Cvar_Get("sv_showCommands", "0", 0);
	sv_pure = Cvar_Get("sv_pure", "0", 12);
	sv_serverid = Cvar_Get("sv_serverid", "0", 72);
	rconPassword = Cvar_Get("rconPassword", "", 256);
	sv_privatePassword = Cvar_Get("sv_privatePassword", "", 256);
	sv_fps = Cvar_Get("sv_fps", "20", 256);
	sv_timeout = Cvar_Get("sv_timeout", "240", 256);
	sv_zombietime = Cvar_Get("sv_zombietime", "2", 256);
	sv_allowDownload = Cvar_Get("sv_allowDownload", "1", 1);
	Cvar_Get("sv_wwwDownload", "1", 1);
	Cvar_Get("sv_wwwBaseURL", "", CVAR_SYSTEMINFO | 1);
	sv_reconnectlimit = Cvar_Get("sv_reconnectlimit", "3", 0);
	sv_showloss = Cvar_Get("sv_showloss", "0", 0);
	sv_padPackets = Cvar_Get("sv_padPackets", "0", 0);
	sv_killserver = Cvar_Get("sv_killserver", "0", 0);
	sv_onlyVisibleClients = Cvar_Get("sv_onlyVisibleClients", "0", 0);
	sv_showAverageBPS = Cvar_Get("sv_showAverageBPS", "0", 0);
	sv_mapRotation = Cvar_Get("sv_mapRotation", "", 0);
	sv_mapRotationCurrent = Cvar_Get("sv_mapRotationCurrent", "", 0);
	protocol = Cvar_Get("protocol", "1", 68);
	shortversion = Cvar_Get("shortversion", "1.1", 68);
	dedicated = Cvar_Get("dedicated", "2", 64);
	x_spectator_noclip = Cvar_Get("x_spectator_noclip", "0", CVAR_ARCHIVE);
	cl_allowDownload = Cvar_Get("cl_allowDownload", "0", CVAR_SYSTEMINFO);
	x_nodownload_paks = Cvar_Get("x_nodownload_paks", "", 0);
	Cvar_Get("rate", "25000", CVAR_SYSTEMINFO);
	Cvar_Get("snaps", "40", CVAR_SYSTEMINFO);
	extern cvar_t *x_cl_adsair;
	x_cl_adsair = Cvar_Get("x_cl_adsair", "0", 0);
	extern cvar_t *x_cl_bounce;
	x_cl_bounce = Cvar_Get("x_cl_bounce", "0", CVAR_SYSTEMINFO | 1);
	#if CODPATCH == 5
	sv_disableClientConsole = Cvar_Get("sv_disableClientConsole", "0", 8);
	#endif
	x_authorize = Cvar_Get("x_authorize", "0", 0);
	//Used to prevent players from blocking each other
	x_contents = Cvar_Get("x_contents", "-1", 0);
	Cvar_Get("codextended", va("CoDExtended %s", CODEXTENDED_VERSION), CVAR_SERVERINFO | CVAR_ROM | CVAR_NORESTART);
	sv_master[0] = Cvar_Get("sv_master1", "codmaster.activision.com", 0);
	sv_master[1] = Cvar_Get("sv_master2", "", CVAR_ARCHIVE);
	sv_master[2] = Cvar_Get("sv_master3", "", CVAR_ARCHIVE);
	sv_master[3] = Cvar_Get("sv_master4", "", CVAR_ARCHIVE);
	sv_master[4] = Cvar_Get("sv_master5", "", CVAR_ARCHIVE);
	
	#define MASTER_SERVER_NAME "codmaster.activision.com"
	cprintf(PRINT_GOOD, "Resolving %s\n", MASTER_SERVER_NAME );
	if ( NET_StringToAdr( MASTER_SERVER_NAME, &masterAddress ) )
	{
		masterAddress.port = BigShort( 20510 );
	}	
	#define AUTHORIZE_SERVER_NAME "codauthorize.activision.com"
	if ( !authorizeAddress.ip[0] && authorizeAddress.type != NA_BAD )
	{
		cprintf(PRINT_GOOD, "Resolving %s\n", AUTHORIZE_SERVER_NAME );
		if ( !NET_StringToAdr( AUTHORIZE_SERVER_NAME, &authorizeAddress ) )
		{
			cprintf(PRINT_ERR, "Couldn't resolve address\n" );
			return;
		}
		authorizeAddress.port = BigShort( 20500 );
		cprintf(PRINT_GOOD, "%s resolved to %i.%i.%i.%i:%i\n", AUTHORIZE_SERVER_NAME,
					authorizeAddress.ip[0], authorizeAddress.ip[1],
					authorizeAddress.ip[2], authorizeAddress.ip[3],
					BigShort( authorizeAddress.port ) );
	}
}