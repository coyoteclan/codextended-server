#include "script.h"

int callbackEntityDamage, callbackEntityKilled;

static void die(gentity_t* self, gentity_t* inflictor, gentity_t* attacker, int damage, unsigned int mod)
{
    if(!callbackEntityKilled) {
        Scr_Error("ERROR: EntityDeath(eAttacker, eInflictor, iDamage, sMeansOfDeath) was not found!");
        return;
    }
    //printf(">> DIE [self=%p] [attacker=%p] [damage=%i] [mod=%d]\n", self, attacker, damage, mod);
    if(mod > 0x18)
        Scr_AddString((char*)modNames[0]);
    else
        Scr_AddString((char*)modNames[mod]);
    Scr_AddInt(damage);
    Scr_AddEntity(inflictor);
    Scr_AddEntity(attacker);
    int result = Scr_ExecEntThread(self->s.number, 0, callbackEntityKilled, 4);
    Scr_FreeThread(result);
}
static void pain(gentity_t* self, gentity_t* attacker, int damage, vec3_t point)
{
    if(!callbackEntityDamage) {
        Scr_Error("ERROR: EntityDamage(eAttacker, vPoint, iDamage) was not found!");
        return;
    }
    //Com_Printf(">> pain [self=%p] [attacker=%p] [damage=%i] [point=%f %f %f]\n", self, attacker, damage, point);
    Scr_AddInt(damage);
    Scr_AddVector(point);
    Scr_AddEntity(attacker);
    int result = Scr_ExecEntThread(self->s.number, 0, callbackEntityDamage, 2);
    Scr_FreeThread(result);
}

void ScriptEnt_GetPosition(int self)
{
    gentity_t *ent = &g_entities[self];
    Scr_AddVector(ent->s.pos.trBase);
}
void ScriptEnt_SetLight(int ent)
{
    gentity_t *e = &g_entities[ent];
    int r, g, b, i;
    r = Scr_GetInt(0);
    g = Scr_GetInt(1);
    b = Scr_GetInt(2);
    i = Scr_GetInt(3);
    e->s.constantLight = r | ( g << 8 ) | ( b << 16 ) | ( i << 24 );
}

#if 0
void ScriptEnt_SetAnim(int entityNum)
{
    char* animation = Scr_GetString(0);
    //TODO: check animation is valid string

    gentity_t *entity = &g_entities[entityNum];

    if (entity->s.eType == ET_CORPSE)
	{
        cprintf(PRINT_UNDERLINE | PRINT_DEBUG, "ScriptEnt_SetAnim: ET_CORPSE \n");

		int index = oBG_AnimationIndexForString(animation);
        entity->s.legsAnim = index;
		Scr_AddBool(true);
		return;
	}
    if(!entity->client)
    {
        Scr_Error("ScriptEnt_SetAnim: entity is not a player");
        return;
    }

    extern int custom_animation[MAX_CLIENTS];
	int animationIndex = 0;

    if (strcmp(animation, "none") != 0)
		animationIndex = oBG_AnimationIndexForString(animation);

    cprintf(PRINT_UNDERLINE | PRINT_DEBUG, "ScriptEnt_SetAnim: animationIndex = %i \n", animationIndex);

    custom_animation[entityNum] = animationIndex;
    Scr_AddBool(true);

    cprintf(PRINT_UNDERLINE | PRINT_DEBUG, "ScriptEnt_SetAnim: end \n");
}
#endif

void ScriptEnt_SetBounds(int entityNum)
{
    float width = Scr_GetFloat(0);
    float height = Scr_GetFloat(1);
    gentity_t *ent = &g_entities[entityNum];
    
    vec3_t mins = {-width,-width,0};
    vec3_t maxs = {width,width,height};
    
    VectorCopy(mins,ent->mins);
    VectorCopy(maxs,ent->maxs);
    VectorCopy(mins,ent->absmin);
    VectorCopy(maxs,ent->absmax);
}
void ScriptEnt_SetMins(int num)
{
    gentity_t *ent = &g_entities[num];
    vec3_t vec;
    Scr_GetVector(0, vec);
    VectorCopy(vec,ent->mins);
}
void ScriptEnt_SetMaxs(int num)
{
    gentity_t *ent = &g_entities[num];
    vec3_t vec;
    Scr_GetVector(0, vec);
    VectorCopy(vec,ent->maxs);
}
void ScriptEnt_SetAbsMax(int num)
{
    gentity_t *ent = &g_entities[num];
    vec3_t vec;
    Scr_GetVector(0, vec);
    VectorCopy(vec,ent->absmax);
}
void Ent_ShowToPlayer(unsigned num)
{
    if(num >= MAX_ENTITIES)
    {
        Scr_Error(va("%i is not a valid entity number", num));
        return;
    }
    
    gentity_t *self = &g_entities[num], *player = NULL;
    
    if (Scr_GetType(0) != VT_UNDEFINED)
        player = Scr_GetEntity(0);
    
    if (player == NULL)
    {
        self->svFlags &= ~SVF_SINGLECLIENT;
        self->singleClient = 0;
        return;
    }
    if (!player->client)
    {
        Scr_Error(va("entity %i is not a player", player->s.number));
        return;
    }
    self->svFlags |= SVF_SINGLECLIENT;
    self->singleClient = player->s.number;
}
void ScriptEnt_SetAbsMin(int num)
{
    gentity_t *ent = &g_entities[num];
    vec3_t vec;
    Scr_GetVector(0, vec);
    VectorCopy(vec,ent->absmin);
}
void ScriptEnt_SetTakeDamage(int entityIndex)
{
    gentity_t *ent = &g_entities[entityIndex];
    int flag = Scr_GetInt(0);
    int p = (int)pain, d = (int)die;
    
    if(!flag)
        p = d = 0;
    ent->takedamage = flag;
    ENT_SET(ent, EOFF_PAIN, &p, sizeof(int));
    ENT_SET(ent, EOFF_DIE, &d, sizeof(int));
}