#ifndef DLLS_OBJECTS_224_SWARMBADDIE_H_
#define DLLS_OBJECTS_224_SWARMBADDIE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/curve_walker.h"

typedef struct SwarmBaddiePlacement {
    ObjPlacement base;   /* 0x00 */
    u8 pad18;            /* 0x18 */
    s8 chaseRadiusScale; /* 0x19 */
    s16 curveStepParam;  /* 0x1A */
} SwarmBaddiePlacement;

typedef struct SwarmBaddieState {
    RomCurveWalker* curve; /* 0x00 */
    GameObject* player;    /* 0x04 */
    f32 curveStep;         /* 0x08 */
    f32 playerDistance;    /* 0x0C */
    f32 pathDistance;      /* 0x10 */
    f32 chaseRadius;       /* 0x14 */
    f32 hitVolumeEnvelope; /* 0x18 */
    u8 flags;              /* 0x1C */
    u8 pad1D;              /* 0x1D */
    s16 yawWavePhase;      /* 0x1E */
    s16 rollWavePhase;     /* 0x20 */
    u8 pad22[2];           /* 0x22 */
} SwarmBaddieState;

STATIC_ASSERT(offsetof(SwarmBaddiePlacement, base) == 0x0);
STATIC_ASSERT(offsetof(SwarmBaddiePlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(SwarmBaddiePlacement, chaseRadiusScale) == 0x19);
STATIC_ASSERT(offsetof(SwarmBaddiePlacement, curveStepParam) == 0x1A);
STATIC_ASSERT(sizeof(SwarmBaddiePlacement) == 0x1C);

STATIC_ASSERT(offsetof(SwarmBaddieState, curve) == 0x0);
STATIC_ASSERT(offsetof(SwarmBaddieState, player) == 0x4);
STATIC_ASSERT(offsetof(SwarmBaddieState, curveStep) == 0x8);
STATIC_ASSERT(offsetof(SwarmBaddieState, playerDistance) == 0xC);
STATIC_ASSERT(offsetof(SwarmBaddieState, pathDistance) == 0x10);
STATIC_ASSERT(offsetof(SwarmBaddieState, chaseRadius) == 0x14);
STATIC_ASSERT(offsetof(SwarmBaddieState, hitVolumeEnvelope) == 0x18);
STATIC_ASSERT(offsetof(SwarmBaddieState, flags) == 0x1C);
STATIC_ASSERT(offsetof(SwarmBaddieState, pad1D) == 0x1D);
STATIC_ASSERT(offsetof(SwarmBaddieState, yawWavePhase) == 0x1E);
STATIC_ASSERT(offsetof(SwarmBaddieState, rollWavePhase) == 0x20);
STATIC_ASSERT(offsetof(SwarmBaddieState, pad22) == 0x22);
STATIC_ASSERT(sizeof(SwarmBaddieState) == 0x24);

void SwarmBaddie_updateMovement(GameObject* obj, SwarmBaddieState* state);
int SwarmBaddie_getExtraSize(void);
int SwarmBaddie_getObjectTypeId(void);
void SwarmBaddie_free(GameObject* obj);
void SwarmBaddie_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void SwarmBaddie_hitDetect(GameObject* obj);
void SwarmBaddie_update(GameObject* obj);
void SwarmBaddie_init(GameObject* obj, SwarmBaddiePlacement* placement, int skipAlloc);
void SwarmBaddie_release(void);
void SwarmBaddie_initialise(void);

extern ObjectDescriptor gSwarmBaddieObjDescriptor;
extern int gSwarmBaddieCurveInitData[2];
extern int gSwarmBaddieLastCurvePoint;

#endif /* DLLS_OBJECTS_224_SWARMBADDIE_H_ */
