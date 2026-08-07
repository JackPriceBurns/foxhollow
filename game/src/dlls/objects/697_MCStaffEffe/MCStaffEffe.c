/*
 * MCStaffEffe (DLL 0x2B9) - staff visual-effect object.
 *
 * From its placement effectProfile, init selects a particle type and a
 * staff glow level (profiles 0-3, default = profile 0); each render tick
 * spawns the staff particle fx (objfx_spawnPulseBurst) scaled by the object's
 * root-motion scale. update is a no-op. The anim-event callback
 * mcstaffeffe_SeqFn (provided by the preceding MCUpgradeMa TU) drives the
 * player's staff glow from sequence events.
 */
#include "main/dll/dll_02B9_mcstaffeffe.h"
#include "main/objfx.h"

void mcstaffeffe_render(GameObject* staffEffect)
{
    objfx_spawnPulseBurst(staffEffect, staffEffect->anim.rootMotionScale, (u8)staffEffect->userData1, 0, 0, NULL);
}

void mcstaffeffe_update(void)
{
}

void mcstaffeffe_init(GameObject* staffEffect, McStaffEffectSetup* placement)
{
    staffEffect->animEventCallback = mcstaffeffe_SeqFn;
    switch (placement->effectProfile)
    {
    case 0:
        staffEffect->userData1 = 4;
        staffEffect->userData2 = 1;
        break;
    case 1:
        staffEffect->userData1 = 5;
        staffEffect->userData2 = 5;
        break;
    case 2:
        staffEffect->userData1 = 6;
        staffEffect->userData2 = 2;
        break;
    case 3:
        staffEffect->userData1 = 0xb;
        staffEffect->userData2 = 3;
        break;
    default:
        staffEffect->userData1 = 4;
        staffEffect->userData2 = 1;
        break;
    }
}

ObjectDescriptor gMCStaffEffeObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)mcstaffeffe_init,
    (ObjectDescriptorCallback)mcstaffeffe_update,
    0,
    (ObjectDescriptorCallback)mcstaffeffe_render,
    0,
    0,
    0,
};
