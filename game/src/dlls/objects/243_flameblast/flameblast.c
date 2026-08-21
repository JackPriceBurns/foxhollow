#include "dlls/objects/243_flameblast.h"

#include "main/dll/dll_80136a40.h"
#include "main/frame_timing.h"
#include "main/objfx.h"
#include "main/objhits.h"
#include "main/vecmath.h"
#include "sys/objects/lifecycle.h"

typedef struct FlameblastState {
    f32 cycleTimer;
    Vec3f launchOrigin;
    u8 freeRequested;
    u8 hitVolumeDelayCycles;
    u8 pad12[2];
} FlameblastState;

STATIC_ASSERT(sizeof(FlameblastState) == 0x14);
STATIC_ASSERT(offsetof(FlameblastState, cycleTimer) == 0x00);
STATIC_ASSERT(offsetof(FlameblastState, launchOrigin) == 0x04);
STATIC_ASSERT(offsetof(FlameblastState, freeRequested) == 0x10);
STATIC_ASSERT(offsetof(FlameblastState, hitVolumeDelayCycles) == 0x11);

void flameblast_requestFree(GameObject* obj) {
    FlameblastState* state = obj->extra;
    state->freeRequested = 1;
}

static int flameblast_resetFlight(GameObject* obj, FlameblastState* state) {
    GameObject* tricky = getTrickyObject();
    const Vec3f* origin;
    MatrixTransform rotation;

    if (state->freeRequested != 0 || tricky == NULL) {
        Obj_FreeObject(obj);
        return 0;
    }

    obj->anim.velocity = (Vec3f){0.0f, 0.0f, -1.5f};
    rotation = (MatrixTransform){
        .rotX = tricky->anim.rotX + trickyGetAimPitchOffset(tricky),
        .rotY = tricky->anim.rotY,
        .rotZ = tricky->anim.rotZ,
        .scale = 1.0f,
    };
    vecRotateZXY(&rotation.rotX, &obj->anim.velocity.x);
    if ((tricky->objectFlags & OBJECT_OBJFLAG_RENDERED) != 0) {
        origin = trickyGetQueuedPathParticlePos(tricky);
    } else {
        origin = &tricky->anim.localPos;
    }
    state->launchOrigin.x = -(0.4f * obj->anim.velocity.x - origin->x);
    state->launchOrigin.y = -(0.4f * obj->anim.velocity.y - origin->y);
    state->launchOrigin.z = -(0.4f * obj->anim.velocity.z - origin->z);
    if (state->hitVolumeDelayCycles != 0) {
        state->hitVolumeDelayCycles--;
    } else {
        ObjHits_ClearHitVolumes(&obj->anim);
    }
    return 1;
}

static int flameblast_getExtraSize(void) {
    return sizeof(FlameblastState);
}

static void flameblast_render(GameObject* obj) {
    FlameblastState* state = obj->extra;
    Vec3f offset = {0.0f, 1.0f, 0.0f};
    f32 scale = 0.033333335f * state->cycleTimer + 0.2f;

    objfx_spawnPulseBurst(obj, scale, 2, 0, 0, &offset);
}

static void flameblast_update(GameObject* obj) {
    FlameblastState* state = obj->extra;

    state->cycleTimer += timeDelta;
    if (state->cycleTimer > 24.0f) {
        state->cycleTimer -= 24.0f;
        if (flameblast_resetFlight(obj, state) == 0) {
            return;
        }
    } else if (state->cycleTimer > 6.0f && state->hitVolumeDelayCycles == 0) {
        ObjHits_SetHitVolumeSlot(&obj->anim, 0x1A, 1, 0);
    }

    obj->anim.localPos.x = obj->anim.velocity.x * state->cycleTimer + state->launchOrigin.x;
    obj->anim.localPos.y = obj->anim.velocity.y * state->cycleTimer + state->launchOrigin.y;
    obj->anim.localPos.z = obj->anim.velocity.z * state->cycleTimer + state->launchOrigin.z;
}

static void flameblast_init(GameObject* obj, const FlameblastPlacement* placement) {
    FlameblastState* state = obj->extra;

    flameblast_resetFlight(obj, state);
    state->cycleTimer = 3.4285715f * ObjAnim_ReadPlacementS16(&obj->anim, &placement->streamIndex);
    state->hitVolumeDelayCycles = 2;
}

ObjectDescriptor gFlameblastObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .init = (ObjectDescriptorCallback)flameblast_init,
    .update = (ObjectDescriptorCallback)flameblast_update,
    .render = (ObjectDescriptorCallback)flameblast_render,
    .getExtraSize = flameblast_getExtraSize,
};
