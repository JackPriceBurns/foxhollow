#include "main/crfueltank.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/gamebits.h"
#include "main/maketex_timer_api.h"
#include "main/objhits.h"
#include "sys/objects.h"

typedef enum CrFuelTankObjectId {
    CR_FUEL_TANK_TRIGGER_OBJECT_ID = 0x38C,
} CrFuelTankObjectId;

typedef enum CrFuelTankHitVolume {
    CR_FUEL_TANK_HIT_VOLUME_SLOT = 0x1D,
} CrFuelTankHitVolume;

typedef enum CrFuelTankTiming {
    CR_FUEL_TANK_RESPAWN_DURATION = 0x708,
} CrFuelTankTiming;

typedef struct CrFuelTankPlacement {
    ObjPlacement base;
    u8 pad18[2];
    s16 idleFrameCount;
    u8 pad1C[2];
    s16 destructionGameBit;
} CrFuelTankPlacement;

typedef struct CrFuelTankState {
    u8 pad00[0x0C];
    f32 respawnTimer;
} CrFuelTankState;

STATIC_ASSERT(sizeof(CrFuelTankPlacement) == 0x20);
STATIC_ASSERT(offsetof(CrFuelTankPlacement, idleFrameCount) == 0x1A);
STATIC_ASSERT(offsetof(CrFuelTankPlacement, destructionGameBit) == 0x1E);

STATIC_ASSERT(sizeof(CrFuelTankState) == 0x10);
STATIC_ASSERT(offsetof(CrFuelTankState, respawnTimer) == 0x0C);

static int crFuelTank_hitVolumeMode(GameObject* obj, const CrFuelTankPlacement* placement) {
    return ObjAnim_ReadPlacementS16(&obj->anim, &placement->idleFrameCount) / 10;
}

static int crFuelTank_getExtraSize(void) {
    return sizeof(CrFuelTankState);
}

static int crFuelTank_getObjectTypeId(void) {
    return 0;
}

static void crFuelTank_free(void) {
}

static void crFuelTank_render(void) {
}

static void crFuelTank_hitDetect(GameObject* obj) {
    const CrFuelTankPlacement* placement = (const CrFuelTankPlacement*)obj->anim.placementData;
    ObjHitsPriorityState* hitState = ObjAnim_GetPriorityHitState(&obj->anim);
    s16 destructionGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->destructionGameBit);

    if (hitState == NULL || hitState->lastHitObject == 0) {
        return;
    }

    GameObject* hitObj = (GameObject*)hitState->lastHitObject;
    if (hitObj->anim.romDefNo != CR_FUEL_TANK_TRIGGER_OBJECT_ID) {
        return;
    }

    ObjHits_DisableObject(obj);
    Sfx_PlayFromObject(Obj_GetPlayerObject(), SFXTRIG_ar_barrel16);
    obj->anim.alpha = 0xFA;
    obj->userData2 = 1;
    if (destructionGameBit != -1) {
        mainSetBits(destructionGameBit, 1);
    }
    obj->anim.velocity = hitObj->anim.velocity;
    obj->anim.velocity.y += 0.07f;
}

static void crFuelTank_update(GameObject* obj) {
    const CrFuelTankPlacement* placement = (const CrFuelTankPlacement*)obj->anim.placementData;
    CrFuelTankState* state = obj->extra;

    if (timerIsActive(&state->respawnTimer) != 0) {
        if (timerCountDown(&state->respawnTimer) != 0) {
            ObjHits_EnableObject(obj);
            obj->anim.flags &= (s16)~OBJANIM_FLAG_HIDDEN;
            obj->anim.alpha = 0xFF;
        }
    } else if (obj->anim.alpha < 0xFF) {
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        s16toFloat(&state->respawnTimer, CR_FUEL_TANK_RESPAWN_DURATION);
    } else {
        ObjHits_SetHitVolumeSlot(&obj->anim, CR_FUEL_TANK_HIT_VOLUME_SLOT,
                                 crFuelTank_hitVolumeMode(obj, placement), 0);
    }
}

static void crFuelTank_init(GameObject* obj, const CrFuelTankPlacement* placement) {
    CrFuelTankState* state = obj->extra;
    s16 destructionGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->destructionGameBit);

    ObjHits_EnableObject(obj);
    ObjHits_SetHitVolumeSlot(&obj->anim, CR_FUEL_TANK_HIT_VOLUME_SLOT,
                             crFuelTank_hitVolumeMode(obj, placement), 0);
    storeZeroToFloatParam(&state->respawnTimer);
    if (destructionGameBit != -1 && mainGetBit(destructionGameBit) != 0) {
        s16toFloat(&state->respawnTimer, CR_FUEL_TANK_RESPAWN_DURATION);
        ObjHits_DisableObject(obj);
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        obj->anim.alpha = 0;
    }
}

static void crFuelTank_release(void) {
}

static void crFuelTank_initialise(void) {
}

ObjectDescriptor gCrFuelTankObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .initialise = (ObjectDescriptorCallback)crFuelTank_initialise,
    .release = (ObjectDescriptorCallback)crFuelTank_release,
    .init = (ObjectDescriptorCallback)crFuelTank_init,
    .update = (ObjectDescriptorCallback)crFuelTank_update,
    .hitDetect = (ObjectDescriptorCallback)crFuelTank_hitDetect,
    .render = (ObjectDescriptorCallback)crFuelTank_render,
    .free = (ObjectDescriptorCallback)crFuelTank_free,
    .getObjectTypeId = (ObjectDescriptorCallback)crFuelTank_getObjectTypeId,
    .getExtraSize = crFuelTank_getExtraSize,
};
