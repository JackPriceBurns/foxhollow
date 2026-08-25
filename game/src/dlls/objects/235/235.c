/*
 * Side-repel object family (DLL slot 235 / 0xEB).
 *
 * These invisible, non-updating volumes register with the side-repel object
 * group used by Tricky. Their hit-sphere radius is one eighth of the
 * placement radius.
 */
#include "dlls/objects/235.h"
#include "main/objhits.h"
#include "main/objtype.h"

#define SIDEREPEL_OBJECT_GROUP 0x40

int siderepel_getExtraSize(void) {
    return sizeof(SideRepelState);
}

void siderepel_free(GameObject* obj) {
    objFreeObjectType(obj, SIDEREPEL_OBJECT_GROUP);
}

void siderepel_init(GameObject* obj, SideRepelPlacement* placement) {
    obj->objectFlags |= OBJECT_OBJFLAG_UPDATE_DISABLED | OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
    objAddObjectType(obj, SIDEREPEL_OBJECT_GROUP);
    if (obj->anim.hitReactState == NULL) {
        return;
    }
    ObjHitbox_SetSphereRadius((ObjAnimComponent*)obj, ObjAnim_ReadPlacementU16(&obj->anim, &placement->radius) >> 3);
}

OBJECT_INIT_ADAPTER(gSiderepelObjDescriptorInitAdapter, siderepel_init, obj, placement)
OBJECT_FREE_ADAPTER(gSiderepelObjDescriptorFreeAdapter, siderepel_free, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gSiderepelObjDescriptorExtraSizeAdapter, siderepel_getExtraSize)

ObjectDescriptor gSiderepelObjDescriptor = {
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
    gSiderepelObjDescriptorInitAdapter,
    0,
    0,
    0,
    gSiderepelObjDescriptorFreeAdapter,
    0,
    gSiderepelObjDescriptorExtraSizeAdapter,
};
