/*
 * Static rendered-model family with placement rotation, optional uniform
 * scale, and disabled update/hit-detection callbacks.
 */
#include "dlls/objects/302.h"
#include "main/object_render.h"

int CFLightWall_getExtraSize(void) {
    return 0;
}

int CFLightWall_getObjectTypeId(void) {
    return 0;
}

void CFLightWall_free(void) {
}

void CFLightWall_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void CFLightWall_hitDetect(void) {
}

void CFLightWall_update(void) {
}

void CFLightWall_init(GameObject* obj, CFLightWallPlacement* placement) {
    obj->anim.rotX = placement->initialRotX << 8;
    obj->anim.rotY = placement->initialRotY << 8;
    obj->anim.rotZ = placement->initialRotZ << 8;
    if (placement->scale == 0) {
        obj->objectFlags |= OBJECT_OBJFLAG_UPDATE_DISABLED | OBJECT_OBJFLAG_HITDETECT_DISABLED;
        return;
    }

    obj->anim.rootMotionScale = placement->scale / 255.0f;
    if (!obj->anim.rootMotionScale) {
        obj->anim.rootMotionScale = 1.0f;
    }
    obj->anim.rootMotionScale *= obj->anim.modelInstance->rootMotionScaleBase;
    obj->objectFlags |= OBJECT_OBJFLAG_UPDATE_DISABLED | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void CFLightWall_release(void) {
}

void CFLightWall_initialise(void) {
}

OBJECT_INIT_ADAPTER(gCFLightWallObjDescriptorInitAdapter, CFLightWall_init, obj, placement)
OBJECT_UPDATE_ADAPTER(gCFLightWallObjDescriptorUpdateAdapter, CFLightWall_update)
OBJECT_HIT_DETECT_ADAPTER(gCFLightWallObjDescriptorHitDetectAdapter, CFLightWall_hitDetect)
OBJECT_FREE_ADAPTER(gCFLightWallObjDescriptorFreeAdapter, CFLightWall_free)
OBJECT_TYPE_ID_ADAPTER(gCFLightWallObjDescriptorTypeIdAdapter, CFLightWall_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gCFLightWallObjDescriptorExtraSizeAdapter, CFLightWall_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gCFLightWallObjDescriptorAcquire, CFLightWall_initialise)

ObjectDescriptor gCFLightWallObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gCFLightWallObjDescriptorAcquire,
        CFLightWall_release,
    },
    0,
    gCFLightWallObjDescriptorInitAdapter,
    gCFLightWallObjDescriptorUpdateAdapter,
    gCFLightWallObjDescriptorHitDetectAdapter,
    CFLightWall_render,
    gCFLightWallObjDescriptorFreeAdapter,
    gCFLightWallObjDescriptorTypeIdAdapter,
    gCFLightWallObjDescriptorExtraSizeAdapter,
};
