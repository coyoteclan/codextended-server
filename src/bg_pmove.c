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

#include "cracking.h"

pmove_t *pm;

//Slide off of the impacting surface
void PM_ClipVelocity(vec3_t in, vec3_t normal, vec3_t out, float overbounce)
{
	float backoff;
	float change;
	int i;
	backoff = DotProduct (in, normal);

	if (backoff < 0)
	{
		backoff *= overbounce;
	}
	else
	{
		backoff /= overbounce;
	}
	for (i=0 ; i<3 ; i++)
	{
		change = normal[i]*backoff;
		out[i] = in[i] - change;
	}
}

cvar_t *x_cl_adsair;
void _PM_ClearAimDownSightFlag()
{
	/*
	qpmove_t *pp = (qpmove_t*)pm;
	qps *ps = pp->ps;
	
	ps->pm_flags &= 0xDFu;
	*/
	if(!x_cl_adsair->integer)
		((void(*)())GAME("PM_ClearAimDownSightFlag"))();
}
void _PM_UpdateAimDownSightFlag()
{
	#if 0
		void (*_BG_UpdateConditionValue)(int,int,int,qboolean);
		*(int*)&_BG_UpdateConditionValue = GAME("BG_UpdateConditionValue");
		
		qpmove_t *pp = (qpmove_t*)pm;
		qps *ps = pp->ps;
		
		int v3 = *(int*)(&ps->cmdtime + 180);
		
		//something = *(int *)( *(int *)&pml[132] + 716)
		
		if(ps->pm_type <= 5 && pp->cmd.buttons & 0x10 && /*&& something*/v3 != 2 && v3 != 1 && v3 != 10 && v3 != 11 /*&& (*(int *)&pml[48] || pm_type == 1) )*/) {
			if(ps->pm_flags & 1) {
				if(!pp->oldcmd.flags & 0x10 || !pp->oldcmd.serverTime) {
					ps->pm_flags |= 0x20;
					*(byte*)&ps->pm_flags |= 4;
				}
			} else {
				ps->pm_flags |= 0x20;
			}
		} else {
			ps->pm_flags &= 0xDFu;
		}
		
		//for animations
		if ( ps->pm_flags & 0x20 )
			_BG_UpdateConditionValue(*(int*)(ps + 172), 7, 1, 1);
		else
			_BG_UpdateConditionValue(*(int*)(ps + 172), 7, 0, 1);
	#endif
	
	int *pp = (int*)pm;
	int *ps = *pp;
	int *gclient = *ps;
	int *v4 = (int *)(ps + 12);
	int val = *(int*)(gclient + 21); //336? 84*4=336 /84/4=21??
	
	//Com_DPrintf("val = %d\n", val);
	
	if (val == 1023 && x_cl_adsair->integer)
	{
		*v4 |= 0x20;
		return;
	}
		
	void (*call)();
	*(int*)&call=GAME("PM_UpdateAimDownSightFlag");
	call();
}

// by xoxor4d
void PM_ProjectVelocity(vec3_t in, vec3_t normal, vec3_t out)
{
	float speedXY, DotNormalXY, normalisedNormalXY, projectionZ, projectionXYZ;
	speedXY = in[1] * in[1] + in[0] * in[0];

	if ((normal[2]) < 0.001f || (speedXY == 0.0f))
	{
		VectorCopy(in, out);
	}
	else
	{
		DotNormalXY = normal[1] * in[1] + in[0] * normal[0];
		normalisedNormalXY = -DotNormalXY / normal[2];
		projectionZ = in[2] * in[2] + speedXY;
		projectionXYZ = sqrtf((projectionZ / (speedXY + normalisedNormalXY * normalisedNormalXY)));

		if (projectionXYZ < 1.0f || normalisedNormalXY < 0.0f || in[2] > 0.0f)
		{
			out[0] = projectionXYZ * in[0];
			out[1] = projectionXYZ * in[1];
			out[2] = projectionXYZ * normalisedNormalXY;
		}
	}
}

cvar_t *x_cl_bounce;
void PM_Bounce(vec3_t in, vec3_t normal, vec3_t out)
{
	if (x_cl_bounce->integer)
	{
		PM_ProjectVelocity(in, normal, out);
	}
	else
	{
		PM_ClipVelocity(in, normal, out, 1.001f);
	}
}

static void (*PM_CheckForChangeWeapon)();
static void (*PM_BeginWeaponChange)(int param_1, int param_2);
static void (*PM_FinishWeaponChange)();
void _PM_CheckForChangeWeapon()
{
	pmove_t *xm = *(pmove_t**)(int)pm;

	if ((xm->ps->pm_flags & 0x20000))
	{
		int *weapon = (int*)((int)xm->ps + 176);
		if (*weapon)
		{
			PM_BeginWeaponChange(*weapon, 0);
		}
		return;
	}
	PM_CheckForChangeWeapon();
}

void _PM_FinishWeaponChange()
{
	pmove_t *xm = *(pmove_t**)(int)pm;

	if ((xm->ps->pm_flags & 0x20000))
	{
		int *weapon = (int*)((int)xm->ps + 176);
		if (*weapon)
		{
			*weapon = 0;
		}
		return;
	}
	PM_FinishWeaponChange();
}

int player_g_gravity[MAX_CLIENTS] = {0};
cHook *hook_play_endframe;
int play_endframe(gentity_t *ent)
{
	cHook_unhook(hook_play_endframe);
	int (*sig)(gentity_t *ent);
	*(int *)&sig = hook_play_endframe->from;
	int ret = sig(ent);
	cHook_hook(hook_play_endframe);
	//TODO: check STATE_PLAYING
	int num = ent->s.number;
	if (player_g_gravity[num] > 0)
	{
		ent->client->ps.gravity = player_g_gravity[num];
	}
	return ret;
}

void BG_Link()
{
	__call(GAME("PM_Weapon") + 0x121, (int)_PM_CheckForChangeWeapon);
	PM_CheckForChangeWeapon = (void(*)())GAME("PM_AdjustAimSpreadScale") + 0x330;
	PM_BeginWeaponChange = (void(*)())GAME("PM_InteruptWeaponWithProneMove") + 0x614;
	__call(GAME("PM_Weapon") + 0x1BA, (int)_PM_FinishWeaponChange);
	PM_FinishWeaponChange = (void(*)())GAME("PM_InteruptWeaponWithProneMove") + 0x8FC;

	__call(GAME("PM_StepSlideMove") + 0x40A, (int)PM_Bounce);

	/*
		aim in air if client allows it
		maybe add a groundEntityNum = 1023; force???
		- Richard
	*/
	//__jmp( dlsym(gamelib, "PM_UpdateAimDownSightFlag"), _PM_UpdateAimDownSightFlag);
	int thk = GAME("PmoveSingle");
	__call(thk + 0x3cc, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x3ea, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x404, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x441, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x49e, _PM_UpdateAimDownSightFlag);
	__call(thk + 0x4d7, _PM_UpdateAimDownSightFlag);
	//__jmp( GAME("PM_ClearAimDownSightFlag"), _PM_ClearAimDownSightFlag);
	__call( thk + 0xFD, _PM_ClearAimDownSightFlag);
	__call( GAME("vmMain") - 0x1F119, _PM_ClearAimDownSightFlag);

	hook_play_endframe = (cHook *)malloc(sizeof(cHook));
	cHook_init(hook_play_endframe, GAME("ClientEndFrame"), (int)play_endframe);
	cHook_hook(hook_play_endframe);
}