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

void SV_Status_f()
{
    void (*call)();
    *(int*)&call = 0x80846B4;
    call();
}
void SV_XStatus_f()
{
    if (!sv_running->integer)
    {
        Com_Printf("Server is not running.\n");
        return ;
    }

    Com_Printf( "map: %s\n", mapname->string );
    Com_Printf( "num mUID                             name            lastmsg ip                    qport rate  ping\n" );
    Com_Printf( "--- -------------------------------- --------------- ------- --------------------- ----- ----- ----\n" );
    
    int i , j, l;
    const char* s;
    challenge_t* challenge;
    int guid ;
    int qport;
    int ping;
    client_t* cl;

    for (i = 0; i < sv_maxclients->integer; i++)
    {
        cl = getclient(i);
        if(!cl)
            continue;
        if(!cl->state)
            continue;

        Com_Printf("%3i ", i); //num	
        
        for(int k = 0; k != 33; k++)
            Com_Printf(" ");
        
        Com_Printf( "%.15s", cl->name);
        
        l = 16 - strlen(cl->name);
        for (j = 0 ; j < l ; j++)
            Com_Printf(" ");
        
        int tt = *(int*)0x83B67A4 - *(int *)((int)cl + 68364);
        Com_Printf("%7i ", tt);
        
        s = NET_AdrToString(cl->netchan.remoteAddress);
        Com_Printf("%s", s);

        l = 22 - strlen(s);
        for (j = 0 ; j < l ; j++)
            Com_Printf(" ");

        Com_Printf("%5i ", cl->netchan.qport);
        Com_Printf("%5i ", cl->rate);

        if (cl->state == CS_CONNECTED)
        {
            Com_Printf("CNCT ");
        }
        else if (cl->state == CS_ZOMBIE)
        {
            Com_Printf("ZMBI ");
        }
        else
        {
            ping = cl->ping < 9999 ? cl->ping : 9999;
            Com_Printf("%4i", ping);
        }

        Com_Printf("\n");
    }
    Com_Printf("\n");
}

void Cmd_SetConfigstring()
{
    if (Cmd_Argc() != 3)
    {
        printf("Usage: setcs <index> \"string\"\n");
        return;
    }
    int index = atoi(Cmd_Argv(1));
    char* str = Cmd_Argv(2);
    char cs[MAX_INFO_STRING];
    SV_GetConfigstring(index, cs, sizeof(cs));
    Com_Printf("The configstring %d was changed from '%s' to ", index, cs);
    SV_SetConfigstring(index, str);
    SV_GetConfigstring(index, cs, sizeof(cs));
    Com_Printf("'%s'.\n", cs);
}
void Cmd_FindConfigstringIndex()
{
    if (Cmd_Argc() != 4)
    {
        Com_Printf("Usage: findcs \"string\" <min> <max>\n");
        return;
    }
    char* str = Cmd_Argv(1);
    int min = atoi(Cmd_Argv(2));
    int max = atoi(Cmd_Argv(3));
    int i;
    char cs[MAX_INFO_STRING];
    for (i = 1; i < max; i++)
    {
        SV_GetConfigstring(i, cs, sizeof(cs));
        if (!strcasecmp(str, cs))
        {
            Com_Printf("%d: %s\n", i, cs);
            return;
        }
    }
}
void Cmd_GetConfigstrings()
{
    if (Cmd_Argc() != 2)
    {
        Com_Printf("Usage: cs \"outputfile.txt\"\n");
        return;
    }
    char* out = Cmd_Argv(1);
    FILE* f = fopen(out, "wb");
    if (!f)
        return;
    int i;
    char cs[MAX_INFO_STRING];
    fprintf(f, "CONFIGSTRINGS CALL OF DUTY DUMP\n");
    for (i = 0; i < 2048; i++)
    {
        SV_GetConfigstring(i, cs, sizeof(cs));
        fprintf(f, "%d: %s\n", i, cs);
    }
}

void SV_Heartbeat_f(void)
{
    *(int*)0x83B67F4 = -9999999;
}

static void SV_ServerInfo_f(void)
{
    Com_Printf("Server info settings:\n");
    Info_Print( Cvar_InfoString(CVAR_SERVERINFO));
}
static void SV_SystemInfo_f(void)
{
    Com_Printf("System info settings:\n");
    Info_Print(Cvar_InfoString(CVAR_SYSTEMINFO));
}

static void SV_DumpUser_f(void)
{
    if (!sv_running->integer)
    {
        Com_Printf("Server is not running.\n");
        return;
    }
    if (Cmd_Argc() != 2)
    {
        Com_Printf("Usage: info <userid>\n");
        return;
    }

    client_t *cl;
    cl = getclient(atoi(Cmd_Argv(1)));
    if (!cl)
        return;
    Com_Printf("userinfo\n");
    Com_Printf("--------\n");
    Info_Print(cl->userinfo);
}

//stubs
static void SV_MapRestart_f(void)
{
    void(*call)(void);
    *(int*)&call = 0x8083DE4;
    call();
}
static void SV_Map_f(void)
{
    void(*call)(void);
    *(int*)&call = 0x8083C68;
    call();
}
static void SV_MapRotate_f(void)
{
    void(*call)(void);
    *(int*)&call = 0x80840C8;
    call();
}
void SV_GameCompleteStatus_f(void)
{
    void(*call)(void);
    *(int*)&call = 0x8084D54;
    call();
}
static void SV_KillServer_f(void)
{
    void(*call)(void);
    *(int*)&call = 0x8084D3C;
    call();
}
void SV_Say_f(void)
{
    void(*call)(void);
    *(int*)&call = 0x8084974;
    call();
}

// say without "console:"
void SV_Say2_f(void)
{
    if (Cmd_Argc() != 2)
    {
        Com_Printf("Usage: say2 <message>\n");
        return;
    }
    char *msg = Cmd_Argv(1);
    if (msg == "")
    {
        Com_Printf("Invalid message.\n");
        return;
    }
    SV_SendServerCommand(NULL, 1, "i \"%s\"", msg);
}

// say to specific player
void SV_Tell_f(void)
{
    if (Cmd_Argc() != 3)
    {
        Com_Printf("Usage: tell <id> <message>\n");
        return;
    }

    int id = atoi( Cmd_Argv(1) );
    char *msg = Cmd_Argv(2);

    if (id < 0 || id > 64)
    {
        Com_Printf("Invalid ID.\n");
        return;
    }
    if (msg == "")
    {
        Com_Printf("Invalid message.\n");
        return;
    }

    client_t *cl = getclient(id);
    if (cl == NULL)
    {
        Com_Printf("Client doesn't exist.\n");
        return;
    }
    SV_SendServerCommand(cl, 1, "i \"%s\"", msg);
}

void SV_StringUsage_f(void)
{
    void(*call)(void);
    *(int*)&call = 0x8084D74;
    call();
}
void SV_ScriptUsage_f(void)
{
    void(*call)(void);
    *(int*)&call = 0x8084D64;
    call();
}
void SV_KickNum_f(void)
{
    void(*call)(void);
    *(int*)&call = 0x8084BE4;
    call();
}
void SV_Kick_f(void)
{
    void(*call)(void);
    *(int*)&call = 0x8084288;
    call();
}

void SV_AddOperatorCommands(void)
{
    static qboolean* initialized = (qboolean*)0x8160680;
    if(*initialized)
        return;
    *initialized = qtrue;
    
    Cmd_AddCommand("heartbeat", SV_Heartbeat_f);
    Cmd_AddCommand("kick", SV_Kick_f);
    Cmd_AddCommand("clientkick", SV_KickNum_f);
    Cmd_AddCommand("status", SV_Status_f);
    Cmd_AddCommand("xstatus", SV_XStatus_f);
    Cmd_AddCommand("serverinfo", SV_ServerInfo_f);
    Cmd_AddCommand("systeminfo", SV_SystemInfo_f);
    Cmd_AddCommand("dumpuser", SV_DumpUser_f);
    Cmd_AddCommand("map_restart", SV_MapRestart_f);
    Cmd_AddCommand("map", SV_Map_f);
    Cmd_AddCommand("devmap", SV_Map_f);
    Cmd_AddCommand("map_rotate", SV_MapRotate_f);
    Cmd_AddCommand("gameCompleteStatus", SV_GameCompleteStatus_f);
    Cmd_AddCommand("killserver", SV_KillServer_f);
    Cmd_AddCommand("scriptUsage", SV_ScriptUsage_f);
    Cmd_AddCommand("stringUsage", SV_StringUsage_f);
    Cmd_AddCommand("cs", Cmd_GetConfigstrings);
    Cmd_AddCommand("findcs", Cmd_FindConfigstringIndex);
    Cmd_AddCommand("setcs", Cmd_SetConfigstring);
    Cmd_AddCommand("say", SV_Say_f);
    Cmd_AddCommand("say2", SV_Say2_f);
    Cmd_AddCommand("tell", SV_Tell_f);
}