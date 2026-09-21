/*
 * DLL 675 - a short-lived spinning debris/particle object.
 *
 * On init it fades in from alpha 0, picks a random starting orientation
 * and random per-axis spin speeds. Each frame it fades the alpha up to a
 * cap, advances its rotation by the spin speeds, and drifts along its
 * velocity (gravity/launch supplied by dll_2A3_setVelocity). It self-frees once
 * its lifetime decays past a threshold.
 *
 * gDll2A3InstanceCount is a live-instance refcount (bumped on init, dropped on
 * free); gDll2A3UpdatedThisFrame is a once-per-frame "an instance updated" flag,
 * cleared by hitDetect and set by the first update.
 */
#include "main/dll/ARW/dll_02A3.h"
#include "main/frame_timing.h"
#include "dlls/object_descriptor.h"
#include "main/object_render.h"
#include "sys/objects.h"
#include "main/vecmath.h"
#include "sys/objects/lifecycle.h"

int gDll2A3UpdatedThisFrame;
int gDll2A3InstanceCount;

void dll_2A3_setLifetime(GameObject* obj, int lifetime) {
    Dll2A3State* state = obj->extra;
    state->lifetime = lifetime;
}

void dll_2A3_setVelocity(GameObject* obj, Vec3f* velocity) {
    obj->anim.velocityX = velocity->x;
    obj->anim.velocityY = velocity->y;
    obj->anim.velocityZ = velocity->z;
}

int dll_2A3_getExtraSize_ret_12(void) {
    return sizeof(Dll2A3State);
}

int dll_2A3_getObjectTypeId(void) {
    return 0x0;
}

void dll_2A3_free(void) {
    gDll2A3InstanceCount--;
}

void dll_2A3_render(GameObject* obj, int p2, int p3, int p4, int p5) {
    objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
}

void dll_2A3_hitDetect(void) {
    gDll2A3UpdatedThisFrame = 0;
}

void dll_2A3_update(GameObject* obj) {
    Dll2A3State* state = obj->extra;

    if (state->lifetime > 0.0f) {
        state->lifetime -= timeDelta;
        if (state->lifetime <= 0.0f) {
            state->lifetime = 0.0f;
            Obj_FreeObject(obj);
            return;
        }
    }

    f32 alpha = obj->anim.alpha + 8.0f * timeDelta;
    if (alpha > 255.0f) {
        alpha = 255.0f;
    }

    obj->anim.alpha = alpha;
    obj->anim.rotX += state->spinRateX * timeDelta;
    obj->anim.rotY += state->spinRateY * timeDelta;
    obj->anim.rotZ += state->spinRateZ * timeDelta;

    objMove(obj, obj->anim.velocityX * timeDelta, obj->anim.velocityY * timeDelta, obj->anim.velocityZ * timeDelta);

    if (gDll2A3UpdatedThisFrame == 0) {
        gDll2A3UpdatedThisFrame = 1;
    }
}

void dll_2A3_init(GameObject* obj) {
    Dll2A3State* state = obj->extra;
    obj->anim.alpha = 0;
    obj->anim.rotX = randomGetRange(0, 65535);
    obj->anim.rotY = randomGetRange(0, 65535);
    obj->anim.rotZ = randomGetRange(0, 65535);
    state->spinRateX = randomGetRange(-50, 50);
    state->spinRateY = randomGetRange(-50, 50);
    state->spinRateZ = randomGetRange(-50, 50);
    gDll2A3InstanceCount++;
}

void dll_2A3_release_nop(void) {
}

void dll_2A3_initialise_nop(void) {
}

OBJECT_INIT_ADAPTER(gDll2A3ObjDescriptorInitAdapter, dll_2A3_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gDll2A3ObjDescriptorHitDetectAdapter, dll_2A3_hitDetect)
OBJECT_RENDER_ADAPTER(gDll2A3ObjDescriptorRenderAdapter, dll_2A3_render, obj, arg2, arg3, arg4, arg5)
OBJECT_FREE_ADAPTER(gDll2A3ObjDescriptorFreeAdapter, dll_2A3_free)
OBJECT_TYPE_ID_ADAPTER(gDll2A3ObjDescriptorTypeIdAdapter, dll_2A3_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll2A3ObjDescriptorExtraSizeAdapter, dll_2A3_getExtraSize_ret_12)

RESOURCE_ACQUIRE_ADAPTER(gDll2A3ObjDescriptorAcquire, dll_2A3_initialise_nop)

ObjectDescriptor gDll2A3ObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll2A3ObjDescriptorAcquire,
        dll_2A3_release_nop,
    },
    0,
    gDll2A3ObjDescriptorInitAdapter,
    dll_2A3_update,
    gDll2A3ObjDescriptorHitDetectAdapter,
    gDll2A3ObjDescriptorRenderAdapter,
    gDll2A3ObjDescriptorFreeAdapter,
    gDll2A3ObjDescriptorTypeIdAdapter,
    gDll2A3ObjDescriptorExtraSizeAdapter,
};
