/*
 * MMP_Bridge (DLL slot 271).
 *
 * Deploys the Moon Mountain Pass bridge and scrolls its surface texture.
 */
#include "dlls/objects/271_MMP_Bridge.h"

#include "main/debug.h"
#include "main/frame_timing.h"
#include "main/gamebits_api.h"
#include "main/objtexture.h"
#include "main/objhits.h"

char sMMPBridgeTextureOffsetDebugFormat[] = "S %d\n";

#define MMP_BRIDGE_TEXTURE_SLOT         0
#define MMP_BRIDGE_TEXTURE_INDEX        0
#define MMP_BRIDGE_TEXTURE_OFFSET_START 0x800
#define MMP_BRIDGE_TEXTURE_OFFSET_MAX   0x131F
#define MMP_BRIDGE_TEXTURE_SCROLL_SHIFT 3
#define MMP_BRIDGE_ROTATION_SHIFT       8

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
    ObjTextureRuntimeSlot* texture;
    int nextOffset;

    if (mainGetBit(placement->enableGameBit) != 0) {
        texture = objFindTexture(obj, MMP_BRIDGE_TEXTURE_SLOT, MMP_BRIDGE_TEXTURE_INDEX);
        if (texture != NULL) {
            nextOffset = texture->offsetS + ((int)timeDelta << MMP_BRIDGE_TEXTURE_SCROLL_SHIFT);
            texture->offsetS = nextOffset;
            nextOffset = texture->offsetS + ((int)timeDelta << MMP_BRIDGE_TEXTURE_SCROLL_SHIFT);
            if (nextOffset >= MMP_BRIDGE_TEXTURE_OFFSET_MAX) {
                texture->offsetS = MMP_BRIDGE_TEXTURE_OFFSET_MAX;
            }
            logPrintf(sMMPBridgeTextureOffsetDebugFormat, texture->offsetS);
        }
        ObjHits_EnableObject(obj);
    }
}

void MMP_Bridge_init(GameObject* obj) {
    MMPBridgePlacement* placement = (MMPBridgePlacement*)obj->anim.placementData;
    ObjTextureRuntimeSlot* texture = objFindTexture(obj, MMP_BRIDGE_TEXTURE_SLOT, MMP_BRIDGE_TEXTURE_INDEX);

    if (texture != NULL) {
        texture->offsetS = MMP_BRIDGE_TEXTURE_OFFSET_START;
    }
    obj->anim.rotX = (s16)(placement->rotXByte << MMP_BRIDGE_ROTATION_SHIFT);
    obj->objectFlags |= (OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED);
    ObjHits_DisableObject(obj);
    if (mainGetBit(placement->enableGameBit) != 0) {
        ObjHits_EnableObject(obj);
    }
}

void MMP_Bridge_release(void) {
}

void MMP_Bridge_initialise(void) {
}

ObjectDescriptor gMMP_BridgeObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)MMP_Bridge_initialise,
    (ObjectDescriptorCallback)MMP_Bridge_release,
    0,
    (ObjectDescriptorCallback)MMP_Bridge_init,
    (ObjectDescriptorCallback)MMP_Bridge_update,
    (ObjectDescriptorCallback)MMP_Bridge_hitDetect,
    (ObjectDescriptorCallback)MMP_Bridge_render,
    (ObjectDescriptorCallback)MMP_Bridge_free,
    (ObjectDescriptorCallback)MMP_Bridge_getObjectTypeId,
    MMP_Bridge_getExtraSize,
};
