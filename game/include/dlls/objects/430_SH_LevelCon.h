#ifndef DLLS_OBJECTS_430_SH_LEVEL_CON_H_
#define DLLS_OBJECTS_430_SH_LEVEL_CON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/gamebit_latch.h"

typedef struct ShLevelControlState {
    u32 flags;
    u8 sceneDelay;
    u8 mapAct;
    u8 eventState;
    u8 mapEventCountdown;
    f32 airMeterTimer;
    f32 hudTextTimer;
    s16 dayNightMusicLatch;
    s16 musicLatch;
} ShLevelControlState;

STATIC_ASSERT(sizeof(ShLevelControlState) == 0x14);
STATIC_ASSERT(offsetof(ShLevelControlState, flags) == 0x00);
STATIC_ASSERT(offsetof(ShLevelControlState, sceneDelay) == 0x04);
STATIC_ASSERT(offsetof(ShLevelControlState, mapAct) == 0x05);
STATIC_ASSERT(offsetof(ShLevelControlState, eventState) == 0x06);
STATIC_ASSERT(offsetof(ShLevelControlState, mapEventCountdown) == 0x07);
STATIC_ASSERT(offsetof(ShLevelControlState, airMeterTimer) == 0x08);
STATIC_ASSERT(offsetof(ShLevelControlState, hudTextTimer) == 0x0C);
STATIC_ASSERT(offsetof(ShLevelControlState, dayNightMusicLatch) == 0x10);
STATIC_ASSERT(offsetof(ShLevelControlState, musicLatch) == 0x12);

struct ObjSeqState;

int SH_LevelControl_getExtraSize(void);
void SH_LevelControl_free(void);
int SH_LevelControl_sequenceCallback(void* obj, void* unused, struct ObjSeqState* updateState);
void SH_LevelControl_updateTotemPuzzleMapState(void* obj, void* state);
void SH_LevelControl_update(GameObject* obj);
void SH_LevelControl_init(GameObject* obj);

extern ObjectDescriptor gSH_LevelControlObjDescriptor;

#endif /* DLLS_OBJECTS_430_SH_LEVEL_CON_H_ */
