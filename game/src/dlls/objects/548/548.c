#include "dlls/object_descriptor.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/game_ui_interface.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/objprint_render.h"
#include "main/vecmath_distance.h"
#include "sys/objects.h"

typedef enum SpellStoneUseMode {
    SPELL_STONE_USE_MODE_A = 1,
    SPELL_STONE_USE_MODE_B = 2,
    SPELL_STONE_USE_MODE_C = 3,
} SpellStoneUseMode;

typedef enum SpellStoneUseUiEvent {
    SPELL_STONE_USE_EVENT_A = 0x123,
    SPELL_STONE_USE_EVENT_B = 0x83B,
    SPELL_STONE_USE_EVENT_C = 0x83C,
} SpellStoneUseUiEvent;

typedef struct SpellStoneUsePlacement {
    ObjPlacement base;
    s8 rotXByte;
    u8 pad19[5];
    s16 completionGameBit;
    s16 requiredGameBit;
} SpellStoneUsePlacement;

typedef struct SpellStoneUseState {
    s16 completionGameBit;
    s16 requiredGameBit;
    u8 used;
    u8 pad05;
} SpellStoneUseState;

STATIC_ASSERT(sizeof(SpellStoneUsePlacement) == 0x24);
STATIC_ASSERT(offsetof(SpellStoneUsePlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(SpellStoneUsePlacement, completionGameBit) == 0x1E);
STATIC_ASSERT(offsetof(SpellStoneUsePlacement, requiredGameBit) == 0x20);

STATIC_ASSERT(sizeof(SpellStoneUseState) == 0x06);
STATIC_ASSERT(offsetof(SpellStoneUseState, completionGameBit) == 0x00);
STATIC_ASSERT(offsetof(SpellStoneUseState, requiredGameBit) == 0x02);
STATIC_ASSERT(offsetof(SpellStoneUseState, used) == 0x04);

static void spellStoneUse_updateInteraction(GameObject* obj, SpellStoneUseUiEvent event) {
    SpellStoneUseState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();
    s16 requirementMet = 1;

    if (player == NULL) {
        return;
    }
    if (state->requiredGameBit != -1) {
        requirementMet = mainGetBit(state->requiredGameBit);
    }
    if ((s16)mainGetBit(state->completionGameBit) != 0 || state->used != 0 || requirementMet == 0) {
        return;
    }

    obj->anim.resetHitboxFlags &= (u8)~INTERACT_FLAG_DISABLED;
    if ((*gGameUIInterface)->isItemBeingUsed(event) != 0 &&
        Vec_distance(&obj->anim.worldPosX, &player->anim.worldPosX) < 100.0f) {
        mainSetBits(state->completionGameBit, 1);
        state->used = 1;
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    }
}

static int spellStoneUse_getExtraSize(void) {
    return sizeof(SpellStoneUseState);
}

static int spellStoneUse_getObjectTypeId(void) {
    return 0;
}

static void spellStoneUse_free(void) {
}

static void spellStoneUse_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                 s8 visible) {
    (void)obj;
    (void)renderArg2;
    (void)renderArg3;
    (void)renderArg4;
    (void)renderArg5;
    (void)visible;
}

static void spellStoneUse_hitDetect(GameObject* obj) {
    if (obj->anim.hitVolumeTransforms != NULL) {
        objUpdateHitVolumeTransforms(obj);
    }
}

static void spellStoneUse_update(GameObject* obj) {
    SpellStoneUseUiEvent event = SPELL_STONE_USE_EVENT_A;

    switch ((SpellStoneUseMode)(u8)(*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot)) {
    case SPELL_STONE_USE_MODE_A:
        break;
    case SPELL_STONE_USE_MODE_B:
        event = SPELL_STONE_USE_EVENT_B;
        break;
    case SPELL_STONE_USE_MODE_C:
        event = SPELL_STONE_USE_EVENT_C;
        break;
    }
    spellStoneUse_updateInteraction(obj, event);
}

static void spellStoneUse_init(GameObject* obj, const SpellStoneUsePlacement* placement) {
    SpellStoneUseState* state = obj->extra;

    obj->anim.rotX = (s16)placement->rotXByte * 0x100;
    state->completionGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->completionGameBit);
    state->requiredGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->requiredGameBit);
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
}

static void spellStoneUse_release(void) {
}

static void spellStoneUse_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSpellStoneUseObjDescriptorInitAdapter, spellStoneUse_init, obj, placement)
OBJECT_FREE_ADAPTER(gSpellStoneUseObjDescriptorFreeAdapter, spellStoneUse_free)
OBJECT_TYPE_ID_ADAPTER(gSpellStoneUseObjDescriptorTypeIdAdapter, spellStoneUse_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSpellStoneUseObjDescriptorExtraSizeAdapter, spellStoneUse_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gSpellStoneUseObjDescriptorAcquire, spellStoneUse_initialise)

ObjectDescriptor gSpellStoneUseObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gSpellStoneUseObjDescriptorAcquire,
        .release = spellStoneUse_release,
    },
    .init = gSpellStoneUseObjDescriptorInitAdapter,
    .update = spellStoneUse_update,
    .hitDetect = spellStoneUse_hitDetect,
    .render = spellStoneUse_render,
    .free = gSpellStoneUseObjDescriptorFreeAdapter,
    .getObjectTypeId = gSpellStoneUseObjDescriptorTypeIdAdapter,
    .getExtraSize = gSpellStoneUseObjDescriptorExtraSizeAdapter,
};;
