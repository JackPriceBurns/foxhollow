#include "dlls/objects/410.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/baddie_placement.h"
#include "main/dll/baddie_state.h"
#include "main/dll/dll_0082_modgfx.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/object_render.h"
#include "main/resource.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

enum Dll410ObjectId {
    DLL410_CHILD_OBJECT = 0x2D0,
};

enum Dll410DroppedItemId {
    DLL410_CHILD_DROPPED_ITEM = 0x49,
};

typedef struct Dll410Placement {
    ObjPlacement base;
    u8 pad18[6];
    s8 initialYaw;
    s8 triggerGameBitOffset;
} Dll410Placement;

typedef struct Dll410State {
    s16 spawnTimer;
    s16 spawnTimerRate;
} Dll410State;

STATIC_ASSERT(sizeof(Dll410Placement) == 0x20);
STATIC_ASSERT(offsetof(Dll410Placement, initialYaw) == 0x1E);
STATIC_ASSERT(offsetof(Dll410Placement, triggerGameBitOffset) == 0x1F);

STATIC_ASSERT(sizeof(Dll410State) == 0x04);
STATIC_ASSERT(offsetof(Dll410State, spawnTimer) == 0x00);
STATIC_ASSERT(offsetof(Dll410State, spawnTimerRate) == 0x02);

static int dll410_getExtraSize(void) {
    return sizeof(Dll410State);
}

static int dll410_getObjectTypeId(void) {
    return 0;
}

static void dll410_free(void) {
}

static void dll410_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void dll410_hitDetect(void) {
}

static void dll410_update(GameObject* obj) {
    const Dll410Placement* placement = (const Dll410Placement*)obj->anim.placementData;
    Dll410State* state = obj->extra;

    if (mainGetBit(GAMEBIT_DLL410_Reset) != 0) {
        obj->userData2 = 0;
        state->spawnTimer = 100;
        state->spawnTimerRate = 0;
        obj->anim.renderAlpha = 0xFF;
        obj->anim.alpha = 0xFF;
        return;
    }

    if (obj->userData2 == 0 && mainGetBit(placement->triggerGameBitOffset + GAMEBIT_DLL410_TriggerBase) != 0) {
        Dll82Interface** effectResource = Resource_Acquire(DLL_82_RESOURCE_ID, 1);

        (*effectResource)->spawn(obj, 0, NULL, 1, -1, NULL);
        (*effectResource)->spawn(obj, 1, NULL, 1, -1, NULL);
        Sfx_PlayFromObject(obj, SFXTRIG_hitpos_6);
        Resource_Release(effectResource);
        state->spawnTimerRate = 1;
        obj->userData2 = 1;
    }

    if (state->spawnTimerRate != 0) {
        state->spawnTimer -= state->spawnTimerRate * framesThisStep;
    }
    if (state->spawnTimer > 0 || Obj_IsLoadingLocked() == 0) {
        return;
    }

    EnemyPlacement* spawnSetup = (EnemyPlacement*)Obj_AllocObjectSetup(sizeof(EnemyPlacement), DLL410_CHILD_OBJECT);

    spawnSetup->base.posX = placement->base.posX;
    spawnSetup->base.posY = placement->base.posY;
    spawnSetup->base.posZ = placement->base.posZ;
    for (int channel = 0; channel < ARRAY_COUNT(spawnSetup->base.color); channel++) {
        spawnSetup->base.color[channel] = placement->base.color[channel];
    }
    spawnSetup->initialWeaponId = 1;
    spawnSetup->gameBit = GAMEBIT_SHARPCLAW_STATE_01E7;
    spawnSetup->unk30 = -1;
    spawnSetup->initialYaw = (s8)(obj->anim.rotX >> 8);
    spawnSetup->flags = 2;
    if (mainGetBit(GAMEBIT_DLL410_DroppedItem) != 0) {
        spawnSetup->droppedItemId = DLL410_CHILD_DROPPED_ITEM;
    } else {
        spawnSetup->droppedItemId = -1;
    }
    spawnSetup->aggroRangeByte = 0xFF;
    spawnSetup->triggerSequenceId = -1;
    spawnSetup->hitPoints = (u8)placement->triggerGameBitOffset;

    GameObject* child = objSetupObject(&spawnSetup->base, 5, obj->anim.mapEventSlot, -1, obj->anim.parent);
    if (child != NULL && child->extra != NULL) {
        ((GroundBaddieState*)child->extra)->configFlags = GROUND_BADDIE_CONFIG_DISABLE_CAMERA_TARGET;
    }
    state->spawnTimer = 100;
    state->spawnTimerRate = 0;
}

static void dll410_init(GameObject* obj, const Dll410Placement* placement) {
    Dll410State* state = obj->extra;

    obj->anim.rotX = (s16)(placement->initialYaw * 0x100);
    obj->userData2 = 0;
    state->spawnTimer = 100;
    state->spawnTimerRate = 0;
    obj->anim.renderAlpha = 0xFF;
    obj->anim.alpha = 0xFF;
}

static void dll410_release(void) {
}

static void dll410_initialise(void) {
}

ObjectDescriptor gDll19AObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .initialise = (ObjectDescriptorCallback)dll410_initialise,
    .release = (ObjectDescriptorCallback)dll410_release,
    .init = (ObjectDescriptorCallback)dll410_init,
    .update = (ObjectDescriptorCallback)dll410_update,
    .hitDetect = (ObjectDescriptorCallback)dll410_hitDetect,
    .render = (ObjectDescriptorCallback)dll410_render,
    .free = (ObjectDescriptorCallback)dll410_free,
    .getObjectTypeId = (ObjectDescriptorCallback)dll410_getObjectTypeId,
    .getExtraSize = dll410_getExtraSize,
};
