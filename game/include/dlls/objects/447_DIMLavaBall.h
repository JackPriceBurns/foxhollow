#ifndef DLLS_OBJECTS_447_DIMLAVABALL_H_
#define DLLS_OBJECTS_447_DIMLAVABALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* All 13 active-retail DIMLavaBall placements are fixed 10-word records. */
typedef struct DimLavaBallPlacement {
    union {
        ObjPlacement base;
        struct {
            u8 unknown00[0x14];
            s32 projectileTargetObjectId;
        };
    };
    s16 firePeriod;
    u8 verticalSpeed;
    u8 horizontalSpeed;
    u8 rotXByte;
    u8 pendingEnabled;
    s16 triggerGameBit;
    u8 triggeredVerticalSpeed;
    u8 unknown21;
    s16 triggeredLaunchGameBit;
    s16 stateGameBit;
    u8 unknown26[2];
} DimLavaBallPlacement;

typedef struct DimLavaBallState {
    u8 unknown00[8];
    GameObject* projectile;
    f32 fireTimer;
    f32 firePeriod;
    s16 pendingEnabled;
    s16 pending;
    u8 triggeredLaunchUsed;
    u8 unknown19;
    u8 fireEnabled;
    u8 awaitingTrigger;
} DimLavaBallState;

STATIC_ASSERT(offsetof(DimLavaBallPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, projectileTargetObjectId) == 0x14);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, firePeriod) == 0x18);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, verticalSpeed) == 0x1A);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, horizontalSpeed) == 0x1B);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, rotXByte) == 0x1C);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, pendingEnabled) == 0x1D);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, triggerGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, triggeredVerticalSpeed) == 0x20);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, unknown21) == 0x21);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, triggeredLaunchGameBit) == 0x22);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, stateGameBit) == 0x24);
STATIC_ASSERT(offsetof(DimLavaBallPlacement, unknown26) == 0x26);
STATIC_ASSERT(sizeof(DimLavaBallPlacement) == 0x28);

STATIC_ASSERT(offsetof(DimLavaBallState, unknown00) == 0x00);
STATIC_ASSERT(offsetof(DimLavaBallState, projectile) == 0x08);
STATIC_ASSERT(offsetof(DimLavaBallState, fireTimer) == 0x0C);
STATIC_ASSERT(offsetof(DimLavaBallState, firePeriod) == 0x10);
STATIC_ASSERT(offsetof(DimLavaBallState, pendingEnabled) == 0x14);
STATIC_ASSERT(offsetof(DimLavaBallState, pending) == 0x16);
STATIC_ASSERT(offsetof(DimLavaBallState, triggeredLaunchUsed) == 0x18);
STATIC_ASSERT(offsetof(DimLavaBallState, unknown19) == 0x19);
STATIC_ASSERT(offsetof(DimLavaBallState, fireEnabled) == 0x1A);
STATIC_ASSERT(offsetof(DimLavaBallState, awaitingTrigger) == 0x1B);
STATIC_ASSERT(sizeof(DimLavaBallState) == 0x1C);

void lavaball1bf_clearPending(GameObject* obj);
int lavaball1bf_trySetPending(GameObject* obj);
int lavaball1bf_getExtraSize(void);
int lavaball1bf_getObjectTypeId(void);
void lavaball1bf_free(GameObject* obj, int mode);
void lavaball1bf_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void lavaball1bf_hitDetect(void);
void lavaball1bf_update(GameObject* obj);
void lavaball1bf_init(GameObject* obj, const DimLavaBallPlacement* placement);
void lavaball1bf_release(void);
void lavaball1bf_initialise(void);

extern ObjectDescriptor12 gLavaBall1BFObjDescriptor;

#endif /* DLLS_OBJECTS_447_DIMLAVABALL_H_ */
