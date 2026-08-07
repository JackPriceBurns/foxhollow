#ifndef DLLS_OBJECTS_450_DIMSNOWBALL_H_
#define DLLS_OBJECTS_450_DIMSNOWBALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct DimSnowBallSpawnerPlacement {
    ObjPlacement base;
    s16 spawnPeriod;
    u8 childRotationParam1A;
    u8 childRotationParam1CBase;
    union {
        s8 childRotationXByte;
        u8 parentRotationXByte;
    };
    u8 unknown1D[3];
} DimSnowBallSpawnerPlacement;

typedef struct DimSnowBallSpawnerState {
    s16 spawnCountdown;
    s16 spawnPeriod;
} DimSnowBallSpawnerState;

STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, spawnPeriod) == 0x18);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, childRotationParam1A) == 0x1A);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, childRotationParam1CBase) == 0x1B);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, childRotationXByte) == 0x1C);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, parentRotationXByte) == 0x1C);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, unknown1D) == 0x1D);
STATIC_ASSERT(sizeof(DimSnowBallSpawnerPlacement) == 0x20);

STATIC_ASSERT(offsetof(DimSnowBallSpawnerState, spawnCountdown) == 0x00);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerState, spawnPeriod) == 0x02);
STATIC_ASSERT(sizeof(DimSnowBallSpawnerState) == 0x04);

int dimsnowball1c2_getExtraSize(void);
int dimsnowball1c2_getObjectTypeId(void);
void dimsnowball1c2_free(void);
void dimsnowball1c2_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dimsnowball1c2_hitDetect(void);
void dimsnowball1c2_update(GameObject* obj);
void dimsnowball1c2_init(GameObject* obj, DimSnowBallSpawnerPlacement* placement);
void dimsnowball1c2_release(void);
void dimsnowball1c2_initialise(void);

extern ObjectDescriptor gDIMSnowBall1C2ObjDescriptor;

#endif /* DLLS_OBJECTS_450_DIMSNOWBALL_H_ */
