/*
 * SB_ShipGunB (DLL 0x01F7) - the wrecked variant of the galleon's deck
 * gun (SB_ShipGun) in the ShipBattle prologue (SB = the retail "ShipBattle"
 * map), shown after the gun has been shot out.
 *
 * It is purely cosmetic: a static prop that is only rendered (and plays a
 * looping electrical-damage sfx) while a placement-supplied GameBit is set.
 * That GameBit is the gun's "destroyed" flag - its index is stored in the
 * placement record at offset 0x1E.
 */
#include "dlls/objects/503_SB_ShipGunB.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/gamebits.h"
#include "main/object_render.h"

int SB_ShipGunBroke_getExtraSize(void) {
    return sizeof(SBShipGunBrokeState);
}

int SB_ShipGunBroke_getObjectTypeId(void) {
    return 0;
}

void SB_ShipGunBroke_free(void) {
}

void SB_ShipGunBroke_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5) {
    SBShipGunBrokePlacementView* placement = (SBShipGunBrokePlacementView*)obj->anim.placementData;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->destroyedGameBit)) != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void SB_ShipGunBroke_hitDetect(void) {
}

void SB_ShipGunBroke_update(GameObject* obj) {
    SBShipGunBrokePlacementView* placement = (SBShipGunBrokePlacementView*)obj->anim.placementData;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->destroyedGameBit)) != 0) {
        Sfx_PlayFromObject(obj, SFXTRIG_en_trpopn_c);
    }
}

void SB_ShipGunBroke_init(void) {
}

void SB_ShipGunBroke_release(void) {
}

void SB_ShipGunBroke_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSB_ShipGunBrokeObjDescriptorInitAdapter, SB_ShipGunBroke_init)
OBJECT_HIT_DETECT_ADAPTER(gSB_ShipGunBrokeObjDescriptorHitDetectAdapter, SB_ShipGunBroke_hitDetect)
OBJECT_RENDER_ADAPTER(gSB_ShipGunBrokeObjDescriptorRenderAdapter, SB_ShipGunBroke_render, obj, arg2, arg3, arg4, arg5)
OBJECT_FREE_ADAPTER(gSB_ShipGunBrokeObjDescriptorFreeAdapter, SB_ShipGunBroke_free)
OBJECT_TYPE_ID_ADAPTER(gSB_ShipGunBrokeObjDescriptorTypeIdAdapter, SB_ShipGunBroke_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSB_ShipGunBrokeObjDescriptorExtraSizeAdapter, SB_ShipGunBroke_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gSB_ShipGunBrokeObjDescriptorAcquire, SB_ShipGunBroke_initialise)

ObjectDescriptor gSB_ShipGunBrokeObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gSB_ShipGunBrokeObjDescriptorAcquire,
        SB_ShipGunBroke_release,
    },
    0,
    gSB_ShipGunBrokeObjDescriptorInitAdapter,
    SB_ShipGunBroke_update,
    gSB_ShipGunBrokeObjDescriptorHitDetectAdapter,
    gSB_ShipGunBrokeObjDescriptorRenderAdapter,
    gSB_ShipGunBrokeObjDescriptorFreeAdapter,
    gSB_ShipGunBrokeObjDescriptorTypeIdAdapter,
    gSB_ShipGunBrokeObjDescriptorExtraSizeAdapter,
};
