/*
 * SH_staffHaz (DLL 0x1B2) - the shimmering haze drawn around the staff.
 *
 * The render callback draws the child flame model and emits its haze pulse.
 * Once the parent hides a flame, the update callback releases the child.
 */
#include "dlls/objects/434_SH_staffHaz.h"
#include "main/object_render.h"
#include "main/objfx.h"
#include "sys/objects/lifecycle.h"

void SH_StaffHaze_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5) {
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    Vec offset;
    offset.x = 0.0f;
    offset.y = 0.5f;
    offset.z = 0.0f;
    objfx_spawnPulseBurst(obj, obj->anim.rootMotionScale, 4, 0, 0, &offset);
}

void SH_StaffHaze_update(GameObject* obj) {
    if ((obj->anim.flags & OBJANIM_FLAG_HIDDEN) != 0) {
        Obj_FreeObject(obj);
    }
}

ObjectDescriptor gSH_staffHazeObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    0,
    (ObjectDescriptorCallback)SH_StaffHaze_update,
    0,
    (ObjectDescriptorCallback)SH_StaffHaze_render,
    0,
    0,
    0,
};
