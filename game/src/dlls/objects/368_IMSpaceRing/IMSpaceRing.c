#include "dlls/objects/368_IMSpaceRing.h"

#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/vecmath.h"
#include "sys/objects.h"

#define IM_SPACE_RING_SPIN_AXIS(obj) ((obj)->userData1)

GameObject* gIMSpaceRingLeader;

int imSpaceRing_getExtraSize(void) {
    return 0;
}

int imSpaceRing_getObjectTypeId(void) {
    return 0;
}

void imSpaceRing_free(void) {
}

void imSpaceRing_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void imSpaceRing_hitDetect(void) {
}

void imSpaceRing_update(GameObject* obj) {
    const IMSpaceRingPlacement* placement = (const IMSpaceRingPlacement*)obj->anim.placementData;

    if (IM_SPACE_RING_SPIN_AXIS(obj) != 0) {
        obj->anim.rotX += ObjAnim_ReadPlacementS16(&obj->anim, &placement->spinSpeed) * framesThisStep;
    } else {
        obj->anim.rotY += ObjAnim_ReadPlacementS16(&obj->anim, &placement->spinSpeed) * framesThisStep;
    }

    obj->anim.rotZ += ObjAnim_ReadPlacementS16(&obj->anim, &placement->tiltSpeed) * framesThisStep;

    if (gIMSpaceRingLeader == NULL) {
        return;
    }

    obj->anim.alpha = gIMSpaceRingLeader->anim.alpha;
    objMove(obj, gIMSpaceRingLeader->anim.localPosX - obj->anim.localPosX,
            gIMSpaceRingLeader->anim.localPosY - obj->anim.localPosY,
            gIMSpaceRingLeader->anim.localPosZ - obj->anim.localPosZ);
}

void imSpaceRing_init(GameObject* obj, const IMSpaceRingPlacement* placement) {
    obj->anim.rotX = placement->initialRotX << 8;
    IM_SPACE_RING_SPIN_AXIS(obj) = randomGetRange(0, 1);
}

void imSpaceRing_release(void) {
}

void imSpaceRing_initialise(void) {
}

OBJECT_INIT_ADAPTER(gIMSpaceRingObjDescriptorInitAdapter, imSpaceRing_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gIMSpaceRingObjDescriptorHitDetectAdapter, imSpaceRing_hitDetect)
OBJECT_FREE_ADAPTER(gIMSpaceRingObjDescriptorFreeAdapter, imSpaceRing_free)
OBJECT_TYPE_ID_ADAPTER(gIMSpaceRingObjDescriptorTypeIdAdapter, imSpaceRing_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gIMSpaceRingObjDescriptorExtraSizeAdapter, imSpaceRing_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gIMSpaceRingObjDescriptorAcquire, imSpaceRing_initialise)

ObjectDescriptor gIMSpaceRingObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gIMSpaceRingObjDescriptorAcquire,
        imSpaceRing_release,
    },
    0,
    gIMSpaceRingObjDescriptorInitAdapter,
    imSpaceRing_update,
    gIMSpaceRingObjDescriptorHitDetectAdapter,
    imSpaceRing_render,
    gIMSpaceRingObjDescriptorFreeAdapter,
    gIMSpaceRingObjDescriptorTypeIdAdapter,
    gIMSpaceRingObjDescriptorExtraSizeAdapter,
};
