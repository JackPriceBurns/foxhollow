#include "dolphin/os.h"
#include "main/dll/VF/platform1.h"

int platform1_getExtraSize(void) {
    return 0;
}

int platform1_getObjectTypeId(void) {
    return 0;
}

void platform1_free(void) {
}

void platform1_drawUnsupported(void) {
    OSReport("<platform1 draw>No Longer supported \n");
}

void platform1_hitDetect(void) {
}

void platform1_controlUnsupported(void) {
    OSReport("<platform1 control>No Longer supported \n");
}

void platform1_init(void) {
    OSReport("<platform1 Init>No Longer supported \n");
}

void platform1_release(void) {
}

void platform1_initialise(void) {
}

OBJECT_INIT_ADAPTER(gPlatform1ObjDescriptorInitAdapter, platform1_init)
OBJECT_UPDATE_ADAPTER(gPlatform1ObjDescriptorUpdateAdapter, platform1_controlUnsupported)
OBJECT_HIT_DETECT_ADAPTER(gPlatform1ObjDescriptorHitDetectAdapter, platform1_hitDetect)
OBJECT_RENDER_ADAPTER(gPlatform1ObjDescriptorRenderAdapter, platform1_drawUnsupported)
OBJECT_FREE_ADAPTER(gPlatform1ObjDescriptorFreeAdapter, platform1_free)
OBJECT_TYPE_ID_ADAPTER(gPlatform1ObjDescriptorTypeIdAdapter, platform1_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gPlatform1ObjDescriptorExtraSizeAdapter, platform1_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gPlatform1ObjDescriptorAcquire, platform1_initialise)

ObjectDescriptor gPlatform1ObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gPlatform1ObjDescriptorAcquire,
        platform1_release,
    },
    0,
    gPlatform1ObjDescriptorInitAdapter,
    gPlatform1ObjDescriptorUpdateAdapter,
    gPlatform1ObjDescriptorHitDetectAdapter,
    gPlatform1ObjDescriptorRenderAdapter,
    gPlatform1ObjDescriptorFreeAdapter,
    gPlatform1ObjDescriptorTypeIdAdapter,
    gPlatform1ObjDescriptorExtraSizeAdapter,
};
