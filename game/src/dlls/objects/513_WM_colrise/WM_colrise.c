/*
 * WM_colrise (DLL 0x0201) - the rising column platform at Krazoa Palace.
 *
 * While its game bit permits movement and a registered rider stands more than
 * 3.0 above it, the column rises toward placement height + 120 and plays its
 * rumble. Otherwise it sinks back to placement height.
 */
#include "dlls/objects/513_WM_colrise.h"

#include "game/objects/object.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_stop_channel_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/gamebits_api.h"
#include "main/object_render.h"
#include "main/objseq.h"

#define WM_COLRISE_RENDER_SCALE       1.0f
#define WM_COLRISE_TYPE_ID            0
#define WM_COLRISE_GAME_BIT_NONE      -1
#define WM_COLRISE_ROTATION_SHIFT     8
#define WM_COLRISE_RIDER_HEIGHT       3.0f
#define WM_COLRISE_RAISE_TIMER_FRAMES 60
#define WM_COLRISE_RAISED_OFFSET_LOW  20.0f
#define WM_COLRISE_RAISED_OFFSET_HIGH 100.0f
#define WM_COLRISE_SETTLE_SPEED       0.5f
#define WM_COLRISE_RISE_SPEED         0.25f
#define WM_COLRISE_SINK_SPEED         0.125f
#define WM_COLRISE_MOVE_SFX_CHANNEL   8

int WM_colrise_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    animUpdate->flags = -1;
    animUpdate->movementState = 0;
    return 0;
}

int WM_colrise_getExtraSize(void) {
    return sizeof(WMColriseState);
}

int WM_colrise_getObjectTypeId(void) {
    return WM_COLRISE_TYPE_ID;
}

void WM_colrise_free(void) {
}

void WM_colrise_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, WM_COLRISE_RENDER_SCALE);
    }
}

void WM_colrise_hitDetect(void) {
}

void WM_colrise_update(GameObject* obj) {
    const WMColrisePlacementView* placement;
    WMColriseState* state;
    s8 isMoving;
    f32 targetY;
    int contactIndex;

    placement = (const WMColrisePlacementView*)obj->anim.placement;
    state = obj->extra;
    state->raiseTimer--;
    if (state->raiseTimer < 0) {
        state->raiseTimer = 0;
    }
    /* Rearm the rise window while a rider is more than 3.0 above the column. */
    if (obj->anim.hitboxTransformState->contactObjectCount > 0) {
        for (contactIndex = 0; contactIndex < obj->anim.hitboxTransformState->contactObjectCount; contactIndex++) {
            GameObject* rider = obj->anim.hitboxTransformState->contactObjects[contactIndex];
            if (rider->anim.localPosY - obj->anim.localPosY > WM_COLRISE_RIDER_HEIGHT) {
                state->raiseTimer = WM_COLRISE_RAISE_TIMER_FRAMES;
            }
        }
    }
    isMoving = 0;
    if ((state->riseGameBit == WM_COLRISE_GAME_BIT_NONE || mainGetBit(state->riseGameBit) != 0) &&
        state->raiseTimer != 0) {
        targetY = WM_COLRISE_RAISED_OFFSET_LOW + (WM_COLRISE_RAISED_OFFSET_HIGH + placement->base.posY);
        if (obj->anim.localPosY > targetY) {
            obj->anim.localPosY = obj->anim.localPosY - WM_COLRISE_SETTLE_SPEED * timeDelta;
            if (obj->anim.localPosY > targetY) {
                obj->anim.localPosY = targetY;
            }
        } else {
            obj->anim.localPosY = WM_COLRISE_RISE_SPEED * timeDelta + obj->anim.localPosY;
            if (obj->anim.localPosY > targetY) {
                obj->anim.localPosY = targetY;
            } else {
                isMoving = 1;
            }
        }
    } else {
        obj->anim.localPosY = obj->anim.localPosY - WM_COLRISE_SINK_SPEED * timeDelta;
        if (obj->anim.localPosY < placement->base.posY) {
            obj->anim.localPosY = placement->base.posY;
        } else {
            isMoving = 1;
        }
    }
    if (isMoving != 0) {
        Sfx_PlayFromObject(obj, SFXTRIG_en_treedrum16_7d);
    } else {
        Sfx_StopObjectChannel(obj, WM_COLRISE_MOVE_SFX_CHANNEL);
    }
}

void WM_colrise_init(GameObject* obj, const WMColrisePlacementView* placement) {
    WMColriseState* state = obj->extra;

    obj->animEventCallback = WM_colrise_SeqFn;
    obj->anim.rotX = (s16)(placement->rotationXByte << WM_COLRISE_ROTATION_SHIFT);
    state->riseGameBit = placement->riseGameBit;
}

void WM_colrise_release(void) {
}

void WM_colrise_initialise(void) {
}

ObjectDescriptor gWM_colriseObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    WM_colrise_initialise,
    WM_colrise_release,
    0,
    (ObjectDescriptorCallback)WM_colrise_init,
    (ObjectDescriptorCallback)WM_colrise_update,
    WM_colrise_hitDetect,
    (ObjectDescriptorCallback)WM_colrise_render,
    WM_colrise_free,
    (ObjectDescriptorCallback)WM_colrise_getObjectTypeId,
    WM_colrise_getExtraSize,
};
