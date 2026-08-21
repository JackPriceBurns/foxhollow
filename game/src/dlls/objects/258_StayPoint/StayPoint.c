/*
 * DLL 0x102 - StayPoint.
 *
 * Offers a conditional stay command and tracks whether Tricky is holding
 * position within this object's engagement radius.
 */
#include "dlls/objects/258_StayPoint.h"
#include "main/dll/dll_00C4_tricky.h"
#include "main/gamebits.h"
#include "main/vecmath_distance_api.h"
#include "sys/objects/lifecycle.h"
#include "main/dll/dll_0000_gameui_api.h"
#include "main/dll/dll_80136a40.h"
#include "main/objprint_render_api.h"

#define STAYPOINT_COMMAND_KIND       1
#define STAYPOINT_COMMAND_TYPE       3

void StayPoint_update(GameObject* obj) {
    StayPointPlacement* placement = (StayPointPlacement*)obj->anim.placementData;
    GameObject* tricky = getTrickyObject();
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;

    if (tricky == NULL) {
        return;
    }

    if (obj != trickyGetStayPoint(tricky) && ObjAnim_ReadPlacementS16(&obj->anim, &placement->activeGameBit) != -1) {
        mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->activeGameBit), 0);
    }

    if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->requiredGameBit) != -1 &&
        mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->requiredGameBit)) == 0) {
        return;
    }

    if (obj == trickyGetStayPoint(tricky) &&
        vec3f_distanceSquared(&obj->anim.worldPosX, &tricky->anim.worldPosX) < 100.0f) {
        if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->activeGameBit) != -1) {
            mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->activeGameBit), 1);
        }

        return;
    }

    if (cMenuGetSelectedItem() == -1) {
        obj->anim.modelInstance->hitVolumes[0].priority = 0;
    } else {
        obj->anim.modelInstance->hitVolumes[0].priority = 16;
    }

    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    if ((obj->anim.modelInstance->flags & OBJDEF_FLAG_HAS_MODELS) != 0 && obj->anim.hitVolumeTransforms != NULL) {
        objUpdateHitVolumeTransforms(obj);
    }

    if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) != 0) {
        TRICKY_INTERFACE(tricky)->sideCommandEnable(tricky, obj, STAYPOINT_COMMAND_KIND, STAYPOINT_COMMAND_TYPE);
    }
}

void StayPoint_init(GameObject* obj) {
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
}

ObjectDescriptor gStayPointObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)StayPoint_init,
    (ObjectDescriptorCallback)StayPoint_update,
    0,
    0,
    0,
    0,
    0,
};
