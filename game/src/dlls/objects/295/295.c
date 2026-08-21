/*
 * Unnamed object DLL slot 295 / 0x127.
 *
 * Placement selects the model bank, initial yaw, and model/shadow scale.
 * A short cooldown in object-local scratch state is re-armed after the
 * collision system applies an object-pair response.
 */
#include "dlls/objects/295.h"

#include "main/frame_timing.h"
#include "main/object_render.h"

#define DLL_127_OBJECT_TYPE_ID            0x13
#define DLL_127_HIT_REACT_COOLDOWN_FRAMES 100

int dll_127_getExtraSize(void) {
    return 0;
}

int dll_127_getObjectTypeId(void) {
    return DLL_127_OBJECT_TYPE_ID;
}

void dll_127_free(void) {
}

void dll_127_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (!visible) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void dll_127_hitDetect(void) {
}

void dll_127_update(GameObject* obj) {
    if (obj->anim.hitReactState == 0) {
        return;
    }

    if (obj->userData2 > 0) {
        obj->userData2 -= framesThisStep;
    }

    ObjHitsPriorityState* hitState = ObjAnim_GetPriorityHitState(&obj->anim);
    if ((hitState->flags & OBJHITS_PRIORITY_STATE_PAIR_RESPONSE_APPLIED) == 0) {
        return;
    }

    if (obj->userData2 > 0) {
        return;
    }

    obj->userData2 = DLL_127_HIT_REACT_COOLDOWN_FRAMES;
}

void dll_127_init(GameObject* obj, Dll127Placement* placement) {
    ObjAnimComponent* objAnim = &obj->anim;
    objAnim->flags |= 2;

    f32 scale = placement->modelScale;
    if (placement->modelScale < 10.0f) {
        scale = 10.0f;
    }
    scale *= 0.015625f;

    objAnim->rootMotionScale = objAnim->modelInstance->rootMotionScaleBase * scale;
    if (objAnim->modelState != NULL) {
        objAnim->modelState->shadowScale = objAnim->modelInstance->shadowScaleBase * scale;
    }

    objAnim->bankIndex = placement->modelBankIndex;
    objAnim->rotX = (placement->initialYaw & 0x3F) << 10;
    if (objAnim->bankIndex >= objAnim->modelInstance->modelCount) {
        objAnim->bankIndex = 0;
    }
    obj->userData1 = 0;
    obj->userData2 = 0;
}

void dll_127_release(void) {
}

void dll_127_initialise(void) {
}

ObjectDescriptor gDll127ObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)dll_127_initialise,
    (ObjectDescriptorCallback)dll_127_release,
    0,
    (ObjectDescriptorCallback)dll_127_init,
    (ObjectDescriptorCallback)dll_127_update,
    (ObjectDescriptorCallback)dll_127_hitDetect,
    (ObjectDescriptorCallback)dll_127_render,
    (ObjectDescriptorCallback)dll_127_free,
    (ObjectDescriptorCallback)dll_127_getObjectTypeId,
    dll_127_getExtraSize,
};
