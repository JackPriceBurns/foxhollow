/*
 * Object DLL 0x17D implements the retail DIM2_barrel and MMP_barrel
 * definitions.
 */
#include "dlls/objects/381.h"

#include "main/audio/sfx_trigger_ids.h"
#include "main/camera_shake.h"
#include "main/dll/rom_curve_interface.h"
#include "main/frame_timing.h"
#include "main/objtype.h"
#include "main/object_render.h"
#include "main/objfx.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "main/curve.h"
#include "main/pad.h"
#include "main/audio/sfx.h"
#include "main/objhits.h"
#include "sys/objects/lifecycle.h"

typedef struct RollingBarrelCurveInitPair {
    s32 unknown00;
    s32 unknown04;
} RollingBarrelCurveInitPair;

#define ROLLING_BARREL_OBJECT_GROUP_ID    0x2F
#define ROLLING_BARREL_DIM2_OBJECT_ID     0x72A
#define ROLLING_BARREL_MODE_ROLLING       0
#define ROLLING_BARREL_MODE_EXPLODED_WAIT 1
#define ROLLING_BARREL_MODE_RESPAWN_WAIT  2
#define ROLLING_BARREL_MODE_CLEANUP       3

s16 gRollingBarrelExplodingCount;
const RollingBarrelCurveInitPair gRollingBarrelCurveInitPair = {21, 0};

void rollingBarrel_explode(GameObject* obj, int unusedExplosionVariant) {
    RollingBarrelState* state = obj->extra;
    u32 debrisType;
    GameObject* player;
    f32 distance;
    f32 falloff;
    gRollingBarrelExplodingCount += 1;
    Sfx_PlayFromObject(obj, SFXTRIG_wp_dsmk2_c_106);
    if (gRollingBarrelExplodingCount > 1) {
        debrisType = randomGetRange(0, 1) & 0xff;
        spawnExplosion(obj, (f32)(int)randomGetRange(0x32, 0x3c), 1, 1, 0, debrisType, 0, 0, 0);
    } else {
        debrisType = randomGetRange(0, 1) & 0xff;
        spawnExplosion(obj, (f32)(int)randomGetRange(0x32, 0x3c), 1, 1, 0, debrisType, 0, 1, 0);
    }
    state->mode = ROLLING_BARREL_MODE_EXPLODED_WAIT;
    state->timer = 0.0f;
    obj->anim.flags = (s16)(obj->anim.flags | OBJANIM_FLAG_HIDDEN);
    ObjHitbox_SetSphereRadius((ObjAnimComponent*)obj,
                              (s32)(3.0f * (f32)(u32)obj->anim.modelInstance->primaryHitboxRadius));
    player = Obj_GetPlayerObject();
    if ((player->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) == 0) {
        distance = Vec_distance(&obj->anim.worldPosX, &player->anim.worldPosX);
        if (distance <= 500.0f) {
            falloff = 1.0f - distance / 500.0f;
            CameraShake_StartDampened(5.0f * falloff, 10.0f * falloff, 4.0f);
            doRumble(15.0f * falloff);
        }
    }
}

int rollingBarrel_getExtraSize(void) {
    return sizeof(RollingBarrelState);
}

int rollingBarrel_getObjectTypeId(void) {
    return 0;
}

void rollingBarrel_free(GameObject* obj) {
    RollingBarrelState* state = obj->extra;
    int count;
    u32* groupObjects = (u32*)objGetAllOfType(ROLLING_BARREL_OBJECT_GROUP_ID, &count);
    int i;
    u32 groupObject;
    for (i = 0; i < count; i++) {
        groupObject = groupObjects[i];
        if (groupObject == (u32)obj) {
            objFreeObjectType(obj, ROLLING_BARREL_OBJECT_GROUP_ID);
            break;
        }
    }
    if (state->mode == ROLLING_BARREL_MODE_EXPLODED_WAIT) {
        gRollingBarrelExplodingCount -= 1;
    }
}

void rollingBarrel_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    RollingBarrelState* state = obj->extra;
    if (visible == 0 || state->mode >= ROLLING_BARREL_MODE_EXPLODED_WAIT) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void rollingBarrel_hitDetect(void) {
}

void rollingBarrel_update(GameObject* obj) {
    RollingBarrelState* state;
    RollingBarrelPlacement* placement;
    f32 floorY;
    f32 distanceSquared;
    int blocked;
    GameObject* hitObject;
    int hitSphereIndex;
    u32 hitVolume;
    int hitType;
    u32 explosionVariant;
    u8 mode;

    state = obj->extra;
    hitObject = NULL;
    placement = (RollingBarrelPlacement*)obj->anim.placementData;
    blocked = 0;
    distanceSquared = 0.0f;
    mode = state->mode;

    switch (mode) {
    case ROLLING_BARREL_MODE_ROLLING: {
        if (placement->base.objectId == ROLLING_BARREL_DIM2_OBJECT_ID) {
            f32 maxAdvanceDistance = 3.0f;
            while (blocked == 0 && distanceSquared < maxAdvanceDistance * timeDelta) {
                blocked = Curve_AdvanceAlongPath(&state->curve.curve, state->curveSpeed);
                if (blocked == 0 && state->curve.curve.idx != 0) {
                    (*gRomCurveInterface)->goNextPoint(&state->curve);
                }
                {
                    f32 dx = state->curve.curve.sample[0] - obj->anim.previousLocalPosX;
                    f32 dz = state->curve.curve.sample[2] - obj->anim.previousLocalPosZ;
                    distanceSquared = dx * dx + dz * dz;
                }
            }
        } else {
            blocked = Curve_AdvanceAlongPath(&state->curve.curve, state->curveSpeed);
            if (blocked == 0 && state->curve.curve.idx != 0) {
                (*gRomCurveInterface)->goNextPoint(&state->curve);
            }
        }

        state->hitVolumeSlot = 10;
        ObjHitbox_SetSphereRadius((ObjAnimComponent*)obj, obj->anim.modelInstance->primaryHitboxRadius);

        if (placement->base.objectId == ROLLING_BARREL_DIM2_OBJECT_ID) {
            floorY = 5.0f + state->curve.curve.sample[1];
        } else {
            floorY = state->curve.curve.sample[1];
        }

        state->verticalSpeed = -0.1f * timeDelta + state->verticalSpeed;
        obj->anim.localPosY = state->verticalSpeed * timeDelta + obj->anim.localPosY;

        if (obj->anim.localPosY < floorY) {
            if (placement->base.objectId == ROLLING_BARREL_DIM2_OBJECT_ID && obj->anim.localPosY < -2680.0f) {
                blocked = 1;
            }
            if (blocked == 0 && state->verticalSpeed * state->verticalSpeed > 3.0f) {
                Sfx_PlayFromObjectLimited(obj, SFXTRIG_mfin2_c, 6);
            }
            state->verticalSpeed *= -0.6f;
            obj->anim.localPosY = 2.0f * floorY - obj->anim.localPosY;
        }
        obj->anim.localPosX = state->curve.curve.sample[0];
        obj->anim.localPosZ = state->curve.curve.sample[2];
        obj->anim.rotX = (s16)getAngle(state->curve.curve.tangent[0], state->curve.curve.tangent[2]);

        if (state->pitchRising != 0) {
            obj->anim.rotZ = (s16)(32.0f * timeDelta + (f32)(int)obj->anim.rotZ);
            if (obj->anim.rotZ > 0x5000) {
                state->pitchRising = 0;
            }
        } else {
            obj->anim.rotZ = (s16) - (32.0f * timeDelta - (f32)(int)obj->anim.rotZ);
            if (obj->anim.rotZ < 0x3a00) {
                state->pitchRising = 1;
            }
        }

        {
            f32 rotYStep = 512.0f * timeDelta;
            obj->anim.rotY = (s16)(rotYStep * state->curveSpeed + (f32)(int)obj->anim.rotY);
        }
        hitType = ObjHits_GetPriorityHit(obj, &hitObject, &hitSphereIndex, &hitVolume);

        if (blocked != 0 || hitObject == Obj_GetPlayerObject() || (u32)(hitType - 0xe) <= 1u ||
            hitType == 0x13) {
            if (blocked == 0) {
                state->hitVolumeSlot = 0;
            } else {
                state->hitVolumeSlot = 5;
            }
            explosionVariant = randomGetRange(0, 2);
            rollingBarrel_explode(obj, explosionVariant);
        }
        break;
    }
    case ROLLING_BARREL_MODE_EXPLODED_WAIT:
        state->timer += timeDelta;
        if (state->timer >= 30.0f) {
            state->mode = ROLLING_BARREL_MODE_RESPAWN_WAIT;
            state->timer -= 30.0f;
        }
        break;
    case ROLLING_BARREL_MODE_RESPAWN_WAIT:
        state->timer += timeDelta;
        if (state->timer >= 30.0f) {
            state->hitVolumeSlot = 0;
            state->mode = ROLLING_BARREL_MODE_CLEANUP;
            state->timer -= 30.0f;
            objAddObjectType(obj, ROLLING_BARREL_OBJECT_GROUP_ID);
            gRollingBarrelExplodingCount -= 1;
        }
        break;
    case ROLLING_BARREL_MODE_CLEANUP:
        state->timer += timeDelta;
        if (state->timer >= 3000.0f) {
            Obj_FreeObject(obj);
            return;
        }
        break;
    }

    if (state->hitVolumeSlot != 0) {
        ObjHits_EnableObject(obj);
        ObjHits_SetHitVolumeSlot((ObjAnimComponent*)obj, state->hitVolumeSlot, 1, 0);
    } else {
        ObjHits_DisableObject(obj);
        ObjHits_SetHitVolumeSlot((ObjAnimComponent*)obj, state->hitVolumeSlot, 0, 0);
    }
}

void rollingBarrel_init(GameObject* obj, RollingBarrelPlacement* placement) {
    RollingBarrelState* state = obj->extra;
    int curveInitParams[2];

    *(RollingBarrelCurveInitPair*)curveInitParams = gRollingBarrelCurveInitPair;
    placement->base.ident = -1;
    obj->anim.flags = (s16)(obj->anim.flags & ~OBJANIM_FLAG_HIDDEN);
    obj->anim.rotZ = 0x4000;

    obj->anim.localPosX = placement->base.posX;
    obj->anim.worldPosX = placement->base.posX;
    obj->anim.localPosY = placement->base.posY;
    obj->anim.worldPosY = placement->base.posY;
    obj->anim.localPosZ = placement->base.posZ;
    obj->anim.worldPosZ = placement->base.posZ;

    state->verticalSpeed = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->initialVerticalSpeed)) / 10.0f;
    state->curveSpeed = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->curveSpeed)) / 10.0f;
    state->mode = ROLLING_BARREL_MODE_ROLLING;
    state->pitchRising = 1;
    state->timer = 0.0f;

    (*gRomCurveInterface)->initCurve(&state->curve, (void*)obj, 100.0f, curveInitParams, -1);
}

void rollingBarrel_release(void) {
}

void rollingBarrel_initialise(void) {
    gRollingBarrelExplodingCount = 0;
}

OBJECT_INIT_ADAPTER(gRollingBarrelObjDescriptorInitAdapter, rollingBarrel_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gRollingBarrelObjDescriptorHitDetectAdapter, rollingBarrel_hitDetect)
OBJECT_FREE_ADAPTER(gRollingBarrelObjDescriptorFreeAdapter, rollingBarrel_free, obj)
OBJECT_TYPE_ID_ADAPTER(gRollingBarrelObjDescriptorTypeIdAdapter, rollingBarrel_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gRollingBarrelObjDescriptorExtraSizeAdapter, rollingBarrel_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gRollingBarrelObjDescriptorAcquire, rollingBarrel_initialise)

ObjectDescriptor gRollingBarrelObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gRollingBarrelObjDescriptorAcquire,
        rollingBarrel_release,
    },
    0,
    gRollingBarrelObjDescriptorInitAdapter,
    rollingBarrel_update,
    gRollingBarrelObjDescriptorHitDetectAdapter,
    rollingBarrel_render,
    gRollingBarrelObjDescriptorFreeAdapter,
    gRollingBarrelObjDescriptorTypeIdAdapter,
    gRollingBarrelObjDescriptorExtraSizeAdapter,
};
