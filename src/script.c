#include "script.h"

void GScr_SendServerCommand(int a1);
void PlayerCmd_SendConnectionlessPacket(int a1);
void GScr_killserver(int);
void GScr_typeof(int);

void Scr_fuckPrecacheString(int a1);

void GScr_seconds(int a1)
{
    Scr_AddInt(time(NULL));
}

void Math_Scr_sqrt(int a)
{
    float f = Scr_GetFloat(0);
    Scr_AddFloat(sqrt(f));
}

void GScr_system(int a)
{
    #ifdef uFEATUREUNSAFE
        char* command = Scr_GetString(0);
        Scr_AddBool(system(command));
    #else
        Scr_AddBool(0); // Scr_AddString("Unsafe features are disabled");?
    #endif
}

void Scr_GetArrayKeys(int a);
void Scr_PassArray(int);

void GScr_SetTempVec(unsigned n);

SCRIPTFUNCTION scriptFunctions[] =
{
    // name, function, developer
    {"settempvec", GScr_SetTempVec, 0},
    // MySQL
    #ifdef uMYSQL
    {"mysql_init", GScr_mysql_init, 0},
    {"mysql_close", GScr_mysql_close, 0},
    {"mysql_affected_rows", GScr_mysql_affected_rows, 0},
    {"mysql_errno", GScr_mysql_errno, 0},
    {"mysql_error", GScr_mysql_error, 0},
    {"mysql_fetch_field", GScr_mysql_fetch_field, 0},
    {"mysql_fetch_row", GScr_mysql_fetch_row, 0},
    {"mysql_field_seek", GScr_mysql_field_seek, 0},
    {"mysql_free_result", GScr_mysql_free_result, 0},
    {"mysql_num_fields", GScr_mysql_num_fields, 0},
    {"mysql_num_rows", GScr_mysql_num_rows, 0},
    {"mysql_query", GScr_mysql_query, 0},
    {"mysql_real_connect", GScr_mysql_real_connect, 0},
    {"mysql_real_escape_string", GScr_mysql_real_escape_string, 0},
    {"mysql_store_result", GScr_mysql_store_result, 0},
    {"mysql_get_connection", GScr_mysql_get_connection, 0},
    #endif
    
    // Math
    {"sqrt", Math_Scr_sqrt, 0},

    // String
    {"strtok", Scr_StrTok, 0},
    {"issubstr", Scr_IsSubStr, 0},
    {"tolower", Scr_ToLower, 0},
    {"toupper", Scr_ToUpper, 0},
    {"ucfirst", Scr_ucfirst, 0},
    {"replace", Scr_replace, 0},
    {"trim", Scr_trim, 0},

    // Util
    {"convert_string", Scr_convertToIString, 0},
    {"xprecachestring", Scr_fuckPrecacheString, 0},
    {"printconsole", GScr_printconsole, 0},
    {"seconds", GScr_seconds, 0},
    {"typeof", GScr_typeof, 0},
    {"sendservercommand", GScr_SendServerCommand, 0},
    {"trace", GScr_Trace, 0},
    {"md5", GScr_md5, 0},
    {"getarraykeys", Scr_GetArrayKeys, 0},
    {"passarray", Scr_PassArray, 0},
    {"creturn", GScr_return, 0},
    {"cmd_argc", GScr_Cmd_Argc, 0},
    {"cmd_argv", GScr_Cmd_Argv, 0},
    {"cmd_argvbuffer", GScr_trap_Argv, 0},
    {"concatargs", GScr_ConcatArgs, 0},
    {"getchat", GScr_getChat, 0},
    {"fopen", GScr_fopen, 0},
    {"fread", GScr_fread, 0},
    {"fclose", GScr_fclose, 0},
    {"fwrite", GScr_fwrite, 0},
    {"fexists", GScr_fexists, 0},
    {"fsize", GScr_fsize, 0},
    {"strpos", GScr_strpos, 0},
    {"salt_password", GScr_salt_password, 0},
    {"getconfigstring", GScr_getconfigstring, 0},
    {"configstringindex", GScr_configstringindex, 0},
    {"system", GScr_system, 0},
    {NULL, NULL, 0}
};

SCRIPTFUNCTION scriptMethods[] =
{
    /*
    ======
    ENTITY
    ======
    */
    {"getposition", ScriptEnt_GetPosition, 0},
    {"setbounds", ScriptEnt_SetBounds, 0},
    {"setsize", ScriptEnt_SetBounds, 0},
    {"settakedamage", ScriptEnt_SetTakeDamage, 0},
    {"setmaxs", ScriptEnt_SetMaxs, 0},
    {"setmins", ScriptEnt_SetMins, 0},
    {"setabsmins", ScriptEnt_SetAbsMin, 0},
    {"setabsmaxs", ScriptEnt_SetAbsMax, 0},
    {"setlight", ScriptEnt_SetLight, 0},
    //{"setanim", ScriptEnt_SetAnim, 0},
    {"showtoplayer", Ent_ShowToPlayer, 0},
    
    /*
    ======
    PLAYER
    ======
    */
    {"setvelocity", PlayerCmd_SetVelocity, 0},
    {"getvelocity", PlayerCmd_GetVelocity, 0},
    {"getplayerangles", PlayerCmd_getPlayerAngles, 0},
    {"getspectatorclient", PlayerCmd_getSpectatorClient, 0},
    {"getip", PlayerCmd_getip, 0},
    {"sendconnectionlesspacket", PlayerCmd_SendConnectionlessPacket, 0},
    {"sendservercommand", PlayerCmd_SendServerCommand, 0},
    {"getint", PlayerCmd_GetInt, 0},
    {"setint", PlayerCmd_SetInt, 0},
    {"getbyte", PlayerCmd_GetByte, 0},
    {"getfloat", PlayerCmd_GetFloat, 0},
    {"setfloat", PlayerCmd_SetFloat, 0},
    {"getstance", PlayerCmd_GetStance, 0},
    {"setbyte", PlayerCmd_SetByte, 0},
    {"getuserinfokey", PlayerCmd_GetUserInfoKey, 0},
    {"getuserinfo", PlayerCmd_GetUserInfo, 0},
    {"dropclient", PlayerCmd_DropClient, 0},
    {"kickbot", PlayerCmd_kickbot, 0},
    {"renamebot", PlayerCmd_renamebot, 0},
    {"isbot", PlayerCmd_isbot, 0},
    {"backbuttonpressed", PlayerCmd_backButtonPressed, 0},
    {"forwardbuttonpressed", PlayerCmd_forwardButtonPressed, 0},
    {"leftbuttonpressed", PlayerCmd_leftButtonPressed, 0},
    {"rightbuttonpressed", PlayerCmd_rightButtonPressed, 0},
    {"moveupbuttonpressed", PlayerCmd_moveupButtonPressed, 0},
    {"movedownbuttonpressed", PlayerCmd_movedownButtonPressed, 0},
    {"aimbuttonpressed", PlayerCmd_aimButtonPressed, 0},
    {"reloadbuttonpressed", PlayerCmd_reloadButtonPressed, 0},
    {"leanleftbuttonpressed", PlayerCmd_leanLeftButtonPressed, 0},
    {"leanrightbuttonpressed", PlayerCmd_leanRightButtonPressed, 0},
    {"getping", PlayerCmd_GetPing, 0},
    {"setmaxspeed", PlayerCmd_SetMaxSpeed, 0},
    {"setmovespeedscale", PlayerCmd_SetMoveSpeedScale, 0},
    {"freeze_controls", PlayerCmd_FreezeControls, 0},
    {"setgravity", PlayerCmd_SetGravity, 0},
    {"setspeed", PlayerCmd_SetSpeed, 0},
    {NULL, NULL, 0}
};

game_script_data* g_scr_data;
scr_const_t* scr_const;
xscr_const_t xscr_const;

unsigned char* g_clients;

int callbackTest;
int callbackPlayerCommand;
int callbackRemoteCommand;
int callbackFireGrenade;

bool scr_return = 0;

Scr_LoadScr_t Scr_LoadScript;
Scr_GetFunctionHandle_t Scr_GetFunctionHandle;

#ifdef _WIN32
Scr_ExecThread_t Scr_ExecThread = (Scr_ExecThread_t)0x481E10;
Scr_ExecEntThread_t Scr_ExecEntThread = (Scr_ExecEntThread_t)0x481EC0;
Scr_FreeThread_t Scr_FreeThread = (Scr_FreeThread_t)0x482070;
SL_ConvertToString_t SL_ConvertToString = (SL_ConvertToString_t)0x474EE0;
#else
Scr_ExecThread_t Scr_ExecThread = (Scr_ExecThread_t)0x80A95EC;
Scr_ExecEntThread_t Scr_ExecEntThread = (Scr_ExecEntThread_t)0x80a9674; //actually is Scr_ExecEntThreadNum
Scr_FreeThread_t Scr_FreeThread = (Scr_FreeThread_t)0x80A97D4; //RemoveRefToObject__FUs
SL_ConvertToString_t SL_ConvertToString;
#endif

SL_GetString_t SL_GetString;

/*
    Can't use Scr_GetMethod/Scr_GetFunction because those names already exists and aren't mangled.
*/

Scr_GetFunction_t Scr_GetFunction;
Scr_GetMethod_t Scr_GetMethod;

#ifdef _WIN32
Scr_MakeArray_t Scr_MakeArray = (Scr_MakeArray_t)0x483330;
Scr_AddArray_t Scr_AddArray = (Scr_AddArray_t)0x483380;
Scr_Error_t Scr_Error = (Scr_Error_t)0x483470;
#else
Scr_MakeArray_t Scr_MakeArray;
Scr_AddArray_t Scr_AddArray;
Scr_Error_t Scr_Error;
#endif

Scr_AddArrayStringIndexed_t Scr_AddArrayStringIndexed;

#ifdef _WIN32
Scr_GetNumParam_t Scr_GetNumParam = (Scr_GetNumParam_t)0x483060;
Scr_GetPointerType_t Scr_GetPointerType = (Scr_GetPointerType_t)0x482FC0;
Scr_GetType_t Scr_GetType = (Scr_GetType_t)0x482F50;
Scr_AddInt_t Scr_AddInt = (Scr_AddInt_t)0x483090;
Scr_AddBool_t Scr_AddBool = (Scr_AddBool_t)0x483070;
Scr_AddString_t Scr_AddString = (Scr_AddString_t)0x483210;
Scr_AddVector_t Scr_AddVector = (Scr_AddVector_t)0x4832E0;
Scr_AddUndefined_t Scr_AddUndefined = (Scr_AddUndefined_t)0x4830F0;
Scr_AddEntity_t Scr_AddEntity = (Scr_AddEntity_t)0x483140;
#else
Scr_GetNumParam_t Scr_GetNumParam;
Scr_GetPointerType_t Scr_GetPointerType;
Scr_GetEntity_t Scr_GetEntity;
Scr_GetType_t Scr_GetType;
Scr_AddFloat_t Scr_AddFloat;
Scr_AddInt_t Scr_AddInt;
Scr_AddBool_t Scr_AddBool;
Scr_AddString_t Scr_AddString;
Scr_AddIString_t Scr_AddIString;
Scr_AddVector_t Scr_AddVector;
Scr_AddUndefined_t Scr_AddUndefined;
Scr_AddEntity_t Scr_AddEntity;
#endif

Scr_AllocString_t Scr_AllocString;
Scr_SetString_t Scr_SetString;
Scr_GetConstString_t Scr_GetConstString;

oBG_AnimationIndexForString_t oBG_AnimationIndexForString;
CallSpawnEntity_t CallSpawnEntity;

Scr_GetBool_t Scr_GetBool;
Scr_GetInt_t Scr_GetInt;
Scr_GetAnim_t Scr_GetAnim = (Scr_GetAnim_t)0x80A7FAC;
Scr_GetAnimsIndex_t Scr_GetAnimsIndex;
Scr_GetFloat_t Scr_GetFloat;
Scr_GetVector_t Scr_GetVector;
Scr_GetString_t Scr_GetString;
Scr_GetFunc_t Scr_GetFunc;
Scr_GetOffset_t Scr_GetOffset;

SCRIPTFUNCTIONCALL Scr_GetCustomFunction(const char** fname, int* fdev)
{
    SCRIPTFUNCTIONCALL m = Scr_GetFunction(fname, fdev);
    void (*fc)(int);
    *(int*)&fc = (int)m;
    if(!m)
    {
        for(unsigned int i = 0; scriptFunctions[i].name; i++)
        {
            if(!strcmp(*fname, scriptFunctions[i].name))
            {
                SCRIPTFUNCTION func = scriptFunctions[i];
                *fname = func.name;
                *fdev = func.developer;
                return func.call;
            }
        }
    }
    return fc;
}
SCRIPTFUNCTIONCALL Scr_GetCustomMethod(const char** fname, int* fdev)
{
    SCRIPTFUNCTIONCALL m = Scr_GetMethod(fname, fdev);
    void (*fc)(int);
    *(int*)&fc = (int)m;
    if(!m)
    {
        for(unsigned int i = 0; scriptMethods[i].name; i++)
        {
            if(!strcmp(*fname, scriptMethods[i].name))
            {
                SCRIPTFUNCTION func = scriptMethods[i];
                *fname = func.name;
                *fdev = func.developer;
                return func.call;
            }
        }
    }
    return fc;
}

static int load_callback(const char* file, const char* functionname, bool flag)
{
    if(!Scr_LoadScript(file) && !flag)
        Com_Error(ERR_DROP, "Could not find script '%s'.", file);
    int v4 = Scr_GetFunctionHandle(file, functionname);
    if(!v4 && !flag)
        Com_Error(ERR_DROP, "Could not find label '%s'.", functionname, file);
    if(!flag || v4)
        cprintf(PRINT_GOOD|PRINT_UNDERLINE,"callback[%s] = %x\n", functionname, v4);
    return v4;
}

void GScr_typeof(int a1)
{
    int type = Scr_GetType(0);
    Scr_AddString(Scr_GetVariableType(type));
}
char* Scr_GetVariableType(int type)
{
    switch(type)
    {
        default:
        case VT_UNDEFINED: {
            return "undefined";
        }
        break;
        case VT_INT: {
            return "integer";
        } 
        break;
        case VT_FLOAT: {
            return "float";
        } 
        break;
        case VT_STRING: {
            return "string";
        } 
        break;
        case VT_VECTOR: {
            return "vector";
        } 
        break;
        case VT_LOCALIZED_STRING: {
            return "localized-string";
        } 
        break;
        case VT_OBJECT: {
            return "object";
        } 
        break;
        case VT_ARRAY: {
            return "array";
        } 
        break;
    }
}

/*
=============
FUNCTIONS
=============
*/

void GScr_printconsole(int entityIndex) //if this was a method the index would be the entity's number
{
    const char* txt = Scr_GetString(0);
    printf(txt);
}

void GScr_salt_password(int a1)
{
    char* password = Scr_GetString(0);
    char* salt = Scr_GetString(1);
    Scr_AddString(get_pass_hash(password, salt));
}

void GScr_configstringindex(int a1)
{
    char* str = Scr_GetString(0);
    int min = Scr_GetInt(1);
    int max = Scr_GetInt(2);
    int i;
    char cs[MAX_INFO_STRING];
    for (i = 1; i < max; i++)
    {
        SV_GetConfigstring(i, cs, sizeof(cs));
        if (!strcasecmp(str, cs))
        {
            Scr_AddInt(i);
            return;
        }
    }
    Scr_AddInt(0);
}
void GScr_getconfigstring(int a1)
{
    int i = Scr_GetInt(0);
    char cs[MAX_INFO_STRING];
    SV_GetConfigstring(i, cs, sizeof(cs));
    Scr_AddString(cs);
}

void GScr_strpos(int a1)
{
    char* haystack = Scr_GetString(0);
    char* needle = Scr_GetString(1);
    char* p = strstr(haystack, needle);
    if (p)
        Scr_AddInt(((int)p-(int)haystack));
    else
        Scr_AddInt(-1);
}

void GScr_SendServerCommand(int a1)
{
    char* cmd = Scr_GetString(0);
    SV_SendServerCommand(NULL, 1, cmd);
}

void PlayerCmd_SendConnectionlessPacket(int a1)
{
    char *msg = Scr_GetString(0);
    for(char *i = msg; *i != '\0'; *i++)
        if(*i == '\'')
            *i = '"';
            
    client_t *cl = getclient(a1);
    if(cl)
    NET_OutOfBandPrint(NS_SERVER, cl->netchan.remoteAddress, "%s", msg);
}

void GScr_Trace(int a1)
{
    trace_t tr;
    vec3_t start, end, mins, maxs;
    Scr_GetVector(0, start);
    Scr_GetVector(1, mins);
    Scr_GetVector(2, maxs);
    Scr_GetVector(3, end);
    int ignore = Scr_GetInt(4);
    int mask = Scr_GetInt(5);
    int locational = 0;
    int staticmodels = 0;

    if(Scr_GetNumParam() > 6)
        locational = Scr_GetInt(6);
    if(Scr_GetNumParam() > 7)
        staticmodels = Scr_GetInt(7);
    
    void (*trace)(void*,float*,float*,float*,float*,int ignore,int contentmask,int locational,char *priorityMap,int staticmodels);
    *(int*)&trace = 0x80916F4;
    
    trace(&tr,start,mins,maxs,end,-1,mask,locational,NULL,staticmodels);
    Scr_MakeArray();
    Scr_AddVector(tr.endpos); Scr_AddArrayStringIndexed(scr_const->position);

    if ((tr.entityNum - 1022) > 1)
    {
        Scr_AddInt(tr.entityNum); //Scr_AddEntity(&g_entities[tr.entityNum]); //scr_addentity crashed sometime
    }
    else
    {
        Scr_AddUndefined();
    }
    
    //All this below maybe go in the above else statement
    Scr_AddArrayStringIndexed(scr_const->entity);
    Scr_AddFloat(tr.fraction); Scr_AddArrayStringIndexed(scr_const->fraction);
    Scr_AddVector(tr.normal); Scr_AddArrayStringIndexed(scr_const->normal);
    Scr_AddInt(tr.contents); Scr_AddArrayStringIndexed(xscr_const.contents);
    Scr_AddInt(tr.surfaceFlags); Scr_AddArrayStringIndexed(scr_const->surfacetype);
    Scr_AddInt(tr.textureName); Scr_AddArrayStringIndexed(xscr_const.texturename);
}

void GScr_Cmd_Argc(int a1)
{
    Scr_AddInt(Cmd_Argc());
}
void GScr_Cmd_Argv(int a1)
{
    unsigned int idx = Scr_GetInt(0);
    if (idx > Cmd_Argc())
    {
        Scr_AddString("");
        return;
    }
    Scr_AddString(Cmd_Argv(idx));
}
void GScr_trap_Argv(int a1)
{
    unsigned int idx = Scr_GetInt(0);
    if (idx > Cmd_Argc())
    {
        Scr_AddString("");
        return;
    }
    char buf[MAX_STRING_CHARS];
    Cmd_ArgvBuffer(idx, buf, sizeof(buf));
    Scr_AddString(buf);
}
void GScr_ConcatArgs(int a1)
{
    unsigned int idx = Scr_GetInt(0);
    if (idx > Cmd_Argc())
    {
        Scr_AddString("");
        return;
    }
    Scr_AddString(ConcatArgs(idx));
}
void GScr_md5(int a1)
{
    char* str = Scr_GetString(0);
    Scr_AddString(get_md5(str));
}
void GScr_getChat(int a1)
{
    unsigned int idx = Scr_GetInt(0);
    if (idx > Cmd_Argc())
    {
        Scr_AddString("");
        return;
    }
    char* chat = ConcatArgs(idx);
    if (strlen(chat) == 0 || (chat[0] == 0x15 && strlen(chat)==1))
    {
        Scr_AddString("");
        return;
    }
    if(chat[0] == 0x15)
        Scr_AddString(&chat[1]);
    else
        Scr_AddString(chat);
}

void GScr_return(int a1)
{
    scr_return = 1;
}

bool Scr_Continue()
{
    bool ret = scr_return;
    scr_return = 0;
    return(!ret);
}

/*
=============
FILE FUNCTIONS
=============
*/

void GScr_fopen(int entityIndex)
{
    char* name = Scr_GetString(0);
    char* mode = Scr_GetString(1);
    FILE* f = fopen(name, mode);
    if(f)
        Scr_AddInt((int)(f));
    else
        Scr_AddInt(-1);
}
void GScr_fexists(int entityIndex)
{
    char* name = Scr_GetString(0);
    FILE* f;
    if (f = fopen(name, "r"))
    {
        fclose(f);
        Scr_AddBool(true);
    }
    else
    {
        Scr_AddBool(false);
    }
}
void GScr_fread(int entityIndex)
{
    char *buf = NULL;
    size_t size = 0;
    int len = Scr_GetInt(0);
    FILE *f = (FILE*)(Scr_GetInt(1));
    if (f == NULL)
    {
        Scr_AddUndefined();
    }
    else
    {
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        rewind(f);
        buf = (char *) malloc(size);
        fread(buf, size, 1, f);
        Scr_AddString(buf);
        free(buf);
    }
}
void GScr_fwrite(int entityIndex)
{
    char* text = Scr_GetString(0);
    FILE *f = (FILE*)(Scr_GetInt(1));
    if (f == NULL)
    {
        Scr_AddBool(false);
    }
    else
    {
        Scr_AddBool(true);
        fprintf(f, text);
    }
}
void GScr_fsize(int entityIndex)
{
    char* name = Scr_GetString(0);
    FILE *f = fopen(name, "r");
    size_t len;
    if (f)
    {
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        rewind(f);
        Scr_AddInt((int)len);
    }
    else
    {
        Scr_AddInt(-1);
    }
}
void GScr_fclose(int entityIndex)
{
    FILE *f = (FILE*)(Scr_GetInt(0));
    if (f)
    {
        Scr_AddBool(true);
        fclose(f);
    }
    else
    {
        Scr_AddBool(false);
    }
}

void Scr_LoadConsts()
{
    xscr_const.texturename = Scr_AllocString("texturename", 1);
    xscr_const.contents = Scr_AllocString("contents", 1);
    xscr_const.perks[PERK_QUICK_RELOAD] = Scr_AllocString("sleight_of_hand", 1);
}

void _Scr_StartupGameType()
{
    unsigned short ret = Scr_ExecThread(g_scr_data->startgametype, 0);
    Scr_FreeThread(ret);
}
void _Scr_LoadGameType()
{
    unsigned short ret = Scr_ExecThread(g_scr_data->gametype_main, 0);
    Scr_FreeThread(ret);
}
void _Scr_PlayerConnect(gentity_t *self)
{
    unsigned short ret = Scr_ExecEntThread(self->s.number, 0, g_scr_data->playerconnect, 0);
    Scr_FreeThread(ret);
}
void _Scr_PlayerDisconnect(gentity_t *self)
{
    unsigned short ret = Scr_ExecEntThread(self->s.number, 0, g_scr_data->playerdisconnect, 0);
    Scr_FreeThread(ret);
}
#if 0
void _Scr_PlayerDamage(
    gentity_t *self,
    gentity_t *inflictor,
    gentity_t *attacker,
    int damage,
    int dmgflags, unsigned mod, int weapon, vec3_t point, vec3_t dir, int hitloc)
{
    short (*_G_GetHitLocationString)(int) = (short(*)(int))GAME("G_GetHitLocationString");
    Scr_AddConstString(G_GetHitLocationString(hitloc));
    int (*getinfo)(int);
    *(int*)&getinfo = GAME("BG_GetInfoForWeapon");
    unsigned info = (unsigned)getinfo(weapon);
    char **_modNames = (char**)GAME("modNames");
    
    if(dir)
        Scr_AddVector(dir);
    else
        Scr_AddUndefined();
    if(point)
        Scr_AddVector(point);
    else
        Scr_AddUndefined();

    Scr_AddString((char*)*(int*)(info + 4));
    if(mod < 0x18)
        Scr_AddString(_modNames[mod]);
    else
        Scr_AddString("badMOD");

    Scr_AddInt(dmgflags);
    Scr_AddInt(damage);
    if(attacker) Scr_AddEntity(attacker); else Scr_AddUndefined();
    if(inflictor) Scr_AddEntity(inflictor); else Scr_AddUndefined();

    unsigned short ret = Scr_ExecEntThread(self->s.number, 0, g_scr_data->playerdamage, 9);
    Scr_FreeThread(ret);
}
#endif

void _Cmd_MenuResponse_f(gentity_t *self)
{
    int svId = atoi(Cmd_Argv(1));
    cvar_t *svid = Cvar_Get("sv_serverId", "0", 72);
    
    if(svid->integer != svId)
        return;
    
    char cs[MAX_STRING_CHARS] = {0};
    unsigned menuIndex = atoi(Cmd_Argv(2));
    SV_GetConfigstring(1180 + menuIndex, cs, sizeof(cs));
    
    void (*o)(gentity_t*) = (void(*)(gentity_t*))GAME("Cmd_MenuResponse_f");
    o(self);
}

void GScr_LoadGametypeScript(void)
{
    char v1[64];
    extern int callbackEntityDamage, callbackEntityKilled;

    snprintf(v1, 64, "maps/mp/gametypes/%s", g_gametype->string);

    g_scr_data->gametype_main = load_callback(v1, "main", 0);
    g_scr_data->startgametype = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_StartGameType", 0);
    g_scr_data->playerconnect = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerConnect", 0);
    g_scr_data->playerdisconnect = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerDisconnect", 0);
    g_scr_data->playerdamage = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerDamage", 0);
    g_scr_data->playerkilled = load_callback("maps/mp/gametypes/_callbacksetup", "CodeCallback_PlayerKilled", 0);
    callbackPlayerCommand = load_callback("callback", "CodeCallback_PlayerCommand", 1);
    callbackRemoteCommand = load_callback("callback", "CodeCallback_RemoteCommand", 1);
    callbackFireGrenade = load_callback("callback", "CodeCallback_FireGrenade", 1);
    callbackEntityDamage = load_callback("callback", "EntityDamage", 1);
    callbackEntityKilled = load_callback("callback", "EntityDeath", 1);
    
    Scr_LoadConsts();
}

static int x_localized_string_index = 128;
int X_SetLocalizedStringIndex(int index, const char* str)
{
    int i = index;
    
    if(!str || !*str)
        return 0;
    
    if(i >= 256)
        i = x_localized_string_index;
    
    if(i >= 256)
        i = x_localized_string_index = 0;

    SV_SetConfigstring(i + 1244, str);
    return i;
}

int X_LocalizedStringIndex(const char* str)
{
    int i;
    char s[MAX_STRING_CHARS];
    
    if(x_localized_string_index >= 256)
        x_localized_string_index = 128;
    
    if(!str || !*str)
        return 0;
    
    int start = 1244;
    for (i = 1; i < 256; i++)
    {
        SV_GetConfigstring(start + i, s, sizeof(s));
        if (!*s)
            break;
        if (!strcmp(s, str))
            return i;
    }
    
    if (i == 256) //idea to add ignoring strings, maybe first bit. or use half/half 128 for tmp storage
    {
        //overflow
        //i = 2047;
        i = x_localized_string_index;
    }
    
    SV_SetConfigstring(i + 1244, str); //would require to call the precacheString(..) each time you use that string so let's just make a new function
    ++x_localized_string_index;
    return i;
}

/*
    //TODO
        * reserve player's name up to <sv_maxclients> configstrings which can only be overwritten by connecting players.
        * reserve "real precached" strings up to e.g 128~?
        * reserve some more
        
        - Richard
*/

void Scr_fuckPrecacheString(int a1)
{
    int argc = Scr_GetNumParam();
    char* str;
    int index = 256;

    if (argc == 1)
    {
        str = Scr_GetString(0);
    }
    else
    {
        index = Scr_GetInt(0);
        str = Scr_GetString(1);
    }
    int idx = X_SetLocalizedStringIndex(index, str);
    Scr_AddInt(idx);
}

gentity_t *_fire_grenade(gentity_t *self, vec3_t start, vec3_t dir, int grenadeWPID)
{
    if (callbackFireGrenade)
    {
        int result = Scr_ExecEntThread(self->s.number, 0, callbackFireGrenade, 0);
        Scr_FreeThread(result);
    }

    void (*o)(gentity_t*, vec3_t, vec3_t, int) = (void(*)(gentity_t*, vec3_t, vec3_t, int))GAME("fire_grenade");
    o(self, start, dir, grenadeWPID);
}

gitem_t *bg_itemlist;

unsigned short (*GetVariableName)(unsigned short) = (unsigned short(*)(unsigned short))0x80A3060;
unsigned short (*GetNextVariable)(unsigned short) = (unsigned short(*)(unsigned short))0x80A3028; //idk original funcname
unsigned short (*get_var_by_idx)(unsigned short) = (unsigned short(*)(unsigned short))0x80A3210;

unsigned short Scr_GetArray(int index)
{
    int *params = (int*)0x82F5944;
    int stack = *(int*)0x82F5948;
    int base = (stack - 8 * index);
    int vartype = *(int*)(base + 4);
    int *params2 = (int*)0x82F5888;
    //printf("params=%d,params2=%d\n",*params,*params2);
    //printf("stack type = %s\n", Scr_GetVariableType(*(int*)(base + 4)));

    if (*params > index)
    {
        //printf("stack type = %s(%d)\n", Scr_GetVariableType(vartype), vartype);
        //printf("base = 0x%x\n", base);
        //raise(SIGINT);
        if(vartype == VT_OBJECT)
            return *(unsigned short*)base;
    }
    //error param doesn't exist.
    //Scr_Error("error parameter does not exist!\n");
    printf("scr_error\n");
    return 0;
}

unsigned GetVariableXD(unsigned int idx)
{
    int *variables = (int*)0x81F17C0;
    return ((unsigned)variables + 12 * idx);
}

void Scr_PassArray(int n)
{
    unsigned short i, var, a, arr;
    for (a = 0; a < Scr_GetNumParam(); a++)
    {
        arr = Scr_GetArray(a);
        for (i = GetNextVariable(arr), var = get_var_by_idx(i); i != 0;)
        {
            printf("%d: %s = %s (var=%x)\n", i, SL_ConvertToString(GetVariableName(i)), SL_ConvertToString(var), GetVariableXD(var));
            i = GetNextVariable(i);
            var = get_var_by_idx(i);
        }
    }
}

void Scr_GetArrayKeys(int a)
{
    unsigned short arrIndex = Scr_GetArray(0);
    Scr_MakeArray();
    unsigned short i, var;
    for (i = GetNextVariable(arrIndex); i != 0;)
    {
        //printf("%d: %s = %s\n", i, SL_ConvertToString(GetVariableName(i)), SL_ConvertToString(var));
        Scr_AddString(SL_ConvertToString(GetVariableName(i)));
        Scr_AddArray();
        i = GetNextVariable(i);
        var = get_var_by_idx(i);
    }
}

char rct_bytes[5];
void _Scr_RunCurrentThreads()
{
    memcpy((void*)GAME("Scr_RunCurrentThreads"), (void*)&rct_bytes[0], 5);
    void (*o)() = (void(*)())GAME("Scr_RunCurrentThreads");
    o();
    __jmp(GAME("Scr_RunCurrentThreads"), (unsigned)_Scr_RunCurrentThreads);
}
#include <netinet/in.h>
void scriptInitializing()
{
    memcpy((void*)&rct_bytes[0], (void*)GAME("Scr_RunCurrentThreads"), 5);
    __jmp(GAME("Scr_RunCurrentThreads"), (unsigned)_Scr_RunCurrentThreads);
    //__jmp(GAME("Scr_GetGenericField"), _Scr_GetGenericField);
    SCRIPTFUNCTION *it = (SCRIPTFUNCTION*)GAME("functions");
    //printf("Patched developer functions:\n");
    //FILE *log_file = fopen("cod1_1_functiondump.txt", "w");
    for (int i = 0; i != 0x69; i++, it++)
    {
        //fprintf(log_file, "%s, ", it->name);
        if(!it->developer)
            continue;
        it->developer = 0;
    }
    //fclose(log_file);
    //printf("\n");

    __jmp(GAME("Scr_StartupGameType"), (unsigned)_Scr_StartupGameType);
    __jmp(GAME("Scr_LoadGameType"), (unsigned)_Scr_LoadGameType);
    __jmp(GAME("Scr_PlayerConnect"), (unsigned)_Scr_PlayerConnect);
    __jmp(GAME("Scr_PlayerDisconnect"), (unsigned)_Scr_PlayerDisconnect);
    //__jmp(GAME("Scr_PlayerDamage"), (unsigned)_Scr_PlayerDamage);
    __jmp((int)GAME("GScr_LoadGameTypeScript"), (int)GScr_LoadGametypeScript);
    __jmp(GAME("G_LocalizedStringIndex"), (int)X_LocalizedStringIndex);
    __call(GAME("ClientCommand")+0x140, (unsigned)_Cmd_MenuResponse_f);
    __call(GAME("FireWeapon")+0x2E5, (unsigned)_fire_grenade);
    
    g_scr_data = (game_script_data*)dlsym(gamelib, "g_scr_data");
    scr_const = (scr_const_t*)GAME("scr_const");
    g_clients = (unsigned char*)GAME("g_clients");
    bg_itemlist = (gitem_t*)GAME("bg_itemlist");
    CallSpawnEntity = (CallSpawnEntity_t)GAME("G_CallSpawnEntity");
    SL_GetString = (SL_GetString_t)GAME("SL_GetString");
    Scr_GetFunction = (Scr_GetFunction_t)GAME("Scr_GetFunction");
    Scr_GetMethod = (Scr_GetMethod_t)GAME("Scr_GetMethod");
    Scr_GetString = (Scr_GetString_t)dlsym(gamelib, "Scr_GetString");
    Scr_GetConstString = (Scr_GetConstString_t)GAME("Scr_GetConstString");
    Scr_SetString = (Scr_SetString_t)GAME("Scr_SetString");
    Scr_AllocString = (Scr_AllocString_t)GAME("Scr_AllocString");
    oBG_AnimationIndexForString = (oBG_AnimationIndexForString_t)GAME("BG_AnimationIndexForString");
    Scr_GetInt = (Scr_GetInt_t)dlsym(gamelib, "Scr_GetInt");
    //Scr_GetAnim = (Scr_GetAnim_t)dlsym(gamelib, "Scr_GetAnim");
    Scr_GetAnimsIndex = (Scr_GetAnimsIndex_t)dlsym(gamelib, "Scr_GetAnimsIndex");
    Scr_GetFloat = (Scr_GetFloat_t)dlsym(gamelib, "Scr_GetFloat");
    Scr_GetVector = (Scr_GetVector_t)dlsym(gamelib, "Scr_GetVector");
    Scr_GetNumParam = (Scr_GetNumParam_t)dlsym(gamelib, "Scr_GetNumParam");
    Scr_GetBool = (Scr_GetBool_t)dlsym(gamelib, "Scr_GetBool");
    Scr_GetFunc = (Scr_GetFunc_t)dlsym(gamelib, "Scr_GetFunc");
    Scr_GetOffset = (Scr_GetOffset_t)GAME("Scr_GetOffset");
    Scr_GetType = (Scr_GetType_t)dlsym(gamelib, "Scr_GetType");
    Scr_GetPointerType = (Scr_GetPointerType_t)dlsym(gamelib, "Scr_GetPointerType");
    Scr_GetEntity = (Scr_GetEntity_t)GAME("Scr_GetEntity");
    Scr_AddInt = (Scr_AddInt_t)dlsym(gamelib, "Scr_AddInt");
    Scr_AddFloat = (Scr_AddFloat_t)dlsym(gamelib, "Scr_AddFloat");
    Scr_AddVector = (Scr_AddVector_t)dlsym(gamelib, "Scr_AddVector");
    Scr_AddString = (Scr_AddString_t)dlsym(gamelib, "Scr_AddString");
    Scr_AddIString = (Scr_AddIString_t)dlsym(gamelib, "Scr_AddIString");
    Scr_AddEntity = (Scr_AddEntity_t)dlsym(gamelib, "Scr_AddEntity");
    Scr_AddUndefined = (Scr_AddUndefined_t)dlsym(gamelib, "Scr_AddUndefined");
    Scr_AddBool = (Scr_AddBool_t)dlsym(gamelib, "Scr_AddBool");
    Scr_MakeArray = (Scr_MakeArray_t)dlsym(gamelib, "Scr_MakeArray");
    Scr_AddArray = (Scr_AddArray_t)dlsym(gamelib, "Scr_AddArray");
    Scr_AddArrayStringIndexed = (Scr_AddArrayStringIndexed_t)dlsym(gamelib, "Scr_AddArrayStringIndexed");
    Scr_Error = (Scr_Error_t)dlsym(gamelib, "Scr_Error");
    SL_ConvertToString = (SL_ConvertToString_t)GAME("SL_ConvertToString");
    Scr_GetFunctionHandle = (Scr_GetFunctionHandle_t)GAME("Scr_GetFunctionHandle");
    Scr_LoadScript = (Scr_LoadScr_t)GAME("Scr_LoadScript");
}