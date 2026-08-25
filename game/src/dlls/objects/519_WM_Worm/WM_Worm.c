#include "dlls/objects/519_WM_Worm.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/dll/expgfx_interface.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/vecmath_distance.h"
#include "sys/objects.h"

struct WmWormPlacement {
    ObjPlacement base;
    s8 effectScale;
    u8 pad19;
    s16 particleEffectId;
    s16 spawnCountOrCooldown;
};

typedef struct WmWormState {
    f32 effectScale;
    s16 particleEffectId;
    u8 pad06[2];
    s16 spawnCountOrCooldown;
    u8 pad0A[2];
    s16 zeroIntervalActive;
    u8 pad0E[2];
    Vec3f homePosition;
} WmWormState;

STATIC_ASSERT(offsetof(WmWormPlacement, effectScale) == 0x18);
STATIC_ASSERT(offsetof(WmWormPlacement, particleEffectId) == 0x1A);
STATIC_ASSERT(offsetof(WmWormPlacement, spawnCountOrCooldown) == 0x1C);
STATIC_ASSERT(offsetof(WmWormState, particleEffectId) == 0x04);
STATIC_ASSERT(offsetof(WmWormState, spawnCountOrCooldown) == 0x08);
STATIC_ASSERT(offsetof(WmWormState, zeroIntervalActive) == 0x0C);
STATIC_ASSERT(offsetof(WmWormState, homePosition) == 0x10);
STATIC_ASSERT(sizeof(WmWormState) == 0x1C);

int WM_Worm_getExtraSize(void) {
    return sizeof(WmWormState);
}

int WM_Worm_getObjectTypeId(void) {
    return 0;
}

void WM_Worm_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void WM_Worm_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }
}

void WM_Worm_hitDetect(void) {
}

void WM_Worm_update(GameObject* obj) {
    f32 dx;
    f32 dy;
    f32 dz;
    GameObject* player;
    WmWormState* state;
    const WmWormPlacement* placement;
    int spawnCountOrCooldown;
    f32 distance;

    state = obj->extra;
    placement = (const WmWormPlacement*)obj->anim.placementData;
    player = Obj_GetPlayerObject();
    if (player != NULL) {
        distance = Vec_xzDistance(&player->anim.worldPosX, &placement->base.posX);
        if (distance > 440.0f) {
            obj->anim.localPosX = state->homePosition.x;
            obj->anim.localPosY = state->homePosition.y;
            obj->anim.localPosZ = state->homePosition.z;
        } else {
            dx = player->anim.worldPosX - obj->anim.localPosX;
            dy = player->anim.worldPosY - obj->anim.localPosY;
            dz = player->anim.worldPosZ - obj->anim.localPosZ;
            if ((dx > 0.0f) || (dx < 0.0f)) {
                dx = 0.01f * dx;
                obj->anim.localPosX = dx * timeDelta + obj->anim.localPosX;
            }
            if ((dy > 0.0f) || (dy < 0.0f)) {
                dy = 0.01f * dy;
                obj->anim.localPosY = dy * timeDelta + obj->anim.localPosY;
            }
            if ((dz > 0.0f) || (dz < 0.0f)) {
                dz = 0.01f * dz;
                obj->anim.localPosZ = dz * timeDelta + obj->anim.localPosZ;
            }
            spawnCountOrCooldown = state->spawnCountOrCooldown;
            if (spawnCountOrCooldown >= 0 || (spawnCountOrCooldown < 0 && obj->userData1 <= 0)) {
                if (spawnCountOrCooldown == 0) {
                    state->zeroIntervalActive = 1;
                }
                obj->anim.rotX += 300;
                if (state->spawnCountOrCooldown > 0) {
                    for (s16 effectIndex = 0; effectIndex < state->spawnCountOrCooldown; effectIndex++) {
                        (*gPartfxInterface)->spawnObject(obj, state->particleEffectId, NULL, 4, -1, NULL);
                    }
                } else {
                    (*gPartfxInterface)->spawnObject(obj, state->particleEffectId, NULL, 4, -1, NULL);
                }
                obj->userData1 = -state->spawnCountOrCooldown;
            } else if (spawnCountOrCooldown < 0 && obj->userData1 > 0) {
                obj->userData1 -= framesThisStep;
            }
        }
    }
}

void WM_Worm_init(GameObject* obj, const WmWormPlacement* placement) {
    WmWormState* state;

    obj->anim.rotX = 0;
    state = obj->extra;
    state->effectScale = placement->effectScale * 4.0f;
    state->particleEffectId = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->particleEffectId));
    state->spawnCountOrCooldown = ObjAnim_ReadPlacementS16(&obj->anim, &placement->spawnCountOrCooldown);
    state->zeroIntervalActive = 0;
    obj->userData1 = state->spawnCountOrCooldown < 1 ? state->spawnCountOrCooldown : 0;
    state->homePosition.x = obj->anim.localPosX;
    state->homePosition.y = obj->anim.localPosY;
    state->homePosition.z = obj->anim.localPosZ;
}

void WM_Worm_release(void) {
}

void WM_Worm_initialise(void) {
}

OBJECT_INIT_ADAPTER(gWM_WormObjDescriptorInitAdapter, WM_Worm_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gWM_WormObjDescriptorHitDetectAdapter, WM_Worm_hitDetect)
OBJECT_FREE_ADAPTER(gWM_WormObjDescriptorFreeAdapter, WM_Worm_free, obj)
OBJECT_TYPE_ID_ADAPTER(gWM_WormObjDescriptorTypeIdAdapter, WM_Worm_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gWM_WormObjDescriptorExtraSizeAdapter, WM_Worm_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gWM_WormObjDescriptorAcquire, WM_Worm_initialise)

ObjectDescriptor gWM_WormObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gWM_WormObjDescriptorAcquire,
        WM_Worm_release,
    },
    0,
    gWM_WormObjDescriptorInitAdapter,
    WM_Worm_update,
    gWM_WormObjDescriptorHitDetectAdapter,
    WM_Worm_render,
    gWM_WormObjDescriptorFreeAdapter,
    gWM_WormObjDescriptorTypeIdAdapter,
    gWM_WormObjDescriptorExtraSizeAdapter,
};
