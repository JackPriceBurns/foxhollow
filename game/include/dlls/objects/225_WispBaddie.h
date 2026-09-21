#ifndef DLLS_OBJECTS_225_WISPBADDIE_H_
#define DLLS_OBJECTS_225_WISPBADDIE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/curve_walker.h"

typedef struct WispBaddiePlacement {
    ObjPlacement base;         /* 0x00 */
    u8 pad18;                  /* 0x18 */
    s8 triggerDistanceScale;   /* 0x19 */
    s16 maxHitRadiusParameter; /* 0x1A */
} WispBaddiePlacement;

typedef struct WispBaddieState {
    RomCurveWalker* curve; /* 0x00 */
    GameObject* player;    /* 0x04 */
    f32 hitRadius;         /* 0x08 */
    f32 maxHitRadius;      /* 0x0C */
    f32 playerDistance;    /* 0x10 */
    f32 curveDistance;     /* 0x14 */
    f32 triggerDistance;   /* 0x18 */
    f32 cryTimer;          /* 0x1C */
    int particleId;        /* 0x20 */
    u8 flags;              /* 0x24 */
    u8 pad25;              /* 0x25 */
    s16 pathWavePhase;     /* 0x26 */
    s16 hoverWavePhase;    /* 0x28 */
    u8 pad2A[2];           /* 0x2A */
} WispBaddieState;

STATIC_ASSERT(offsetof(WispBaddiePlacement, base) == 0x0);
STATIC_ASSERT(offsetof(WispBaddiePlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(WispBaddiePlacement, triggerDistanceScale) == 0x19);
STATIC_ASSERT(offsetof(WispBaddiePlacement, maxHitRadiusParameter) == 0x1A);
STATIC_ASSERT(sizeof(WispBaddiePlacement) == 0x1C);

STATIC_ASSERT(offsetof(WispBaddieState, curve) == 0x0);
STATIC_ASSERT(offsetof(WispBaddieState, player) == 0x4);
STATIC_ASSERT(offsetof(WispBaddieState, hitRadius) == 0x8);
STATIC_ASSERT(offsetof(WispBaddieState, maxHitRadius) == 0xC);
STATIC_ASSERT(offsetof(WispBaddieState, playerDistance) == 0x10);
STATIC_ASSERT(offsetof(WispBaddieState, curveDistance) == 0x14);
STATIC_ASSERT(offsetof(WispBaddieState, triggerDistance) == 0x18);
STATIC_ASSERT(offsetof(WispBaddieState, cryTimer) == 0x1C);
STATIC_ASSERT(offsetof(WispBaddieState, particleId) == 0x20);
STATIC_ASSERT(offsetof(WispBaddieState, flags) == 0x24);
STATIC_ASSERT(offsetof(WispBaddieState, pad25) == 0x25);
STATIC_ASSERT(offsetof(WispBaddieState, pathWavePhase) == 0x26);
STATIC_ASSERT(offsetof(WispBaddieState, hoverWavePhase) == 0x28);
STATIC_ASSERT(offsetof(WispBaddieState, pad2A) == 0x2A);
STATIC_ASSERT(sizeof(WispBaddieState) == 0x2C);

void WispBaddie_updateMovement(GameObject* obj, WispBaddieState* state);
int WispBaddie_getExtraSize(void);
int WispBaddie_getObjectTypeId(void);
void WispBaddie_free(GameObject* obj);
void WispBaddie_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void WispBaddie_hitDetect(GameObject* obj);
void WispBaddie_update(GameObject* obj);
void WispBaddie_init(GameObject* obj, WispBaddiePlacement* placement, int skipAlloc);
void WispBaddie_release(void);
void WispBaddie_initialise(void);

extern ObjectDescriptor gWispBaddieObjDescriptor;
extern int gWispBaddieCurveInitData[2];
extern int gWispBaddieLastSegmentEnd;
extern u32 gGroundBaddieModelChainIds[4];

#endif /* DLLS_OBJECTS_225_WISPBADDIE_H_ */
