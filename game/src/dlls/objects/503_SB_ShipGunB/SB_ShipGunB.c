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

#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/gamebits_api.h"
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
    const SBShipGunBrokePlacementView* placement = (const SBShipGunBrokePlacementView*)obj->anim.placementData;

    if (mainGetBit(placement->destroyedGameBit) != 0u) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void SB_ShipGunBroke_hitDetect(void) {
}

void SB_ShipGunBroke_update(GameObject* obj) {
    const SBShipGunBrokePlacementView* placement = (const SBShipGunBrokePlacementView*)obj->anim.placementData;

    if (mainGetBit(placement->destroyedGameBit) != 0u) {
        Sfx_PlayFromObject(obj, SFXTRIG_en_trpopn_c);
    }
}

void SB_ShipGunBroke_init(void) {
}

void SB_ShipGunBroke_release(void) {
}

void SB_ShipGunBroke_initialise(void) {
}

ObjectDescriptor gSB_ShipGunBrokeObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    SB_ShipGunBroke_initialise,
    SB_ShipGunBroke_release,
    0,
    SB_ShipGunBroke_init,
    (ObjectDescriptorCallback)SB_ShipGunBroke_update,
    SB_ShipGunBroke_hitDetect,
    (ObjectDescriptorCallback)SB_ShipGunBroke_render,
    SB_ShipGunBroke_free,
    (ObjectDescriptorCallback)SB_ShipGunBroke_getObjectTypeId,
    SB_ShipGunBroke_getExtraSize,
};
