/*
 * Fall_Ladder (DLL 0x10A) - a placed falling-ladder prop driven by the
 * object sequence/trigger system.
 *
 * Two game bits from the placement record control it:
 *   triggerGameBit (placement 0x20): when set the ladder begins to fall;
 *     during FALL_LADDER_SEQUENCE_ID it also selects trigger sequence 0.
 *   baseGameBit (placement 0x1E): during FALL_LADDER_SEQUENCE_ID it selects
 *     trigger sequence 1 (the alternate path).
 *
 * While the object is playing FALL_LADDER_SEQUENCE_ID it runs trigger
 * sequence 0 when only the trigger bit is set, and trigger sequence 1 when
 * only the base bit is set. Otherwise it runs a simple gravity drop: once the
 * trigger bit is set it waits a short delay, plays its fall sound, then falls
 * under gravity toward the placement rest Y. It bounces with damped velocity
 * until its speed drops below the settling threshold.
 *
 * The placement supplies the initial height offset, the two game bits, and
 * the model index; init pre-positions the object above its resting Y.
 */
#include "dlls/objects/266_Fall_Ladder.h"

#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/expgfx_interface.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/objseq.h"
#include "sys/objects.h"

#define FALL_LADDER_SEQUENCE_ID 0x548

#define FALL_LADDER_TRIGGER_SEQUENCE 0
#define FALL_LADDER_BASE_SEQUENCE    1
#define FALL_LADDER_SEQUENCE_FLAGS   -1

#define FALL_LADDER_ROTATION_SHIFT 8
#define FALL_LADDER_FALL_DELAY     10
#define FALL_LADDER_GRAVITY        0.9f
#define FALL_LADDER_BOUNCE_DAMPING 0.3f
#define FALL_LADDER_SETTLE_SPEED   0.01f

int Fall_Ladders_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    (void)obj;
    (void)unused;
    (void)animUpdate;
    return 0;
}

int Fall_Ladders_getExtraSize(void) {
    return sizeof(FallLadderState);
}

int Fall_Ladders_getObjectTypeId(void) {
    return 0;
}

void Fall_Ladders_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((u32)obj);
}

void Fall_Ladders_render(void) {
}

void Fall_Ladders_hitDetect(void) {
}

void Fall_Ladders_update(GameObject* obj) {
    FallLadderPlacement* placement;
    FallLadderState* state;
    f32 speed;

    placement = (FallLadderPlacement*)obj->anim.placementData;
    state = obj->extra;
    if (obj->anim.romDefNo == FALL_LADDER_SEQUENCE_ID) {
        if (mainGetBit(state->triggerGameBit) != 0 && mainGetBit(state->baseGameBit) == 0) {
            (*gObjectTriggerInterface)->runSequence(FALL_LADDER_TRIGGER_SEQUENCE, obj, FALL_LADDER_SEQUENCE_FLAGS);
        }
        if (mainGetBit(state->triggerGameBit) == 0 && mainGetBit(state->baseGameBit) != 0) {
            (*gObjectTriggerInterface)->runSequence(FALL_LADDER_BASE_SEQUENCE, obj, FALL_LADDER_SEQUENCE_FLAGS);
        }
    } else if (state->fallDelay != 0) {
        state->fallDelay -= (s16)timeDelta;
        if (state->fallDelay <= 0) {
            state->motionState = FALL_LADDER_MOTION_FALLING;
            if (state->playFallSound != 0) {
                Sfx_PlayFromObject(obj, SFXTRIG_totem_slide);
                state->playFallSound = 0;
            }
            state->fallDelay = 0;
        }
    } else {
        if (state->motionState == FALL_LADDER_MOTION_WAITING && mainGetBit(state->triggerGameBit) != 0) {
            state->fallDelay = FALL_LADDER_FALL_DELAY;
        }
        if (state->motionState == FALL_LADDER_MOTION_FALLING && obj->anim.localPosY >= placement->base.posY) {
            obj->anim.velocityY -= FALL_LADDER_GRAVITY;
            obj->anim.localPosY = obj->anim.velocityY * timeDelta + obj->anim.localPosY;
            if (obj->anim.localPosY <= placement->base.posY) {
                obj->anim.localPosY = placement->base.posY;
                obj->anim.velocityY = FALL_LADDER_BOUNCE_DAMPING * -obj->anim.velocityY;
                speed = obj->anim.velocityY;
                speed = (speed >= 0.0f) ? speed : -speed;
                if (speed < FALL_LADDER_SETTLE_SPEED) {
                    state->motionState = FALL_LADDER_MOTION_SETTLED;
                }
            }
        }
    }
}

void Fall_Ladders_init(GameObject* obj, FallLadderPlacement* placement) {
    FallLadderState* state = obj->extra;

    obj->anim.rotX = (s16)((s32)placement->rotXByte << FALL_LADDER_ROTATION_SHIFT);
    state->triggerGameBit = placement->triggerGameBit;
    state->baseGameBit = placement->baseGameBit;
    state->initialHeightOffset = (f32)(s32)placement->initialHeightOffset;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
    obj->animEventCallback = Fall_Ladders_SeqFn;
    obj->anim.localPosY = placement->base.posY + state->initialHeightOffset;
    Obj_SetActiveModelIndex(obj, placement->modelIndex);
    state->motionState = FALL_LADDER_MOTION_WAITING;
    if (mainGetBit(state->triggerGameBit) == 0) {
        state->playFallSound = 1;
    }
}

void Fall_Ladders_release(void) {
}

void Fall_Ladders_initialise(void) {
}

ObjectDescriptor gFall_LaddersObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)Fall_Ladders_initialise,
    (ObjectDescriptorCallback)Fall_Ladders_release,
    0,
    (ObjectDescriptorCallback)Fall_Ladders_init,
    (ObjectDescriptorCallback)Fall_Ladders_update,
    (ObjectDescriptorCallback)Fall_Ladders_hitDetect,
    (ObjectDescriptorCallback)Fall_Ladders_render,
    (ObjectDescriptorCallback)Fall_Ladders_free,
    (ObjectDescriptorCallback)Fall_Ladders_getObjectTypeId,
    Fall_Ladders_getExtraSize,
};
