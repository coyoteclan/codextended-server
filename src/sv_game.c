#include "server.h"
#include "script.h"

SV_SendServerCommand_t SV_SendServerCommand = (SV_SendServerCommand_t)0x808B900;
SV_GentityNum_t SV_GentityNum = (SV_GentityNum_t)0x8089258;

T_SetBrushModel_t T_SetBrushModel;
T_UnlinkEntity_t T_UnlinkEntity;
T_LinkEntity_t T_LinkEntity;
BG_EvaluateTrajectory_t BG_EvaluateTrajectory;
BG_EvaluateTrajectoryDelta_t BG_EvaluateTrajectoryDelta;

void set_trap_func_ptr( void )
{
    T_SetBrushModel = (T_SetBrushModel_t)GAME("trap_SetBrushModel");
    T_LinkEntity = (T_LinkEntity_t)GAME("trap_LinkEntity");
    T_UnlinkEntity = (T_UnlinkEntity_t)GAME("trap_UnlinkEntity");
    G_SetOrigin = (G_SetOrigin_t)GAME("G_SetOrigin");
    G_SetAngle = (G_SetAngle_t)GAME("G_SetAngle");
    BG_EvaluateTrajectory = (BG_EvaluateTrajectory_t)GAME("BG_EvaluateTrajectory");
    BG_EvaluateTrajectoryDelta = (BG_EvaluateTrajectoryDelta_t)GAME("BG_EvaluateTrajectoryDelta");
    
    void _Scr_FreeEntity(gentity_t*);
    __jmp(GAME("Scr_FreeEntity"), (unsigned)_Scr_FreeEntity);
    
    #if 0
    scr_entityfield_t *fields = (scr_entityfield_t*)( GAME("vmMain") + 0x28094 );
    FILE *fp = fopen("entfields.txt", "w");
    
    fprintf(fp, "scr_entityfield_t scr_entityfields[] = {\n");
    for(int i = 0; fields[i].name; i++) {
        fprintf(fp, "{\"%s\", %u, %u, %x},\n", fields[i].name, fields[i].offset, fields[i].type, fields[i].set);
    }
    fprintf(fp, "};\n");
    fclose(fp);
    #endif
}