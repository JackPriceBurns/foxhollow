#ifndef DLLS_OBJECTS_268_LANTERNFIRE_H_
#define DLLS_OBJECTS_268_LANTERNFIRE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/modellight_api.h"

#define LANTERN_FIREFLY_PLACEMENT_SIZE 0x24
#define LANTERN_FIREFLY_STATE_SIZE     0x74

/* FireFlyLant allocates this complete setup before spawning each child. */
typedef struct LanternFireFlyControlBits {
    u8 motionMode : 2;
    u8 unused : 6;
} LanternFireFlyControlBits;

typedef struct LanternFireFlyPlacement {
    ObjPlacement base; /* 0x00 */
    s8 wanderRange;    /* 0x18 */
    u8 stateId;        /* 0x19 */
    s16 timer;         /* 0x1A: active lifetime */
    s16 driftRangeZ;   /* 0x1C: Z drift distance */
    u8 pad1E[6];       /* 0x1E */
} LanternFireFlyPlacement;

typedef struct LanternFireFlyState {
    ModelLightStruct* light; /* 0x00 */
    f32 controlX[4];         /* 0x04: B-spline control-point ring */
    f32 controlY[4];         /* 0x14 */
    f32 controlZ[4];         /* 0x24 */
    f32 offsetX;             /* 0x34 */
    f32 offsetY;             /* 0x38 */
    f32 offsetZ;             /* 0x3C */
    f32 splineT;             /* 0x40 */
    f32 speed;               /* 0x44 */
    f32 unk48;               /* 0x48 */
    f32 unk4C;               /* 0x4C */
    f32 driftRangeZ;         /* 0x50 */
    f32 anchorX;             /* 0x54 */
    f32 anchorY;             /* 0x58 */
    f32 anchorZ;             /* 0x5C */
    s32 timer;               /* 0x60 */
    s16 randomAngle;         /* 0x64 */
    s16 randomPeriod;        /* 0x66 */
    s16 wanderRange;         /* 0x68 */
    u8 stateId;              /* 0x6A */
    u8 unk6B;                /* 0x6B */
    u8 segmentIndex;         /* 0x6C */
    u8 pad6D;                /* 0x6D */
    u8 lightSpawned;         /* 0x6E */
    u8 unk6F;                /* 0x6F */
    LanternFireFlyControlBits modeFlags;
    u8 pad71[3];             /* 0x71 */
} LanternFireFlyState;

typedef void (*LanternFireFlyReleaseFromLanternCallback)(GameObject* firefly);
typedef void (*LanternFireFlySetAnchorCallback)(GameObject* firefly, f32 x, f32 y, f32 z);

typedef struct LanternFireFlyRuntimeInterface {
    void* callbacksBeforeRelease[9];
    LanternFireFlyReleaseFromLanternCallback releaseFromLantern;
    LanternFireFlySetAnchorCallback setAnchor;
} LanternFireFlyRuntimeInterface;

STATIC_ASSERT(offsetof(LanternFireFlyPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(LanternFireFlyPlacement, wanderRange) == 0x18);
STATIC_ASSERT(offsetof(LanternFireFlyPlacement, stateId) == 0x19);
STATIC_ASSERT(offsetof(LanternFireFlyPlacement, timer) == 0x1A);
STATIC_ASSERT(offsetof(LanternFireFlyPlacement, driftRangeZ) == 0x1C);
STATIC_ASSERT(offsetof(LanternFireFlyPlacement, pad1E) == 0x1E);
STATIC_ASSERT(sizeof(LanternFireFlyPlacement) == LANTERN_FIREFLY_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(LanternFireFlyState, light) == 0x0);
STATIC_ASSERT(offsetof(LanternFireFlyState, controlX) == 0x4);
STATIC_ASSERT(offsetof(LanternFireFlyState, controlY) == 0x14);
STATIC_ASSERT(offsetof(LanternFireFlyState, controlZ) == 0x24);
STATIC_ASSERT(offsetof(LanternFireFlyState, offsetX) == 0x34);
STATIC_ASSERT(offsetof(LanternFireFlyState, offsetY) == 0x38);
STATIC_ASSERT(offsetof(LanternFireFlyState, offsetZ) == 0x3C);
STATIC_ASSERT(offsetof(LanternFireFlyState, splineT) == 0x40);
STATIC_ASSERT(offsetof(LanternFireFlyState, speed) == 0x44);
STATIC_ASSERT(offsetof(LanternFireFlyState, unk48) == 0x48);
STATIC_ASSERT(offsetof(LanternFireFlyState, unk4C) == 0x4C);
STATIC_ASSERT(offsetof(LanternFireFlyState, driftRangeZ) == 0x50);
STATIC_ASSERT(offsetof(LanternFireFlyState, anchorX) == 0x54);
STATIC_ASSERT(offsetof(LanternFireFlyState, anchorY) == 0x58);
STATIC_ASSERT(offsetof(LanternFireFlyState, anchorZ) == 0x5C);
STATIC_ASSERT(offsetof(LanternFireFlyState, timer) == 0x60);
STATIC_ASSERT(offsetof(LanternFireFlyState, randomAngle) == 0x64);
STATIC_ASSERT(offsetof(LanternFireFlyState, randomPeriod) == 0x66);
STATIC_ASSERT(offsetof(LanternFireFlyState, wanderRange) == 0x68);
STATIC_ASSERT(offsetof(LanternFireFlyState, stateId) == 0x6A);
STATIC_ASSERT(offsetof(LanternFireFlyState, unk6B) == 0x6B);
STATIC_ASSERT(offsetof(LanternFireFlyState, segmentIndex) == 0x6C);
STATIC_ASSERT(offsetof(LanternFireFlyState, pad6D) == 0x6D);
STATIC_ASSERT(offsetof(LanternFireFlyState, lightSpawned) == 0x6E);
STATIC_ASSERT(offsetof(LanternFireFlyState, unk6F) == 0x6F);
STATIC_ASSERT(offsetof(LanternFireFlyState, modeFlags) == 0x70);
STATIC_ASSERT(offsetof(LanternFireFlyState, pad71) == 0x71);
STATIC_ASSERT(sizeof(LanternFireFlyState) == LANTERN_FIREFLY_STATE_SIZE);

STATIC_ASSERT(offsetof(LanternFireFlyRuntimeInterface, callbacksBeforeRelease) == 0x0);
STATIC_ASSERT(offsetof(LanternFireFlyRuntimeInterface, releaseFromLantern) == 0x24);
STATIC_ASSERT(offsetof(LanternFireFlyRuntimeInterface, setAnchor) == 0x28);
STATIC_ASSERT(sizeof(LanternFireFlyRuntimeInterface) == 0x2C);

void LanternFireFly_setAnchor(GameObject* obj, f32 anchorX, f32 anchorY, f32 anchorZ);
void LanternFireFly_releaseFromLantern(GameObject* obj);
void LanternFireFly_setTargetPosition(GameObject* obj, f32* position);
int LanternFireFly_getExtraSize(void);
int LanternFireFly_getObjectTypeId(void);
void LanternFireFly_free(GameObject* obj, int flag);
void LanternFireFly_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void LanternFireFly_hitDetect(void);
void LanternFireFly_update(GameObject* obj);
void LanternFireFly_init(GameObject* obj, LanternFireFlyPlacement* placement);
void LanternFireFly_release(void);
void LanternFireFly_initialise(void);

extern ObjectDescriptor13WithPadding gLanternFireFlyObjDescriptor;

#endif /* DLLS_OBJECTS_268_LANTERNFIRE_H_ */
