/*
 * Door object family (DLL slot 272).
 */
#include "dlls/objects/272.h"

#include "main/audio/sfx_trigger_ids.h"
#include "main/gamebits_api.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "main/objtexture.h"
#include "main/audio/sfx_object_query_api.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_stop_object_api.h"
#include "main/objhits.h"

#define DOOR_PHASE_OPEN    0
#define DOOR_PHASE_CLOSED  1
#define DOOR_PHASE_CLOSING 2
#define DOOR_PHASE_OPENING 3

#define DOOR_CLOSE_FLAG_REQUESTED 1
#define DOOR_CLOSE_FLAG_READY     2

#define DOOR_CLOSE_REQUEST_TEXTURE_SLOT 0
#define DOOR_CLOSE_READY_TEXTURE_SLOT   1
#define DOOR_TEXTURE_INDEX              0
#define DOOR_LATCHED_TEXTURE_ID         0x100

#define DOOR_ANIM_EVENT_OPENED 1
#define DOOR_ANIM_EVENT_CLOSED 2

#define DOOR_NO_GAME_BIT               -1
#define DOOR_SEQUENCE_ID_NONE          -1
#define DOOR_TRIGGER_ARG_NONE          -1
#define DOOR_TRIGGER_ARG_MASK          0x7F
#define DOOR_ROTATION_SHIFT            8
#define DOOR_ROOT_MOTION_SCALE_DIVISOR 64.0f
#define DOOR_DEFAULT_ROOT_MOTION_SCALE 1.0f
#define DOOR_MODEL_SCALE               1.0f
#define DOOR_SEQUENCE_1101             1101
#define DOOR_SEQUENCE_358              358
#define DOOR_MOVEMENT_SFX_1101_SPECIAL 832
#define DOOR_ENDPOINT_SFX_1101_SPECIAL 833
#define DOOR_MOVEMENT_SFX_1101_DEFAULT 1154
#define DOOR_ENDPOINT_SFX_1101_DEFAULT 1155
#define DOOR_MOVEMENT_SFX_358          275
#define DOOR_ENDPOINT_SFX_358          504

int Door_animEventCallback(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    int eventIndex;
    DoorState* state;
    DoorPlacement* placement;
    int closeRequested;
    int closeReady;
    ObjTextureRuntimeSlot* texture;
    int isStationary;

    (void)unused;
    state = obj->extra;
    placement = (DoorPlacement*)obj->anim.placementData;
    if (obj->anim.alpha == 0) {
        ObjHits_DisableObject(obj);
    }
    if (obj->anim.modelInstance->textureSlotCount != 0) {
        if ((state->closeFlags & DOOR_CLOSE_FLAG_REQUESTED) != 0) {
            texture = objFindTexture(obj, DOOR_CLOSE_REQUEST_TEXTURE_SLOT, DOOR_TEXTURE_INDEX);
            if (texture != NULL) {
                texture->textureId = DOOR_LATCHED_TEXTURE_ID;
            }
        }
        if ((state->closeFlags & DOOR_CLOSE_FLAG_READY) != 0) {
            texture = objFindTexture(obj, DOOR_CLOSE_READY_TEXTURE_SLOT, DOOR_TEXTURE_INDEX);
            if (texture != NULL) {
                texture->textureId = DOOR_LATCHED_TEXTURE_ID;
            }
        }
    }
    if (state->phase == DOOR_PHASE_OPEN) {
        closeRequested = mainGetBit(placement->closeRequestGameBit);
        closeReady = 0;
        if (placement->closeReadyGameBit == DOOR_NO_GAME_BIT || mainGetBit(placement->closeReadyGameBit) != 0) {
            closeReady = 1;
        }
        if (closeRequested != 0 && (state->closeFlags & DOOR_CLOSE_FLAG_REQUESTED) == 0) {
            if (obj->anim.modelInstance->textureSlotCount != 0) {
                Sfx_PlayFromObject(obj, SFXTRIG_littletink22);
            }
            state->closeFlags |= DOOR_CLOSE_FLAG_REQUESTED;
        }
        if (closeReady != 0 && (state->closeFlags & DOOR_CLOSE_FLAG_READY) == 0) {
            if (obj->anim.modelInstance->textureSlotCount != 0) {
                Sfx_PlayFromObject(obj, SFXTRIG_littletink22);
            }
            state->closeFlags |= DOOR_CLOSE_FLAG_READY;
        }
        if (state->closeFlags == (DOOR_CLOSE_FLAG_REQUESTED | DOOR_CLOSE_FLAG_READY)) {
            state->phase = DOOR_PHASE_CLOSING;
            if (state->movementSfx != 0) {
                Sfx_PlayFromObject(obj, state->movementSfx);
            }
        }
    } else if (state->phase == DOOR_PHASE_CLOSED) {
        if (mainGetBit(placement->closeRequestGameBit) == 0) {
            state->phase = DOOR_PHASE_OPENING;
            if (state->movementSfx != 0) {
                Sfx_PlayFromObject(obj, state->movementSfx);
            }
        }
    }
    if (state->phase == DOOR_PHASE_CLOSING) {
        for (eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++) {
            if (animUpdate->eventIds[eventIndex] == DOOR_ANIM_EVENT_CLOSED) {
                state->phase = DOOR_PHASE_CLOSED;
                if (placement->closedLatchGameBit != DOOR_NO_GAME_BIT) {
                    mainSetBits(placement->closedLatchGameBit, 1);
                }
                if (state->movementSfx != 0 && Sfx_IsPlayingFromObject(obj, state->movementSfx) != 0) {
                    Sfx_StopFromObject(obj, state->movementSfx);
                }
                if (state->endpointSfx != 0) {
                    Sfx_PlayFromObject(obj, state->endpointSfx);
                }
            }
        }
    } else if (state->phase == DOOR_PHASE_OPENING) {
        for (eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++) {
            if (animUpdate->eventIds[eventIndex] == DOOR_ANIM_EVENT_OPENED) {
                state->phase = DOOR_PHASE_OPEN;
                state->closeFlags = 0;
                if (placement->closedLatchGameBit != DOOR_NO_GAME_BIT) {
                    mainSetBits(placement->closedLatchGameBit, 0);
                }
                if (state->movementSfx != 0 && Sfx_IsPlayingFromObject(obj, state->movementSfx) != 0) {
                    Sfx_StopFromObject(obj, state->movementSfx);
                }
                if (state->endpointSfx != 0) {
                    Sfx_PlayFromObject(obj, state->endpointSfx);
                }
            }
        }
    }
    isStationary = 0;
    if (state->phase != DOOR_PHASE_CLOSING && state->phase != DOOR_PHASE_OPENING) {
        isStationary = 1;
    }
    return isStationary;
}

int Door_getExtraSize(void) {
    return sizeof(DoorState);
}

void Door_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    (void)visible;
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, DOOR_MODEL_SCALE);
}

void Door_update(GameObject* obj) {
    DoorState* state;
    DoorPlacement* placement;
    int triggerArg;
    int triggerId;

    state = obj->extra;
    placement = (DoorPlacement*)obj->anim.placementData;
    if (state->initPending != 0) {
        triggerId = placement->triggerSequenceId;
        if (triggerId != 0 && state->phase != DOOR_PHASE_OPEN) {
            triggerArg = placement->triggerArg & DOOR_TRIGGER_ARG_MASK;
            (*gObjectTriggerInterface)->preempt((int)obj, triggerId);
        } else {
            triggerArg = DOOR_TRIGGER_ARG_NONE;
        }
        if (placement->runSequenceId != DOOR_SEQUENCE_ID_NONE) {
            (*gObjectTriggerInterface)->runSequence(placement->runSequenceId, (void*)obj, triggerArg);
        }
        state->initPending = 0;
    }
}

void Door_init(GameObject* obj, DoorPlacement* placement) {
    DoorState* state = obj->extra;

    state->initPending = 1;
    obj->anim.rotX = (s16)(placement->rotXByte << DOOR_ROTATION_SHIFT);
    obj->animEventCallback = Door_animEventCallback;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
    obj->anim.rootMotionScale = (f32)(u32)placement->rootMotionScaleInput / DOOR_ROOT_MOTION_SCALE_DIVISOR;
    if (!obj->anim.rootMotionScale) {
        obj->anim.rootMotionScale = DOOR_DEFAULT_ROOT_MOTION_SCALE;
    }
    obj->anim.rootMotionScale *= obj->anim.modelInstance->rootMotionScaleBase;
    if (placement->closedLatchGameBit != DOOR_NO_GAME_BIT) {
        state->phase = mainGetBit(placement->closedLatchGameBit);
    } else {
        state->phase = DOOR_PHASE_OPEN;
    }
    state->closeFlags = 0;
    if (mainGetBit(placement->closeRequestGameBit) != 0) {
        state->closeFlags |= DOOR_CLOSE_FLAG_REQUESTED;
    }
    if (mainGetBit(placement->closeReadyGameBit) != 0) {
        state->closeFlags |= DOOR_CLOSE_FLAG_READY;
    }
    switch (obj->anim.romDefNo) {
    case DOOR_SEQUENCE_1101: {
        s32 subtype = obj->anim.mapEventSlot;
        switch (subtype) {
        case 31:
        case 32:
        case 33:
        case 34:
        case 40:
        case 41:
        case 42:
            state->movementSfx = DOOR_MOVEMENT_SFX_1101_SPECIAL;
            state->endpointSfx = DOOR_ENDPOINT_SFX_1101_SPECIAL;
            break;
        default:
            state->movementSfx = DOOR_MOVEMENT_SFX_1101_DEFAULT;
            state->endpointSfx = DOOR_ENDPOINT_SFX_1101_DEFAULT;
            break;
        }
        break;
    }
    case DOOR_SEQUENCE_358:
        state->movementSfx = DOOR_MOVEMENT_SFX_358;
        state->endpointSfx = DOOR_ENDPOINT_SFX_358;
        break;
    }
}

ObjectDescriptor gDoorObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)Door_init,
    (ObjectDescriptorCallback)Door_update,
    0,
    (ObjectDescriptorCallback)Door_render,
    0,
    0,
    Door_getExtraSize,
};
