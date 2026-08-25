/*
 * DLL 0x1CF has no active-target object definition. Its init callback reads a
 * placement prefix, applies two encoded rotations, and disables visibility,
 * updates, and hit detection.
 */
#include "dlls/objects/463.h"

#include "main/object_render.h"
#include "main/gamebits.h"

int dll_1CF_getExtraSize(void) {
    return 0;
}

int dll_1CF_getObjectTypeId(void) {
    return 0;
}

void dll_1CF_free(void) {
}

void dll_1CF_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void dll_1CF_hitDetect(void) {
}

void dll_1CF_update(void) {
}

void dll_1CF_init(GameObject* obj, const Dll1CFPlacementView* placement) {
    s16 gateGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gateGameBit);
    s16 rotationYDegrees = ObjAnim_ReadPlacementS16(&obj->anim, &placement->rotationYDegrees);
    if (mainGetBit(gateGameBit) != 0) {
        obj->anim.rotY = (rotationYDegrees << 13) / 45;
    }
    obj->anim.rotX = placement->rotationXByte << 8;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED | OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_UPDATE_DISABLED;
}

void dll_1CF_release(void) {
}

void dll_1CF_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll1CFObjDescriptorInitAdapter, dll_1CF_init, obj, placement)
OBJECT_UPDATE_ADAPTER(gDll1CFObjDescriptorUpdateAdapter, dll_1CF_update)
OBJECT_HIT_DETECT_ADAPTER(gDll1CFObjDescriptorHitDetectAdapter, dll_1CF_hitDetect)
OBJECT_FREE_ADAPTER(gDll1CFObjDescriptorFreeAdapter, dll_1CF_free)
OBJECT_TYPE_ID_ADAPTER(gDll1CFObjDescriptorTypeIdAdapter, dll_1CF_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll1CFObjDescriptorExtraSizeAdapter, dll_1CF_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll1CFObjDescriptorAcquire, dll_1CF_initialise)

ObjectDescriptor gDll1CFObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll1CFObjDescriptorAcquire,
        dll_1CF_release,
    },
    0,
    gDll1CFObjDescriptorInitAdapter,
    gDll1CFObjDescriptorUpdateAdapter,
    gDll1CFObjDescriptorHitDetectAdapter,
    dll_1CF_render,
    gDll1CFObjDescriptorFreeAdapter,
    gDll1CFObjDescriptorTypeIdAdapter,
    gDll1CFObjDescriptorExtraSizeAdapter,
};
