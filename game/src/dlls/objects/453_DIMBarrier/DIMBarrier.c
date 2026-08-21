#include "dlls/objects/453_DIMBarrier.h"

#include "dlls/objects/454_DIMCannon.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/gamebits_api.h"
#include "main/object_render.h"
#include "main/audio/sfx_play_api.h"

typedef enum DimBarrierPhase {
    DIM_BARRIER_PHASE_ARMED,
    DIM_BARRIER_PHASE_FADING,
    DIM_BARRIER_PHASE_RESOLVED
} DimBarrierPhase;

typedef struct DimBarrierState {
    s16 fadeTimer;
    u8 phase;
    s8 triggerCountdown;
} DimBarrierState;

int dimbarrier_getExtraSize(void) {
    return sizeof(DimBarrierState);
}

int dimbarrier_getObjectTypeId(void) {
    return 0;
}

void dimbarrier_free(void) {
}

void dimbarrier_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void dimbarrier_hitDetect(void) {
}

void dimbarrier_update(GameObject* obj) {
    const DimBarrierPlacement* placement = (const DimBarrierPlacement*)obj->anim.placementData;
    DimBarrierState* state = obj->extra;

    switch (state->phase) {
    case DIM_BARRIER_PHASE_ARMED: {
        s32 triggerFound = FALSE;

        for (s32 contactIndex = 0; contactIndex < obj->anim.hitboxTransformState->contactObjectCount; contactIndex++) {
            GameObject* contact = obj->anim.hitboxTransformState->contactObjects[contactIndex];

            if (contact->anim.romDefNo == DIM_CANNON_BALL_SEQUENCE_ID &&
                ((DimCannonBallState*)contact->extra)->variant != 0) {
                triggerFound = 1;
                break;
            }
        }
        if (triggerFound) {
            if (--state->triggerCountdown <= 0) {
                state->phase = DIM_BARRIER_PHASE_FADING;
                state->fadeTimer = 30;
                Sfx_PlayFromObject(obj, SFXTRIG_wp_dsmk2_c_206);
            } else {
                Sfx_PlayFromObject(obj, SFXTRIG_wp_dsmk2_c_207);
            }
        }
        break;
    }
    case DIM_BARRIER_PHASE_FADING: {
        ObjHitsPriorityState* hitState;
        s32 alpha = obj->anim.alpha - framesThisStep * 16;

        if (alpha < 0) {
            alpha = 0;
        }
        hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
        hitState->flags &= ~OBJHITS_PRIORITY_STATE_ENABLED;
        obj->anim.alpha = alpha;
        state->fadeTimer -= framesThisStep;
        if (state->fadeTimer <= 0) {
            mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->barrierGameBit)), 1);
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

    obj->anim.rotX = (s16)((s32)placement->rotationXByte * 256);
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
    state = obj->extra;
    state->triggerCountdown = 1;
    state->phase = DIM_BARRIER_PHASE_ARMED;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->barrierGameBit))) != 0) {
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
