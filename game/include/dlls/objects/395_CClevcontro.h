#ifndef DLLS_OBJECTS_395_CCLEVCONTRO_H_
#define DLLS_OBJECTS_395_CCLEVCONTRO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "dlls/objects/430_SH_LevelCon.h"
#include "main/sky.h"
#include "main/objseq.h"

typedef struct CCLevelControlPlacement {
    ObjPlacement base;
    u8 unknown18[0x20 - 0x18];
} CCLevelControlPlacement;

STATIC_ASSERT(sizeof(CCLevelControlPlacement) == 0x20);
STATIC_ASSERT(offsetof(CCLevelControlPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CCLevelControlPlacement, unknown18) == 0x18);

typedef struct CCLevelControlState {
    f32 textTimer;
    GameBitLatchState gameBitLatch;
    int musicTriggerId;
    int mapAct;
} CCLevelControlState;

STATIC_ASSERT(sizeof(CCLevelControlState) == 0x10);
STATIC_ASSERT(offsetof(CCLevelControlState, textTimer) == 0x00);
STATIC_ASSERT(offsetof(CCLevelControlState, gameBitLatch) == 0x04);
STATIC_ASSERT(offsetof(CCLevelControlState, musicTriggerId) == 0x08);
STATIC_ASSERT(offsetof(CCLevelControlState, mapAct) == 0x0C);


extern SkyEnvFxRampTables gCCLevelControlEnvFxRampTables;
extern ObjectDescriptor gCCLevelControlObjDescriptor;

int ccLevelControl_animationEventCallback(GameObject* obj, int unusedArg, ObjSeqState* animUpdate);
int ccLevelControl_getExtraSize(void);
void ccLevelControl_free(void);
void ccLevelControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                           s8 unusedVisible);
void ccLevelControl_update(GameObject* obj);
void ccLevelControl_init(GameObject* obj);

#endif /* DLLS_OBJECTS_395_CCLEVCONTRO_H_ */
