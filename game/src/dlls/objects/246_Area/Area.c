/*
 * Area (DLL 0xF6) - inert area marker objects.
 *
 * Instances carry no extra state and disable both updates and hit detection.
 */
#include "dlls/objects/246_Area.h"
#include "game/objects/object.h"

int area_getExtraSize(void) {
    return 0;
}

int area_getObjectTypeId(void) {
    return 0;
}

void area_free(void) {
}

void area_render(void) {
}

void area_hitDetect(void) {
}

void area_update(void) {
}

void area_init(GameObject* obj) {
    obj->objectFlags |= OBJECT_OBJFLAG_UPDATE_DISABLED | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void area_release(void) {
}

void area_initialise(void) {
}

OBJECT_INIT_ADAPTER(gAreaObjDescriptorInitAdapter, area_init, obj)
OBJECT_UPDATE_ADAPTER(gAreaObjDescriptorUpdateAdapter, area_update)
OBJECT_HIT_DETECT_ADAPTER(gAreaObjDescriptorHitDetectAdapter, area_hitDetect)
OBJECT_RENDER_ADAPTER(gAreaObjDescriptorRenderAdapter, area_render)
OBJECT_FREE_ADAPTER(gAreaObjDescriptorFreeAdapter, area_free)
OBJECT_TYPE_ID_ADAPTER(gAreaObjDescriptorTypeIdAdapter, area_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gAreaObjDescriptorExtraSizeAdapter, area_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gAreaObjDescriptorAcquire, area_initialise)

ObjectDescriptor gAreaObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gAreaObjDescriptorAcquire,
        area_release,
    },
    0,
    gAreaObjDescriptorInitAdapter,
    gAreaObjDescriptorUpdateAdapter,
    gAreaObjDescriptorHitDetectAdapter,
    gAreaObjDescriptorRenderAdapter,
    gAreaObjDescriptorFreeAdapter,
    gAreaObjDescriptorTypeIdAdapter,
    gAreaObjDescriptorExtraSizeAdapter,
};
