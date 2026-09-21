/* Supplies the render and message-queue callbacks for FElevControl. */
#include "dlls/objects/322_FElevContro.h"
#include "main/obj_message.h"
#include "main/object_render.h"

int FElevControl_getExtraSize(void) {
    return 0;
}

int FElevControl_getObjectTypeId(void) {
    return 0;
}

void FElevControl_free(void) {
}

void FElevControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void FElevControl_hitDetect(void) {
}

void FElevControl_update(void) {
}

void FElevControl_init(GameObject* obj) {
    ObjMsg_AllocQueue(obj, 2);
}

void FElevControl_release(void) {
}

void FElevControl_initialise(void) {
}

OBJECT_INIT_ADAPTER(gFElevControlObjDescriptorInitAdapter, FElevControl_init, obj)
OBJECT_UPDATE_ADAPTER(gFElevControlObjDescriptorUpdateAdapter, FElevControl_update)
OBJECT_HIT_DETECT_ADAPTER(gFElevControlObjDescriptorHitDetectAdapter, FElevControl_hitDetect)
OBJECT_FREE_ADAPTER(gFElevControlObjDescriptorFreeAdapter, FElevControl_free)
OBJECT_TYPE_ID_ADAPTER(gFElevControlObjDescriptorTypeIdAdapter, FElevControl_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gFElevControlObjDescriptorExtraSizeAdapter, FElevControl_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gFElevControlObjDescriptorAcquire, FElevControl_initialise)

ObjectDescriptor gFElevControlObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gFElevControlObjDescriptorAcquire,
        FElevControl_release,
    },
    0,
    gFElevControlObjDescriptorInitAdapter,
    gFElevControlObjDescriptorUpdateAdapter,
    gFElevControlObjDescriptorHitDetectAdapter,
    FElevControl_render,
    gFElevControlObjDescriptorFreeAdapter,
    gFElevControlObjDescriptorTypeIdAdapter,
    gFElevControlObjDescriptorExtraSizeAdapter,
};
