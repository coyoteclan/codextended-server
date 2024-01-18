#include "server.h"

int call_cmd_callvote = 0;
int call_onsameteam = 0;
void Cmd_CallVote(unsigned gentity)
{
    return;
    #if 0
    if(!call_cmd_callvote)
        call_cmd_callvote = GAME("Cmd_CallVote_f");
    void (*callvote)(unsigned);
    *(int*)&callvote = call_cmd_callvote;
    char arg1[MAX_STRING_TOKENS];
    char arg2[MAX_STRING_TOKENS];
    Cmd_ArgvBuffer( 1, arg1, sizeof( arg1 ) );
    Cmd_ArgvBuffer( 2, arg2, sizeof( arg2 ) );
    if( strchr( arg1, ';' ) || strchr( arg2, ';' ) || !is_good_string(arg1) || !is_good_string(arg2)) {
        SV_SendServerCommand(*(int*)gentity, 0, "e \"GAME_INVALIDVOTESTRING\"");
        return;
    }
    callvote( gentity );
    #endif
}

void G_SayTo(int *ent, int *other, int mode, int color, const char *name, const char *message)
{
    cprintf(PRINT_UNDERLINE | PRINT_DEBUG, "##### G_SayTo \n");

    cvar_t* x_deadchat = Cvar_Get("x_deadchat", "1", 0);
    
    if (!call_onsameteam)
        call_onsameteam = GAME("OnSameTeam");
    
    qboolean (*OnSameTeam)(int*, int*);
    *(int*)&OnSameTeam = call_onsameteam;
    
    if (other)
    {
        if (*(unsigned char *)(other + 352))
        {
            int v6 = *(int *)(other + 344);
            if (v6)
            {
                if (*(int *)(v6 + 8428) == 2 || x_deadchat->integer == 1)
                {
                    if (mode != 1 || OnSameTeam(ent, other))
                    {
                        if (!*(int *)(*(int *)(ent + 344) + 8400) || *(int*)(*(int *)(other + 344) + 8400))
                        {
                            SV_SendServerCommand(getclient(*other), 0, va("%s \"", (mode==1) ? "i" : "h", name, '^', color, message));
                        }
                    }
                }
            }
        }
    }
}