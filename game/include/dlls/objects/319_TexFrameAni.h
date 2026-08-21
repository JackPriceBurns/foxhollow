#ifndef DLLS_OBJECTS_319_TEXFRAMEANI_H_
#define DLLS_OBJECTS_319_TEXFRAMEANI_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct TexFrameAnimatorPlacement {
    ObjPlacement base;
    s8 wrapFrame;
    s8 textureSlot;
    s16 endFrame;
    s16 speed;
    s16 completionGameBit;
    s16 triggerGameBit;
} TexFrameAnimatorPlacement;

STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, wrapFrame) == 0x18);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, textureSlot) == 0x19);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, endFrame) == 0x1A);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, speed) == 0x1C);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, completionGameBit) == 0x1E);
STATIC_ASSERT(offsetof(TexFrameAnimatorPlacement, triggerGameBit) == 0x20);

int TexFrameAnimator_getExtraSize(void);
int TexFrameAnimator_getObjectTypeId(void);
void TexFrameAnimator_free(void);
void TexFrameAnimator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible);
void TexFrameAnimator_hitDetect(void);
void TexFrameAnimator_update(GameObject* obj);
void TexFrameAnimator_init(GameObject* obj, const TexFrameAnimatorPlacement* placement);
void TexFrameAnimator_release(void);
void TexFrameAnimator_initialise(void);

extern ObjectDescriptor gTexFrameAnimatorObjDescriptor;

#endif /* DLLS_OBJECTS_319_TEXFRAMEANI_H_ */
