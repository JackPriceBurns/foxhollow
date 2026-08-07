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
#define DLL_127_MINIMUM_SCALE             10.0f
#define DLL_127_SCALE_FACTOR              0.015625f
#define DLL_127_INITIAL_YAW_MASK          0x3F
#define DLL_127_INITIAL_YAW_SHIFT         10

#define DLL_127_HIT_REACT_COOLDOWN(obj) (*(s16*)&(obj)->userData2)

int dll_127_getExtraSize(void) {
    return 0;
}

int dll_127_getObjectTypeId(void) {
    return DLL_127_OBJECT_TYPE_ID;
}

void dll_127_free(void) {
}

void dll_127_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void dll_127_hitDetect(void) {
}

void dll_127_update(GameObject* obj) {
    ObjHitsPriorityState* hitState;

    if (obj->anim.hitReactState == 0) {
        return;
    }
    if (DLL_127_HIT_REACT_COOLDOWN(obj) > 0) {
        DLL_127_HIT_REACT_COOLDOWN(obj) -= framesThisStep;
    }
    hitState = ObjAnim_GetPriorityHitState(&obj->anim);
    if ((hitState->flags & OBJHITS_PRIORITY_STATE_PAIR_RESPONSE_APPLIED) == 0) {
        return;
    }
    if (DLL_127_HIT_REACT_COOLDOWN(obj) > 0) {
        return;
    }
    DLL_127_HIT_REACT_COOLDOWN(obj) = DLL_127_HIT_REACT_COOLDOWN_FRAMES;
}

void dll_127_init(GameObject* obj, Dll127Placement* placement) {
    ObjAnimComponent* objAnim;
    f32 scale;
    u32 initialYaw;
    u8 scaleByte;

    objAnim = &obj->anim;
    objAnim->flags |= 2;
    scaleByte = placement->modelScale;
    scale = (f32)(int)scaleByte;
    if ((f32)(int)scaleByte < DLL_127_MINIMUM_SCALE) {
        scale = DLL_127_MINIMUM_SCALE;
    }
    scale *= DLL_127_SCALE_FACTOR;
    objAnim->rootMotionScale = objAnim->modelInstance->rootMotionScaleBase * scale;
    if (objAnim->modelState != NULL) {
        objAnim->modelState->shadowScale = objAnim->modelInstance->shadowScaleBase * scale;
    }
    objAnim->bankIndex = placement->modelBankIndex;
    initialYaw = placement->initialYaw & DLL_127_INITIAL_YAW_MASK;
    objAnim->rotX = (s16)(initialYaw << DLL_127_INITIAL_YAW_SHIFT);
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
