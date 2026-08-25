/*
 * SB_ShipMast (DLL 0x1EB) - the mast/rigging attachment of the SB Galleon
 * boss ship. It rides its parent galleon object, pinning its local position
 * to the origin every frame, and picks one of three animation play speeds
 * depending on the World Map galleon's object ID and its
 * userData1 phase counter. The remaining handlers (free/hitDetect/init/release/
 * initialise) are stubs - the mast is purely cosmetic.
 */
#include "dlls/objects/491_SB_ShipMast.h"
#include "dlls/objects/504_WM_Galleon.h"
#include "game/objects/object.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/objanim.h"

int SB_ShipMast_getExtraSize(void) {
    return 0;
}

int SB_ShipMast_getObjectTypeId(void) {
    return 0;
}

void SB_ShipMast_free(void) {
}

void SB_ShipMast_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void SB_ShipMast_hitDetect(void) {
}

void SB_ShipMast_update(GameObject* obj) {
    GameObject* parent = obj->anim.parent;
    if (parent == NULL) {
        return;
    }

    obj->anim.localPosX = 0.0f;
    obj->anim.localPosY = 0.0f;
    obj->anim.localPosZ = 0.0f;

    if (((GameObject*)obj->anim.parent)->anim.romDefNo != WM_GALLEON_OBJECT_ID) {
        if (obj->anim.currentMove != 1) {
            ObjAnim_SetCurrentMove(obj, 1, 0.0f, 0);
        }

        ObjAnim_AdvanceCurrentMove(obj, 0.03f, framesThisStep, NULL);
        return;
    }

    if (parent->userData1 < 0xa || parent->userData1 >= 0xd) {
        if (obj->anim.currentMove != 1) {
            ObjAnim_SetCurrentMove(obj, 1, 0.0f, 0);
        }

        ObjAnim_AdvanceCurrentMove(obj, 0.03f, framesThisStep, NULL);
        return;
    }

    if (obj->anim.currentMove != 0) {
        ObjAnim_SetCurrentMove(obj, 0, 0.0f, 0);
    }

    f32 speed = parent->userData1 >= 0xc ? -0.003f : 0.003f;
    ObjAnim_AdvanceCurrentMove(obj, speed, framesThisStep, NULL);
}

void SB_ShipMast_init(void) {
}

void SB_ShipMast_release(void) {
}

void SB_ShipMast_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSB_ShipMastObjDescriptorInitAdapter, SB_ShipMast_init)
OBJECT_HIT_DETECT_ADAPTER(gSB_ShipMastObjDescriptorHitDetectAdapter, SB_ShipMast_hitDetect)
OBJECT_FREE_ADAPTER(gSB_ShipMastObjDescriptorFreeAdapter, SB_ShipMast_free)
OBJECT_TYPE_ID_ADAPTER(gSB_ShipMastObjDescriptorTypeIdAdapter, SB_ShipMast_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSB_ShipMastObjDescriptorExtraSizeAdapter, SB_ShipMast_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gSB_ShipMastObjDescriptorAcquire, SB_ShipMast_initialise)

ObjectDescriptor gSB_ShipMastObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gSB_ShipMastObjDescriptorAcquire,
        SB_ShipMast_release,
    },
    0,
    gSB_ShipMastObjDescriptorInitAdapter,
    SB_ShipMast_update,
    gSB_ShipMastObjDescriptorHitDetectAdapter,
    SB_ShipMast_render,
    gSB_ShipMastObjDescriptorFreeAdapter,
    gSB_ShipMastObjDescriptorTypeIdAdapter,
    gSB_ShipMastObjDescriptorExtraSizeAdapter,
};
