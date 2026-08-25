/*
 * DLL 0x1A3 (slot 419) - a hidden object-group anchor.
 *
 * Instances join object group 0x3D. Slot 420 searches that group for an
 * object with the same placement pair ID, then follows its transform.
 */
#include "dlls/objects/419.h"
#include "game/objects/object.h"
#include "main/objseq.h"
#include "main/objtype.h"

int dll419_processAnimEvents(GameObject*, int, ObjSeqState*) {
    return 0;
}

int dll419_getExtraSize(void) {
    return 0;
}

int dll419_getObjectTypeId(void) {
    return 0;
}

void dll419_free(GameObject* obj) {
    objFreeObjectType(obj, DLL1A3_OBJECT_GROUP_ID);
}

void dll419_render(GameObject*, int, int, int, int, s8) {
}

void dll419_hitDetect(void) {
}

void dll419_update(GameObject*) {
}

void dll419_init(GameObject* obj) {
    obj->animEventCallback = dll419_processAnimEvents;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
    objAddObjectType(obj, DLL1A3_OBJECT_GROUP_ID);
}

void dll419_release(void) {
}

void dll419_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll1A3ObjDescriptorInitAdapter, dll419_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gDll1A3ObjDescriptorHitDetectAdapter, dll419_hitDetect)
OBJECT_FREE_ADAPTER(gDll1A3ObjDescriptorFreeAdapter, dll419_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDll1A3ObjDescriptorTypeIdAdapter, dll419_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll1A3ObjDescriptorExtraSizeAdapter, dll419_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll1A3ObjDescriptorAcquire, dll419_initialise)

ObjectDescriptor gDll1A3ObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll1A3ObjDescriptorAcquire,
        dll419_release,
    },
    0,
    gDll1A3ObjDescriptorInitAdapter,
    dll419_update,
    gDll1A3ObjDescriptorHitDetectAdapter,
    dll419_render,
    gDll1A3ObjDescriptorFreeAdapter,
    gDll1A3ObjDescriptorTypeIdAdapter,
    gDll1A3ObjDescriptorExtraSizeAdapter,
};
