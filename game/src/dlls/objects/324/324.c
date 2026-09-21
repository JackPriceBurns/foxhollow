/* Clears sequence-event state and supplies otherwise minimal callbacks. */
#include "dlls/objects/324.h"
#include "main/object_render.h"
#include "main/objseq.h"

int dll_144_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    animUpdate->movementState = 0;
    return 0;
}

int dll_144_getExtraSize(void) {
    return 0;
}

int dll_144_getObjectTypeId(void) {
    return 0;
}

void dll_144_free(void) {
}

void dll_144_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void dll_144_hitDetect(void) {
}

void dll_144_update(void) {
}

void dll_144_init(GameObject* obj) {
    obj->anim.rotX = 0;
    obj->animEventCallback = dll_144_SeqFn;
}

void dll_144_release(void) {
}

void dll_144_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll144ObjDescriptorInitAdapter, dll_144_init, obj)
OBJECT_UPDATE_ADAPTER(gDll144ObjDescriptorUpdateAdapter, dll_144_update)
OBJECT_HIT_DETECT_ADAPTER(gDll144ObjDescriptorHitDetectAdapter, dll_144_hitDetect)
OBJECT_FREE_ADAPTER(gDll144ObjDescriptorFreeAdapter, dll_144_free)
OBJECT_TYPE_ID_ADAPTER(gDll144ObjDescriptorTypeIdAdapter, dll_144_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll144ObjDescriptorExtraSizeAdapter, dll_144_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll144ObjDescriptorAcquire, dll_144_initialise)

ObjectDescriptor gDll144ObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll144ObjDescriptorAcquire,
        dll_144_release,
    },
    0,
    gDll144ObjDescriptorInitAdapter,
    gDll144ObjDescriptorUpdateAdapter,
    gDll144ObjDescriptorHitDetectAdapter,
    dll_144_render,
    gDll144ObjDescriptorFreeAdapter,
    gDll144ObjDescriptorTypeIdAdapter,
    gDll144ObjDescriptorExtraSizeAdapter,
};
