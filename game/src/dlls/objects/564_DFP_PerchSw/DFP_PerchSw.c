/*
 * Ocean Force Point Temple perch switch. This legacy object is no longer
 * supported; its init and update callbacks only report that fact.
 */
#include "main/dll/DF/dll_0234_dfperchwitch.h"
#include "dolphin/os/OSReport.h"

int dfperchwitch_getExtraSize(void) {
    return 0x0;
}
int dfperchwitch_getObjectTypeId(void) {
    return 0x0;
}

void dfperchwitch_free(void) {
}

void dfperchwitch_render(void) {
}

void dfperchwitch_hitDetect(void) {
}

void dfperchwitch_update(void) {
    OSReport("<dfperchwitch Init>No Longer supported \n");
}
void dfperchwitch_init(void) {
    OSReport("<dfperchwitch Init>No Longer supported \n");
}

void dfperchwitch_release(void) {
}

void dfperchwitch_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDfperchwitchObjDescriptorInitAdapter, dfperchwitch_init)
OBJECT_UPDATE_ADAPTER(gDfperchwitchObjDescriptorUpdateAdapter, dfperchwitch_update)
OBJECT_HIT_DETECT_ADAPTER(gDfperchwitchObjDescriptorHitDetectAdapter, dfperchwitch_hitDetect)
OBJECT_RENDER_ADAPTER(gDfperchwitchObjDescriptorRenderAdapter, dfperchwitch_render)
OBJECT_FREE_ADAPTER(gDfperchwitchObjDescriptorFreeAdapter, dfperchwitch_free)
OBJECT_TYPE_ID_ADAPTER(gDfperchwitchObjDescriptorTypeIdAdapter, dfperchwitch_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDfperchwitchObjDescriptorExtraSizeAdapter, dfperchwitch_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDfperchwitchObjDescriptorAcquire, dfperchwitch_initialise)

ObjectDescriptor gDfperchwitchObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDfperchwitchObjDescriptorAcquire,
        dfperchwitch_release,
    },
    0,
    gDfperchwitchObjDescriptorInitAdapter,
    gDfperchwitchObjDescriptorUpdateAdapter,
    gDfperchwitchObjDescriptorHitDetectAdapter,
    gDfperchwitchObjDescriptorRenderAdapter,
    gDfperchwitchObjDescriptorFreeAdapter,
    gDfperchwitchObjDescriptorTypeIdAdapter,
    gDfperchwitchObjDescriptorExtraSizeAdapter,
};
