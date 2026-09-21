/*
 * MMP_Bridge (DLL slot 271).
 *
 * Deploys the Moon Mountain Pass bridge and scrolls its surface texture.
 */
#include "dlls/objects/271_MMP_Bridge.h"
#include "main/debug.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/objtexture.h"
#include "main/objhits.h"

#define MMP_BRIDGE_TEXTURE_SLOT         0
#define MMP_BRIDGE_TEXTURE_INDEX        0
#define MMP_BRIDGE_TEXTURE_OFFSET_START 0x800
#define MMP_BRIDGE_TEXTURE_OFFSET_MAX   0x131F
#define MMP_BRIDGE_TEXTURE_SCROLL_SHIFT 3

int MMP_Bridge_getExtraSize(void) {
    return 0;
}

int MMP_Bridge_getObjectTypeId(void) {
    return 0;
}

void MMP_Bridge_free(GameObject* obj) {
    (void)obj;
}

void MMP_Bridge_render(void) {
}

void MMP_Bridge_hitDetect(void) {
}

void MMP_Bridge_update(GameObject* obj) {
    MMPBridgePlacement* placement = (MMPBridgePlacement*)obj->anim.placementData;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->enableGameBit)) == 0) {
        return;
    }

    ObjTextureRuntimeSlot* texture = objFindTexture(obj, MMP_BRIDGE_TEXTURE_SLOT, MMP_BRIDGE_TEXTURE_INDEX);
    if (texture == NULL) {
        ObjHits_EnableObject(obj);
        return;
    }

    texture->offsetS += (int)timeDelta << MMP_BRIDGE_TEXTURE_SCROLL_SHIFT;
    int nextOffset = texture->offsetS + ((int)timeDelta << MMP_BRIDGE_TEXTURE_SCROLL_SHIFT);
    if (nextOffset >= MMP_BRIDGE_TEXTURE_OFFSET_MAX) {
        texture->offsetS = MMP_BRIDGE_TEXTURE_OFFSET_MAX;
    }
    logPrintf("S %d\n", texture->offsetS);

    ObjHits_EnableObject(obj);
}

void MMP_Bridge_init(GameObject* obj) {
    MMPBridgePlacement* placement = (MMPBridgePlacement*)obj->anim.placementData;
    ObjTextureRuntimeSlot* texture = objFindTexture(obj, MMP_BRIDGE_TEXTURE_SLOT, MMP_BRIDGE_TEXTURE_INDEX);
    if (texture != NULL) {
        texture->offsetS = MMP_BRIDGE_TEXTURE_OFFSET_START;
    }
    obj->anim.rotX = placement->rotXByte << 8;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
    ObjHits_DisableObject(obj);
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->enableGameBit)) != 0) {
        ObjHits_EnableObject(obj);
    }
}

void MMP_Bridge_release(void) {
}

void MMP_Bridge_initialise(void) {
}

OBJECT_INIT_ADAPTER(gMMP_BridgeObjDescriptorInitAdapter, MMP_Bridge_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gMMP_BridgeObjDescriptorHitDetectAdapter, MMP_Bridge_hitDetect)
OBJECT_RENDER_ADAPTER(gMMP_BridgeObjDescriptorRenderAdapter, MMP_Bridge_render)
OBJECT_FREE_ADAPTER(gMMP_BridgeObjDescriptorFreeAdapter, MMP_Bridge_free, obj)
OBJECT_TYPE_ID_ADAPTER(gMMP_BridgeObjDescriptorTypeIdAdapter, MMP_Bridge_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gMMP_BridgeObjDescriptorExtraSizeAdapter, MMP_Bridge_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gMMP_BridgeObjDescriptorAcquire, MMP_Bridge_initialise)

ObjectDescriptor gMMP_BridgeObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gMMP_BridgeObjDescriptorAcquire,
        MMP_Bridge_release,
    },
    0,
    gMMP_BridgeObjDescriptorInitAdapter,
    MMP_Bridge_update,
    gMMP_BridgeObjDescriptorHitDetectAdapter,
    gMMP_BridgeObjDescriptorRenderAdapter,
    gMMP_BridgeObjDescriptorFreeAdapter,
    gMMP_BridgeObjDescriptorTypeIdAdapter,
    gMMP_BridgeObjDescriptorExtraSizeAdapter,
};
