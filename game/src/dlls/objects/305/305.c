/*
 * Door-light texture animator. A trigger game bit starts the animation;
 * completion either loops to a configured frame or grants a game bit.
 */
#include "dlls/objects/305.h"

#include "main/gamebits.h"
#include "main/objtexture.h"

#define CFDOORLIGHT_GAME_BIT_NONE -1

#define CFDOORLIGHT_FRAME_SHIFT    8
#define CFDOORLIGHT_ROTATION_SHIFT 9

#define CFDOORLIGHT_DEFAULT_TEXTURE_ID 0
#define CFDOORLIGHT_MIN_FRAME          0

int CF_DoorLight_getExtraSize(void) {
    return sizeof(CFDoorLightState);
}

int CF_DoorLight_getObjectTypeId(void) {
    return 0;
}

void CF_DoorLight_free(void) {
}

void CF_DoorLight_render(void) {
}

void CF_DoorLight_hitDetect(void) {
}

void CF_DoorLight_update(GameObject* obj) {
    CFDoorLightState* state;
    CFDoorLightPlacement* placement;
    ObjTextureRuntimeSlot* textureFrame;

    state = obj->extra;
    placement = (CFDoorLightPlacement*)obj->anim.placement;
    if (state->flags.active == 0 && mainGetBit(placement->triggerGameBit) != 0 && state->flags.done == 0) {
        state->flags.active = 1;
        state->currentFrame = CFDOORLIGHT_MIN_FRAME;
    }
    if (state->flags.active != 0) {
        textureFrame = objFindTexture(obj, state->textureId, 0);
        if (textureFrame != 0) {
            state->currentFrame += state->frameStep;
            if (state->currentFrame < CFDOORLIGHT_MIN_FRAME) {
                state->currentFrame = CFDOORLIGHT_MIN_FRAME;
            } else if (state->currentFrame > state->maxFrame) {
                if (placement->doneGameBit != CFDOORLIGHT_GAME_BIT_NONE) {
                    mainSetBits(placement->doneGameBit, 1);
                    state->flags.active = 0;
                    state->flags.done = 1;
                    state->currentFrame = state->maxFrame;
                } else {
                    state->currentFrame = state->resetFrame;
                }
            }
            textureFrame->textureId = state->currentFrame;
        }
    }
}

void CF_DoorLight_init(GameObject* obj, CFDoorLightPlacement* placement) {
    register CFDoorLightState* state = obj->extra;

    state->textureId = CFDOORLIGHT_DEFAULT_TEXTURE_ID;
    obj->anim.rotX = (s16)(placement->initialRotX << CFDOORLIGHT_ROTATION_SHIFT);
    state->maxFrame = placement->maxFrame << CFDOORLIGHT_FRAME_SHIFT;
    state->frameStep = placement->frameStep;
    state->resetFrame = placement->resetFrame << CFDOORLIGHT_FRAME_SHIFT;
    if ((state->flags.done = mainGetBit(placement->doneGameBit))) {
        state->currentFrame = state->maxFrame;
        state->flags.active = 1;
    }
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
}

void CF_DoorLight_release(void) {
}

void CF_DoorLight_initialise(void) {
}

ObjectDescriptor gCF_DoorLightObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)CF_DoorLight_initialise,
    (ObjectDescriptorCallback)CF_DoorLight_release,
    0,
    (ObjectDescriptorCallback)CF_DoorLight_init,
    (ObjectDescriptorCallback)CF_DoorLight_update,
    (ObjectDescriptorCallback)CF_DoorLight_hitDetect,
    (ObjectDescriptorCallback)CF_DoorLight_render,
    (ObjectDescriptorCallback)CF_DoorLight_free,
    (ObjectDescriptorCallback)CF_DoorLight_getObjectTypeId,
    CF_DoorLight_getExtraSize,
};
