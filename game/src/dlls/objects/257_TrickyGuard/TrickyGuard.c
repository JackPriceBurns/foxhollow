/*
 * DLL 0x101 - TrickyGuard.
 *
 * Directs an available Tricky to guard this object's position when its
 * optional game-bit condition is met and the player enters range.
 */
#include "dlls/objects/257_TrickyGuard.h"
#include "dlls/objects/288_TrickyGuard.h"
#include "main/dll/dll_00C4_tricky.h"
#include "main/gamebits.h"
#include "main/objprint_render.h"
#include "sys/objects/lifecycle.h"

void TrickyGuard_update(GameObject* obj) {
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;

    TrickyGuardPlacement* placement = (TrickyGuardPlacement*)obj->anim.placementData;
    if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->armingGameBit) != -1 &&
        mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->armingGameBit)) == 0) {
        return;
    }

    GameObject* tricky = getTrickyObject();
    if (tricky == NULL) {
        return;
    }

    if (TRICKY_INTERFACE(tricky)->isGuarding(tricky) != 0) {
        return;
    }

    if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) != 0) {
        TRICKY_INTERFACE(tricky)->sideCommandEnable(tricky, obj, TRICKY_GUARD_COMMAND_KIND, TRICKY_GUARD_COMMAND_TYPE);
    }

    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    objUpdateHitVolumeTransforms(obj);
}

void TrickyGuard_init(GameObject* obj, TrickyGuardPlacement* placement) {

    obj->anim.rotX = placement->rotXByte << 8;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
}

OBJECT_INIT_ADAPTER(gTrickyGuardObjDescriptorInitAdapter, TrickyGuard_init, obj, placement)

ObjectDescriptor gTrickyGuardObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        0,
        0,
    },
    0,
    gTrickyGuardObjDescriptorInitAdapter,
    TrickyGuard_update,
    0,
    0,
    0,
    0,
    0,
};
