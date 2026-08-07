#ifndef DLLS_OBJECTS_259_CURVEFISH_H_
#define DLLS_OBJECTS_259_CURVEFISH_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/curve_walker.h"

#define CURVEFISH_PLACEMENT_SIZE 0x24
#define CURVEFISH_STATE_SIZE     0x120

/* Retail CurveFish placements have a fixed 0x0C-byte parameter tail. */
typedef struct CurveFishPlacement {
    ObjPlacement base;         /* 0x00 */
    u8 rootMotionScalePercent; /* 0x18 */
    u8 speedChange;            /* 0x19 */
    u8 pad1A[6];               /* 0x1A */
    u16 waitSeconds;           /* 0x20 */
    u8 targetYOffset;          /* 0x22 */
    u8 playerRadius;           /* 0x23 */
} CurveFishPlacement;

/* CurveFish_getExtraSize allocates the complete 0x120-byte state block. */
typedef struct CurveFishState {
    RomCurveWalker route; /* 0x000 */
    u8 mode;              /* 0x108: CurveFishMode */
    u8 pad109[3];         /* 0x109 */
    f32 moveTimer;        /* 0x10C */
    f32 maxSpeed;         /* 0x110 */
    f32 speed;            /* 0x114 */
    f32 animStep;         /* 0x118 */
    f32 modeTimer;        /* 0x11C */
} CurveFishState;

typedef union CurveFishCurveQueryKey {
    u32 type;
} CurveFishCurveQueryKey;

STATIC_ASSERT(offsetof(CurveFishPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(CurveFishPlacement, rootMotionScalePercent) == 0x18);
STATIC_ASSERT(offsetof(CurveFishPlacement, speedChange) == 0x19);
STATIC_ASSERT(offsetof(CurveFishPlacement, pad1A) == 0x1A);
STATIC_ASSERT(offsetof(CurveFishPlacement, waitSeconds) == 0x20);
STATIC_ASSERT(offsetof(CurveFishPlacement, targetYOffset) == 0x22);
STATIC_ASSERT(offsetof(CurveFishPlacement, playerRadius) == 0x23);
STATIC_ASSERT(sizeof(CurveFishPlacement) == CURVEFISH_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(CurveFishState, route) == 0x0);
STATIC_ASSERT(offsetof(CurveFishState, mode) == 0x108);
STATIC_ASSERT(offsetof(CurveFishState, pad109) == 0x109);
STATIC_ASSERT(offsetof(CurveFishState, moveTimer) == 0x10C);
STATIC_ASSERT(offsetof(CurveFishState, maxSpeed) == 0x110);
STATIC_ASSERT(offsetof(CurveFishState, speed) == 0x114);
STATIC_ASSERT(offsetof(CurveFishState, animStep) == 0x118);
STATIC_ASSERT(offsetof(CurveFishState, modeTimer) == 0x11C);
STATIC_ASSERT(sizeof(CurveFishState) == CURVEFISH_STATE_SIZE);

int CurveFish_getExtraSize(void);
void CurveFish_update(GameObject* obj);
void CurveFish_init(GameObject* obj, CurveFishPlacement* placement);

extern const CurveFishCurveQueryKey gCurveFishCurveQueryKey;
extern ObjectDescriptor gCurveFishObjDescriptor;

#endif /* DLLS_OBJECTS_259_CURVEFISH_H_ */
