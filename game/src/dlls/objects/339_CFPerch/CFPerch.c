/* CloudRunner Fortress prison-perch behavior. */

#include "dlls/objects/339_CFPerch.h"

#include "main/gamebits.h"
#include "main/obj_message.h"
#include "main/objseq.h"

#define CFPERCH_PRISON_UNCLE_CLASS_ID 62
#define CFPERCH_REMOVED_MESSAGE       0x40001
#define CFPERCH_SEQUENCE_SQUAWK       0

int cfPerch_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    if (mainGetBit(GAMEBIT_CF_PrisonCageOpened) != 0) {
        animUpdate->sequenceControlFlags = OBJSEQ_CONTROL_SET_LATCH_A;
    }
    return 0;
}

int cfPerch_getExtraSize(void) {
    return 0;
}

int cfPerch_getObjectTypeId(void) {
    return 0;
}

void cfPerch_free(GameObject* obj) {
    ObjMsg_SendToObjects(CFPERCH_PRISON_UNCLE_CLASS_ID, 0, obj, CFPERCH_REMOVED_MESSAGE, 0);
}

void cfPerch_render(void) {
}

void cfPerch_hitDetect(void) {
}

void cfPerch_update(GameObject* obj) {
    if (obj->userData1 != 0) {
        if (mainGetBit(GAMEBIT_CF_UncleFlewOff) == 0) {
            (*gObjectTriggerInterface)->runSequence(CFPERCH_SEQUENCE_SQUAWK, obj, -1);
        }
    }
    obj->userData1 = FALSE;
}

void cfPerch_init(GameObject* obj) {
    obj->userData1 = TRUE;
    obj->animEventCallback = cfPerch_sequenceCallback;
}

void cfPerch_release(void) {
}

void cfPerch_initialise(void) {
}

OBJECT_INIT_ADAPTER(gCFPerchObjDescriptorInitAdapter, cfPerch_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gCFPerchObjDescriptorHitDetectAdapter, cfPerch_hitDetect)
OBJECT_RENDER_ADAPTER(gCFPerchObjDescriptorRenderAdapter, cfPerch_render)
OBJECT_FREE_ADAPTER(gCFPerchObjDescriptorFreeAdapter, cfPerch_free, obj)
OBJECT_TYPE_ID_ADAPTER(gCFPerchObjDescriptorTypeIdAdapter, cfPerch_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gCFPerchObjDescriptorExtraSizeAdapter, cfPerch_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gCFPerchObjDescriptorAcquire, cfPerch_initialise)

ObjectDescriptor gCFPerchObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gCFPerchObjDescriptorAcquire,
        cfPerch_release,
    },
    0,
    gCFPerchObjDescriptorInitAdapter,
    cfPerch_update,
    gCFPerchObjDescriptorHitDetectAdapter,
    gCFPerchObjDescriptorRenderAdapter,
    gCFPerchObjDescriptorFreeAdapter,
    gCFPerchObjDescriptorTypeIdAdapter,
    gCFPerchObjDescriptorExtraSizeAdapter,
};
