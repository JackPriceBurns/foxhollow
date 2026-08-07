#ifndef DLLS_OBJECTS_240_WARPPOINT_H_
#define DLLS_OBJECTS_240_WARPPOINT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

struct ObjSeqState;

typedef enum WarpPointMode {
    WARPPOINT_MODE_PROXIMITY = 0,
    WARPPOINT_MODE_HINT_TIMER = 1,
    WARPPOINT_MODE_GATED_WARP = 2,
    WARPPOINT_MODE_ONESHOT_SEQ = 3,
    WARPPOINT_MODE_GATED_WARP2 = 4
} WarpPointMode;

typedef struct WarpPointPlacement {
    ObjPlacement base;     /* 0x00 */
    u8 yawByte;            /* 0x18 */
    s8 arrivalWarpId;      /* 0x19 */
    s8 warpId;             /* 0x1A */
    s8 sequenceId;         /* 0x1B */
    s8 enabled;            /* 0x1C */
    s8 mode;               /* 0x1D: WarpPointMode */
    s8 radius;             /* 0x1E */
    u8 savePointArmed;     /* 0x1F */
    s16 gameBit;           /* 0x20 */
    u8 pad22[0x28 - 0x22]; /* 0x22 */
} WarpPointPlacement;

typedef struct WarpPointState {
    s16 warpDelay;        /* 0x00 */
    s16 gameBit;          /* 0x02 */
    s16 sequenceId;       /* 0x04 */
    u8 pad06[2];          /* 0x06 */
    f32 triggerRadius;    /* 0x08 */
    u8 sequenceTriggered; /* 0x0C */
    u8 savePointRecorded; /* 0x0D */
    u8 pad0E[2];          /* 0x0E */
} WarpPointState;

STATIC_ASSERT(offsetof(WarpPointPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(WarpPointPlacement, yawByte) == 0x18);
STATIC_ASSERT(offsetof(WarpPointPlacement, arrivalWarpId) == 0x19);
STATIC_ASSERT(offsetof(WarpPointPlacement, warpId) == 0x1A);
STATIC_ASSERT(offsetof(WarpPointPlacement, sequenceId) == 0x1B);
STATIC_ASSERT(offsetof(WarpPointPlacement, enabled) == 0x1C);
STATIC_ASSERT(offsetof(WarpPointPlacement, mode) == 0x1D);
STATIC_ASSERT(offsetof(WarpPointPlacement, radius) == 0x1E);
STATIC_ASSERT(offsetof(WarpPointPlacement, savePointArmed) == 0x1F);
STATIC_ASSERT(offsetof(WarpPointPlacement, gameBit) == 0x20);
STATIC_ASSERT(offsetof(WarpPointPlacement, pad22) == 0x22);
STATIC_ASSERT(sizeof(WarpPointPlacement) == 0x28);

STATIC_ASSERT(offsetof(WarpPointState, warpDelay) == 0x0);
STATIC_ASSERT(offsetof(WarpPointState, gameBit) == 0x2);
STATIC_ASSERT(offsetof(WarpPointState, sequenceId) == 0x4);
STATIC_ASSERT(offsetof(WarpPointState, pad06) == 0x6);
STATIC_ASSERT(offsetof(WarpPointState, triggerRadius) == 0x8);
STATIC_ASSERT(offsetof(WarpPointState, sequenceTriggered) == 0xC);
STATIC_ASSERT(offsetof(WarpPointState, savePointRecorded) == 0xD);
STATIC_ASSERT(offsetof(WarpPointState, pad0E) == 0xE);
STATIC_ASSERT(sizeof(WarpPointState) == 0x10);

int WarpPoint_animEventCallback(GameObject* obj, int unused, struct ObjSeqState* animUpdate);
int WarpPoint_getExtraSize(void);
int WarpPoint_getObjectTypeId(void);
void WarpPoint_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void WarpPoint_update(GameObject* obj);
void WarpPoint_init(GameObject* obj, WarpPointPlacement* placement);

extern ObjectDescriptor gWarpPointObjDescriptor;

#endif /* DLLS_OBJECTS_240_WARPPOINT_H_ */
