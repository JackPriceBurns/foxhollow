/*
 * DIMBarrier (DLL 0x1C5) - barrier object for Dinosaur Island Mission.
 * While an active sequence-0x1D6 object is in the trigger list, counts down
 * an arm timer; on expiry fades the barrier out and latches its game bit.
 */

#include "dlls/objects/453_DIMBarrier.h"

#include "dlls/objects/454_DIMCannon.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/gamebits_api.h"
#include "main/object_render.h"
#include "main/audio/sfx_play_api.h"

#define DIM_BARRIER_PHASE_ARMED    0
#define DIM_BARRIER_PHASE_FADING   1
#define DIM_BARRIER_PHASE_RESOLVED 2

#define DIM_BARRIER_INITIAL_TRIGGER_COUNTDOWN 1
#define DIM_BARRIER_FADE_DURATION             30
#define DIM_BARRIER_ALPHA_FADE_PER_FRAME      16

#define DIM_BARRIER_RENDER_SCALE 1.0f

int dimbarrier_getExtraSize(void) {
    return sizeof(DimBarrierState);
}

int dimbarrier_getObjectTypeId(void) {
    return 0x0;
}

void dimbarrier_free(void) {
}

void dimbarrier_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    s32 visibleValue = visible;

    if (visibleValue != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, DIM_BARRIER_RENDER_SCALE);
    }
}

void dimbarrier_hitDetect(void) {
}

void dimbarrier_update(GameObject* obj) {
    const DimBarrierPlacement* placement = (const DimBarrierPlacement*)obj->anim.placementData;
    DimBarrierState* state = obj->extra;

    switch (state->phase) {
    case DIM_BARRIER_PHASE_ARMED: {
        GameObject* contact;
        DimCannonBallState* triggerState;
        int triggerFound;
        int contactIndex;

        triggerFound = 0;
        for (contactIndex = 0; contactIndex < obj->anim.hitboxTransformState->contactObjectCount; contactIndex++) {
            contact = obj->anim.hitboxTransformState->contactObjects[contactIndex];
            triggerState = contact->extra;
            if (contact->anim.romDefNo == DIM_CANNON_BALL_SEQUENCE_ID && triggerState->variant != 0) {
                triggerFound = 1;
                break;
            }
        }
        if (triggerFound) {
            if (--state->triggerCountdown <= 0) {
                state->phase = DIM_BARRIER_PHASE_FADING;
                state->fadeTimer = DIM_BARRIER_FADE_DURATION;
                Sfx_PlayFromObject(obj, SFXTRIG_wp_dsmk2_c_206);
            } else {
                Sfx_PlayFromObject(obj, SFXTRIG_wp_dsmk2_c_207);
            }
        }
        break;
    }
    case DIM_BARRIER_PHASE_FADING: {
        ObjHitsPriorityState* hitState;
        int alpha = obj->anim.alpha - framesThisStep * DIM_BARRIER_ALPHA_FADE_PER_FRAME;

        if (alpha < 0) {
            alpha = 0;
        }
        hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
        hitState->flags &= ~OBJHITS_PRIORITY_STATE_ENABLED;
        obj->anim.alpha = alpha;
        state->fadeTimer -= framesThisStep;
        if (state->fadeTimer <= 0) {
            mainSetBits(placement->barrierGameBit, 1);
            state->phase = DIM_BARRIER_PHASE_RESOLVED;
        }
        break;
    }
    case DIM_BARRIER_PHASE_RESOLVED:
        break;
    }
}

void dimbarrier_init(GameObject* obj, const DimBarrierPlacement* placement) {
    DimBarrierState* state;

    obj->anim.rotX = (s16)((s32)placement->rotationXByte << 8);
    obj->objectFlags |= (OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED);
    state = obj->extra;
    state->triggerCountdown = DIM_BARRIER_INITIAL_TRIGGER_COUNTDOWN;
    state->phase = DIM_BARRIER_PHASE_ARMED;
    if (mainGetBit(placement->barrierGameBit) != 0) {
        ObjHitsPriorityState* hitState;

        state->triggerCountdown = 0;
        hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
        hitState->flags &= ~OBJHITS_PRIORITY_STATE_ENABLED;
        obj->anim.alpha = 0;
        state->phase = DIM_BARRIER_PHASE_RESOLVED;
    }
}

void dimbarrier_release(void) {
}

void dimbarrier_initialise(void) {
}

ObjectDescriptor gDIMBarrierObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)dimbarrier_initialise,
    (ObjectDescriptorCallback)dimbarrier_release,
    0,
    (ObjectDescriptorCallback)dimbarrier_init,
    (ObjectDescriptorCallback)dimbarrier_update,
    (ObjectDescriptorCallback)dimbarrier_hitDetect,
    (ObjectDescriptorCallback)dimbarrier_render,
    (ObjectDescriptorCallback)dimbarrier_free,
    (ObjectDescriptorCallback)dimbarrier_getObjectTypeId,
    dimbarrier_getExtraSize,
};
