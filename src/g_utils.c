#include "script.h"
#include "server.h"
#include "bg_public.h"

G_SetAngle_t G_SetAngle;
G_SetOrigin_t G_SetOrigin;

void _Scr_FreeEntity(gentity_t *ent)
{
    scr_entityfield_t *fields = (scr_entityfield_t*)(GAME("vmMain") + 0x28094);
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