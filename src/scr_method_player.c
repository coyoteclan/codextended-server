#include "script.h"
#include "server.h"

void PlayerCmd_SendServerCommand(int a1)
{
    char* cmd = Scr_GetString(0);
    SV_SendServerCommand(getclient(a1), 1, cmd);
}

void PlayerCmd_isbot(int a1)
{
    client_t* cl = getclient(a1);
    if (cl)
    {
        if(cl->netchan.remoteAddress.type == NA_BOT)
            Scr_AddBool(true);
        else
            Scr_AddBool(false);
    }
    else
    {
        Scr_AddBool(false);
    }
}
void PlayerCmd_renamebot(int a1)
{
    char* key = Scr_GetString(0);
    char userinfo[MAX_STRING_CHARS];
    getuserinfo(a1, userinfo, sizeof(userinfo));
    
    /*char* value = Info_ValueForKey(userinfo, key);
    if(value == NULL)
        Scr_AddString("");
    else
        Scr_AddString(value);*/
    Info_SetValueForKey(userinfo, "name", key);
    setuserinfo(a1, userinfo);
    
    client_t* cl = getclient(a1);
    if (cl)
    {
        memcpy(&cl->name, key, 32);
        cl->name[31] = '\0';
    }
}
void PlayerCmd_kickbot(int a1)
{
    client_t* cl = getclient(a1);
    if(cl) {
        SV_DropClient(cl, "");
        cl->state = CS_FREE;
    }
}

void PlayerCmd_getip(int a1)
{
    client_t* cl = getclient(a1);
    Scr_AddString(NET_BaseAdrToString(cl->netchan.remoteAddress));
}
void PlayerCmd_GetPing(int self)
{
    client_t *cl = getclient(self);
    if (!cl)
    {
        Scr_AddInt(0);
        return;
    }
    Scr_AddInt(cl->ping);
}
void PlayerCmd_DropClient(int a1)
{
    char* reason = Scr_GetString(0);
    if(Scr_GetNumParam() > 0)
        SV_DropClient(getclient(a1), reason);
    else
        SV_DropClient(getclient(a1), NULL);
}

void PlayerCmd_GetUserInfoKey(int a1)
{
    char* key = Scr_GetString(0);
    char userinfo[MAX_STRING_CHARS];
    getuserinfo(a1, userinfo, sizeof(userinfo));
    
    char* value = Info_ValueForKey(userinfo, key);
    if(value == NULL)
        Scr_AddString("");
    else
        Scr_AddString(value);
}
void PlayerCmd_GetUserInfo(int a1)
{
    char userinfo[MAX_STRING_CHARS];
    getuserinfo(a1, userinfo, sizeof(userinfo));
    Scr_AddString(userinfo);
}

void PlayerCmd_SetMaxSpeed(int self)
{
    gentity_t *ent = &g_entities[self];
    if(!ent->client)
        return;
    gclient_t *gclient = ent->client;
    *(float*)((int)gclient + 68) = Scr_GetFloat(0);
}
void PlayerCmd_SetMoveSpeedScale(int self)
{
    gentity_t *ent = &g_entities[self];
    if(!ent->client)
        return;
    gclient_t *gclient = ent->client;
    *(float*)((int)gclient + 848) = Scr_GetFloat(0);
}
void PlayerCmd_SetVelocity(int self)
{
    gentity_t *e = &g_entities[self];
    if(!e->client)
    {
        Scr_Error("entity is not a player");
        return;
    }
    vec3_t vec;
    Scr_GetVector(0, vec);
    VectorCopy(vec, (float*)((int)e->client + POFF_VELOCITY));
}
void PlayerCmd_GetVelocity(int self)
{
    gentity_t *e = &g_entities[self];
    if(!e->client)
    {
        Scr_Error("entity is not a player");
        return;
    }
    vec3_t vec;
    VectorCopy((float*)((int)e->client + POFF_VELOCITY), vec);
    Scr_AddVector(vec);
}
void PlayerCmd_SetGravity(int self)
{
    gentity_t *e = &g_entities[self];
    if (!e->client)
    {
        Scr_Error("entity is not a player");
        return;
    }

    int gravity = Scr_GetInt(0);
    if (gravity < 0)
    {
        Scr_Error("negative gravity");
        return;
    }
    extern int player_g_gravity[MAX_CLIENTS];
    int num = e->s.number;
    player_g_gravity[num] = gravity;
}
void PlayerCmd_SetSpeed(int self)
{
    gentity_t *e = &g_entities[self];
    if (!e->client)
    {
        Scr_Error("entity is not a player");
        return;
    }

    int speed = Scr_GetInt(0);
    if (speed < 0)
    {
        Scr_Error("negative speed");
        return;
    }
    extern int player_g_speed[MAX_CLIENTS];
    int num = e->s.number;
    player_g_speed[num] = speed;
}

void PlayerCmd_GetInt(int self)
{
    gentity_t *ent = &g_entities[self];
    int off = Scr_GetInt(0);
    int flag = Scr_GetInt(1);
    int base = (int)ent, value;
    if(flag)
        base = (int)ent->client;
    value = *(int*)(base + off);
    Scr_AddInt(value);
}
void PlayerCmd_GetByte(int self)
{
    gentity_t *ent = &g_entities[self];
    int off = Scr_GetInt(0);
    unsigned char value;
    int flag = Scr_GetInt(1);
    int base = (int)ent;
    if(flag)
        base = (int)ent->client;
    value = *(unsigned char*)(base + off);
    Scr_AddInt(value);
}
void PlayerCmd_GetFloat(int self)
{
    gentity_t *ent = &g_entities[self];
    int off = Scr_GetInt(0);
    float value;
    int flag = Scr_GetInt(1);
    int base = (int)ent;
    if(flag)
        base = (int)ent->client;
    value = *(float*)(base + off);
    Scr_AddFloat(value);
}
void PlayerCmd_SetInt(int self)
{
    gentity_t *ent = &g_entities[self];
    int off = Scr_GetInt(0);
    int value = Scr_GetInt(1);
    int flag = Scr_GetInt(2);
    int base = (int)ent;
    if(flag)
        base = (int)ent->client;
    *(int*)(base + off) = value;
}
void PlayerCmd_SetByte(int self)
{
    gentity_t *ent = &g_entities[self];
    int off = Scr_GetInt(0);
    unsigned char value = Scr_GetInt(1) & 0xff;
    int flag = Scr_GetInt(2);
    int base = (int)ent;
    if(flag)
        base = (int)ent->client;
    *(unsigned char*)(base + off) = value;
}
void PlayerCmd_SetFloat(int self)
{
    gentity_t *ent = &g_entities[self];
    int off = Scr_GetInt(0);
    float value = Scr_GetFloat(1);
    int flag = Scr_GetInt(2);
    int base = (int)ent;
    if(flag)
        base = (int)ent->client;
    *(float*)(base + off) = value;
}

void PlayerCmd_GetStance(int self)
{
    gentity_t *ent = &g_entities[self];
    int base = (int)ent;
    unsigned char value = *(unsigned char*)(base + EOFF_EFLAGS);
    if(value & EF_PRONE)
        Scr_AddString("prone");
    else if(value & EF_CROUCH)
        Scr_AddString("crouch");
    else
        Scr_AddString("stand");
}
void PlayerCmd_forwardButtonPressed(int a1)
{
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.forwardmove & 0x7f) == 0x7f) {
            Scr_AddBool(true);
            return;
        }
    }
    Scr_AddBool(false);
}
void PlayerCmd_leanLeftButtonPressed(int a1)
{
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.wbuttons & 0x10)
        {
            Scr_AddBool(true);
            return;
        }
    }
    Scr_AddBool(false);
}
void PlayerCmd_leanRightButtonPressed(int a1)
{
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.wbuttons & 0x20)
        {
            Scr_AddBool(true);
            return;
        }
    }
    Scr_AddBool(false);
}
void PlayerCmd_aimButtonPressed(int a1)
{
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.buttons & 0x10)
        {
            Scr_AddBool(true);
            return;
        }
    }
    Scr_AddBool(false);
}
void PlayerCmd_reloadButtonPressed(int a1)
{
    client_t *cl = getclient(a1);
    if(cl) {
        if(cl->lastUsercmd.wbuttons & 0x8)
        {
            Scr_AddBool(true);
            return;
        }
    }
    Scr_AddBool(false);
}
void PlayerCmd_backButtonPressed(int a1)
{
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.forwardmove & 0x81) == 0x81)
        {
            Scr_AddBool(true);
            return;
        }
    }
    Scr_AddBool(false);
}
void PlayerCmd_leftButtonPressed(int a1)
{
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.rightmove & 0x81) == 0x81)
        {
            Scr_AddBool(true);
            return;
        }
    }
    Scr_AddBool(false);
}
void PlayerCmd_rightButtonPressed(int a1)
{
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.rightmove & 0x7f) == 0x7f)
        {
            Scr_AddBool(true);
            return;
        }
    }
    Scr_AddBool(false);
}
void PlayerCmd_moveupButtonPressed(int a1)
{
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.upmove & 0x7f) == 0x7f)
        {
            Scr_AddBool(true);
            return;
        }
    }
    Scr_AddBool(false);
}
void PlayerCmd_movedownButtonPressed(int a1)
{
    client_t *cl = getclient(a1);
    if(cl) {
        if((cl->lastUsercmd.upmove & 0x81) == 0x81)
        {
            Scr_AddBool(true);
            return;
        }
    }
    Scr_AddBool(false);
}
void PlayerCmd_getPlayerAngles(int self)
{
    gentity_t *ent = &g_entities[self];
    vec3_t vec;
    VectorCopy((float*)((int)ent->client + POFF_ANGLES), vec);
    Scr_AddVector(vec);
}
void PlayerCmd_getSpectatorClient(int self)
{
    gentity_t *ent = &g_entities[self];
    if(ent->client->spectatorClient == -1)
    {
        Scr_AddUndefined();
    }
    else
    {
        Scr_AddEntity(&g_entities[ent->client->spectatorClient]);
    }
}
void PlayerCmd_FreezeControls(int self)
{
    gentity_t *e = &g_entities[self];
    qboolean freeze;
    if(!e->client)
    {
        Scr_Error("entity is not a player");
        return;
    }
    freeze = Scr_GetBool(0);
    if (freeze)
        e->client->ps.pm_flags |= 0x4000;
    else
        e->client->ps.pm_flags &= ~0x4000;
}