/*
 * Door-light texture animator. A trigger game bit starts the animation;
 * completion either loops to a configured frame or grants a game bit.
 */
#include "dlls/objects/305.h"
#include "main/gamebits.h"
#include "main/objtexture.h"

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
    CFDoorLightState* state = obj->extra;
    CFDoorLightPlacement* placement = (CFDoorLightPlacement*)obj->anim.placement;
    if (state->flags.active == 0 &&
        mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->triggerGameBit))) != 0 && state->flags.done == 0) {
        state->flags.active = 1;
        state->currentFrame = 0;
    }

    if (state->flags.active == 0) {
        return;
    }

    ObjTextureRuntimeSlot* textureFrame = objFindTexture(obj, state->textureId, 0);
    if (textureFrame == 0) {
        return;
    }

    state->currentFrame += state->frameStep;
    if (state->currentFrame < 0) {
        state->currentFrame = 0;
        textureFrame->textureId = state->currentFrame;
        return;
    }

    if (state->currentFrame <= state->maxFrame) {
        textureFrame->textureId = state->currentFrame;
        return;
    }

    if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->doneGameBit) == -1) {
        state->currentFrame = state->resetFrame;
        textureFrame->textureId = state->currentFrame;
        return;
    }

    mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->doneGameBit), 1);
    state->flags.active = 0;
    state->flags.done = 1;
    state->currentFrame = state->maxFrame;
    textureFrame->textureId = state->currentFrame;
}

void CF_DoorLight_init(GameObject* obj, CFDoorLightPlacement* placement) {
    CFDoorLightState* state = obj->extra;
    state->textureId = 0;
    obj->anim.rotX = placement->initialRotX << 9;
    state->maxFrame = ObjAnim_ReadPlacementS16(&obj->anim, &placement->maxFrame) << 8;
    state->frameStep = ObjAnim_ReadPlacementS16(&obj->anim, &placement->frameStep);
    state->resetFrame = placement->resetFrame << 8;
    if ((state->flags.done = mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->doneGameBit)))) {
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
