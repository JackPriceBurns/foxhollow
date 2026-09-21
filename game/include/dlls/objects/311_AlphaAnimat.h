#ifndef DLLS_OBJECTS_311_ALPHAANIMAT_H_
#define DLLS_OBJECTS_311_ALPHAANIMAT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum AlphaAnimatorMode {
    ALPHA_ANIMATOR_MODE_ONESHOT = 0,
    ALPHA_ANIMATOR_MODE_PINGPONG = 1,
    ALPHA_ANIMATOR_MODE_GATED = 2,
    ALPHA_ANIMATOR_MODE_TIMED = 3,
} AlphaAnimatorMode;

typedef struct AlphaAnimatorPlacement {
    ObjPlacement base;     /* 0x00 */
    s16 triggerGameBit;    /* 0x18 */
    s16 completionGameBit; /* 0x1A */
    u8 startAlpha;         /* 0x1C */
    u8 targetAlpha;        /* 0x1D */
    u8 active;             /* 0x1E */
    s8 rate;               /* 0x1F */
    u8 modeFlags;          /* 0x20: mode in low two bits */
    u8 pad21;              /* 0x21 */
    u16 fadeLimit;         /* 0x22 */
    u16 sfxId;             /* 0x24 */
    u8 pad26[2];           /* 0x26 */
} AlphaAnimatorPlacement;

/* AlphaAnimator_getExtraSize proves the complete 0x1C-byte allocation. */
typedef struct AlphaAnimatorState {
    int vertexCount;         /* 0x00 */
    f32 fadeProgress;        /* 0x04 */
    f32 fadeOffset;          /* 0x08 */
    f32 fadeLimit;           /* 0x0C */
    void* vertexAlphaBuffer; /* 0x10: timed-mode per-vertex alpha buffer */
    s16 alphaLevel;          /* 0x14 */
    u8 active;               /* 0x16 */
    s8 gameBitValue;         /* 0x17 */
    s8 completedCycles;      /* 0x18 */
    s8 previousGameBitValue; /* 0x19 */
    u8 pad1A[2];             /* 0x1A */
} AlphaAnimatorState;

STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, triggerGameBit) == 0x18);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, completionGameBit) == 0x1A);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, startAlpha) == 0x1C);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, targetAlpha) == 0x1D);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, active) == 0x1E);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, rate) == 0x1F);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, modeFlags) == 0x20);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, pad21) == 0x21);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, fadeLimit) == 0x22);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, sfxId) == 0x24);
STATIC_ASSERT(offsetof(AlphaAnimatorPlacement, pad26) == 0x26);
STATIC_ASSERT(sizeof(AlphaAnimatorPlacement) == 0x28);

STATIC_ASSERT(offsetof(AlphaAnimatorState, vertexCount) == 0x00);
STATIC_ASSERT(offsetof(AlphaAnimatorState, fadeProgress) == 0x04);
STATIC_ASSERT(offsetof(AlphaAnimatorState, fadeOffset) == 0x08);
STATIC_ASSERT(offsetof(AlphaAnimatorState, fadeLimit) == 0x0C);
STATIC_ASSERT(offsetof(AlphaAnimatorState, vertexAlphaBuffer) == 0x10);
STATIC_ASSERT(offsetof(AlphaAnimatorState, alphaLevel) == 0x14);
STATIC_ASSERT(offsetof(AlphaAnimatorState, active) == 0x16);
STATIC_ASSERT(offsetof(AlphaAnimatorState, gameBitValue) == 0x17);
STATIC_ASSERT(offsetof(AlphaAnimatorState, completedCycles) == 0x18);
STATIC_ASSERT(offsetof(AlphaAnimatorState, previousGameBitValue) == 0x19);
STATIC_ASSERT(offsetof(AlphaAnimatorState, pad1A) == 0x1A);
STATIC_ASSERT(sizeof(AlphaAnimatorState) == 0x1C);

int AlphaAnimator_getExtraSize(void);
int AlphaAnimator_getObjectTypeId(void);
void AlphaAnimator_free(GameObject* obj);
void AlphaAnimator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void AlphaAnimator_hitDetect(void);
void AlphaAnimator_update(GameObject* obj);
void AlphaAnimator_init(GameObject* obj);
void AlphaAnimator_release(void);
void AlphaAnimator_initialise(void);

extern ObjectDescriptor gAlphaAnimatorObjDescriptor;

#endif /* DLLS_OBJECTS_311_ALPHAANIMAT_H_ */
