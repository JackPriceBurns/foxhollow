#include "dolphin/os.h"
#include "main/textblock.h"

int textblockObj_getExtraSize(void) {
    return 0;
}

int textblockObj_getObjectTypeId(void) {
    return 0;
}

void textblockObj_freeUnsupported(void) {
    OSReport("<textblock.c Init>No Longer supported \n");
}

void textblockObj_render(void) {
}

void textblockObj_hitDetect(void) {
}

void textblockObj_updateUnsupported(void) {
    OSReport("<textblock.c Init>No Longer supported \n");
}

void textblockObj_init(void) {
    OSReport("<textblock.c Init>No Longer supported \n");
}

void textblockObj_release(void) {
}

void textblockObj_initialise(void) {
}

OBJECT_INIT_ADAPTER(gTextBlockObjDescriptorInitAdapter, textblockObj_init)
OBJECT_UPDATE_ADAPTER(gTextBlockObjDescriptorUpdateAdapter, textblockObj_updateUnsupported)
OBJECT_HIT_DETECT_ADAPTER(gTextBlockObjDescriptorHitDetectAdapter, textblockObj_hitDetect)
OBJECT_RENDER_ADAPTER(gTextBlockObjDescriptorRenderAdapter, textblockObj_render)
OBJECT_FREE_ADAPTER(gTextBlockObjDescriptorFreeAdapter, textblockObj_freeUnsupported)
OBJECT_TYPE_ID_ADAPTER(gTextBlockObjDescriptorTypeIdAdapter, textblockObj_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gTextBlockObjDescriptorExtraSizeAdapter, textblockObj_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gTextBlockObjDescriptorAcquire, textblockObj_initialise)

ObjectDescriptor gTextBlockObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gTextBlockObjDescriptorAcquire,
        textblockObj_release,
    },
    0,
    gTextBlockObjDescriptorInitAdapter,
    gTextBlockObjDescriptorUpdateAdapter,
    gTextBlockObjDescriptorHitDetectAdapter,
    gTextBlockObjDescriptorRenderAdapter,
    gTextBlockObjDescriptorFreeAdapter,
    gTextBlockObjDescriptorTypeIdAdapter,
    gTextBlockObjDescriptorExtraSizeAdapter,
};
