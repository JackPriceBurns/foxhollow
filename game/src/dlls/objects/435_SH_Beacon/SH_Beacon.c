#include "dlls/objects/435_SH_Beacon.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/dll_00C4_tricky.h"
#include "main/dll_000A_expgfx.h"
#include "main/frame_timing.h"
#include "main/game_ui_interface.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/objfx.h"
#include "main/objseq.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"
#include "main/audio/sfx.h"
#include "main/gameloop_gamebit.h"
#include "main/objhits.h"

enum ShBeaconMode {
    SH_BEACON_MODE_UNLIT,
    SH_BEACON_MODE_LIT,
    SH_BEACON_MODE_IGNITING,
};

enum ShBeaconFlag {
    SH_BEACON_LOOP_SOUND_ACTIVE = 1 << 7,
};

enum ShBeaconObjectId {
    SH_BEACON_TWINKLE_OBJECT_ID = 0x55,
};

typedef struct ShBeaconPlacement {
    ObjPlacement base;
    s8 initialRotationX;
    u8 unused19[5];
    s16 litGameBit;
    s16 igniteGameBit;
    u8 unused22[2];
} ShBeaconPlacement;

typedef struct ShBeaconTwinklePlacement {
    ObjPlacement base;
    u8 unused18[8];
} ShBeaconTwinklePlacement;

typedef struct ShBeaconState {
    GameObject* twinkleObject;
    f32 pulseTimer;
    f32 fadeTimer;
    f32 pulseSpawnTimer;
    f32 pulseModeTimer;
    u8 mode;
    u8 flags;
    u8 unused1A[2];
} ShBeaconState;

STATIC_ASSERT(offsetof(ShBeaconPlacement, initialRotationX) == sizeof(ObjPlacement));
STATIC_ASSERT(offsetof(ShBeaconPlacement, litGameBit) == sizeof(ObjPlacement) + 6);
STATIC_ASSERT(offsetof(ShBeaconPlacement, igniteGameBit) == sizeof(ObjPlacement) + 8);
STATIC_ASSERT(sizeof(ShBeaconPlacement) == 0x24);
STATIC_ASSERT(sizeof(ShBeaconTwinklePlacement) == 0x20);
STATIC_ASSERT(offsetof(ShBeaconState, pulseTimer) == sizeof(GameObject*));
STATIC_ASSERT(offsetof(ShBeaconState, fadeTimer) == offsetof(ShBeaconState, pulseTimer) + sizeof(f32));
STATIC_ASSERT(offsetof(ShBeaconState, mode) == offsetof(ShBeaconState, pulseModeTimer) + sizeof(f32));
STATIC_ASSERT(offsetof(ShBeaconState, flags) == offsetof(ShBeaconState, mode) + 1);
STATIC_ASSERT(sizeof(ShBeaconState) == 0x20);

static f32 sShBeaconHitEffectCooldown;

static GameObject* shBeacon_spawnTwinkle(GameObject* obj, const ShBeaconPlacement* placement) {
    ShBeaconTwinklePlacement* twinklePlacement =
        (ShBeaconTwinklePlacement*)Obj_AllocObjectSetup(sizeof(ShBeaconTwinklePlacement), SH_BEACON_TWINKLE_OBJECT_ID);
    twinklePlacement->base.posX = obj->anim.localPosX;
    twinklePlacement->base.posY = obj->anim.localPosY;
    twinklePlacement->base.posZ = obj->anim.localPosZ;
    twinklePlacement->base.color[0] = 2;
    twinklePlacement->base.color[1] = placement->base.color[1];
    twinklePlacement->base.color[3] = placement->base.color[3];
    return loadObjectAtObject(obj, &twinklePlacement->base);
}

static void shBeacon_updateIgnitionPulse(GameObject* obj, ShBeaconState* state) {
    state->pulseTimer += timeDelta;
    if (state->pulseTimer >= 20.0f) {
        state->pulseTimer -= 20.0f;
        if ((obj->objectFlags & OBJECT_OBJFLAG_RENDERED) != 0) {
            objfx_spawnPulseBurst(obj, obj->anim.rootMotionScale, 0, 2, 0, NULL);
        }
    }
}

static int sh_beacon_sequenceCallback(GameObject* obj) {
    shBeacon_updateIgnitionPulse(obj, obj->extra);
    return 0;
}

int sh_beacon_resetFadeTimerCallback(GameObject* obj, int amount) {
    (void)amount;
    ((ShBeaconState*)obj->extra)->fadeTimer = 6.0f;
    return 1;
}

static int sh_beacon_getExtraSize(void) {
    return sizeof(ShBeaconState);
}

static void sh_beacon_free(GameObject* obj, int keepChild) {
    ShBeaconState* state = obj->extra;
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
    if (keepChild == 0) {
        GameObject* twinkleObject = state->twinkleObject;
        if (twinkleObject != NULL && (twinkleObject->objectFlags & OBJECT_OBJFLAG_FREED) == 0) {
            Obj_FreeObject(twinkleObject);
        }
    }
}

static void sh_beacon_update(GameObject* obj) {
    ShBeaconState* state = obj->extra;
    const ShBeaconPlacement* placement = (const ShBeaconPlacement*)obj->anim.placementData;

    switch (state->mode) {
    case SH_BEACON_MODE_UNLIT:
        if (((obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) != 0) &&
            ((*gGameUIInterface)->isItemBeingUsed(GAMEBIT_ITEM_FireWeed_Count) != 0)) {
            gameBitDecrement(GAMEBIT_ITEM_FireWeed_Count);
            mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->igniteGameBit), 1);
            if (Obj_IsLoadingLocked() != 0) {
                state->twinkleObject = shBeacon_spawnTwinkle(obj, placement);
            }
            (*gObjectTriggerInterface)->runSequence(0, obj, -1);
            state->mode = SH_BEACON_MODE_IGNITING;
        }
        __attribute__((fallthrough));
    case SH_BEACON_MODE_IGNITING:
        shBeacon_updateIgnitionPulse(obj, state);
        break;
    case SH_BEACON_MODE_LIT:
        if ((state->flags & SH_BEACON_LOOP_SOUND_ACTIVE) == 0) {
            Sfx_AddLoopedObjectSound(obj, SFXTRIG_forcecryslp11);
            state->flags |= SH_BEACON_LOOP_SOUND_ACTIVE;
        }
        if ((obj->objectFlags & OBJECT_OBJFLAG_RENDERED) != 0) {
            int pulseMode;
            state->pulseModeTimer += timeDelta;
            if (state->pulseModeTimer > 10.0f) {
                pulseMode = 2;
                state->pulseModeTimer -= 10.0f;
            } else {
                pulseMode = 0;
            }
            state->pulseSpawnTimer += timeDelta;
            if (state->pulseSpawnTimer > 2.0f) {
                state->pulseSpawnTimer -= 2.0f;
                objfx_spawnPulseBurst(obj, obj->anim.rootMotionScale, 2, pulseMode, 0, NULL);
            }
        }
        break;
    }
    if (state->mode != SH_BEACON_MODE_LIT) {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
        if (state->mode == SH_BEACON_MODE_IGNITING) {
            Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, 8);
        } else if ((state->mode == SH_BEACON_MODE_UNLIT) && (mainGetBit(GAMEBIT_ITEM_FireWeed_Count) == 0)) {
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
        } else {
            obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_PROMPT_SUPPRESSED;
        }
        GameObject* tricky = getTrickyObject();
        if ((tricky != NULL) && ((obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) != 0)) {
            TRICKY_INTERFACE(tricky)->sideCommandEnable(tricky, obj, 1, 4);
        }
    } else {
        if ((mainGetBit(GAMEBIT_ITEM_MoonPassKey_Got) != 0) ||
            (ObjAnim_ReadPlacementS16(&obj->anim, &placement->litGameBit) != GAMEBIT_Always1)) {
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        } else {
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
        }
    }
    if (state->fadeTimer > 0.0f) {
        state->fadeTimer -= timeDelta;
        if ((obj->objectFlags & OBJECT_OBJFLAG_RENDERED) != 0) {
            objfx_spawnPulseBurst(obj, 0.6f * obj->anim.rootMotionScale, 3, 0, 0, NULL);
        }
        if ((state->fadeTimer <= 0.0f) && (state->mode == SH_BEACON_MODE_IGNITING)) {
            state->mode = SH_BEACON_MODE_LIT;
            mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->litGameBit), 1);
            if ((mainGetBit(GAMEBIT_SH_FireWeed_190) != 0) && (mainGetBit(GAMEBIT_SH_FireWeed_191) != 0) &&
                (mainGetBit(GAMEBIT_SH_FireWeed_192) != 0)) {
                Sfx_PlayFromObject(0, SFXTRIG_mpick1_b);
            } else {
                Sfx_PlayFromObject(0, SFXTRIG_sc_menuups16k_409);
            }
        }
    }
    ObjHits_PollPriorityHitEffectWithCooldown(obj, 8, 0xff, 0xff, 0x78, SFXTRIG_swdtest222,
                                              &sShBeaconHitEffectCooldown);
}

static void sh_beacon_init(GameObject* obj, const ShBeaconPlacement* placement) {
    ShBeaconState* state = obj->extra;

    obj->anim.rotX = (s16)((s16)placement->initialRotationX * 0x100);
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;

    state->mode = mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->litGameBit));
    if (state->mode == SH_BEACON_MODE_UNLIT) {
        if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->igniteGameBit)) != 0) {
            state->mode = SH_BEACON_MODE_IGNITING;
        }
    }

    if (state->mode != SH_BEACON_MODE_UNLIT && Obj_IsLoadingLocked() != 0) {
        state->twinkleObject = shBeacon_spawnTwinkle(obj, placement);
    }

    obj->animEventCallback = sh_beacon_sequenceCallback;
}

OBJECT_INIT_ADAPTER(gSH_BeaconObjDescriptorInitAdapter, sh_beacon_init, obj, placement)
OBJECT_EXTRA_SIZE_ADAPTER(gSH_BeaconObjDescriptorExtraSizeAdapter, sh_beacon_getExtraSize)

ObjectDescriptor gSH_BeaconObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = NULL,
        .release = NULL,
    },
    .init = gSH_BeaconObjDescriptorInitAdapter,
    .update = sh_beacon_update,
    .free = sh_beacon_free,
    .getExtraSize = gSH_BeaconObjDescriptorExtraSizeAdapter,
};;
