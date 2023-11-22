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

#if CODPATCH == 1
static Cvar_FindVar_t Cvar_FindVar = (Cvar_FindVar_t)0x806E9B4;
Cvar_Set_t Cvar_Set = (Cvar_Set_t)0x806ECD4;
Cvar_Get_t Cvar_Get = (Cvar_Get_t)0x806EA34;
#else 
static Cvar_FindVar_t Cvar_FindVar = (Cvar_FindVar_t)0x8072916;
Cvar_Set_t Cvar_Set = (Cvar_Set_t)0x8073100;
Cvar_Get_t Cvar_Get = (Cvar_Get_t)0x8072A7C;
#endif

float Cvar_VariableValue( const char *var_name )
{
	cvar_t  *var;

	var = Cvar_FindVar( var_name );
	if ( !var ) {
		return 0;
	}
	return var->value;
}
int Cvar_VariableIntegerValue( const char *var_name )
{
	cvar_t  *var;

	var = Cvar_FindVar( var_name );
	if ( !var ) {
		return 0;
	}
	return var->integer;
}
char *Cvar_VariableString( const char *var_name )
{
	cvar_t *var;

	var = Cvar_FindVar( var_name );
	if ( !var ) {
		return "";
	}
	return var->string;
}
void Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize )
{
	cvar_t *var;

	var = Cvar_FindVar( var_name );
	if ( !var ) {
		*buffer = 0;
	} else {
		Q_strncpyz( buffer, var->string, bufsize );
	}
}
void Cvar_LatchedVariableStringBuffer( const char *var_name, char *buffer, int bufsize )
{
	cvar_t *var;

	var = Cvar_FindVar( var_name );
	if ( !var ) {
		*buffer = 0;
	} else {
		if ( var->latchedString ) {
			Q_strncpyz( buffer, var->latchedString, bufsize );
		} else {
			Q_strncpyz( buffer, var->string, bufsize );
		}
	}
}

cvar_t *Cvar_Set2( const char *var_name, const char *value, qboolean force )
{
	cvar_t * (*ret)(const char*,const char*,int);
	*(int*)&ret = 0x806ECD4;
	return ret(var_name, value, force);
}