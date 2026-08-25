#include "main/dll/spell_place.h"

#include "game/objects/object.h"
#include "main/game_ui_interface.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/objprint_render.h"

typedef enum DfpSpellPlaceUiEvent {
    DFP_SPELL_PLACE_EVENT_SEQUENCE_A = 0x2E8,
    DFP_SPELL_PLACE_EVENT_SEQUENCE_B = 0x83C,
} DfpSpellPlaceUiEvent;

typedef enum DfpSpellPlaceMapAct {
    DFP_SPELL_PLACE_MAP_ACT_A = 7,
    DFP_SPELL_PLACE_MAP_ACT_B = 0xD,
} DfpSpellPlaceMapAct;

static int dfpSpellPlace_getExtraSize(void) {
    return sizeof(SpellPlaceState);
}

static int dfpSpellPlace_getObjectTypeId(void) {
    return 0;
}

static void dfpSpellPlace_free(void) {
}

static void dfpSpellPlace_render(void) {
}

static void dfpSpellPlace_hitDetect(void) {
}

static void dfpSpellPlace_complete(GameObject* obj, SpellPlaceState* state) {
    mainSetBits(state->completionGameBit, 1);
    mainSetBits(state->activationGameBit, 0);
    state->completionLatched = 1;
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
}

static void dfpSpellPlace_update(GameObject* obj) {
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
        if ((*gGameUIInterface)->isItemBeingUsed(DFP_SPELL_PLACE_EVENT_SEQUENCE_A) != 0) {
            dfpSpellPlace_complete(obj, state);
        }
        break;
    case SPELL_PLACE_MODE_SEQUENCE_B:
        if ((*gGameUIInterface)->isItemBeingUsed(DFP_SPELL_PLACE_EVENT_SEQUENCE_B) != 0) {
            dfpSpellPlace_complete(obj, state);
            (*gMapEventInterface)->setMapAct(DFP_SPELL_PLACE_MAP_ACT_A, 8);
            (*gMapEventInterface)->setMapAct(DFP_SPELL_PLACE_MAP_ACT_B, 2);
        }
        break;
    }
}

static void dfpSpellPlace_init(GameObject* obj, const SpellPlacePlacement* placement) {
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

static void dfpSpellPlace_release(void) {
}

static void dfpSpellPlace_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDfpSpellPlaceObjDescriptorInitAdapter, dfpSpellPlace_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDfpSpellPlaceObjDescriptorHitDetectAdapter, dfpSpellPlace_hitDetect)
OBJECT_RENDER_ADAPTER(gDfpSpellPlaceObjDescriptorRenderAdapter, dfpSpellPlace_render)
OBJECT_FREE_ADAPTER(gDfpSpellPlaceObjDescriptorFreeAdapter, dfpSpellPlace_free)
OBJECT_TYPE_ID_ADAPTER(gDfpSpellPlaceObjDescriptorTypeIdAdapter, dfpSpellPlace_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDfpSpellPlaceObjDescriptorExtraSizeAdapter, dfpSpellPlace_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDfpSpellPlaceObjDescriptorAcquire, dfpSpellPlace_initialise)

ObjectDescriptor gDfpSpellPlaceObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gDfpSpellPlaceObjDescriptorAcquire,
        .release = dfpSpellPlace_release,
    },
    .init = gDfpSpellPlaceObjDescriptorInitAdapter,
    .update = dfpSpellPlace_update,
    .hitDetect = gDfpSpellPlaceObjDescriptorHitDetectAdapter,
    .render = gDfpSpellPlaceObjDescriptorRenderAdapter,
    .free = gDfpSpellPlaceObjDescriptorFreeAdapter,
    .getObjectTypeId = gDfpSpellPlaceObjDescriptorTypeIdAdapter,
    .getExtraSize = gDfpSpellPlaceObjDescriptorExtraSizeAdapter,
};;
