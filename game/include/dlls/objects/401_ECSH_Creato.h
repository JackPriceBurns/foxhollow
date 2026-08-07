#ifndef DLLS_OBJECTS_401_ECSH_CREATO_H_
#define DLLS_OBJECTS_401_ECSH_CREATO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct ECSHCreatorPlacement {
    ObjPlacement base;
    s16 triggerGameBit;
    u8 unknown1A[0x1E - 0x1A];
    s8 initialYaw;
    s8 childGameBitOffset;
    u8 hitPointsOffset;
    u8 unknown21[0x24 - 0x21];
} ECSHCreatorPlacement;

STATIC_ASSERT(sizeof(ECSHCreatorPlacement) == 0x24);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, triggerGameBit) == 0x18);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, unknown1A) == 0x1A);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, initialYaw) == 0x1E);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, childGameBitOffset) == 0x1F);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, hitPointsOffset) == 0x20);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, unknown21) == 0x21);

typedef struct ECSHCreatorState {
    s16 spawnTimer;
    s16 spawnTimerRate;
    s16 triggerGameBit;
    s16 unknown06;
    s16 sharpClawHitPoints;
} ECSHCreatorState;

STATIC_ASSERT(sizeof(ECSHCreatorState) == 0x0A);
STATIC_ASSERT(offsetof(ECSHCreatorState, spawnTimer) == 0x00);
STATIC_ASSERT(offsetof(ECSHCreatorState, spawnTimerRate) == 0x02);
STATIC_ASSERT(offsetof(ECSHCreatorState, triggerGameBit) == 0x04);
STATIC_ASSERT(offsetof(ECSHCreatorState, unknown06) == 0x06);
STATIC_ASSERT(offsetof(ECSHCreatorState, sharpClawHitPoints) == 0x08);

extern ObjectDescriptor gECSHCreatorObjDescriptor;

int ecshCreator_getExtraSize(void);
int ecshCreator_getObjectTypeId(void);
void ecshCreator_free(void);
void ecshCreator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void ecshCreator_hitDetect(void);
void ecshCreator_update(GameObject* obj);
void ecshCreator_init(GameObject* obj, const ECSHCreatorPlacement* placement);
void ecshCreator_release(void);
void ecshCreator_initialise(void);

#endif /* DLLS_OBJECTS_401_ECSH_CREATO_H_ */
