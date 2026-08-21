/*
 * ECSH_Creato (DLL 0x191) - ECSH shrine SharpClaw encounter spawner.
 *
 * A game bit triggers its activation effects and starts a countdown. Once the
 * countdown expires, the creator spawns object ID 0x11, which retail
 * OBJINDEX.bin maps to "sharpclawGr" (DLL 0xC9).
 */
#include "dlls/objects/401_ECSH_Creato.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/baddie_placement.h"
#include "main/dll/baddie_state.h"
#include "main/dll/dll_0082_modgfx.h"
#include "main/frame_timing.h"
#include "main/gamebits_api.h"
#include "main/mm.h"
#include "main/object_render.h"
#include "main/resource.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

typedef struct ECSHCreatorPlacement {
    ObjPlacement base;
    s16 triggerGameBit;
    u8 unk1A[4];
    s8 initialYaw;
    s8 childGameBitOffset;
    u8 hitPointsOffset;
    u8 unk21[3];
} ECSHCreatorPlacement;

typedef struct ECSHCreatorState {
    s16 spawnTimer;
    s16 spawnTimerRate;
    s16 triggerGameBit;
    s16 unk06;
    s16 sharpClawHitPoints;
} ECSHCreatorState;

STATIC_ASSERT(sizeof(ECSHCreatorPlacement) == 0x24);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, triggerGameBit) == 0x18);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, initialYaw) == 0x1E);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, childGameBitOffset) == 0x1F);
STATIC_ASSERT(offsetof(ECSHCreatorPlacement, hitPointsOffset) == 0x20);
STATIC_ASSERT(sizeof(ECSHCreatorState) == 0x0A);
STATIC_ASSERT(offsetof(ECSHCreatorState, triggerGameBit) == 0x04);
STATIC_ASSERT(offsetof(ECSHCreatorState, sharpClawHitPoints) == 0x08);

static int ecshCreator_getExtraSize(void) {
    return sizeof(ECSHCreatorState);
}

static int ecshCreator_getObjectTypeId(void) {
    return 0;
}

static void ecshCreator_free(void) {
}

static void ecshCreator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                               s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void ecshCreator_hitDetect(void) {
}

static void ecshCreator_update(GameObject* obj) {
    const ECSHCreatorPlacement* placement = (const ECSHCreatorPlacement*)obj->anim.placementData;
    ECSHCreatorState* state = obj->extra;

    if (obj->userData2 == 0 && mainGetBit(state->triggerGameBit) != 0) {
        Dll82Interface** effectResource = Resource_Acquire(DLL_82_RESOURCE_ID, 1);

        (*effectResource)->spawn(obj, 0, NULL, 1, -1, NULL);
        (*effectResource)->spawn(obj, 1, NULL, 1, -1, NULL);
        Sfx_PlayFromObject(obj, SFXTRIG_wp_hitpos_6);
        Resource_Release(effectResource);
        state->spawnTimerRate = 1;
        obj->userData2 = 1;
    }

    if (state->spawnTimerRate != 0) {
        state->spawnTimer -= state->spawnTimerRate * framesThisStep;
    }

    if (Obj_IsLoadingLocked() != 0 && state->spawnTimer <= 0) {
        EnemyPlacement* spawnSetup = mmAlloc(sizeof(EnemyPlacement), 0xE, 0);

        spawnSetup->base.posX = placement->base.posX;
        spawnSetup->base.posY = placement->base.posY;
        spawnSetup->base.posZ = placement->base.posZ;
        spawnSetup->base.objectId = 0x11;
        spawnSetup->base.ident = -1;
        spawnSetup->base.color[0] = placement->base.color[0];
        spawnSetup->base.color[1] = placement->base.color[1];
        spawnSetup->base.color[2] = placement->base.color[2];
        spawnSetup->base.color[3] = placement->base.color[3];
        spawnSetup->initialWeaponId = 3;
        spawnSetup->objectFlagBits = 0;
        spawnSetup->gameBit = state->triggerGameBit + placement->childGameBitOffset;
        spawnSetup->unk30 = -1;
        spawnSetup->initialYaw = (s8)(obj->anim.rotX >> 8);
        spawnSetup->flags = 2;
        spawnSetup->unk20 = 0;
        spawnSetup->unk1E = 0;
        spawnSetup->droppedItemId = -1;
        spawnSetup->aggroRangeByte = 0xFF;
        spawnSetup->triggerSequenceId = -1;
        spawnSetup->unk24 = 0;
        spawnSetup->respawnDelay = 0;
        spawnSetup->unk34 = 0xFFFF;
        spawnSetup->gameBit2 = 0;
        spawnSetup->hitPoints = state->sharpClawHitPoints;
        GameObject* sharpClaw = objSetupObject(&spawnSetup->base, 5, obj->anim.mapEventSlot, -1, obj->anim.parent);

        if (sharpClaw != NULL) {
            ((GroundBaddieState*)sharpClaw->extra)->configFlags = GROUND_BADDIE_CONFIG_DISABLE_CAMERA_TARGET;
        }
        state->spawnTimer = 100;
        state->spawnTimerRate = 0;
    }
}

static void ecshCreator_init(GameObject* obj, const ECSHCreatorPlacement* placement) {
    ECSHCreatorState* state = obj->extra;

    obj->anim.rotX = (s16)((s32)placement->initialYaw * 0x100);
    obj->userData2 = 0;
    state->spawnTimer = 100;
    state->spawnTimerRate = 0;
    obj->anim.renderAlpha = 0xFF;
    obj->anim.alpha = 0xFF;
    state->triggerGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->triggerGameBit);
    state->sharpClawHitPoints = 2 + placement->hitPointsOffset;
}

static void ecshCreator_release(void) {
}

static void ecshCreator_initialise(void) {
}

ObjectDescriptor gECSHCreatorObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .initialise = (ObjectDescriptorCallback)ecshCreator_initialise,
    .release = (ObjectDescriptorCallback)ecshCreator_release,
    .init = (ObjectDescriptorCallback)ecshCreator_init,
    .update = (ObjectDescriptorCallback)ecshCreator_update,
    .hitDetect = (ObjectDescriptorCallback)ecshCreator_hitDetect,
    .render = (ObjectDescriptorCallback)ecshCreator_render,
    .free = (ObjectDescriptorCallback)ecshCreator_free,
    .getObjectTypeId = (ObjectDescriptorCallback)ecshCreator_getObjectTypeId,
    .getExtraSize = ecshCreator_getExtraSize,
};
