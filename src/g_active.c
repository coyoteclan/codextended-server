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

#include "shared.h"
#include "script.h"

vec3_t tmp_vec = {0};
void GScr_SetTempVec(unsigned n)
{
	vec3_t vec;
	if(Scr_GetType(0) != VT_VECTOR)
	{
		printf("NOT A VECTOR!\n");
		return;
	}
	Scr_GetVector(0, vec);
	VectorCopy(vec, tmp_vec);
}