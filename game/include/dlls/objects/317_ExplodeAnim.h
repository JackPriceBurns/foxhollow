#ifndef DLLS_OBJECTS_317_EXPLODEANIM_H_
#define DLLS_OBJECTS_317_EXPLODEANIM_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum ExplodeAnimatorObjectGroup {
    EXPLODE_ANIMATOR_OBJECT_GROUP = 0x1A,
} ExplodeAnimatorObjectGroup;

typedef enum ExplodeAnimatorStateFlag {
    EXPLODE_ANIMATOR_STATE_FIRED = 0x01,
} ExplodeAnimatorStateFlag;

/* ExplodeAnimator_getExtraSize proves the complete 0x04-byte allocation. */
typedef struct ExplodeAnimatorState {
    u8 pad00[2]; /* 0x00 */
    u8 flags;    /* 0x02: ExplodeAnimatorStateFlag */
    u8 pad03;    /* 0x03 */
} ExplodeAnimatorState;

/*
 * The setup fields through triggerGameBit are evidenced by this TU. The
 * complete record extent after 0x35 is not yet proven.
 */
typedef struct ExplodeAnimatorPlacement {
    ObjPlacement base;  /* 0x00 */
    s16 posXMin;        /* 0x18 */
    s16 posYMin;        /* 0x1A */
    s16 posZMin;        /* 0x1C */
    s16 posXMax;        /* 0x1E */
    s16 posYMax;        /* 0x20 */
    s16 posZMax;        /* 0x22 */
    s16 effectId;       /* 0x24 */
    u8 pad26[2];        /* 0x26 */
    s16 velXMax;        /* 0x28 */
    s16 velYMax;        /* 0x2A */
    u8 particleCount;   /* 0x2C */
    u8 pad2D;           /* 0x2D */
    s16 velXMin;        /* 0x2E */
    s16 velYMin;        /* 0x30 */
    s16 resultGameBit;  /* 0x32 */
    s16 triggerGameBit; /* 0x34 */
} ExplodeAnimatorPlacement;

STATIC_ASSERT(offsetof(ExplodeAnimatorState, pad00) == 0x00);
STATIC_ASSERT(offsetof(ExplodeAnimatorState, flags) == 0x02);
STATIC_ASSERT(offsetof(ExplodeAnimatorState, pad03) == 0x03);
STATIC_ASSERT(sizeof(ExplodeAnimatorState) == 0x04);

STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, posXMin) == 0x18);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, posYMin) == 0x1A);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, posZMin) == 0x1C);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, posXMax) == 0x1E);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, posYMax) == 0x20);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, posZMax) == 0x22);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, effectId) == 0x24);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, pad26) == 0x26);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, velXMax) == 0x28);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, velYMax) == 0x2A);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, particleCount) == 0x2C);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, pad2D) == 0x2D);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, velXMin) == 0x2E);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, velYMin) == 0x30);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, resultGameBit) == 0x32);
STATIC_ASSERT(offsetof(ExplodeAnimatorPlacement, triggerGameBit) == 0x34);

int ExplodeAnimator_getExtraSize(void);
int ExplodeAnimator_getObjectTypeId(void);
void ExplodeAnimator_free(GameObject* obj);
void ExplodeAnimator_render(void);
void ExplodeAnimator_hitDetect(void);
void ExplodeAnimator_update(GameObject* obj);
void ExplodeAnimator_init(GameObject* obj, ExplodeAnimatorPlacement* placement);
void ExplodeAnimator_release(void);
void ExplodeAnimator_initialise(void);

extern ObjectDescriptor gExplodeAnimatorObjDescriptor;

#endif /* DLLS_OBJECTS_317_EXPLODEANIM_H_ */
