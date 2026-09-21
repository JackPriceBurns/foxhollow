#include "dlls/objects/365_IMIcePillar.h"
#include "main/object_render.h"

int imIcePillar_getExtraSize(void) {
    return 4;
}

int imIcePillar_getObjectTypeId(void) {
    return 0;
}

void imIcePillar_free(void) {
}

void imIcePillar_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void imIcePillar_hitDetect(void) {
}

void imIcePillar_update(void) {
}

void imIcePillar_init(void) {
}

void imIcePillar_release(void) {
}

void imIcePillar_initialise(void) {
}

OBJECT_INIT_ADAPTER(gIMIcePillarObjDescriptorInitAdapter, imIcePillar_init)
OBJECT_UPDATE_ADAPTER(gIMIcePillarObjDescriptorUpdateAdapter, imIcePillar_update)
OBJECT_HIT_DETECT_ADAPTER(gIMIcePillarObjDescriptorHitDetectAdapter, imIcePillar_hitDetect)
OBJECT_FREE_ADAPTER(gIMIcePillarObjDescriptorFreeAdapter, imIcePillar_free)
OBJECT_TYPE_ID_ADAPTER(gIMIcePillarObjDescriptorTypeIdAdapter, imIcePillar_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gIMIcePillarObjDescriptorExtraSizeAdapter, imIcePillar_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gIMIcePillarObjDescriptorAcquire, imIcePillar_initialise)

ObjectDescriptor gIMIcePillarObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gIMIcePillarObjDescriptorAcquire,
        imIcePillar_release,
    },
    0,
    gIMIcePillarObjDescriptorInitAdapter,
    gIMIcePillarObjDescriptorUpdateAdapter,
    gIMIcePillarObjDescriptorHitDetectAdapter,
    imIcePillar_render,
    gIMIcePillarObjDescriptorFreeAdapter,
    gIMIcePillarObjDescriptorTypeIdAdapter,
    gIMIcePillarObjDescriptorExtraSizeAdapter,
};
