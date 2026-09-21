/*
 * MikaBombShadow object (DLL slot 220).
 *
 * Projects the Mika bomb's shadow onto the ground and scales its model and
 * opacity based on the bomb's height.
 */
#include "dlls/objects/220_MikaBombShadow.h"
#include "main/frame_timing.h"
#include "main/objhits.h"
#include "main/track_dolphin.h"

int MikaBombShadow_getExtraSize(void) {
    return sizeof(MikaBombShadowState);
}

int MikaBombShadow_getObjectTypeId(void) {
    return 0;
}

void MikaBombShadow_free(GameObject* obj) {
    (void)obj;
}

void MikaBombShadow_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    if (visible == 0 || obj->anim.modelState->shadowCastSlot == NULL) {
        return;
    }

    objShadowRender(obj, 0, 0, framesThisStep);
}

void MikaBombShadow_hitDetect(GameObject* obj) {
}

void MikaBombShadow_update(GameObject* obj) {
    GameObject* bomb = obj->ownerObj;
    MikaBombShadowState* state = obj->extra;
    f32 scaleFactor = 1.0f - (bomb->anim.localPosY - obj->anim.localPosY) / state->groundOffset;
    obj->anim.modelState->shadowScale = 14.0f * scaleFactor + 1.0f;

    scaleFactor *= 1.5f;
    if (scaleFactor > 1.0f) {
        scaleFactor = 1.0f;
    }

    obj->anim.modelState->shadowAlphaStep = 16384.0f * scaleFactor;
}

void MikaBombShadow_init(GameObject* obj) {
    MikaBombShadowState* state = obj->extra;

    f32 groundDistance;
    trackGetHeightAboveGround(obj, obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ, &groundDistance, 0);

    ObjHits_DisableObject(obj);
    obj->anim.alpha = 0xff;
    obj->anim.rotY = 0x4000;
    obj->anim.rotX = 0;
    obj->anim.rotZ = 0;
    obj->anim.modelState->flags |= OBJ_MODEL_STATE_SHADOW_ALPHA_HOLD;
    state->groundOffset = groundDistance;
    obj->anim.localPosY -= groundDistance;
    obj->anim.modelState->shadowAlphaStep = 0;
    obj->anim.modelState->shadowScale = 1.0f;
}

void MikaBombShadow_release(void) {
}

void MikaBombShadow_initialise(void) {
}

OBJECT_INIT_ADAPTER(gMikaBombShadowObjDescriptorInitAdapter, MikaBombShadow_init, obj)
OBJECT_FREE_ADAPTER(gMikaBombShadowObjDescriptorFreeAdapter, MikaBombShadow_free, obj)
OBJECT_TYPE_ID_ADAPTER(gMikaBombShadowObjDescriptorTypeIdAdapter, MikaBombShadow_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gMikaBombShadowObjDescriptorExtraSizeAdapter, MikaBombShadow_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gMikaBombShadowObjDescriptorAcquire, MikaBombShadow_initialise)

ObjectDescriptor gMikaBombShadowObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gMikaBombShadowObjDescriptorAcquire,
        MikaBombShadow_release,
    },
    0,
    gMikaBombShadowObjDescriptorInitAdapter,
    MikaBombShadow_update,
    MikaBombShadow_hitDetect,
    MikaBombShadow_render,
    gMikaBombShadowObjDescriptorFreeAdapter,
    gMikaBombShadowObjDescriptorTypeIdAdapter,
    gMikaBombShadowObjDescriptorExtraSizeAdapter,
};
