/*
 * SH_EmptyTum (DLL 0x1B4) - the empty (non-rolling) tumbleweed bush.
 *
 * init orients the bush from its placement bytes and sizes a capsule hit
 * volume scaled by the model's root-motion scale; update just polls the
 * shared priority hit-effect handler each frame.
 */
#include "dlls/objects/436_SH_EmptyTum.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/objhits.h"

f32 gShEmptyTumbleweedHitEffectCooldown;

void SH_EmptyTumbleW_update(GameObject* obj) {
    ObjHits_PollPriorityHitEffectWithCooldown(obj, 8, 0xFF, 0xFF, 0x78, SFXTRIG_wp_swdtest222_280,
                                              &gShEmptyTumbleweedHitEffectCooldown);
}

void SH_EmptyTumbleW_init(GameObject* obj, ShEmptyTumbleweedPlacement* placement) {
    obj->anim.rotZ = (placement->rotZByte - 0x7F) * 0x80;
    obj->anim.rotY = (placement->rotYByte - 0x7F) * 0x80;
    obj->anim.rotX = placement->rotXByte << 8;
    obj->anim.rootMotionScale = ObjAnim_ReadPlacementF32(&obj->anim, &placement->scale);
    ObjHitbox_SetCapsuleBounds(&obj->anim, 15.0f * obj->anim.rootMotionScale, -5.0f * obj->anim.rootMotionScale,
                               100.0f * obj->anim.rootMotionScale);
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
}

OBJECT_INIT_ADAPTER(gSH_EmptyTumbleWObjDescriptorInitAdapter, SH_EmptyTumbleW_init, obj, placement)

ObjectDescriptor gSH_EmptyTumbleWObjDescriptor = {
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
    gSH_EmptyTumbleWObjDescriptorInitAdapter,
    SH_EmptyTumbleW_update,
    0,
    0,
    0,
    0,
    0,
};
