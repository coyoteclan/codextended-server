#include "surfaceflags.h"
#include "server.h"

//hardcode patching it to 0x20 = 32 = CONTENTS_WATER
//which takes damage (from game without having to alter the way it works and you have no annoying blockers)
//might have to set 256 etc too
void G_SetPlayerContents(int a1)
{
    if(!strcmp(x_contents->string, "manual"))
        return;
    if(x_contents->integer == -1)
        *(int*)(a1 + 280) = CONTENTS_BODY;
    else
        *(int*)(a1 + 280) = x_contents->integer;
}
int StuckInPlayer(int a1)
{
   return false;
}

void myClientBegin(int);
void hG_Say(gentity_t *ent, gentity_t *target, int mode, const char *chatText);

extern cvar_t *x_spectator_noclip ;

void set_game_ptr( void *ret )
{
    char libn[512];
    char* check = Cvar_VariableString("fs_game");
    if(check[0] == '\0')
        sprintf(libn, "main/game.mp.i386.so");
    else
        sprintf(libn, "%s/game.mp.i386.so", check);

    int unprotect_lib(char *libname);
    unprotect_lib(libn);

    gamelib = ret;
    base = (int)dlsym(ret, "vmMain");
    g_entities = (gentity_t*)dlsym(ret, "g_entities");
    level = (level_locals_t*)GAME("level");
    pml = (char (*)[140])dlsym(ret, "pml");
    pm = (pmove_t*)dlsym(ret, "pm");
    
    void BG_Link();
    BG_Link();
    void init_g_spawn();
    init_g_spawn();
    set_trap_func_ptr();
    scriptInitializing();
    
    int stuck = (int)dlsym(ret, "StuckInClient");
    __jmp(stuck, (int)StuckInPlayer);
    int cont = (int)dlsym(ret, "G_SetClientContents");
    __jmp(cont, (int)G_SetPlayerContents);
    int h66 = (int)dlsym(ret, "ClientEndFrame") + 0x173; //patch contents
    __nop(h66, h66+0xa);

    __call(GAME("ClientCommand")+0x62D, (int)Cmd_CallVote);
    void ClientBegin(int);
    __call(GAME("vmMain")+0xA0, (int)ClientBegin);
    
    /*
        Newline, carriage return say fix.
    */
    int clientcommand_off = GAME("ClientCommand");
    __call(clientcommand_off + 0x6EE, (int)hG_Say);
    __call(clientcommand_off + 0x6FE, (int)hG_Say);
    int g_say_off = GAME("G_Say");
    __call(g_say_off + 0x5EA, (int)hG_Say);
    __call(g_say_off + 0x791, (int)hG_Say);
    __call(g_say_off + 0x77D, (int)hG_Say);
    
    /*
        only one left to patch should be 
            Cmd_GameCommand_f+84 call    G_Say           ; Call Procedure
            Cmd_GameCommand_f+94 call    G_Say           ; Call Procedure
        since we're already ignoring the 'gc' command no need to
    */
    /*
        End of fix
            - Richard
        //i should add more comments to make things more clearer for me when i look at the code later again. :3
    */
    
    /*
        q3msgboom || codmsgboom fix
    */
    int q21 = GAME("G_Say")+0x50e;
    int q22 = GAME("G_Say")+0x5ca;
    *(int*)q21=0x37f;
    *(int*)q22=0x37f;

    __jmp(GAME("G_Printf"), printf);
    /*
        end of fix
    */

    x_deadchat = Cvar_Get("x_deadchat", "1", 0);
    //deadchat fix
    int b2 = GAME("G_SayTo")+0x70;
    __nop(b2, b2+2);
    
    int b3 = GAME("G_Say")+0x3B6;
    int b4 = GAME("G_Say")+0x2B3;
    *(byte*)b3 = 0xeb;
    *(byte*)b4 = 0xeb;
    //end deadchat fix

    //Spectator noclip
    if (x_spectator_noclip->integer)
    {
        int y7 = GAME("SpectatorThink")+0x123;
        *(int*)y7 = 0;
    }
    
    cvar_t* db_password = Cvar_Get("db_password", "", 0);
    cvar_t* db_username = Cvar_Get("db_username", "root", 0);
    cvar_t* db_database = Cvar_Get("db_database", "", 0);
    cvar_t* db_server = Cvar_Get("db_server", "localhost", 0);
    
    #ifdef uMYSQL
    static int sql_do_once = 0;
    if (!sql_do_once)
    {
        if (mysql_real_connect(db,db_server->string,db_username->string,db_password->string,db_database->string,0,NULL,0) == NULL)
        {
            printf("Could not connect to the MySQL Database. [Error: %s]\n", mysql_error(db));
            //COD_Destructor();
            mysql_close(db);
            db = NULL;
        }
        else
        {
            printf("Connected to the MySQL Database.\n");
        }
        sql_do_once = 1;
    }
    #endif
}

void *Sys_LoadDll(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...))
{
    char *err;
    char *error;
    char *fn;
    char *gamedir;
    char *basepath;
    char *homepath;
    char *pwdpath;
    char fname[100];
    void *dllEntry;
    void *libHandle;
    
    void *(*call)(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
    *(int *)&call = 0x80C5FE4;
    void *ret = call(name, dest, entryPoint, systemcalls);
    set_game_ptr(ret);

    return ret;
}