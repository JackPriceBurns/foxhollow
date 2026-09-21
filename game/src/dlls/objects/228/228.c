/*
 * FlameThrowerspe object family (DLL slot 228 / 0xE4).
 *
 * Drives the pooled flame projectiles used by FlameThrowerspe, FlameBall,
 * and BossDarkorF object variants.
 */
#include "dlls/objects/228_FlameThrowerspe.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "sys/objects.h"
#include "main/dll/firepipe_effect.h"
#include "main/maketex_timer.h"
#include "main/objhits.h"
#include "main/vecmath.h"

typedef enum FlameThrowerspePhase {
    FLAMETHROWERSPE_PHASE_LAUNCH = 1,
    FLAMETHROWERSPE_PHASE_ACTIVE = 2,
} FlameThrowerspePhase;

typedef struct FlameThrowerspeState {
    u8 unk0[4];
    f32 lifeTimer;
    f32 sizeScale;
    f32 sphereRadius;
    FlameThrowerspePhase phase;
} FlameThrowerspeState;

typedef struct FlameThrowerspeHitProfile {
    u32 unk0;
    u32 unk4;
    int hitVolumeSlot;
} FlameThrowerspeHitProfile;

static const FlameThrowerspeHitProfile sFlameThrowerspeHitProfiles[] = {
    {0x4F, 0xFFC40000, 0x1F},
    {0x4F, 0x00C4FF00, 0x5},
    {0x4F, 0x00C4FF00, 0x1E},
};

void FlameThrowerspe_modelMtxFn(void) {
}

void FlameThrowerspe_launch(GameObject* obj) {
    FlameThrowerspeState* state = obj->extra;
    state->phase = FLAMETHROWERSPE_PHASE_LAUNCH;
}

void FlameThrowerspe_setTransform(GameObject* obj, s16 rotY, s16 rotX, f32 x, f32 y, f32 z) {
    obj->anim.localPosX = x;
    obj->anim.localPosY = y;
    obj->anim.localPosZ = z;
    obj->anim.rotY = rotY;
    obj->anim.rotX = rotX;
}

int FlameThrowerspe_getExtraSize(void) {
    return sizeof(FlameThrowerspeState);
}

int FlameThrowerspe_getObjectTypeId(void) {
    return 0;
}

void FlameThrowerspe_free(GameObject* obj) {
    (void)obj;
}

void FlameThrowerspe_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    (void)visible;
    objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
}

void FlameThrowerspe_hitDetect(GameObject* obj) {
    (void)obj;
}

void FlameThrowerspe_update(GameObject* obj) {
    FlameThrowerspeState* state = obj->extra;
    FlameThrowerspePlacement* placement = (FlameThrowerspePlacement*)obj->anim.placementData;

    switch (state->phase) {
    case FLAMETHROWERSPE_PHASE_LAUNCH:
        obj->anim.velocityX = 0.0f;
        obj->anim.velocityZ =
            0.1f * (state->sizeScale * (0.12f * (f32)randomGetRange(100, 150)));
        vecRotateZXY(&obj->anim.rotX, &obj->anim.velocityX);
        state->sphereRadius = 8.0f * state->sizeScale;
        s16toFloat(&state->lifeTimer, 35);
        state->phase = FLAMETHROWERSPE_PHASE_ACTIVE;
        break;
    case FLAMETHROWERSPE_PHASE_ACTIVE:
        if (timerCountDown(&state->lifeTimer) != 0) {
            ObjHits_DisableObject(obj);
            firepipe_releaseEffectObject(obj);
            return;
        }
        ObjHits_EnableObject(obj);
        ObjHits_SetHitVolumeSlot(&obj->anim,
                                 sFlameThrowerspeHitProfiles[placement->hitVolumeProfile].hitVolumeSlot, 1, 0);
        f32 dt = (f32)(f64)timeDelta;
        objMove(obj, obj->anim.velocityX * dt, obj->anim.velocityY * dt, obj->anim.velocityZ * dt);
        ObjHitbox_SetSphereRadius(&obj->anim,
                                  (int)(state->sphereRadius * ((35.0f - state->lifeTimer) / 35)));
        break;
    }
}

void FlameThrowerspe_init(GameObject* obj, FlameThrowerspePlacement* placement) {
    FlameThrowerspeState* state = obj->extra;

    storeZeroToFloatParam(&state->lifeTimer);
    f32 scale = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->scaleParam) / 10.0f;
    state->sizeScale = scale * 2.0f;
    obj->anim.velocityY = 0.0f;
    obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
    state->phase = FLAMETHROWERSPE_PHASE_LAUNCH;
    ObjHits_DisableObject(obj);
}

void FlameThrowerspe_release(void) {
}

void FlameThrowerspe_initialise(void) {
}

OBJECT_INIT_ADAPTER(gFlameThrowerspeObjDescriptorInitAdapter, FlameThrowerspe_init, obj, placement)
OBJECT_FREE_ADAPTER(gFlameThrowerspeObjDescriptorFreeAdapter, FlameThrowerspe_free, obj)
OBJECT_TYPE_ID_ADAPTER(gFlameThrowerspeObjDescriptorTypeIdAdapter, FlameThrowerspe_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gFlameThrowerspeObjDescriptorExtraSizeAdapter, FlameThrowerspe_getExtraSize)

typedef struct FlameThrowerspeObjDescriptorTypeInterface {
    OBJECT_INTERFACE_FIELDS;
    __typeof__(FlameThrowerspe_setTransform)* FlameThrowerspe_setTransform;
    __typeof__(FlameThrowerspe_launch)* FlameThrowerspe_launch;
    __typeof__(FlameThrowerspe_modelMtxFn)* FlameThrowerspe_modelMtxFn;
} FlameThrowerspeObjDescriptorTypeInterface;

struct FlameThrowerspeObjDescriptorType {
    ObjectDescriptorHeader header;
    FlameThrowerspeObjDescriptorTypeInterface interface;
};

RESOURCE_ACQUIRE_ADAPTER(gFlameThrowerspeObjDescriptorAcquire, FlameThrowerspe_initialise)

struct FlameThrowerspeObjDescriptorType gFlameThrowerspeObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_13_SLOTS,
        },
        gFlameThrowerspeObjDescriptorAcquire,
        FlameThrowerspe_release,
    },
    {
        0,
        gFlameThrowerspeObjDescriptorInitAdapter,
        FlameThrowerspe_update,
        FlameThrowerspe_hitDetect,
        FlameThrowerspe_render,
        gFlameThrowerspeObjDescriptorFreeAdapter,
        gFlameThrowerspeObjDescriptorTypeIdAdapter,
        gFlameThrowerspeObjDescriptorExtraSizeAdapter,
        FlameThrowerspe_setTransform,
        FlameThrowerspe_launch,
        FlameThrowerspe_modelMtxFn,
    },
};
