#ifndef DLLS_OBJECTS_310_WAVEANIMATO_H_
#define DLLS_OBJECTS_310_WAVEANIMATO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum WaveAnimatorStateFlag {
    WAVE_ANIMATOR_STATE_SCALE_PENDING = 0x01,
    WAVE_ANIMATOR_STATE_FUNC_0B_LATCH = 0x02,
    WAVE_ANIMATOR_STATE_MODEL_MTX_PENDING = 0x04,
} WaveAnimatorStateFlag;

typedef struct WaveAnimatorPlacement {
    ObjPlacement base; /* 0x00 */
    s16 originX;       /* 0x18 */
    s16 originY;       /* 0x1A */
    s8 spanX;          /* 0x1C */
    s8 spanY;          /* 0x1D */
    s8 ampX;           /* 0x1E */
    s8 ampY;           /* 0x1F */
    s8 sinkDepthScale; /* 0x20 */
    s8 period;         /* 0x21 */
    s8 gridN;          /* 0x22 */
    u8 pad23[2];       /* 0x23 */
    u8 pad25[3];       /* 0x25 */
} WaveAnimatorPlacement;

/* WaveAnimator_getExtraSize proves the complete 0x3C-byte allocation. */
typedef struct WaveAnimatorState {
    int originX;        /* 0x00 */
    int originY;        /* 0x04 */
    int spanX;          /* 0x08 */
    int spanY;          /* 0x0C */
    f32 ampX;           /* 0x10 */
    f32 ampY;           /* 0x14 */
    int sinkDepthScale; /* 0x18 */
    int period;         /* 0x1C */
    int gridN;          /* 0x20 */
    f32 minHeight;      /* 0x24 */
    f32 maxHeight;      /* 0x28 */
    f32 scaleA;         /* 0x2C */
    f32 scaleB;         /* 0x30 */
    u8 flags;           /* 0x34: WaveAnimatorStateFlag */
    u8 pad35;           /* 0x35 */
    u8 modelMtxArg0;    /* 0x36 */
    u8 modelMtxArg1;    /* 0x37 */
    u8 modelMtxArg2;    /* 0x38 */
    u8 pad39[3];        /* 0x39 */
} WaveAnimatorState;

STATIC_ASSERT(offsetof(WaveAnimatorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, originX) == 0x18);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, originY) == 0x1A);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, spanX) == 0x1C);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, spanY) == 0x1D);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, ampX) == 0x1E);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, ampY) == 0x1F);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, sinkDepthScale) == 0x20);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, period) == 0x21);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, gridN) == 0x22);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, pad23) == 0x23);
STATIC_ASSERT(offsetof(WaveAnimatorPlacement, pad25) == 0x25);
STATIC_ASSERT(sizeof(WaveAnimatorPlacement) == 0x28);

STATIC_ASSERT(offsetof(WaveAnimatorState, originX) == 0x00);
STATIC_ASSERT(offsetof(WaveAnimatorState, originY) == 0x04);
STATIC_ASSERT(offsetof(WaveAnimatorState, spanX) == 0x08);
STATIC_ASSERT(offsetof(WaveAnimatorState, spanY) == 0x0C);
STATIC_ASSERT(offsetof(WaveAnimatorState, ampX) == 0x10);
STATIC_ASSERT(offsetof(WaveAnimatorState, ampY) == 0x14);
STATIC_ASSERT(offsetof(WaveAnimatorState, sinkDepthScale) == 0x18);
STATIC_ASSERT(offsetof(WaveAnimatorState, period) == 0x1C);
STATIC_ASSERT(offsetof(WaveAnimatorState, gridN) == 0x20);
STATIC_ASSERT(offsetof(WaveAnimatorState, minHeight) == 0x24);
STATIC_ASSERT(offsetof(WaveAnimatorState, maxHeight) == 0x28);
STATIC_ASSERT(offsetof(WaveAnimatorState, scaleA) == 0x2C);
STATIC_ASSERT(offsetof(WaveAnimatorState, scaleB) == 0x30);
STATIC_ASSERT(offsetof(WaveAnimatorState, flags) == 0x34);
STATIC_ASSERT(offsetof(WaveAnimatorState, pad35) == 0x35);
STATIC_ASSERT(offsetof(WaveAnimatorState, modelMtxArg0) == 0x36);
STATIC_ASSERT(offsetof(WaveAnimatorState, modelMtxArg1) == 0x37);
STATIC_ASSERT(offsetof(WaveAnimatorState, modelMtxArg2) == 0x38);
STATIC_ASSERT(offsetof(WaveAnimatorState, pad39) == 0x39);
STATIC_ASSERT(sizeof(WaveAnimatorState) == 0x3C);

void WaveAnimator_modelMtxFn(GameObject* obj, int modelMtxArg0, int modelMtxArg1, int modelMtxArg2);
void WaveAnimator_func0B(GameObject* obj);
void WaveAnimator_setScale(GameObject* obj, f32 scale);
int WaveAnimator_getExtraSize(void);
int WaveAnimator_getObjectTypeId(void);
void WaveAnimator_free(GameObject* obj);
void WaveAnimator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void WaveAnimator_hitDetect(GameObject* obj);
void WaveAnimator_update(void);
void WaveAnimator_init(GameObject* obj, WaveAnimatorPlacement* placement);
void WaveAnimator_release(void);
void WaveAnimator_initialise(void);

extern ObjectDescriptor14 gWaveAnimatorObjDescriptor;

#endif /* DLLS_OBJECTS_310_WAVEANIMATO_H_ */
