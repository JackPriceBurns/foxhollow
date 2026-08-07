#ifndef MAIN_DLL_DLL_0240_GCROBOTBLAST_H_
#define MAIN_DLL_DLL_0240_GCROBOTBLAST_H_

#include "game/objects/object.h"
#include "global.h"
#include "main/objseq.h"
#include "game/objects/object_setup.h"

typedef struct GCRobotBlastPlacement
{
    ObjPlacement base;
    u8 unk18;
    s8 mode;
} GCRobotBlastPlacement;

int GCRobotBlast_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int GCRobotBlast_getExtraSize(void);
int GCRobotBlast_getObjectTypeId(void);
void GCRobotBlast_free(void);
void GCRobotBlast_render(void);
void GCRobotBlast_hitDetect(void);
void GCRobotBlast_update(void);
void GCRobotBlast_init(GameObject* obj, GCRobotBlastPlacement* placement);
void GCRobotBlast_release(void);
void GCRobotBlast_initialise(void);

#endif /* MAIN_DLL_DLL_0240_GCROBOTBLAST_H_ */
