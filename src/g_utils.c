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

#include "script.h"
#include "server.h"
#include "bg_public.h"

G_SetAngle_t G_SetAngle;
G_SetOrigin_t G_SetOrigin;

void _Scr_FreeEntity(gentity_t *ent)
{
	scr_entityfield_t *fields = (scr_entityfield_t*)( GAME("vmMain") + 0x28094 );
	for(int i = 0; fields[i].name; i++) {
		if(fields[i].type == 3)
			Scr_SetString((int*)((unsigned)ent + fields[i].offset), 0);
	}
	
	int i;
	for(i = 0; i < 6; i++) {
		*(unsigned char*)(i + (unsigned)ent + 748) = 0;
		Scr_SetString((int*)((unsigned)ent + (i*2) + 754), 0);
	}
	
	void (*FreeEntityNum)(int, int) = (void(*)(int,int isHudElem))GAME("Scr_FreeEntityNum");
	FreeEntityNum(ent->s.number, 0);
}