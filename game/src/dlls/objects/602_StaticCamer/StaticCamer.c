/*
 * StaticCamer (DLL 0x25A) - static camera object.
 *
 * A passive scene camera placement. init negates the placement's three
 * orientation shorts into the object's rotX/rotY/rotZ, caches a byte
 * setup value and a byte-derived float into the object's extra
 * (StaticCameraState), then (unless deferred) registers the object in
 * object group 7. free unregisters from the same group. render forwards
 * to the shared object render thunk; hitDetect/update/release/initialise
 * are no-ops. The object carries no per-frame logic and reserves 8 bytes
 * of extra state.
 */
#include "main/object_render.h"
#include "main/dll/dll_025A_staticcamera.h"
#include "main/objtype.h"

int StaticCamera_getExtraSize(void) {
    return sizeof(StaticCameraState);
}
int StaticCamera_getObjectTypeId(void) {
    return 0x0;
}

void StaticCamera_free(GameObject* obj) {
    objFreeObjectType(obj, STATIC_CAMERA_OBJECT_GROUP);
}

void StaticCamera_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
}

void StaticCamera_hitDetect(void) {
}

void StaticCamera_update(void) {
}

void StaticCamera_init(GameObject* obj, StaticCameraPlacement* params, int deferAdd) {
    obj->anim.rotX = -ObjAnim_ReadPlacementS16(&obj->anim, &params->rotX);
    obj->anim.rotY = -ObjAnim_ReadPlacementS16(&obj->anim, &params->rotY);
    obj->anim.rotZ = -ObjAnim_ReadPlacementS16(&obj->anim, &params->rotZ);
    StaticCameraState* state = obj->extra;
    state->setupParam = params->setupParam;
    state->fov = params->fov;
    state->unk1 = 0;
    if (deferAdd == 0) {
        objAddObjectType(obj, STATIC_CAMERA_OBJECT_GROUP);
    }
}

void StaticCamera_release(void) {
}

void StaticCamera_initialise(void) {
}

OBJECT_INIT_ADAPTER(gStaticCameraObjDescriptorInitAdapter, StaticCamera_init, obj, placement, flags)
OBJECT_UPDATE_ADAPTER(gStaticCameraObjDescriptorUpdateAdapter, StaticCamera_update)
OBJECT_HIT_DETECT_ADAPTER(gStaticCameraObjDescriptorHitDetectAdapter, StaticCamera_hitDetect)
OBJECT_FREE_ADAPTER(gStaticCameraObjDescriptorFreeAdapter, StaticCamera_free, obj)
OBJECT_TYPE_ID_ADAPTER(gStaticCameraObjDescriptorTypeIdAdapter, StaticCamera_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gStaticCameraObjDescriptorExtraSizeAdapter, StaticCamera_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gStaticCameraObjDescriptorAcquire, StaticCamera_initialise)

ObjectDescriptor gStaticCameraObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gStaticCameraObjDescriptorAcquire,
        StaticCamera_release,
    },
    0,
    gStaticCameraObjDescriptorInitAdapter,
    gStaticCameraObjDescriptorUpdateAdapter,
    gStaticCameraObjDescriptorHitDetectAdapter,
    StaticCamera_render,
    gStaticCameraObjDescriptorFreeAdapter,
    gStaticCameraObjDescriptorTypeIdAdapter,
    gStaticCameraObjDescriptorExtraSizeAdapter,
};
