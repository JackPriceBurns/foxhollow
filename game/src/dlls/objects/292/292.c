/*
 * Death-gas volume family (DLL slot 292 / 0x124).
 *
 * The placement game bit gates the volume. Active volumes manage the heavy-fog
 * state and drain the player's air meter while the player is in range. The
 * deathGasNoF variant suppresses fog and takes its radius from cullDistance2.
 */
#include "dlls/objects/292.h"

#include "main/frame_timing.h"
#include "main/game_ui_interface.h"
#include "main/pi_dolphin_api.h"
#include "main/vecmath_distance_api.h"
#include "sys/objects.h"
#include "main/dll/player_api.h"
#include "main/gamebits_api.h"
#include "main/objhits.h"

#define DEATH_GAS_AIR_METER_CAPACITY              6000
#define DEATH_GAS_AIR_METER_FULL                  6000.0f
#define DEATH_GAS_AIR_METER_BACKGROUND_TEXTURE_ID 0x603
#define DEATH_GAS_DAMAGE_HIT_PRIORITY             0x16
#define DEATH_GAS_NO_FOG_OBJECT_ID                0x837

int DeathGas_getExtraSize(void) {
    return sizeof(DeathGasState);
}

void DeathGas_free(GameObject* obj) {
    DeathGasState* state = obj->extra;
    if (state->flags.fogStateActive) {
        if (!state->flags.suppressFog) {
            disableHeavyFog();
        }
    }
    if (state->flags.airMeterActive) {
        (*gGameUIInterface)->airMeterShutdown();
    }
}

void DeathGas_update(GameObject* obj) {
    DeathGasPlacement* placement = (DeathGasPlacement*)obj->anim.placementData;
    DeathGasState* state = obj->extra;
    GameObject* player;
    u8 isActive;
    int activeGameBit;

    activeGameBit = placement->activeGameBit;
    if (activeGameBit == -1) {
        isActive = 1;
    } else {
        isActive = mainGetBit(activeGameBit);
    }

    if (isActive == 0) {
        if (state->flags.fogStateActive) {
            if (!state->flags.suppressFog) {
                disableHeavyFog();
            }
            state->flags.fogStateActive = 0;
        }
        if (state->flags.airMeterActive) {
            (*gGameUIInterface)->airMeterShutdown();
            state->flags.airMeterActive = 0;
        }
        return;
    }

    if (!state->flags.fogStateActive) {
        if (!state->flags.suppressFog) {
            enableHeavyFog(35.0f + obj->anim.worldPosY, obj->anim.worldPosY - 5.0f, 1000.0f, 0.1f, 0.0005f, 0);
        }
        state->flags.fogStateActive = 1;
    }

    player = Obj_GetPlayerObject();
    if (!playerIsDisguised(player) && player->anim.worldPosY <= 30.0f + obj->anim.worldPosY &&
        Vec_distance(&player->anim.worldPosX, &obj->anim.worldPosX) <= state->effectRadius) {
        if (!state->flags.airMeterActive) {
            (*gGameUIInterface)->initAirMeter(DEATH_GAS_AIR_METER_CAPACITY, DEATH_GAS_AIR_METER_BACKGROUND_TEXTURE_ID);
            state->airRemaining = DEATH_GAS_AIR_METER_FULL;
            state->flags.airMeterActive = 1;
        }
        state->airRemaining -= (timeDelta * placement->drainRate) / 10.0f;
        if (state->airRemaining <= 0.0f) {
            f32 zero = 0.0f;
            state->airRemaining = 0.0f;
            state->damageTimer -= timeDelta;
            if (state->damageTimer < zero) {
                state->damageTimer += 120.0f;
                ObjHits_RecordObjectHit(player, obj, DEATH_GAS_DAMAGE_HIT_PRIORITY, 1, 0);
            }
        }
    } else if (state->flags.airMeterActive) {
        state->airRemaining += (timeDelta * placement->fillRate) / 10.0f;
        if (state->airRemaining > DEATH_GAS_AIR_METER_FULL) {
            (*gGameUIInterface)->airMeterSetShutdown();
            state->flags.airMeterActive = 0;
        }
    }

    if (state->flags.airMeterActive) {
        (*gGameUIInterface)->runAirMeter((int)state->airRemaining);
    }
}

void DeathGas_init(GameObject* obj) {
    register DeathGasState* state = obj->extra;
    obj->objectFlags = (u16)(obj->objectFlags | OBJECT_OBJFLAG_HIDDEN);
    state->effectRadius = 10000.0f;
    if (obj->anim.romDefNo != DEATH_GAS_NO_FOG_OBJECT_ID) {
        return;
    }
    state->flags.suppressFog = 1;
    state->effectRadius = obj->anim.cullDistance2;
}

ObjectDescriptor gDeathGasObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)DeathGas_init,
    (ObjectDescriptorCallback)DeathGas_update,
    0,
    0,
    (ObjectDescriptorCallback)DeathGas_free,
    0,
    DeathGas_getExtraSize,
};
