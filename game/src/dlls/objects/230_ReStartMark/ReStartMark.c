/*
 * ReStartMark object (DLL slot 230 / 0xE6).
 *
 * The placement rotation seeds the marker heading. The marker stays hidden
 * because it exists only as a gameplay respawn anchor.
 */
#include "dlls/objects/230_ReStartMark.h"
#include "game/objects/object.h"

void ReStartMark_init(GameObject* obj, ReStartMarkPlacement* placement) {
    obj->anim.rotX = (s16)(placement->rotXByte << 8);
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
}

OBJECT_INIT_ADAPTER(gReStartMarkObjDescriptorInitAdapter, ReStartMark_init, obj, placement)

ObjectDescriptor gReStartMarkObjDescriptor = {
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
    gReStartMarkObjDescriptorInitAdapter,
    0,
    0,
    0,
    0,
    0,
    0,
};
