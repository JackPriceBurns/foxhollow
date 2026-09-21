/* Tracks a game-bit-controlled visibility state for the containing map block. */
#include "dlls/objects/314_VisAnimator.h"
#include "game/objects/object.h"
#include "main/gamebits.h"
#include "main/lightmap.h"

int VisAnimator_getExtraSize(void) {
    return sizeof(VisAnimatorState);
}

int VisAnimator_getObjectTypeId(void) {
    return 0;
}

void VisAnimator_free(void) {
}

void VisAnimator_render(void) {
}

void VisAnimator_hitDetect(void) {
}

void VisAnimator_update(GameObject* obj) {
    VisAnimatorPlacement* placement = (VisAnimatorPlacement*)obj->anim.placementData;
    VisAnimatorState* state = obj->extra;

    if (mapGetBlock(objPosToMapBlockIdx(obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ)) == NULL) {
        state->flags |= VIS_ANIMATOR_STATE_REFRESH_PENDING;
        return;
    }

    state->currentGateState =
        state->gateMask & mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->gateGameBit));
    if (state->previousGateState != state->currentGateState) {
        state->visibilityBit = state->visibilityBit ^ 1;
        state->flags |= VIS_ANIMATOR_STATE_REFRESH_PENDING;
    }

    state->previousGateState = state->currentGateState;
    if (state->flags & VIS_ANIMATOR_STATE_REFRESH_PENDING) {
        state->flags &= ~VIS_ANIMATOR_STATE_REFRESH_PENDING;
    }
}

void VisAnimator_init(GameObject* obj, VisAnimatorPlacement* placement) {
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;

    VisAnimatorState* state = obj->extra;
    state->visibilityBit = placement->initialVisibilityBit;
    state->gateMask = 1 << placement->gateBitIndex;
    if ((state->gateMask & mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->gateGameBit))) != 0) {
        state->visibilityBit ^= 1;
    }

    // not sure what this does?
    mapGetBlock(objPosToMapBlockIdx(obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ));

    u8 gateState = state->gateMask & mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->gateGameBit));
    state->currentGateState = gateState;
    state->previousGateState = gateState;
    state->flags |= VIS_ANIMATOR_STATE_REFRESH_PENDING;
}

void VisAnimator_release(void) {
}

void VisAnimator_initialise(void) {
}

OBJECT_INIT_ADAPTER(gVisAnimatorObjDescriptorInitAdapter, VisAnimator_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gVisAnimatorObjDescriptorHitDetectAdapter, VisAnimator_hitDetect)
OBJECT_RENDER_ADAPTER(gVisAnimatorObjDescriptorRenderAdapter, VisAnimator_render)
OBJECT_FREE_ADAPTER(gVisAnimatorObjDescriptorFreeAdapter, VisAnimator_free)
OBJECT_TYPE_ID_ADAPTER(gVisAnimatorObjDescriptorTypeIdAdapter, VisAnimator_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gVisAnimatorObjDescriptorExtraSizeAdapter, VisAnimator_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gVisAnimatorObjDescriptorAcquire, VisAnimator_initialise)

ObjectDescriptor gVisAnimatorObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gVisAnimatorObjDescriptorAcquire,
        VisAnimator_release,
    },
    0,
    gVisAnimatorObjDescriptorInitAdapter,
    VisAnimator_update,
    gVisAnimatorObjDescriptorHitDetectAdapter,
    gVisAnimatorObjDescriptorRenderAdapter,
    gVisAnimatorObjDescriptorFreeAdapter,
    gVisAnimatorObjDescriptorTypeIdAdapter,
    gVisAnimatorObjDescriptorExtraSizeAdapter,
};
