/*
 * DLL 670 - an unnamed, empty object-class slot. Every entry point
 * (extra-size, type-id, init/update/render/hitDetect/free, and the
 * (de)initialise pair) is a stub: no per-object state is allocated and no
 * behaviour runs. The DLL exists only to fill the 0x29E id in the object
 * table.
 */
#include "main/dll/dll_029E_dummy.h"
#include "dlls/object_descriptor.h"

int Dummy29E_getExtraSize(void) {
    return 0x0;
}

int Dummy29E_getObjectTypeId(void) {
    return 0x0;
}

void Dummy29E_free(void) {
}

void Dummy29E_render(void) {
}

void Dummy29E_hitDetect(void) {
}

void Dummy29E_update(void) {
}

void Dummy29E_init(void) {
}

void Dummy29E_release(void) {
}

void Dummy29E_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll29EObjDescriptorInitAdapter, Dummy29E_init)
OBJECT_UPDATE_ADAPTER(gDll29EObjDescriptorUpdateAdapter, Dummy29E_update)
OBJECT_HIT_DETECT_ADAPTER(gDll29EObjDescriptorHitDetectAdapter, Dummy29E_hitDetect)
OBJECT_RENDER_ADAPTER(gDll29EObjDescriptorRenderAdapter, Dummy29E_render)
OBJECT_FREE_ADAPTER(gDll29EObjDescriptorFreeAdapter, Dummy29E_free)
OBJECT_TYPE_ID_ADAPTER(gDll29EObjDescriptorTypeIdAdapter, Dummy29E_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll29EObjDescriptorExtraSizeAdapter, Dummy29E_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll29EObjDescriptorAcquire, Dummy29E_initialise)

ObjectDescriptor gDll29EObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll29EObjDescriptorAcquire,
        Dummy29E_release,
    },
    NULL,
    gDll29EObjDescriptorInitAdapter,
    gDll29EObjDescriptorUpdateAdapter,
    gDll29EObjDescriptorHitDetectAdapter,
    gDll29EObjDescriptorRenderAdapter,
    gDll29EObjDescriptorFreeAdapter,
    gDll29EObjDescriptorTypeIdAdapter,
    gDll29EObjDescriptorExtraSizeAdapter,
};
