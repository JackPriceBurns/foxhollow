/*
 * GCbaddieShield object (DLL slot 221).
 *
 * Spins and fades a short-lived billboard shield effect.
 */
#include "dlls/objects/221_GCbaddieShield.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "sys/objects/lifecycle.h"

#define GCBADDIE_SHIELD_ROTATION_SPEED_X 1800.0f
#define GCBADDIE_SHIELD_ROTATION_SPEED_Z 200.0f
#define GCBADDIE_SHIELD_FADE_DURATION    64.0f
#define GCBADDIE_SHIELD_FADE_SCALE       0.015625f
#define GCBADDIE_SHIELD_MAX_ALPHA        255.0f

int GCbaddieShield_getExtraSize(void) {
    return sizeof(GCbaddieShieldState);
}

int GCbaddieShield_getObjectTypeId(void) {
    return 0;
}

void GCbaddieShield_free(GameObject* obj) {
}

void GCbaddieShield_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    switch (obj->userData1) {
    case 0:
        objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
        break;
    default:
        break;
    }
}

void GCbaddieShield_hitDetect(GameObject* obj) {
}

void GCbaddieShield_update(GameObject* obj) {
    GCbaddieShieldState* state = obj->extra;

    state->remainingLifetime -= timeDelta;
    if (state->remainingLifetime <= 0.0f) {
        Obj_FreeObject(obj);
        return;
    }

    obj->anim.rotX += GCBADDIE_SHIELD_ROTATION_SPEED_X * timeDelta;
    obj->anim.rotZ += GCBADDIE_SHIELD_ROTATION_SPEED_Z * timeDelta;

    if (state->remainingLifetime <= GCBADDIE_SHIELD_FADE_DURATION) {
        obj->anim.alpha = GCBADDIE_SHIELD_MAX_ALPHA * (state->remainingLifetime * GCBADDIE_SHIELD_FADE_SCALE);
    } else {
        obj->anim.alpha = 0xff;
    }
}

void GCbaddieShield_init(GameObject* obj, GCbaddieShieldPlacement* placement) {
    GCbaddieShieldState* state = obj->extra;
    state->remainingLifetime = ObjAnim_ReadPlacementS16(&obj->anim, &placement->lifetime);
}

void GCbaddieShield_release(void) {
}

void GCbaddieShield_initialise(void) {
}

OBJECT_INIT_ADAPTER(gGCbaddieShieldObjDescriptorInitAdapter, GCbaddieShield_init, obj, placement)
OBJECT_FREE_ADAPTER(gGCbaddieShieldObjDescriptorFreeAdapter, GCbaddieShield_free, obj)
OBJECT_TYPE_ID_ADAPTER(gGCbaddieShieldObjDescriptorTypeIdAdapter, GCbaddieShield_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gGCbaddieShieldObjDescriptorExtraSizeAdapter, GCbaddieShield_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gGCbaddieShieldObjDescriptorAcquire, GCbaddieShield_initialise)

ObjectDescriptor gGCbaddieShieldObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gGCbaddieShieldObjDescriptorAcquire,
        GCbaddieShield_release,
    },
    0,
    gGCbaddieShieldObjDescriptorInitAdapter,
    GCbaddieShield_update,
    GCbaddieShield_hitDetect,
    GCbaddieShield_render,
    gGCbaddieShieldObjDescriptorFreeAdapter,
    gGCbaddieShieldObjDescriptorTypeIdAdapter,
    gGCbaddieShieldObjDescriptorExtraSizeAdapter,
};
