/*
 * EndObject (DLL 0x108) - terminal marker object with stub callbacks.
 */
#include "dlls/objects/264_EndObject.h"

int EndObject_getExtraSize(void) {
    return 0;
}

int EndObject_getObjectTypeId(void) {
    return 0;
}

void EndObject_free(void) {
}

void EndObject_render(void) {
}

void EndObject_hitDetect(void) {
}

void EndObject_update(void) {
}

void EndObject_init(void) {
}

void EndObject_release(void) {
}

void EndObject_initialise(void) {
}

OBJECT_INIT_ADAPTER(gEndObjectObjDescriptorInitAdapter, EndObject_init)
OBJECT_UPDATE_ADAPTER(gEndObjectObjDescriptorUpdateAdapter, EndObject_update)
OBJECT_HIT_DETECT_ADAPTER(gEndObjectObjDescriptorHitDetectAdapter, EndObject_hitDetect)
OBJECT_RENDER_ADAPTER(gEndObjectObjDescriptorRenderAdapter, EndObject_render)
OBJECT_FREE_ADAPTER(gEndObjectObjDescriptorFreeAdapter, EndObject_free)
OBJECT_TYPE_ID_ADAPTER(gEndObjectObjDescriptorTypeIdAdapter, EndObject_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gEndObjectObjDescriptorExtraSizeAdapter, EndObject_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gEndObjectObjDescriptorAcquire, EndObject_initialise)

ObjectDescriptor gEndObjectObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gEndObjectObjDescriptorAcquire,
        EndObject_release,
    },
    0,
    gEndObjectObjDescriptorInitAdapter,
    gEndObjectObjDescriptorUpdateAdapter,
    gEndObjectObjDescriptorHitDetectAdapter,
    gEndObjectObjDescriptorRenderAdapter,
    gEndObjectObjDescriptorFreeAdapter,
    gEndObjectObjDescriptorTypeIdAdapter,
    gEndObjectObjDescriptorExtraSizeAdapter,
};
