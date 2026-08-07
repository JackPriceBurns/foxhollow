#ifndef DLLS_OBJECTS_382_MMP_LEVELCO_H_
#define DLLS_OBJECTS_382_MMP_LEVELCO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "dlls/objects/430_SH_LevelCon.h"
#include "main/objseq.h"

extern GameBitLatchState gMMPLevelControlMusicLatch;
extern f32 gMMPLevelControlTextCountdown;
extern ObjectDescriptor gMMPLevelControlObjDescriptor;

int mmpLevelControl_processAnimEvents(GameObject* obj, int unusedArg2, ObjSeqState* animUpdate);
int mmpLevelControl_getExtraSize(void);
int mmpLevelControl_getObjectTypeId(void);
void mmpLevelControl_free(GameObject* obj);
void mmpLevelControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                            s8 visible);
void mmpLevelControl_hitDetect(void);
void mmpLevelControl_update(GameObject* obj);
void mmpLevelControl_init(GameObject* obj);
void mmpLevelControl_release(void);
void mmpLevelControl_initialise(void);

#endif /* DLLS_OBJECTS_382_MMP_LEVELCO_H_ */
