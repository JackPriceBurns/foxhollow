/*
 * MMP_gyserve (DLL 0x183) - Moon Mountain Pass geyser vent.
 *
 * While its placement gamebit is clear, the vent alternates between random
 * idle and active periods. The active period emits geyser particles and
 * keeps the vent sound alive.
 */
#include "dlls/objects/387_MMP_gyserve.h"
#include "game/objects/object.h"
#include "main/audio/sfx_keep_alive_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/gamebits_api.h"
#include "main/vecmath.h"

#define MMP_GEYSER_VENT_PARTICLE_GEYSER     0x724

int mmpGeyserVent_getExtraSize(void) {
    return 0;
}

int mmpGeyserVent_getObjectTypeId(void) {
    return 0;
}

void mmpGeyserVent_free(void) {
}

void mmpGeyserVent_render(void) {
}

void mmpGeyserVent_hitDetect(void) {
}

void mmpGeyserVent_update(GameObject* obj) {
    MMPGeyserVentPlacement* placement = (MMPGeyserVentPlacement*)obj->anim.placementData;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->disableGameBit)) != 0) {
        return;
    }

    obj->userData1 -= framesThisStep;
    if (obj->userData1 < 0) {
        obj->userData1 = randomGetRange(70, 240);
        obj->userData2 = randomGetRange(30, 60);
    }

    if (obj->userData2 == 0) {
        return;
    }

    obj->userData2 -= framesThisStep;
    if (obj->userData2 <= 0) {
        obj->userData2 = 0;
    } else {
        (*gPartfxInterface)
            ->spawnObject(obj, MMP_GEYSER_VENT_PARTICLE_GEYSER, NULL, 2,
                          -1, NULL);
        Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_en_diallp_c_450);
    }
}

void mmpGeyserVent_init(GameObject* obj) {
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
    obj->userData1 = randomGetRange(10, 200);
    obj->anim.alpha = 0;
    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
}

void mmpGeyserVent_release(void) {
}

void mmpGeyserVent_initialise(void) {
}

ObjectDescriptor gMMPGeyserVentObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)mmpGeyserVent_initialise,
    (ObjectDescriptorCallback)mmpGeyserVent_release,
    0,
    (ObjectDescriptorCallback)mmpGeyserVent_init,
    (ObjectDescriptorCallback)mmpGeyserVent_update,
    (ObjectDescriptorCallback)mmpGeyserVent_hitDetect,
    (ObjectDescriptorCallback)mmpGeyserVent_render,
    (ObjectDescriptorCallback)mmpGeyserVent_free,
    (ObjectDescriptorCallback)mmpGeyserVent_getObjectTypeId,
    mmpGeyserVent_getExtraSize,
};
