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