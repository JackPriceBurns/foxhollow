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

#define SH_EMPTY_TUMBLEWEED_HIT_EFFECT_MODE      8
#define SH_EMPTY_TUMBLEWEED_HIT_EFFECT_RED       0xFF
#define SH_EMPTY_TUMBLEWEED_HIT_EFFECT_GREEN     0xFF
#define SH_EMPTY_TUMBLEWEED_HIT_EFFECT_BLUE      0x78
#define SH_EMPTY_TUMBLEWEED_ROTATION_BYTE_CENTER 0x7F
#define SH_EMPTY_TUMBLEWEED_ROTATION_BYTE_SCALE  0x80
#define SH_EMPTY_TUMBLEWEED_ROTATION_BYTE_SHIFT  8
#define SH_EMPTY_TUMBLEWEED_HITBOX_RADIUS        15.0f
#define SH_EMPTY_TUMBLEWEED_HITBOX_VERTICAL_MIN  -5.0f
#define SH_EMPTY_TUMBLEWEED_HITBOX_VERTICAL_MAX  100.0f

f32 gShEmptyTumbleweedHitEffectCooldown;

void SH_EmptyTumbleW_update(GameObject* obj) {
    ObjHits_PollPriorityHitEffectWithCooldown(obj, SH_EMPTY_TUMBLEWEED_HIT_EFFECT_MODE,
                                              SH_EMPTY_TUMBLEWEED_HIT_EFFECT_RED, SH_EMPTY_TUMBLEWEED_HIT_EFFECT_GREEN,
                                              SH_EMPTY_TUMBLEWEED_HIT_EFFECT_BLUE, SFXTRIG_wp_swdtest222_280,
                                              &gShEmptyTumbleweedHitEffectCooldown);
}

void SH_EmptyTumbleW_init(GameObject* obj, ShEmptyTumbleweedPlacement* placement) {
    f32 scale;

    obj->anim.rotZ =
        (placement->rotZByte - SH_EMPTY_TUMBLEWEED_ROTATION_BYTE_CENTER) * SH_EMPTY_TUMBLEWEED_ROTATION_BYTE_SCALE;
    obj->anim.rotY =
        (placement->rotYByte - SH_EMPTY_TUMBLEWEED_ROTATION_BYTE_CENTER) * SH_EMPTY_TUMBLEWEED_ROTATION_BYTE_SCALE;
    obj->anim.rotX = placement->rotXByte << SH_EMPTY_TUMBLEWEED_ROTATION_BYTE_SHIFT;
    obj->anim.rootMotionScale = placement->scale;
    scale = obj->anim.rootMotionScale;
    ObjHitbox_SetCapsuleBounds((ObjAnimComponent*)obj, (int)(SH_EMPTY_TUMBLEWEED_HITBOX_RADIUS * scale),
                               (int)(SH_EMPTY_TUMBLEWEED_HITBOX_VERTICAL_MIN * scale),
                               (int)(SH_EMPTY_TUMBLEWEED_HITBOX_VERTICAL_MAX * scale));
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
}

ObjectDescriptor gSH_EmptyTumbleWObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)SH_EmptyTumbleW_init,
    (ObjectDescriptorCallback)SH_EmptyTumbleW_update,
    0,
    0,
    0,
    0,
    0,
};
