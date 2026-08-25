/*
 * ARWSpeedStr (DLL 674) - the streaking "speed line" particles that fly
 * past the camera during the on-rails Arwing sections, conveying forward
 * speed. On first update each streak picks a random spread offset in
 * camera space, transforms it through the inverse view matrix into world
 * space and biases it by the player's map offset. It then drifts along its
 * own velocity, fading its alpha up to a cap over its life timer before
 * freeing itself when the timer runs out.
 */
#include "dolphin/mtx.h"
#include "main/frame_timing.h"
#include "main/shader.h"
#include "main/dll/ARW/dll_02A2_arwspeedstr.h"
#include "main/object_render.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"
#include "main/camera.h"
#include "main/vecmath.h"

static f32 ARWSpeedStr_randomSpread(f32 spread) {
    return randomGetRange(-spread, spread);
}

int ARWSpeedStr_getExtraSize(void) {
    return sizeof(ARWSpeedStrState);
}

int ARWSpeedStr_getObjectTypeId(void) {
    return 0;
}

void ARWSpeedStr_free(void) {
}

void ARWSpeedStr_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible) {
    objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
}

void ARWSpeedStr_hitDetect(void) {
}

void ARWSpeedStr_update(GameObject* obj) {
    ARWSpeedStrState* state = obj->extra;
    if (state->flags == 0) {
        Vec cameraOffset;
        cameraOffset.x = ARWSpeedStr_randomSpread(state->spreadX);
        cameraOffset.y = ARWSpeedStr_randomSpread(state->spreadY);
        cameraOffset.z = state->viewZ;
        PSMTXMultVec((MtxP)Camera_GetInverseViewMatrix(), &cameraOffset, (Vec*)&obj->anim.localPosX);
        obj->anim.localPosX += playerMapOffsetX;
        obj->anim.localPosZ += playerMapOffsetZ;
        state->flags |= ARWSPEEDSTR_FLAG_POSITION_INITIALIZED;
        state->flags &= 0xff;
        state->alpha = 0.0f;
    }

    if (state->lifeTimer <= 0.0f) {
        return;
    }

    state->lifeTimer -= timeDelta;
    if (state->lifeTimer <= 0.0f) {
        state->lifeTimer = 0.0f;
        Obj_FreeObject(obj);
        return;
    }

    objMove(obj, 0.0f, 0.0f, state->speed * timeDelta);
    state->alpha += 2.0f * timeDelta;
    if (state->alpha > 140.0f) {
        state->alpha = 140.0f;
    }

    obj->anim.alpha = state->alpha;
}

void ARWSpeedStr_init(GameObject* obj, ObjPlacement* placement) {
    obj->anim.alpha = 0;
}

void ARWSpeedStr_release(void) {
}

void ARWSpeedStr_initialise(void) {
}

OBJECT_INIT_ADAPTER(gARWSpeedStrObjDescriptorInitAdapter, ARWSpeedStr_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gARWSpeedStrObjDescriptorHitDetectAdapter, ARWSpeedStr_hitDetect)
OBJECT_FREE_ADAPTER(gARWSpeedStrObjDescriptorFreeAdapter, ARWSpeedStr_free)
OBJECT_TYPE_ID_ADAPTER(gARWSpeedStrObjDescriptorTypeIdAdapter, ARWSpeedStr_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gARWSpeedStrObjDescriptorExtraSizeAdapter, ARWSpeedStr_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gARWSpeedStrObjDescriptorAcquire, ARWSpeedStr_initialise)

ObjectDescriptor gARWSpeedStrObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gARWSpeedStrObjDescriptorAcquire,
        ARWSpeedStr_release,
    },
    0,
    gARWSpeedStrObjDescriptorInitAdapter,
    ARWSpeedStr_update,
    gARWSpeedStrObjDescriptorHitDetectAdapter,
    ARWSpeedStr_render,
    gARWSpeedStrObjDescriptorFreeAdapter,
    gARWSpeedStrObjDescriptorTypeIdAdapter,
    gARWSpeedStrObjDescriptorExtraSizeAdapter,
};
