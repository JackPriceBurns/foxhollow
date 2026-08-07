#ifndef DLLS_OBJECTS_228_FLAMETHROWERSPE_H_
#define DLLS_OBJECTS_228_FLAMETHROWERSPE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define FLAMETHROWERSPE_HIT_PROFILE_COUNT 3

typedef struct FlameThrowerspePlacement {
    ObjPlacement base;   /* 0x00 */
    u8 pad18;            /* 0x18 */
    s8 hitVolumeProfile; /* 0x19 */
    s16 scaleParam;      /* 0x1A */
} FlameThrowerspePlacement;

typedef struct FlameThrowerspeState {
    u8 pad0[4];       /* 0x00 */
    f32 lifeTimer;    /* 0x04 */
    f32 sizeScale;    /* 0x08 */
    f32 sphereRadius; /* 0x0C */
    s32 phase;        /* 0x10 */
} FlameThrowerspeState;

typedef struct FlameThrowerspeHitProfile {
    u32 unk0;          /* 0x00 */
    u32 unk4;          /* 0x04 */
    int hitVolumeSlot; /* 0x08 */
} FlameThrowerspeHitProfile;

STATIC_ASSERT(offsetof(FlameThrowerspePlacement, base) == 0x0);
STATIC_ASSERT(offsetof(FlameThrowerspePlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(FlameThrowerspePlacement, hitVolumeProfile) == 0x19);
STATIC_ASSERT(offsetof(FlameThrowerspePlacement, scaleParam) == 0x1A);
STATIC_ASSERT(sizeof(FlameThrowerspePlacement) == 0x1C);

STATIC_ASSERT(offsetof(FlameThrowerspeState, pad0) == 0x0);
STATIC_ASSERT(offsetof(FlameThrowerspeState, lifeTimer) == 0x4);
STATIC_ASSERT(offsetof(FlameThrowerspeState, sizeScale) == 0x8);
STATIC_ASSERT(offsetof(FlameThrowerspeState, sphereRadius) == 0xC);
STATIC_ASSERT(offsetof(FlameThrowerspeState, phase) == 0x10);
STATIC_ASSERT(sizeof(FlameThrowerspeState) == 0x14);

STATIC_ASSERT(offsetof(FlameThrowerspeHitProfile, unk0) == 0x0);
STATIC_ASSERT(offsetof(FlameThrowerspeHitProfile, unk4) == 0x4);
STATIC_ASSERT(offsetof(FlameThrowerspeHitProfile, hitVolumeSlot) == 0x8);
STATIC_ASSERT(sizeof(FlameThrowerspeHitProfile) == 0xC);

void FlameThrowerspe_modelMtxFn(void);
void FlameThrowerspe_launch(GameObject* obj);
void FlameThrowerspe_setTransform(GameObject* obj, s16 rotY, s16 rotX, f32 x, f32 y, f32 z);
int FlameThrowerspe_getExtraSize(void);
int FlameThrowerspe_getObjectTypeId(void);
void FlameThrowerspe_free(GameObject* obj);
void FlameThrowerspe_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void FlameThrowerspe_hitDetect(GameObject* obj);
void FlameThrowerspe_update(GameObject* obj);
void FlameThrowerspe_init(GameObject* obj, FlameThrowerspePlacement* placement);
void FlameThrowerspe_release(void);
void FlameThrowerspe_initialise(void);

extern ObjectDescriptor13 gFlameThrowerspeObjDescriptor;
extern FlameThrowerspeHitProfile gFlameThrowerspeHitProfiles[FLAMETHROWERSPE_HIT_PROFILE_COUNT];
extern f32 gFlameThrowerspeScaleMultiplier;
extern int gFlameThrowerspeLifetimeFrames;
extern f32 gFlameThrowerspeSpeedMultiplier;
extern f32 gFlameThrowerspeRadiusMultiplier;

#endif /* DLLS_OBJECTS_228_FLAMETHROWERSPE_H_ */
