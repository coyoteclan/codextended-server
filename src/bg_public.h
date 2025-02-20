#include "shared.h"

#ifndef BG_PUBLIC_H
#define BG_PUBLIC_H

typedef enum {
    EV_NONE, //0
    EV_FOOTSTEP_RUN_DEFAULT, //1
    EV_FOOTSTEP_RUN_BARK, //2
    EV_FOOTSTEP_RUN_BRICK, //3
    EV_FOOTSTEP_RUN_CARPET, //4
    EV_FOOTSTEP_RUN_CLOTH, //5
    EV_FOOTSTEP_RUN_CONCRETE, //6
    EV_FOOTSTEP_RUN_DIRT, //7
    EV_FOOTSTEP_RUN_FLESH, //8
    EV_FOOTSTEP_RUN_FOLIAGE, //9
    EV_FOOTSTEP_RUN_GLASS, //10
    EV_FOOTSTEP_RUN_GRASS, //11
    EV_FOOTSTEP_RUN_GRAVEL, //12
    EV_FOOTSTEP_RUN_ICE, //13
    EV_FOOTSTEP_RUN_METAL, //14
    EV_FOOTSTEP_RUN_MUD, //15
    EV_FOOTSTEP_RUN_PAPER, //16
    EV_FOOTSTEP_RUN_PLASTER, //17
    EV_FOOTSTEP_RUN_ROCK, //18
    EV_FOOTSTEP_RUN_SAND, //19
    EV_FOOTSTEP_RUN_SNOW, //20
    EV_FOOTSTEP_RUN_WATER, //21
    EV_FOOTSTEP_RUN_WOOD, //22
    EV_FOOTSTEP_RUN_ASPHALT, //23
    EV_FOOTSTEP_WALK_DEFAULT, //24
    EV_FOOTSTEP_WALK_BARK, //25
    EV_FOOTSTEP_WALK_BRICK, //26
    EV_FOOTSTEP_WALK_CARPET, //27
    EV_FOOTSTEP_WALK_CLOTH, //28
    EV_FOOTSTEP_WALK_CONCRETE, //29
    EV_FOOTSTEP_WALK_DIRT, //30
    EV_FOOTSTEP_WALK_FLESH, //31
    EV_FOOTSTEP_WALK_FOLIAGE, //32
    EV_FOOTSTEP_WALK_GLASS, //33
    EV_FOOTSTEP_WALK_GRASS, //34
    EV_FOOTSTEP_WALK_GRAVEL, //35
    EV_FOOTSTEP_WALK_ICE, //36
    EV_FOOTSTEP_WALK_METAL, //37
    EV_FOOTSTEP_WALK_MUD, //38
    EV_FOOTSTEP_WALK_PAPER, //39
    EV_FOOTSTEP_WALK_PLASTER, //40
    EV_FOOTSTEP_WALK_ROCK, //41
    EV_FOOTSTEP_WALK_SAND, //42
    EV_FOOTSTEP_WALK_SNOW, //43
    EV_FOOTSTEP_WALK_WATER, //44
    EV_FOOTSTEP_WALK_WOOD, //45
    EV_FOOTSTEP_WALK_ASPHALT, //46
    EV_FOOTSTEP_PRONE_DEFAULT, //47
    EV_FOOTSTEP_PRONE_BARK, //48
    EV_FOOTSTEP_PRONE_BRICK, //49
    EV_FOOTSTEP_PRONE_CARPET, //50
    EV_FOOTSTEP_PRONE_CLOTH, //51
    EV_FOOTSTEP_PRONE_CONCRETE, //52
    EV_FOOTSTEP_PRONE_DIRT, //53
    EV_FOOTSTEP_PRONE_FLESH, //54
    EV_FOOTSTEP_PRONE_FOLIAGE, //55
    EV_FOOTSTEP_PRONE_GLASS, //56
    EV_FOOTSTEP_PRONE_GRASS, //57
    EV_FOOTSTEP_PRONE_GRAVEL, //58
    EV_FOOTSTEP_PRONE_ICE, //59
    EV_FOOTSTEP_PRONE_METAL, //60
    EV_FOOTSTEP_PRONE_MUD, //61
    EV_FOOTSTEP_PRONE_PAPER, //62
    EV_FOOTSTEP_PRONE_PLASTER, //63
    EV_FOOTSTEP_PRONE_ROCK, //64
    EV_FOOTSTEP_PRONE_SAND, //65
    EV_FOOTSTEP_PRONE_SNOW, //66
    EV_FOOTSTEP_PRONE_WATER, //67
    EV_FOOTSTEP_PRONE_WOOD, //68
    EV_FOOTSTEP_PRONE_ASPHALT, //69
    EV_JUMP_DEFAULT, //70
    EV_JUMP_BARK, //71
    EV_JUMP_BRICK, //72
    EV_JUMP_CARPET, //73
    EV_JUMP_CLOTH, //74
    EV_JUMP_CONCRETE, //75
    EV_JUMP_DIRT, //76
    EV_JUMP_FLESH, //77
    EV_JUMP_FOLIAGE, //78
    EV_JUMP_GLASS, //79
    EV_JUMP_GRASS, //80
    EV_JUMP_GRAVEL, //81
    EV_JUMP_ICE, //82
    EV_JUMP_METAL, //83
    EV_JUMP_MUD, //84
    EV_JUMP_PAPER, //85
    EV_JUMP_PLASTER, //86
    EV_JUMP_ROCK, //87
    EV_JUMP_SAND, //88
    EV_JUMP_SNOW, //89
    EV_JUMP_WATER, //90
    EV_JUMP_WOOD, //91
    EV_JUMP_ASPHALT, //92
    EV_LANDING_DEFAULT, //93
    EV_LANDING_BARK, //94
    EV_LANDING_BRICK, //95
    EV_LANDING_CARPET, //96
    EV_LANDING_CLOTH, //97
    EV_LANDING_CONCRETE, //98
    EV_LANDING_DIRT, //99
    EV_LANDING_FLESH, //100
    EV_LANDING_FOLIAGE, //101
    EV_LANDING_GLASS, //102
    EV_LANDING_GRASS, //103
    EV_LANDING_GRAVEL, //104
    EV_LANDING_ICE, //105
    EV_LANDING_METAL, //106
    EV_LANDING_MUD, //107
    EV_LANDING_PAPER, //108
    EV_LANDING_PLASTER, //109
    EV_LANDING_ROCK, //110
    EV_LANDING_SAND, //111
    EV_LANDING_SNOW, //112
    EV_LANDING_WATER, //113
    EV_LANDING_WOOD, //114
    EV_LANDING_ASPHALT, //115
    EV_LANDING_PAIN_DEFAULT, //116
    EV_LANDING_PAIN_BARK, //117
    EV_LANDING_PAIN_BRICK, //118
    EV_LANDING_PAIN_CARPET, //119
    EV_LANDING_PAIN_CLOTH, //120
    EV_LANDING_PAIN_CONCRETE, //121
    EV_LANDING_PAIN_DIRT, //122
    EV_LANDING_PAIN_FLESH, //123
    EV_LANDING_PAIN_FOLIAGE, //124
    EV_LANDING_PAIN_GLASS, //125
    EV_LANDING_PAIN_GRASS, //126
    EV_LANDING_PAIN_GRAVEL, //127
    EV_LANDING_PAIN_ICE, //128
    EV_LANDING_PAIN_METAL, //129
    EV_LANDING_PAIN_MUD, //130
    EV_LANDING_PAIN_PAPER, //131
    EV_LANDING_PAIN_PLASTER, //132
    EV_LANDING_PAIN_ROCK, //133
    EV_LANDING_PAIN_SAND, //134
    EV_LANDING_PAIN_SNOW, //135
    EV_LANDING_PAIN_WATER, //136
    EV_LANDING_PAIN_WOOD, //137
    EV_LANDING_PAIN_ASPHALT, //138
    EV_FOLIAGE_SOUND, //139
    EV_STANCE_FORCE_STAND, //140
    EV_STANCE_FORCE_CROUCH, //141
    EV_STANCE_FORCE_PRONE, //142
    EV_STEP_VIEW, //143
    EV_WATER_TOUCH, //144
    EV_WATER_LEAVE, //145
    EV_ITEM_PICKUP, //146
    EV_ITEM_PICKUP_QUIET, //147
    EV_AMMO_PICKUP, //148
    EV_NOAMMO, //149
    EV_EMPTYCLIP, //150
    EV_RELOAD, //151
    EV_RELOAD_FROM_EMPTY, //152
    EV_RELOAD_START, //153
    EV_RELOAD_END, //154
    EV_RAISE_WEAPON, //155
    EV_PUTAWAY_WEAPON, //156
    EV_WEAPON_ALT, //157
    EV_PULLBACK_WEAPON, //158
    EV_FIRE_WEAPON, //159
    EV_FIRE_WEAPONB, //160
    EV_FIRE_WEAPON_LASTSHOT, //161
    EV_RECHAMBER_WEAPON, //162
    EV_EJECT_BRASS, //163
    EV_MELEE_SWIPE, //164
    EV_FIRE_MELEE, //165
    EV_MELEE_HIT, //166
    EV_MELEE_MISS, //167
    EV_FIRE_WEAPON_MG42, //168
    EV_FIRE_QUADBARREL_1, //169
    EV_FIRE_QUADBARREL_2, //170
    EV_BULLET_TRACER, //171
    EV_SOUND_ALIAS, //172
    EV_BULLET_HIT_SMALL, //173
    EV_BULLET_HIT_LARGE, //174
    EV_BULLET_HIT_CLIENT_SMALL, //175
    EV_BULLET_HIT_CLIENT_LARGE, //176
    EV_GRENADE_BOUNCE, //177
    EV_GRENADE_EXPLODE, //178
    EV_ROCKET_EXPLODE, //179
    EV_ROCKET_EXPLODE_NOMARKS, //180
    EV_MOLOTOV_EXPLODE, //181
    EV_MOLOTOV_EXPLODE_NOMARKS, //182
    EV_CUSTOM_EXPLODE, //183
    EV_CUSTOM_EXPLODE_NOMARKS, //184
    EV_RAILTRAIL, //185
    EV_BULLET, //186
    EV_PAIN, //187
    EV_CROUCH_PAIN, //188
    EV_DEATH, //189
    EV_DEBUG_LINE, //190
    EV_PLAY_FX, //191
    EV_PLAY_FX_DIR, //192
    EV_PLAY_FX_ON_TAG, //193
    EV_FLAMEBARREL_BOUNCE, //194
    EV_EARTHQUAKE, //195
    EV_DROPWEAPON, //196
    EV_ITEM_RESPAWN, //197
    EV_ITEM_POP, //198
    EV_PLAYER_TELEPORT_IN, //199
    EV_PLAYER_TELEPORT_OUT, //200
    EV_OBITUARY, //201
    EV_MAX_EVENTS //202
} entity_event_t;

typedef void (*G_FreeEntity_t)(gentity_t*);
extern G_FreeEntity_t G_FreeEntity;

typedef gentity_t* (*G_Spawn_t)();
extern G_Spawn_t G_Spawn;

typedef void (*G_SetOrigin_t)( gentity_t*, vec3_t);
extern G_SetOrigin_t G_SetOrigin;

typedef void (*G_SetAngle_t)( gentity_t*, vec3_t);
extern G_SetAngle_t G_SetAngle;

#endif