#include "main/dll/spell_place.h"

#include "game/objects/object.h"
#include "main/game_ui_interface.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/objprint_render_api.h"

typedef enum VfpSpellPlaceUiEvent {
    VFP_SPELL_PLACE_EVENT_SEQUENCE_A = 0x123,
    VFP_SPELL_PLACE_EVENT_SEQUENCE_B = 0x83B,
} VfpSpellPlaceUiEvent;

static int vfpSpellPlace_getExtraSize(void) {
    return sizeof(SpellPlaceState);
}

static int vfpSpellPlace_getObjectTypeId(void) {
    return 0;
}

static void vfpSpellPlace_free(void) {
}

static void vfpSpellPlace_render(void) {
}

static void vfpSpellPlace_hitDetect(void) {
}

static void vfpSpellPlace_complete(GameObject* obj, SpellPlaceState* state) {
    mainSetBits(state->completionGameBit, 1);
    mainSetBits(state->activationGameBit, 0);
    state->completionLatched = 1;
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
}

static void vfpSpellPlace_update(GameObject* obj) {
    SpellPlaceState* state = obj->extra;

    if (state->completionLatched == 0 && mainGetBit(state->activationGameBit) != 0) {
        obj->anim.resetHitboxFlags &= (u8)~INTERACT_FLAG_DISABLED;
    } else {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    }

    objUpdateHitVolumeTransforms(obj);
    if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) == 0) {
        return;
    }

    switch ((SpellPlaceMode)(u8)(*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot)) {
    case SPELL_PLACE_MODE_SEQUENCE_A:
        if ((*gGameUIInterface)->isItemBeingUsed(VFP_SPELL_PLACE_EVENT_SEQUENCE_A) != 0) {
            vfpSpellPlace_complete(obj, state);
        }
        break;
    case SPELL_PLACE_MODE_SEQUENCE_B:
        if ((*gGameUIInterface)->isItemBeingUsed(VFP_SPELL_PLACE_EVENT_SEQUENCE_B) != 0) {
            vfpSpellPlace_complete(obj, state);
        }
        break;
    }
}

static void vfpSpellPlace_init(GameObject* obj, const SpellPlacePlacement* placement) {
    SpellPlaceState* state = obj->extra;

    state->completionGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->completionGameBit);
    state->activationGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->activationGameBit);
    state->completionLatched = 0;
    obj->anim.rotX = (s16)placement->yawByte * 0x100;
    if (mainGetBit(state->completionGameBit) != 0) {
        state->completionLatched = 1;
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    }
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED | OBJECT_OBJFLAG_HIDDEN;
}

static void vfpSpellPlace_release(void) {
}

static void vfpSpellPlace_initialise(void) {
}

ObjectDescriptor gVfpSpellPlaceObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .initialise = (ObjectDescriptorCallback)vfpSpellPlace_initialise,
    .release = (ObjectDescriptorCallback)vfpSpellPlace_release,
    .init = (ObjectDescriptorCallback)vfpSpellPlace_init,
    .update = (ObjectDescriptorCallback)vfpSpellPlace_update,
    .hitDetect = (ObjectDescriptorCallback)vfpSpellPlace_hitDetect,
    .render = (ObjectDescriptorCallback)vfpSpellPlace_render,
    .free = (ObjectDescriptorCallback)vfpSpellPlace_free,
    .getObjectTypeId = (ObjectDescriptorCallback)vfpSpellPlace_getObjectTypeId,
    .getExtraSize = vfpSpellPlace_getExtraSize,
};
