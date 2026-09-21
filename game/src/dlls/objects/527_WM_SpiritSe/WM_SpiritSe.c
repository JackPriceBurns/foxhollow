/*
 * WM_SpiritSe (DLL 0x020F) - Krazoa Spirit display object.
 *
 * The object is oriented from its placement data and rendered while
 * its visibility game bit is set.
 */
#include "dlls/object_descriptor.h"
#include "main/dll/WM/dll_020F_wmspiritset.h"
#include "main/gamebits.h"
#include "main/object_render.h"

/* Object variant handled by this DLL. */
#define WMSPIRITSET_SEQID_SPIRITSET 0x264

int wmspiritset_getExtraSize(void) {
    return sizeof(WmSpiritSetState);
}

int wmspiritset_getObjectTypeId(void) {
    return 0x0;
}

void wmspiritset_free(void) {
}

void wmspiritset_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible) {
    if (visible == 0) {
        return;
    }

    WmSpiritSetState* state = obj->extra;
    if (state->visibilityGameBit == -1 || mainGetBit(state->visibilityGameBit) != 0) {
        objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
    }
}

void wmspiritset_hitDetect(void) {
}

void wmspiritset_update(void) {
}

void wmspiritset_init(GameObject* obj, WmSpiritSetMapData* mapData) {
    WmSpiritSetState* state = obj->extra;
    obj->anim.rotX = mapData->rotXByte << 8;
    if (obj->anim.romDefNo == WMSPIRITSET_SEQID_SPIRITSET) {
        obj->anim.rootMotionScale = 0.0085f;
    }
    state->visibilityGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &(mapData->visibilityGameBit));
}

void wmspiritset_release(void) {
}

void wmspiritset_initialise(void) {
}

OBJECT_INIT_ADAPTER(gWM_SpiritSetObjDescriptorInitAdapter, wmspiritset_init, obj, placement)
OBJECT_UPDATE_ADAPTER(gWM_SpiritSetObjDescriptorUpdateAdapter, wmspiritset_update)
OBJECT_HIT_DETECT_ADAPTER(gWM_SpiritSetObjDescriptorHitDetectAdapter, wmspiritset_hitDetect)
OBJECT_FREE_ADAPTER(gWM_SpiritSetObjDescriptorFreeAdapter, wmspiritset_free)
OBJECT_TYPE_ID_ADAPTER(gWM_SpiritSetObjDescriptorTypeIdAdapter, wmspiritset_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gWM_SpiritSetObjDescriptorExtraSizeAdapter, wmspiritset_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gWM_SpiritSetObjDescriptorAcquire, wmspiritset_initialise)

ObjectDescriptor gWM_SpiritSetObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gWM_SpiritSetObjDescriptorAcquire,
        wmspiritset_release,
    },
    0,
    gWM_SpiritSetObjDescriptorInitAdapter,
    gWM_SpiritSetObjDescriptorUpdateAdapter,
    gWM_SpiritSetObjDescriptorHitDetectAdapter,
    wmspiritset_render,
    gWM_SpiritSetObjDescriptorFreeAdapter,
    gWM_SpiritSetObjDescriptorTypeIdAdapter,
    gWM_SpiritSetObjDescriptorExtraSizeAdapter,
};
