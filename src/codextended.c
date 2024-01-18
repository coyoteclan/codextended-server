/*
    Call of Duty Extended
        founded by Richard
    https://github.com/riicchhaarrd
*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <execinfo.h>
#include "server.h"
#include "surfaceflags.h"

#ifdef uMYSQL
MYSQL *db = NULL;
#endif

FILE *logfile = NULL;
void Log(const char *s) {
    if(!logfile) {
        logfile = fopen("codextended.log", "w");
        if(!logfile)
            return;
    }
    fprintf(logfile, "%s", s);
}

void uCoDExtended()
{
    static int freed = 0;
    if(freed)
        return;
    freed = 1;

    void WebServer_Stop();
    WebServer_Stop();

    if(logfile != NULL)
        fclose(logfile);
    
    #ifdef uMYSQL
    if(db != NULL)
        mysql_close(db);
    mysql_library_end();
    #endif
}

void COD_Destructor()
{
    uCoDExtended();
    //muntrace();
    
    //call original
    #if CODPATCH == 1
    ((void(*)())0x806D910)();
    #endif
}

char* Cvar_InfoString(int bit)
{
    /*static char info[MAX_INFO_STRING];
    cvar_t  *var;

    info[0] = 0;
    
    cvar_t *cvar_vars = (cvar_t*)0x834A0E0;
    
    for ( var = cvar_vars ; var ; var = var->next ) {
        if ( var->flags & bit ) {
            printf("%s, ", var->name);
            Info_SetValueForKey( info, var->name, var->string );
        }
    }
    printf("\n");
    return info;*/
    char* (*call)(int);
    *((int*)(&call)) = 0x806FC30;
    char* ret = call(bit);
    //printf("ret = %s\n", ret);
    return ret;
}

void SV_UserinfoChanged(client_t*);
void SV_ConnectionlessPacket( netadr_t from, msg_t *msg );
void Cbuf_Init( void );

void crash_handler(int sig)
{
    void *array[10];
    size_t size = backtrace(array, 10);
    
    fprintf(stderr, "Error: signal: 0x%x {%d}\n", sig, sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

int main(int argc, char **argv)
{
    bool WebServer_Start();
    WebServer_Start();
    return ((int(*)(int,char**))0x80C6870)(argc, argv);
}

void CoDExtended()
{
    static int loaded = 0;
    if(loaded)
        return;
    loaded = 1;

    unsetenv("LD_PRELOAD");
    signal(SIGSEGV, crash_handler);
    setbuf(stdout, NULL);
    mprotect((void *)0x08048000, 0x135000, PROT_READ | PROT_WRITE | PROT_EXEC);
    *(int*)0x804A698 = (int)main;
    
    srand (time(NULL));
    //mtrace();
    
    #if CODPATCH == 1
    __call(0x8094C54, (int)Scr_GetCustomFunction);
    __call(0x80951C4, (int)Scr_GetCustomMethod);
    void *Sys_LoadDll(char *name, char *dest, int (**entryPoint)(int, ...), int (*systemcalls)(int, ...));
    __call(0x8092122, (int)Sys_LoadDll); //sub_80C5FE4
    __call(0x806BA27, (int)Cbuf_Init);
    __call(0x806C724, (int)Cbuf_Init);
    
    *(byte*)0x80854FF = 0xeb; //skip protocol check
    
    //jz short loc_808C42E (patch rcon time)
    *(unsigned char*)0x808C41F = 0xeb;
    
    //q3infoboom fix
    *(byte*)0x807f459 = 1;

    qboolean SV_ClientCommand(client_t*, msg_t*);
    __call(0x8087473, (int)SV_ClientCommand);
    
    //sendservercommand e game command > dropclient EXE_DISCONNECTED
    *(byte*)0x8085DBE = 0xeb;
    
    __call(0x806CC80, (int)SV_Init);
    __call(0x808C8A1, (int)SV_ConnectionlessPacket);
    __jmp(0x8084A3C, (int)SV_AddOperatorCommands);
    
    void SV_PacketEvent(netadr_t,msg_t*);
    __call(0x806BFEC, (int)SV_PacketEvent);
    __call(0x806C1B8, (int)SV_PacketEvent);
    __call(0x806E15C, (int)SV_PacketEvent);
    qboolean Sys_GetPacket( netadr_t *net_from, msg_t *net_message );
    __jmp(0x80C71F0, (int)Sys_GetPacket);
    
    void SV_DirectConnect( netadr_t from );
    // One of these is for bot, other one is redundant because it's already rewritten in packet stuff
    __call(0x8087602, (unsigned)SV_DirectConnect);
    __call(0x808C7B8, (unsigned)SV_DirectConnect);
    __call(0x808D258, (int)SV_MasterHeartBeat); //COD-1
    __call(0x808D280, (int)SV_MasterHeartBeat); //flatline
    __call(0x8085C0A, (int)SV_UserinfoChanged);
    __call(0x8087B55, (int)SV_UserinfoChanged);

    void (*SV_SendClientGameState)(client_t*);
    void custom_SV_SendClientGameState(client_t* cl); // Forward declaration
    SV_SendClientGameState = custom_SV_SendClientGameState;
    __jmp(0x8085EEC, (int)SV_SendClientGameState);

    void (*SV_ExecuteClientMessage)(client_t*, msg_t*);
    void custom_SV_ExecuteClientMessage(client_t* cl, msg_t* msg); // Forward declaration
    SV_ExecuteClientMessage = custom_SV_ExecuteClientMessage;
    __jmp(0x80872EC, (int)SV_ExecuteClientMessage);

    const char *__cdecl FS_ReferencedPakChecksums();
    const char *__cdecl FS_ReferencedPakNames();
    __jmp(0x80717A4, (int)FS_ReferencedPakChecksums);
    __jmp(0x80716CC, (int)FS_ReferencedPakNames);
    
    //hooking the "quit" commands and when server shutdowns basically for reasons that lib_unload was being a prick
    __call(0x0806B8CE, (int)COD_Destructor);
    //0806CB0C to D since push offset <shutdown offset>
    *(int*)0x0806CB0D = (int)COD_Destructor;
    
    #else
    __call(0x80713BF, (int)SV_Init);
    //__call(0x8093AD7, (int)SV_ConnectionlessPacket);
    //Hardcode crack 1.5
    __nop(0x808966B, 6);
    #endif
    
    #ifdef uMYSQL
    if(db == NULL)
    {
        db = mysql_init(NULL);
        if(!db)
        {
            printf("MySQL initialization failed.\n");
            COD_Destructor();
        }
        else
        {
            printf("MySQL initialized.\n");
        }
    }
    #endif
}