/*
 * ARWBlocker (DLL 680) - an invisible trigger volume in the on-rails
 * Arwing flight sections. It starts hidden with hit-detection disabled;
 * once the Arwing (or, as a fallback, the player object) closes to within
 * a fixed distance it fades in, enables its hitbox and fires one of two
 * object sequences (selected by the placement's sequenceMode). The
 * animEventCallback (ARWBlocker_SeqFn) reports whether the blocker
 * is currently "armed" (mode 1 and not yet locked) to the sequence system.
 */
#include "main/frame_timing.h"
#include "sys/objects.h"
#include "main/objseq.h"
#include "main/vecmath.h"
#include "main/dll/ARW/dll_02A8_arwblocker.h"
#include "main/object_render.h"
#include "dlls/object_descriptor.h"
#include "main/dll/ARW/dll_029A_arwarwing.h"
#include "main/objhits.h"

/* placement sequenceMode: which object sequence the blocker fires on approach */
#define ARWBLOCKER_SEQMODE_DEFAULT 0 /* fires sequence 0; never reports "armed" */
#define ARWBLOCKER_SEQMODE_ARMED   1 /* fires sequence 1; reports armed until locked */

int ARWBlocker_SeqFn(GameObject* obj) {
    ARWBlockerState* state = obj->extra;
    switch (state->sequenceMode) {
    case ARWBLOCKER_SEQMODE_ARMED:
        if (state->sequenceLocked != 0) {
            break;
        }
        return 1;
    case ARWBLOCKER_SEQMODE_DEFAULT:
        break;
    }
    return 0;
}

int ARWBlocker_getExtraSize(void) {
    return 2;
}

int ARWBlocker_getObjectTypeId(void) {
    return 0;
}

void ARWBlocker_free(void) {
}

void ARWBlocker_render(GameObject* obj, int p2, int p3, int p4, int p5, f32 scale) {
    objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
}

void ARWBlocker_hitDetect(void) {
}

void ARWBlocker_update(GameObject* obj) {
    ObjAnimComponent* objAnim = &obj->anim;
    ARWBlockerState* state = obj->extra;
    GameObject* arwing = getArwing();

    if (arwing == NULL) {
        arwing = Obj_GetPlayerObject();
    }

    if (Vec_distance(&objAnim->worldPosX, &arwing->anim.worldPosX) >= 5120.0f) {
        return;
    }

    int alpha = 3.0f * timeDelta + objAnim->alpha;
    if (alpha > 0xff) {
        alpha = 0xff;
    }

    objAnim->alpha = alpha;
    obj->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
    ObjHits_EnableObject(obj);

    if (obj->userData1 == 0) {
        switch (state->sequenceMode) {
        case ARWBLOCKER_SEQMODE_ARMED:
            (*gObjectTriggerInterface)->runSequence(1, obj, -1);
            break;
        case ARWBLOCKER_SEQMODE_DEFAULT:
        default:
            (*gObjectTriggerInterface)->runSequence(0, obj, -1);
            break;
        }
        obj->userData1 = 1;
    }
}

void ARWBlocker_init(GameObject* obj, ARWBlockerSetup* setup) {
    ObjAnimComponent* objAnim = &obj->anim;
    ARWBlockerState* state = obj->extra;
    ARWBlockerSetup* mapData = setup;

    obj->anim.rotX = -0x8000;
    obj->anim.rotZ = mapData->rotZ << 8;
    obj->animEventCallback = ARWBlocker_SeqFn;
    state->sequenceMode = mapData->sequenceMode;
    obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
    objAnim->alpha = 0;
    ObjHits_DisableObject(obj);
}

void ARWBlocker_release(void) {
}

void ARWBlocker_initialise(void) {
}

OBJECT_INIT_ADAPTER(gARWBlockerObjDescriptorInitAdapter, ARWBlocker_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gARWBlockerObjDescriptorHitDetectAdapter, ARWBlocker_hitDetect)
OBJECT_RENDER_ADAPTER(gARWBlockerObjDescriptorRenderAdapter, ARWBlocker_render, obj, arg2, arg3, arg4, arg5, visible)
OBJECT_FREE_ADAPTER(gARWBlockerObjDescriptorFreeAdapter, ARWBlocker_free)
OBJECT_TYPE_ID_ADAPTER(gARWBlockerObjDescriptorTypeIdAdapter, ARWBlocker_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gARWBlockerObjDescriptorExtraSizeAdapter, ARWBlocker_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gARWBlockerObjDescriptorAcquire, ARWBlocker_initialise)

ObjectDescriptor gARWBlockerObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gARWBlockerObjDescriptorAcquire,
        ARWBlocker_release,
    },
    0,
    gARWBlockerObjDescriptorInitAdapter,
    ARWBlocker_update,
    gARWBlockerObjDescriptorHitDetectAdapter,
    gARWBlockerObjDescriptorRenderAdapter,
    gARWBlockerObjDescriptorFreeAdapter,
    gARWBlockerObjDescriptorTypeIdAdapter,
    gARWBlockerObjDescriptorExtraSizeAdapter,
};
