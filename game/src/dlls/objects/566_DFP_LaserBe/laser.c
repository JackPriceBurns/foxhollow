/*
 * Legacy laser-beam object. Its active callbacks only report that the
 * object is no longer supported.
 */
#include "dolphin/os.h"
#include "dlls/object_descriptor.h"

int laser_getExtraSize(void) {
    return 0;
}

int laser_getObjectTypeId(void) {
    return 0;
}

void laser_freeUnsupported(void) {
    OSReport("<textblock.c Init>No Longer supported \n");
}

void laser_renderUnsupported(void) {
    OSReport("<textblock.c Init>No Longer supported \n");
}

void laser_hitDetectUnsupported(void) {
}

void laser_updateUnsupported(void) {
    OSReport("<textblock.c Init>No Longer supported \n");
}

void laser_init(void) {
    OSReport("<laser.c Init>No Longer supported \n");
}

void laser_releaseUnsupported(void) {
}

void laser_initialiseUnsupported(void) {
}

OBJECT_INIT_ADAPTER(gLaserUnsupportedObjDescriptorInitAdapter, laser_init)
OBJECT_UPDATE_ADAPTER(gLaserUnsupportedObjDescriptorUpdateAdapter, laser_updateUnsupported)
OBJECT_HIT_DETECT_ADAPTER(gLaserUnsupportedObjDescriptorHitDetectAdapter, laser_hitDetectUnsupported)
OBJECT_RENDER_ADAPTER(gLaserUnsupportedObjDescriptorRenderAdapter, laser_renderUnsupported)
OBJECT_FREE_ADAPTER(gLaserUnsupportedObjDescriptorFreeAdapter, laser_freeUnsupported)
OBJECT_TYPE_ID_ADAPTER(gLaserUnsupportedObjDescriptorTypeIdAdapter, laser_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gLaserUnsupportedObjDescriptorExtraSizeAdapter, laser_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gLaserUnsupportedObjDescriptorAcquire, laser_initialiseUnsupported)

ObjectDescriptor gLaserUnsupportedObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gLaserUnsupportedObjDescriptorAcquire,
        laser_releaseUnsupported,
    },
    0,
    gLaserUnsupportedObjDescriptorInitAdapter,
    gLaserUnsupportedObjDescriptorUpdateAdapter,
    gLaserUnsupportedObjDescriptorHitDetectAdapter,
    gLaserUnsupportedObjDescriptorRenderAdapter,
    gLaserUnsupportedObjDescriptorFreeAdapter,
    gLaserUnsupportedObjDescriptorTypeIdAdapter,
    gLaserUnsupportedObjDescriptorExtraSizeAdapter,
};
