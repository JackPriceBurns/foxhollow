/* Drives a game-bit-controlled heavy-fog volume. */
#include "dlls/objects/320_fogControl.h"

#include "game/objects/object.h"
#include "main/frame_timing.h"
#include "main/pi_dolphin.h"
#include "main/gamebits.h"

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
    s16 depthOffset = ObjAnim_ReadPlacementS16(&obj->anim, &placement->depthOffset);
    s16 depthScale = ObjAnim_ReadPlacementS16(&obj->anim, &placement->depthScale);
    s16 enableGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->enableGameBit);
    s16 fogBase = ObjAnim_ReadPlacementS16(&obj->anim, &placement->fogBase);
    s16 fogBottom = ObjAnim_ReadPlacementS16(&obj->anim, &placement->fogBottom);
    s16 fogTop = ObjAnim_ReadPlacementS16(&obj->anim, &placement->fogTop);
    u8 gateValue;
    u8 isTransitioning;
    f32 fogTopY;

    if (enableGameBit == -1) {
        gateValue = 1;
    } else {
        gateValue = mainGetBit(enableGameBit);
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
            fogTopY = state->blend * ((f32)fogTop - (f32)fogBase) + (f32)fogBase;
            fogTopY = obj->anim.localPosY + fogTopY;
            enableHeavyFog(fogTopY, ((f32)fogBottom + fogTopY) - (f32)fogTop, (f32)depthScale,
                           (f32)depthOffset / FOG_CONTROL_DEPTH_DENOMINATOR, FOG_CONTROL_WORLD_SCALE,
                           placement->flags & FOG_CONTROL_PLACEMENT_MODE);
        }
    }
}

void FogControl_init(GameObject* obj, FogControlPlacement* placement) {
    FogControlState* state;
    s16 depthOffset;
    s16 depthScale;
    s16 enableGameBit;
    s16 fogBase;
    s16 fogBottom;
    s16 fogTop;
    u8 gateValue;
    f32 fogTopY;

    state = obj->extra;
    depthOffset = ObjAnim_ReadPlacementS16(&obj->anim, &placement->depthOffset);
    depthScale = ObjAnim_ReadPlacementS16(&obj->anim, &placement->depthScale);
    enableGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->enableGameBit);
    fogBase = ObjAnim_ReadPlacementS16(&obj->anim, &placement->fogBase);
    fogBottom = ObjAnim_ReadPlacementS16(&obj->anim, &placement->fogBottom);
    fogTop = ObjAnim_ReadPlacementS16(&obj->anim, &placement->fogTop);
    obj->objectFlags = (u16)(obj->objectFlags | OBJECT_OBJFLAG_HIDDEN);
    state->enabled = 0;
    state->fullyBlended = 0;
    state->blend = 0.0f;
    if ((placement->flags & FOG_CONTROL_PLACEMENT_ENABLED) != 0) {
        if (enableGameBit == -1) {
            gateValue = 1;
        } else {
            gateValue = mainGetBit(enableGameBit);
        }
        if (gateValue != 0) {
            state->fullyBlended = 1;
            state->enabled = 1;
            state->blend = 1.0f;
            fogTopY = state->blend * ((f32)fogTop - (f32)fogBase) + (f32)fogBase;
            fogTopY = obj->anim.localPosY + fogTopY;
            enableHeavyFog(fogTopY, ((f32)fogBottom + fogTopY) - (f32)fogTop, (f32)depthScale,
                           (f32)depthOffset / FOG_CONTROL_DEPTH_DENOMINATOR, FOG_CONTROL_WORLD_SCALE,
                           placement->flags & FOG_CONTROL_PLACEMENT_MODE);
        }
    }
}

OBJECT_INIT_ADAPTER(gFogControlObjDescriptorInitAdapter, FogControl_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gFogControlObjDescriptorHitDetectAdapter, FogControl_hitDetect)
OBJECT_FREE_ADAPTER(gFogControlObjDescriptorFreeAdapter, FogControl_free, obj)
OBJECT_TYPE_ID_ADAPTER(gFogControlObjDescriptorTypeIdAdapter, FogControl_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gFogControlObjDescriptorExtraSizeAdapter, FogControl_getExtraSize)

ObjectDescriptor gFogControlObjDescriptor = {
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
    gFogControlObjDescriptorInitAdapter,
    FogControl_update,
    gFogControlObjDescriptorHitDetectAdapter,
    0,
    gFogControlObjDescriptorFreeAdapter,
    gFogControlObjDescriptorTypeIdAdapter,
    gFogControlObjDescriptorExtraSizeAdapter,
};
