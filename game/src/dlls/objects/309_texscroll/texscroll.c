/*
 * Minimal per-placement texture scroller. It copies the placement rates
 * into object state and resets the running offsets on a cold load.
 */
#include "dlls/objects/309_texscroll.h"
#include "main/object_render.h"

int TexScroll_getExtraSize(void) {
    return sizeof(TexScrollState);
}

int TexScroll_getObjectTypeId(void) {
    return 0;
}

void TexScroll_free(void) {
}

void TexScroll_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void TexScroll_hitDetect(void) {
}

void TexScroll_update(void) {
}

void TexScroll_init(GameObject* obj, TexScrollPlacement* placement, int loadFlags) {
    TexScrollState* state = obj->extra;
    if (state == NULL) {
        return;
    }

    state->initLock = 1;
    state->stepX = placement->stepX;
    state->stepY = placement->stepY;
    state->scrollSlot = 0;
    state->flags = 0;
    state->gameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBit);
    if (loadFlags == 0) {
        state->offsetX = 0;
        state->offsetY = 0;
    }
    state->initLock = 0;
}

void TexScroll_release(void) {
}

void TexScroll_initialise(void) {
}

OBJECT_INIT_ADAPTER(gTexscrollObjDescriptorInitAdapter, TexScroll_init, obj, placement, flags)
OBJECT_UPDATE_ADAPTER(gTexscrollObjDescriptorUpdateAdapter, TexScroll_update)
OBJECT_HIT_DETECT_ADAPTER(gTexscrollObjDescriptorHitDetectAdapter, TexScroll_hitDetect)
OBJECT_FREE_ADAPTER(gTexscrollObjDescriptorFreeAdapter, TexScroll_free)
OBJECT_TYPE_ID_ADAPTER(gTexscrollObjDescriptorTypeIdAdapter, TexScroll_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gTexscrollObjDescriptorExtraSizeAdapter, TexScroll_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gTexscrollObjDescriptorAcquire, TexScroll_initialise)

ObjectDescriptor gTexscrollObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gTexscrollObjDescriptorAcquire,
        TexScroll_release,
    },
    0,
    gTexscrollObjDescriptorInitAdapter,
    gTexscrollObjDescriptorUpdateAdapter,
    gTexscrollObjDescriptorHitDetectAdapter,
    TexScroll_render,
    gTexscrollObjDescriptorFreeAdapter,
    gTexscrollObjDescriptorTypeIdAdapter,
    gTexscrollObjDescriptorExtraSizeAdapter,
};
