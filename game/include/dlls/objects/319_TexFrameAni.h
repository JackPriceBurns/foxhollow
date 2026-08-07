#ifndef DLLS_OBJECTS_319_TEXFRAMEANI_H_
#define DLLS_OBJECTS_319_TEXFRAMEANI_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * The setup fields through triggerGameBit are evidenced by this TU. The
 * complete record extent after 0x21 is not yet proven.
 */
typedef struct TexFrameAnimatorPlacement {
    ObjPlacement base;     /* 0x00 */
    s8 wrapFrame;          /* 0x18 */
    s8 textureSlot;        /* 0x19 */
    s16 endFrame;          /* 0x1A */
    s16 speed;             /* 0x1C */
    s16 completionGameBit; /* 0x1E */
    s16 triggerGameBit;    /* 0x20 */
} TexFrameAnimatorPlacement;

/* TexFrameAnimator_getExtraSize proves the complete 0x18-byte allocation. */
typedef struct TexFrameAnimatorState {
    int textureSlot; /* 0x00 */
    u8 speed;        /* 0x04 */
    u8 pad05[3];     /* 0x05 */
    int endFrame;    /* 0x08: 8.8 fixed-point */
    int wrapFrame;   /* 0x0C: 8.8 fixed-point */
    int frame;       /* 0x10: 8.8 fixed-point */
    union {
        u8 flags; /* 0x14 */
        struct {
            u8 unusedHigh : 1;
            u8 done : 1;
            u8 active : 1;
            u8 unusedLow : 5;
        };
    };
    u8 pad15[3]; /* 0x15 */
} TexFrameAnimatorState;

STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, wrapFrame) == 0x18);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, textureSlot) == 0x19);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, endFrame) == 0x1A);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, speed) == 0x1C);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, completionGameBit) == 0x1E);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, triggerGameBit) == 0x20);

STATIC_ASSERT(offsetof(TexFrameAnimatorState, textureSlot) == 0x00);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, speed) == 0x04);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, pad05) == 0x05);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, endFrame) == 0x08);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, wrapFrame) == 0x0C);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, frame) == 0x10);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, flags) == 0x14);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, pad15) == 0x15);
STATIC_ASSERT(sizeof(TexFrameAnimatorState) == 0x18);

int TexFrameAnimator_getExtraSize(void);
int TexFrameAnimator_getObjectTypeId(void);
void TexFrameAnimator_free(void);
void TexFrameAnimator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible);
void TexFrameAnimator_hitDetect(void);
void TexFrameAnimator_update(GameObject* obj);
void TexFrameAnimator_init(GameObject* obj, TexFrameAnimatorPlacement* placement);
void TexFrameAnimator_release(void);
void TexFrameAnimator_initialise(void);

extern ObjectDescriptor gTexFrameAnimatorObjDescriptor;

#endif /* DLLS_OBJECTS_319_TEXFRAMEANI_H_ */
