/*
 * Ocean Force Point Temple door switch (DLL 0x22E) - a legacy/disabled object.
 * Every callback is either empty or logs
 * "<doorswitch Init>No Longer supported" via OSReport; the object holds no
 * extra state.
 */
#include "main/dll/DF/dll_022E_dfpdoorswitch.h"
#include "dolphin/os/OSReport.h"

int doorswitch_getExtraSize(void) {
    return 0x0;
}
int doorswitch_getObjectTypeId(void) {
    return 0x0;
}

void doorswitch_free(void) {
    OSReport("<doorswitch Init>No Longer supported \n");
}

void doorswitch_render(void) {
}

void doorswitch_hitDetect(void) {
}

void doorswitch_update(void) {
    OSReport("<doorswitch Init>No Longer supported \n");
}
void doorswitch_init(void) {
    OSReport("<doorswitch Init>No Longer supported \n");
}

void doorswitch_release(void) {
}

void doorswitch_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDoorswitchObjDescriptorInitAdapter, doorswitch_init)
OBJECT_UPDATE_ADAPTER(gDoorswitchObjDescriptorUpdateAdapter, doorswitch_update)
OBJECT_HIT_DETECT_ADAPTER(gDoorswitchObjDescriptorHitDetectAdapter, doorswitch_hitDetect)
OBJECT_RENDER_ADAPTER(gDoorswitchObjDescriptorRenderAdapter, doorswitch_render)
OBJECT_FREE_ADAPTER(gDoorswitchObjDescriptorFreeAdapter, doorswitch_free)
OBJECT_TYPE_ID_ADAPTER(gDoorswitchObjDescriptorTypeIdAdapter, doorswitch_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDoorswitchObjDescriptorExtraSizeAdapter, doorswitch_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDoorswitchObjDescriptorAcquire, doorswitch_initialise)

ObjectDescriptor gDoorswitchObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDoorswitchObjDescriptorAcquire,
        doorswitch_release,
    },
    0,
    gDoorswitchObjDescriptorInitAdapter,
    gDoorswitchObjDescriptorUpdateAdapter,
    gDoorswitchObjDescriptorHitDetectAdapter,
    gDoorswitchObjDescriptorRenderAdapter,
    gDoorswitchObjDescriptorFreeAdapter,
    gDoorswitchObjDescriptorTypeIdAdapter,
    gDoorswitchObjDescriptorExtraSizeAdapter,
};
