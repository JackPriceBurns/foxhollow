/*
 * ExplodePlan (DLL 617, object type 0x0) - a static placed prop that is
 * removed from the world by a game bit. The placement stores a removal
 * game bit at +0x1E and a packed rotX byte at +0x18.
 *
 * explodeplan_init applies the rotation and, if the removal bit is already
 * set, hides the model and disables its hit volumes. explodeplan_update
 * re-tests the bit every frame and toggles the hidden flag / hit-detection
 * state so the prop appears or disappears the moment the bit changes.
 * Render is a plain model draw at a fixed scale (lbl_803E69D0).
 */
#include "main/gamebits.h"
#include "main/dll/dll_0269_explodeplan.h"
#include "main/object_render.h"
#include "main/objhits.h"

int explodeplan_getExtraSize(void) {
    return sizeof(ExplodePlanState);
}

int explodeplan_getObjectTypeId(void) {
    return 0;
}

void explodeplan_free(void) {
}

void explodeplan_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
}

void explodeplan_hitDetect(void) {
}

void explodeplan_update(GameObject* obj) {
    ExplodePlanPlacement* placement = (ExplodePlanPlacement*)obj->anim.placementData;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->removeGameBit)) != 0) {
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        ObjHits_DisableObject(obj);
    } else {
        obj->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
        ObjHits_EnableObject(obj);
    }
}

void explodeplan_init(GameObject* obj, ExplodePlanPlacement* placement) {
    ObjHits_EnableObject(obj);
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->removeGameBit)) != 0) {
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        ObjHits_DisableObject(obj);
    }
    obj->anim.rotX = placement->rotXByte << 8;
}

void explodeplan_release(void) {
}

void explodeplan_initialise(void) {
}

OBJECT_INIT_ADAPTER(gExplodePlanObjDescriptorInitAdapter, explodeplan_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gExplodePlanObjDescriptorHitDetectAdapter, explodeplan_hitDetect)
OBJECT_FREE_ADAPTER(gExplodePlanObjDescriptorFreeAdapter, explodeplan_free)
OBJECT_TYPE_ID_ADAPTER(gExplodePlanObjDescriptorTypeIdAdapter, explodeplan_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gExplodePlanObjDescriptorExtraSizeAdapter, explodeplan_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gExplodePlanObjDescriptorAcquire, explodeplan_initialise)

ObjectDescriptor gExplodePlanObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gExplodePlanObjDescriptorAcquire,
        explodeplan_release,
    },
    0,
    gExplodePlanObjDescriptorInitAdapter,
    explodeplan_update,
    gExplodePlanObjDescriptorHitDetectAdapter,
    explodeplan_render,
    gExplodePlanObjDescriptorFreeAdapter,
    gExplodePlanObjDescriptorTypeIdAdapter,
    gExplodePlanObjDescriptorExtraSizeAdapter,
};
