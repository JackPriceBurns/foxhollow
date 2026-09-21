#ifndef DLLS_OBJECTS_280_DUSTER_H_
#define DLLS_OBJECTS_280_DUSTER_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define DUSTER_OBJECT_STATE_SIZE 0x20

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct DusterPlacement {
    ObjPlacement base; /* 0x00 */
    u8 pad18[0xC];     /* 0x18 */
    s16 activeGameBit; /* 0x24 */
} DusterPlacement;

typedef struct DusterObjectStateFlags {
    u8 floorCached : 1;
    u8 pad : 7;
} DusterObjectStateFlags;

typedef struct DusterObjectState {
    f32 moveStepScale;            /* 0x00 */
    f32 floorY;                   /* 0x04 */
    s16 settleTimer;              /* 0x08 */
    s16 hitReactTimer;            /* 0x0A */
    s16 completeGameBit;          /* 0x0C */
    s16 activeGameBit;            /* 0x0E */
    s16 heldObjectId;             /* 0x10 */
    u8 pad12[6];                  /* 0x12 */
    u8 driftDirection;            /* 0x18 */
    u8 hitReactionActive;         /* 0x19 */
    u8 priorityHit;               /* 0x1A */
    u8 active;                    /* 0x1B */
    u8 complete;                  /* 0x1C */
    u8 useLaunchVelocity;         /* 0x1D */
    DusterObjectStateFlags flags; /* 0x1E */
    u8 pad1F;                     /* 0x1F */
} DusterObjectState;

STATIC_ASSERT(offsetof(DusterPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(DusterPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(DusterPlacement, activeGameBit) == 0x24);

STATIC_ASSERT(sizeof(DusterObjectStateFlags) == 0x1);
STATIC_ASSERT(offsetof(DusterObjectState, moveStepScale) == 0x0);
STATIC_ASSERT(offsetof(DusterObjectState, floorY) == 0x4);
STATIC_ASSERT(offsetof(DusterObjectState, settleTimer) == 0x8);
STATIC_ASSERT(offsetof(DusterObjectState, hitReactTimer) == 0xA);
STATIC_ASSERT(offsetof(DusterObjectState, completeGameBit) == 0xC);
STATIC_ASSERT(offsetof(DusterObjectState, activeGameBit) == 0xE);
STATIC_ASSERT(offsetof(DusterObjectState, heldObjectId) == 0x10);
STATIC_ASSERT(offsetof(DusterObjectState, pad12) == 0x12);
STATIC_ASSERT(offsetof(DusterObjectState, driftDirection) == 0x18);
STATIC_ASSERT(offsetof(DusterObjectState, hitReactionActive) == 0x19);
STATIC_ASSERT(offsetof(DusterObjectState, priorityHit) == 0x1A);
STATIC_ASSERT(offsetof(DusterObjectState, active) == 0x1B);
STATIC_ASSERT(offsetof(DusterObjectState, complete) == 0x1C);
STATIC_ASSERT(offsetof(DusterObjectState, useLaunchVelocity) == 0x1D);
STATIC_ASSERT(offsetof(DusterObjectState, flags) == 0x1E);
STATIC_ASSERT(offsetof(DusterObjectState, pad1F) == 0x1F);
STATIC_ASSERT(sizeof(DusterObjectState) == DUSTER_OBJECT_STATE_SIZE);

int duster_SeqFn(GameObject* obj);
int duster_getExtraSize(void);
void duster_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void duster_hitDetect(GameObject* obj);
void duster_update(GameObject* obj);
void duster_init(GameObject* obj, DusterPlacement* placement);

extern ObjectDescriptor gDusterObjDescriptor;

#endif /* DLLS_OBJECTS_280_DUSTER_H_ */
