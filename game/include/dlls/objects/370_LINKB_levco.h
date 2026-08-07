#ifndef DLLS_OBJECTS_370_LINKB_LEVCO_H_
#define DLLS_OBJECTS_370_LINKB_LEVCO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "dlls/objects/430_SH_LevelCon.h"
#include "main/sky.h"

typedef enum LINKBLevelControlStage {
    LINKB_LEVEL_CONTROL_STAGE_START = 0,
    LINKB_LEVEL_CONTROL_STAGE_1 = 1,
    LINKB_LEVEL_CONTROL_STAGE_2 = 2,
    LINKB_LEVEL_CONTROL_STAGE_3 = 3,
    LINKB_LEVEL_CONTROL_STAGE_4 = 4,
    LINKB_LEVEL_CONTROL_STAGE_5 = 5,
} LINKBLevelControlStage;

typedef struct LINKBLevelControlState {
    GameBitLatchState gameBitLatch;
    s8 trickyHitCount : 2;
    u8 stage : 3;
    u8 unusedStageBits : 3;
    u8 alternatePath : 1;
    u8 unusedFlags : 7;
    u8 pad06[0x02];
    f32 trickyTalkTimer;
    s16 musicTriggerId;
    u8 pad0E[0x02];
} LINKBLevelControlState;

STATIC_ASSERT(offsetof(LINKBLevelControlState, gameBitLatch) == 0x00);
STATIC_ASSERT(offsetof(LINKBLevelControlState, pad06) == 0x06);
STATIC_ASSERT(offsetof(LINKBLevelControlState, trickyTalkTimer) == 0x08);
STATIC_ASSERT(offsetof(LINKBLevelControlState, musicTriggerId) == 0x0C);
STATIC_ASSERT(offsetof(LINKBLevelControlState, pad0E) == 0x0E);
STATIC_ASSERT(sizeof(LINKBLevelControlState) == 0x10);


int linkbLevelControl_getExtraSize(void);
void linkbLevelControl_update(GameObject* obj);
void linkbLevelControl_init(GameObject* obj);

extern SkyEnvFxRampTables gLINKBLevelControlEnvFxRampTables;
extern ObjectDescriptor gLINKBLevelControlObjDescriptor;

#endif /* DLLS_OBJECTS_370_LINKB_LEVCO_H_ */
