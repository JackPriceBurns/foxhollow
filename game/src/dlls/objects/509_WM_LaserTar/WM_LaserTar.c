/*
 * WM_LaserTar (DLL 0x01FD) - the Krazoa Palace laser target.
 *
 * A priority hit queues a toggle. Once the cooldown expires, the target
 * switches its model bank and its paired game bits, then rearms the cooldown
 * from its placement.
 */
#include "dlls/objects/509_WM_LaserTar.h"

#include "game/objects/object.h"
#include "main/frame_timing.h"
#include "main/gamebits_api.h"
#include "main/objhits.h"
#include "main/object_render.h"
#include "sys/objects.h"

int WM_LaserTarget_getExtraSize(void) {
    return sizeof(WMLaserTargetState);
}

int WM_LaserTarget_getObjectTypeId(void) {
    return 0;
}

void WM_LaserTarget_free(void) {
}

void WM_LaserTarget_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                           s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void WM_LaserTarget_hitDetect(void) {
}

void WM_LaserTarget_update(GameObject* obj) {
    const WMLaserTargetPlacement* placement;
    WMLaserTargetState* state;

    placement = (const WMLaserTargetPlacement*)obj->anim.placementData;
    state = obj->extra;
    if (ObjHits_GetPriorityHit(obj, NULL, NULL, NULL) != 0) {
        state->toggleQueued = 1;
        state->cooldown = placement->cooldown;
    }
    if (state->cooldown <= 0 && state->toggleQueued != 0) {
        if (mainGetBit(placement->toggleGameBit) != 0) {
            Obj_SetActiveModelIndex(obj, 0);
            mainSetBits(placement->toggleGameBit, 0);
            mainSetBits(placement->pairedGameBit, 0);
        } else {
            Obj_SetActiveModelIndex(obj, 1);
            mainSetBits(placement->toggleGameBit, 1);
            mainSetBits(placement->pairedGameBit, 1);
        }
        state->toggleQueued = 0;
        state->cooldown = placement->cooldown;
    } else if (state->cooldown > 0) {
        state->cooldown -= framesThisStep;
    }
}

void WM_LaserTarget_init(GameObject* obj, const WMLaserTargetPlacement* placement) {
    WMLaserTargetState* state = obj->extra;
    obj->anim.bankIndex = mainGetBit(placement->toggleGameBit);
    state->cooldown = placement->cooldown;
    state->toggleQueued = 0;
}

void WM_LaserTarget_release(void) {
}

void WM_LaserTarget_initialise(void) {
}

ObjectDescriptor gWM_LaserTargetObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    WM_LaserTarget_initialise,
    WM_LaserTarget_release,
    0,
    (ObjectDescriptorCallback)WM_LaserTarget_init,
    (ObjectDescriptorCallback)WM_LaserTarget_update,
    WM_LaserTarget_hitDetect,
    (ObjectDescriptorCallback)WM_LaserTarget_render,
    WM_LaserTarget_free,
    (ObjectDescriptorCallback)WM_LaserTarget_getObjectTypeId,
    WM_LaserTarget_getExtraSize,
};
