/*
 * Death-gas volume family (DLL slot 292 / 0x124).
 *
 * The placement game bit gates the volume. Active volumes manage the heavy-fog
 * state and drain the player's air meter while the player is in range. The
 * deathGasNoF variant suppresses fog and takes its radius from cullDistance2.
 */
#include "dlls/objects/292.h"

#include "game/objects/object_setup.h"
#include "main/frame_timing.h"
#include "main/game_ui_interface.h"
#include "main/pi_dolphin.h"
#include "main/vecmath_distance.h"
#include "sys/objects.h"
#include "main/dll/player.h"
#include "main/gamebits.h"
#include "main/objhits.h"

typedef enum DeathGasFlags {
    DEATH_GAS_FLAG_SUPPRESS_FOG = 1 << 5,
    DEATH_GAS_FLAG_AIR_METER_ACTIVE = 1 << 6,
    DEATH_GAS_FLAG_FOG_STATE_ACTIVE = 1 << 7,
} DeathGasFlags;

typedef enum DeathGasObjectId {
    DEATH_GAS_NO_FOG_OBJECT_ID = 0x837,
} DeathGasObjectId;

typedef struct DeathGasPlacement {
    ObjPlacement base;
    u8 drainRate;
    u8 fillRate;
    s16 activeGameBit;
} DeathGasPlacement;

typedef struct DeathGasState {
    f32 airRemaining;
    f32 damageTimer;
    f32 effectRadius;
    u8 flags;
    u8 padding[3];
} DeathGasState;

STATIC_ASSERT(offsetof(DeathGasPlacement, drainRate) == 0x18);
STATIC_ASSERT(offsetof(DeathGasPlacement, fillRate) == 0x19);
STATIC_ASSERT(offsetof(DeathGasPlacement, activeGameBit) == 0x1A);
STATIC_ASSERT(sizeof(DeathGasState) == 0x10);
STATIC_ASSERT(offsetof(DeathGasState, flags) == 0x0C);

int DeathGas_getExtraSize(void) {
    return sizeof(DeathGasState);
}

void DeathGas_free(GameObject* obj) {
    DeathGasState* state = obj->extra;
    if ((state->flags & DEATH_GAS_FLAG_FOG_STATE_ACTIVE) != 0 &&
        (state->flags & DEATH_GAS_FLAG_SUPPRESS_FOG) == 0) {
        disableHeavyFog();
    }
    if ((state->flags & DEATH_GAS_FLAG_AIR_METER_ACTIVE) != 0) {
        (*gGameUIInterface)->airMeterShutdown();
    }
}

void DeathGas_update(GameObject* obj) {
    const DeathGasPlacement* placement = (const DeathGasPlacement*)obj->anim.placementData;
    DeathGasState* state = obj->extra;
    s16 activeGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->activeGameBit);
    u8 isActive = activeGameBit == -1 || mainGetBit(activeGameBit) != 0;

    if (isActive == 0) {
        if ((state->flags & DEATH_GAS_FLAG_FOG_STATE_ACTIVE) != 0) {
            if ((state->flags & DEATH_GAS_FLAG_SUPPRESS_FOG) == 0) {
                disableHeavyFog();
            }
            state->flags &= (u8)~DEATH_GAS_FLAG_FOG_STATE_ACTIVE;
        }
        if ((state->flags & DEATH_GAS_FLAG_AIR_METER_ACTIVE) != 0) {
            (*gGameUIInterface)->airMeterShutdown();
            state->flags &= (u8)~DEATH_GAS_FLAG_AIR_METER_ACTIVE;
        }
        return;
    }

    if ((state->flags & DEATH_GAS_FLAG_FOG_STATE_ACTIVE) == 0) {
        if ((state->flags & DEATH_GAS_FLAG_SUPPRESS_FOG) == 0) {
            enableHeavyFog(35.0f + obj->anim.worldPosY, obj->anim.worldPosY - 5.0f, 1000.0f, 0.1f, 0.0005f, 0);
        }
        state->flags |= DEATH_GAS_FLAG_FOG_STATE_ACTIVE;
    }

    GameObject* player = Obj_GetPlayerObject();
    if (!playerIsDisguised(player) && player->anim.worldPosY <= 30.0f + obj->anim.worldPosY &&
        Vec_distance(&player->anim.worldPosX, &obj->anim.worldPosX) <= state->effectRadius) {
        if ((state->flags & DEATH_GAS_FLAG_AIR_METER_ACTIVE) == 0) {
            (*gGameUIInterface)->initAirMeter(6000, 0x603);
            state->airRemaining = 6000.0f;
            state->flags |= DEATH_GAS_FLAG_AIR_METER_ACTIVE;
        }
        state->airRemaining -= (timeDelta * placement->drainRate) / 10.0f;
        if (state->airRemaining <= 0.0f) {
            state->airRemaining = 0.0f;
            state->damageTimer -= timeDelta;
            if (state->damageTimer < 0.0f) {
                state->damageTimer += 120.0f;
                ObjHits_RecordObjectHit(player, obj, 0x16, 1, 0);
            }
        }
    } else if ((state->flags & DEATH_GAS_FLAG_AIR_METER_ACTIVE) != 0) {
        state->airRemaining += (timeDelta * placement->fillRate) / 10.0f;
        if (state->airRemaining > 6000.0f) {
            (*gGameUIInterface)->airMeterSetShutdown();
            state->flags &= (u8)~DEATH_GAS_FLAG_AIR_METER_ACTIVE;
        }
    }

    if ((state->flags & DEATH_GAS_FLAG_AIR_METER_ACTIVE) != 0) {
        (*gGameUIInterface)->runAirMeter((int)state->airRemaining);
    }
}

void DeathGas_init(GameObject* obj) {
    DeathGasState* state = obj->extra;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
    state->effectRadius = 10000.0f;
    if (obj->anim.romDefNo != DEATH_GAS_NO_FOG_OBJECT_ID) {
        return;
    }
    state->flags |= DEATH_GAS_FLAG_SUPPRESS_FOG;
    state->effectRadius = obj->anim.cullDistance2;
}

OBJECT_INIT_ADAPTER(gDeathGasObjDescriptorInitAdapter, DeathGas_init, obj)
OBJECT_FREE_ADAPTER(gDeathGasObjDescriptorFreeAdapter, DeathGas_free, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gDeathGasObjDescriptorExtraSizeAdapter, DeathGas_getExtraSize)

ObjectDescriptor gDeathGasObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        0,
        0,
    },
    0,
    gDeathGasObjDescriptorInitAdapter,
    DeathGas_update,
    0,
    0,
    gDeathGasObjDescriptorFreeAdapter,
    0,
    gDeathGasObjDescriptorExtraSizeAdapter,
};
