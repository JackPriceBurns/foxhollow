#ifndef DLLS_OBJECTS_371_LINK_LEVCON_H_
#define DLLS_OBJECTS_371_LINK_LEVCON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "dlls/objects/430_SH_LevelCon.h"
#include "main/sky.h"

typedef struct LINKLevelControlState {
    s8 previousPlayerAreaCell;
    u8 pad01[0x03];
    int unknown04;
    int musicTriggerId;
    GameBitLatchState musicLatch;
} LINKLevelControlState;

STATIC_ASSERT(offsetof(LINKLevelControlState, previousPlayerAreaCell) == 0x00);
STATIC_ASSERT(offsetof(LINKLevelControlState, pad01) == 0x01);
STATIC_ASSERT(offsetof(LINKLevelControlState, unknown04) == 0x04);
STATIC_ASSERT(offsetof(LINKLevelControlState, musicTriggerId) == 0x08);
STATIC_ASSERT(offsetof(LINKLevelControlState, musicLatch) == 0x0C);
STATIC_ASSERT(sizeof(LINKLevelControlState) == 0x10);


void linkLevelControl_updateAreaMusic(GameObject* obj);
void linkLevelControl_applyEnterAreaEffects(GameObject* obj);
int linkLevelControl_getExtraSize(void);
void linkLevelControl_free(GameObject* obj);
void linkLevelControl_update(GameObject* obj);
void linkLevelControl_init(GameObject* obj);

extern SkyEnvFxRampTables gLINKLevelControlEnvFxRampTables;
extern ObjectDescriptor gLINKLevelControlObjDescriptor;

#endif /* DLLS_OBJECTS_371_LINK_LEVCON_H_ */
