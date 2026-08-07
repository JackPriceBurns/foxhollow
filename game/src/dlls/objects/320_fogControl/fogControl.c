/* Drives a game-bit-controlled heavy-fog volume. */
#include "dlls/objects/320_fogControl.h"

#include "game/objects/object.h"
#include "main/frame_timing.h"
#include "main/pi_dolphin_api.h"
#include "main/gamebits_api.h"

#define FOG_CONTROL_BLEND_STEP_FAST   0.02f
#define FOG_CONTROL_BLEND_STEP_SLOW   0.005f
#define FOG_CONTROL_DEPTH_DENOMINATOR 65535.0f
#define FOG_CONTROL_WORLD_SCALE       0.0001f

int FogControl_getExtraSize(void) {
    return sizeof(FogControlState);
}

int FogControl_getObjectTypeId(void) {
    return 0;
}

void FogControl_free(GameObject* obj) {
    FogControlState* state = obj->extra;

    if (state->enabled) {
        disableHeavyFog();
    }
}

void FogControl_hitDetect(void) {
}

void FogControl_update(GameObject* obj) {
    FogControlPlacement* placement = (FogControlPlacement*)obj->anim.placementData;
    FogControlState* state = obj->extra;
    u8 gateValue;
    u8 isTransitioning;
    f32 fogTopY;

    if (placement->enableGameBit == -1) {
        gateValue = 1;
    } else {
        gateValue = mainGetBit(placement->enableGameBit);
    }
    if ((gateValue != 0 && state->fullyBlended == 0) || (gateValue == 0 && state->enabled != 0)) {
        isTransitioning = 1;
    } else {
        isTransitioning = 0;
    }
    if (isTransitioning != 0) {
        if (gateValue != 0) {
            if ((placement->flags & FOG_CONTROL_PLACEMENT_SLOW_FADE_IN) != 0) {
                state->blend = FOG_CONTROL_BLEND_STEP_SLOW * timeDelta + state->blend;
            } else {
                state->blend = FOG_CONTROL_BLEND_STEP_FAST * timeDelta + state->blend;
            }
            state->enabled = 1;
        } else {
            if ((placement->flags & FOG_CONTROL_PLACEMENT_SLOW_FADE_OUT) != 0) {
                state->blend = -(FOG_CONTROL_BLEND_STEP_SLOW * timeDelta - state->blend);
            } else {
                state->blend = -(FOG_CONTROL_BLEND_STEP_FAST * timeDelta - state->blend);
            }
            state->fullyBlended = 0;
        }
        if (state->blend <= 0.0f) {
            state->blend = 0.0f;
            state->enabled = 0;
            disableHeavyFog();
        } else {
            state->enabled = 1;
            if (state->blend > 1.0f) {
                state->blend = 1.0f;
                state->fullyBlended = 1;
            }
            fogTopY = state->blend * ((f32)placement->fogTop - (f32)placement->fogBase) + (f32)placement->fogBase;
            fogTopY = obj->anim.localPosY + fogTopY;
            enableHeavyFog(fogTopY, ((f32)placement->fogBottom + fogTopY) - (f32)placement->fogTop,
                           (f32)placement->depthScale, (f32)placement->depthOffset / FOG_CONTROL_DEPTH_DENOMINATOR,
                           FOG_CONTROL_WORLD_SCALE, placement->flags & FOG_CONTROL_PLACEMENT_MODE);
        }
    }
}

void FogControl_init(GameObject* obj, FogControlPlacement* placement) {
    FogControlState* state;
    u8 gateValue;
    f32 fogTopY;

    state = obj->extra;
    obj->objectFlags = (u16)(obj->objectFlags | OBJECT_OBJFLAG_HIDDEN);
    state->enabled = 0;
    state->fullyBlended = 0;
    state->blend = 0.0f;
    if ((placement->flags & FOG_CONTROL_PLACEMENT_ENABLED) != 0) {
        if (placement->enableGameBit == -1) {
            gateValue = 1;
        } else {
            gateValue = mainGetBit(placement->enableGameBit);
        }
        if (gateValue != 0) {
            state->fullyBlended = 1;
            state->enabled = 1;
            state->blend = 1.0f;
            fogTopY = state->blend * ((f32)placement->fogTop - placement->fogBase) + placement->fogBase;
            fogTopY = obj->anim.localPosY + fogTopY;
            enableHeavyFog(fogTopY, ((f32)placement->fogBottom + fogTopY) - placement->fogTop, placement->depthScale,
                           placement->depthOffset / FOG_CONTROL_DEPTH_DENOMINATOR, FOG_CONTROL_WORLD_SCALE,
                           placement->flags & FOG_CONTROL_PLACEMENT_MODE);
        }
    }
}

ObjectDescriptor gFogControlObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)FogControl_init,
    (ObjectDescriptorCallback)FogControl_update,
    (ObjectDescriptorCallback)FogControl_hitDetect,
    0,
    (ObjectDescriptorCallback)FogControl_free,
    (ObjectDescriptorCallback)FogControl_getObjectTypeId,
    FogControl_getExtraSize,
};
