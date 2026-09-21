/*
 * GPSH_Scene (DLL 0x194) - Test of Knowledge scene geometry.
 */
#include "dlls/objects/404_GPSH_Scene.h"
#include "main/object_render.h"

int gpshScene_getExtraSize(void) {
    return 0;
}

int gpshScene_getObjectTypeId(void) {
    return 0;
}

void gpshScene_free(void) {
}

void gpshScene_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void gpshScene_hitDetect(void) {
}

void gpshScene_update(void) {
}

void gpshScene_init(GameObject* obj, const GPSHScenePlacement* placement) {
    obj->anim.rotX = placement->initialYaw << 8;
    obj->anim.worldPosX = obj->anim.localPosX;
    obj->anim.worldPosY = obj->anim.localPosY;
    obj->anim.worldPosZ = obj->anim.localPosZ;
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
}

void gpshScene_release(void) {
}

void gpshScene_initialise(void) {
}

OBJECT_INIT_ADAPTER(gGPSHSceneObjDescriptorInitAdapter, gpshScene_init, obj, placement)
OBJECT_UPDATE_ADAPTER(gGPSHSceneObjDescriptorUpdateAdapter, gpshScene_update)
OBJECT_HIT_DETECT_ADAPTER(gGPSHSceneObjDescriptorHitDetectAdapter, gpshScene_hitDetect)
OBJECT_FREE_ADAPTER(gGPSHSceneObjDescriptorFreeAdapter, gpshScene_free)
OBJECT_TYPE_ID_ADAPTER(gGPSHSceneObjDescriptorTypeIdAdapter, gpshScene_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gGPSHSceneObjDescriptorExtraSizeAdapter, gpshScene_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gGPSHSceneObjDescriptorAcquire, gpshScene_initialise)

ObjectDescriptor gGPSHSceneObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gGPSHSceneObjDescriptorAcquire,
        gpshScene_release,
    },
    0,
    gGPSHSceneObjDescriptorInitAdapter,
    gGPSHSceneObjDescriptorUpdateAdapter,
    gGPSHSceneObjDescriptorHitDetectAdapter,
    gpshScene_render,
    gGPSHSceneObjDescriptorFreeAdapter,
    gGPSHSceneObjDescriptorTypeIdAdapter,
    gGPSHSceneObjDescriptorExtraSizeAdapter,
};
