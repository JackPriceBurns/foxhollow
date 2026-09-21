/* Physics-driven debris spawned by Explodable objects. */

#include "dlls/objects/358.h"

#include "main/frame_timing.h"
#include "main/model.h"
#include "main/object_render.h"
#include "main/track_dolphin.h"
#include "main/object_transform.h"
#include "main/vecmath.h"

#define EXPLODED_PHYSICS_FLAG_WAS_BELOW_FLOOR 0x04
#define EXPLODED_OBJECT_TYPE_BANK_SHIFT       11
#define EXPLODED_OBJECT_TYPE_BASE             0x400
#define EXPLODED_FULL_ALPHA                   0xFF

void exploded_initDebrisState(GameObject* obj, ExplodedPlacement* placement, int usePresetCenter,
                              ExplodedState* state) {
    obj->anim.localPosX = placement->base.posX;
    obj->anim.localPosY = placement->base.posY;
    obj->anim.localPosZ = placement->base.posZ;

    if (usePresetCenter == 0) {
        register int vertexIndex;
        register ModelFileHeader* model;
        struct {
            f32 vertex[3];
            f32 sum[3];
        } center;
        f32 zero;
        f32 one;

        zero = 0.0f;
        state->localCenter.x = zero;
        state->localCenter.y = zero;
        state->localCenter.z = zero;
        center.sum[0] = zero;
        center.sum[1] = zero;
        center.sum[2] = zero;

        model = (ModelFileHeader*)obj->anim.modelBanks[placement->modelBankIndex];
        for (vertexIndex = 0; vertexIndex < model->vertexCount; vertexIndex++) {
            Model_GetVertexPosition(model, vertexIndex, center.vertex);
            center.sum[0] = center.vertex[0] + center.sum[0];
            center.sum[1] = center.vertex[1] + center.sum[1];
            center.sum[2] = center.vertex[2] + center.sum[2];
        }

        state->localCenter.x = center.sum[0] * ((one = 1.0f) / (f32)(u32)model->vertexCount);
        state->localCenter.y = center.sum[1] * (one / (f32)(u32)model->vertexCount);
        state->localCenter.z = center.sum[2] * (one / (f32)(u32)model->vertexCount);
    }

    state->initialLocalCenter.x = state->localCenter.x;
    state->initialLocalCenter.y = state->localCenter.y;
    state->initialLocalCenter.z = state->localCenter.z;
    exploded_seedDebrisMotion(obj, state, placement);

    {
        f32 rotatedCenter[3];
        rotatedCenter[0] = state->localCenter.x;
        rotatedCenter[1] = state->localCenter.y;
        rotatedCenter[2] = state->localCenter.z;
        vecRotateYXZ(&obj->anim.rotX, rotatedCenter);
        rotatedCenter[0] = rotatedCenter[0] * obj->anim.rootMotionScale;
        rotatedCenter[1] = rotatedCenter[1] * obj->anim.rootMotionScale;
        rotatedCenter[2] = rotatedCenter[2] * obj->anim.rootMotionScale;
    }

    state->unknown67 = EXPLODED_FULL_ALPHA;
    state->physicsFlags = 0;
}

void exploded_seedDebrisMotion(GameObject* obj, ExplodedState* state, ExplodedPlacement* placement) {
    f32 groundHeight[2];
    u16 lifetimeFrames;

    groundHeight[0] = 0.0f;
    obj->anim.rotX = ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialRotation.x);
    obj->anim.rotY = ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialRotation.y);
    obj->anim.rotZ = ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialRotation.z);

    obj->anim.velocityX = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialVelocity.x) / 100.0f;
    obj->anim.velocityY = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialVelocity.y) / 100.0f;
    obj->anim.velocityZ = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialVelocity.z) / 100.0f;
    state->spin.x = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->spin.x);
    state->spin.y = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->spin.y);
    state->spin.z = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->spin.z);

    {
        s16 floorOffset = ObjAnim_ReadPlacementS16(&obj->anim, &placement->floorOffset);
        if (floorOffset == 0) {
            trackGetHeightAboveGround(obj, obj->anim.localPosX, obj->anim.localPosY - 10.0f,
                                      obj->anim.localPosZ, groundHeight, 0);
            state->floorHeight = obj->anim.localPosY - groundHeight[0];
        } else {
            state->floorHeight = obj->anim.localPosY + (f32)floorOffset;
        }
    }

    state->spinVelocity.x = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->spinVelocity.x) / 10.0f;
    state->spinVelocity.y = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->spinVelocity.y) / 10.0f;
    state->spinVelocity.z = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->spinVelocity.z) / 10.0f;
    state->acceleration.x = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->acceleration.x) / 1000.0f;
    state->acceleration.y = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->acceleration.y) / 1000.0f;
    state->acceleration.z = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->acceleration.z) / 1000.0f;

    state->elapsedFrames = 0;
    lifetimeFrames = ObjAnim_ReadPlacementU16(&obj->anim, &placement->lifetimeFrames);
    if (lifetimeFrames != 0) {
        state->durationFrames = lifetimeFrames * ((int)randomGetRange(0, 100) + 100) / 200;
    } else {
        state->durationFrames = -1;
    }
}

u8 exploded_getPhase(GameObject* obj) {
    return ((ExplodedState*)obj->extra)->phase;
}

int exploded_getExtraSize(void) {
    return sizeof(ExplodedState);
}

u32 exploded_getObjectTypeId(GameObject* obj) {
    ExplodedPlacement* placement = (ExplodedPlacement*)obj->anim.placementData;
    return (placement->modelBankIndex << EXPLODED_OBJECT_TYPE_BANK_SHIFT) | EXPLODED_OBJECT_TYPE_BASE;
}

void exploded_free(void) {
}

void exploded_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    s32 v = visible;
    if (v != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void exploded_hitDetect(void) {
}

int exploded_stepDebrisPhysics(GameObject* obj, ExplodedState* state) {
    f32 stopped;
    f32 speed;
    f32 worldAfter[3];
    f32 worldBefore[3];

    stopped = 0.0f;
    Obj_TransformLocalPointByWorldMatrix(obj, &state->localCenter.x, worldBefore, 0);
    obj->anim.velocityX = timeDelta * state->acceleration.x + obj->anim.velocityX;
    obj->anim.velocityY = timeDelta * state->acceleration.y + obj->anim.velocityY;
    obj->anim.velocityZ = timeDelta * state->acceleration.z + obj->anim.velocityZ;
    state->spin.x = timeDelta * state->spinVelocity.x + state->spin.x;
    state->spin.y = timeDelta * state->spinVelocity.y + state->spin.y;
    state->spin.z = timeDelta * state->spinVelocity.z + state->spin.z;

    if (worldBefore[1] < state->floorHeight) {
        if (((obj->anim.velocityY < 0.0f) && ((state->physicsFlags & EXPLODED_PHYSICS_FLAG_WAS_BELOW_FLOOR) != 0)) ||
            (0.0f == obj->anim.velocityY)) {
            f32 t;
            f32 k;
            t = 0.0f;
            state->acceleration.y = t;
            state->spinVelocity.z = t;
            state->spin.z = t;
            state->spinVelocity.y = t;
            state->spin.y = t;
            state->spinVelocity.x = t;
            state->spin.x = t;
            obj->anim.velocityY = t;
            state->acceleration.x = state->acceleration.x * (k = 0.3f);
            obj->anim.velocityX = obj->anim.velocityX * k;
            state->acceleration.z = state->acceleration.z * k;
            obj->anim.velocityZ = obj->anim.velocityZ * k;
            speed = (obj->anim.velocityX >= t) ? obj->anim.velocityX : -obj->anim.velocityX;
            if (speed < 0.15f) {
                speed = (obj->anim.velocityZ >= 0.0f) ? obj->anim.velocityZ : -obj->anim.velocityZ;
                if (speed < 0.15f) {
                    stopped = 1.0f;
                }
            }
        }
        if (obj->anim.velocityY < 0.0f) {
            f32 k2;
            obj->anim.velocityY = 0.5f * -obj->anim.velocityY;
            obj->anim.velocityX = obj->anim.velocityX * (k2 = 0.3f);
            obj->anim.velocityZ = obj->anim.velocityZ * k2;
            state->acceleration.y = -0.07f;
            state->spinVelocity.z = -state->spinVelocity.z;
        }
        state->physicsFlags |= EXPLODED_PHYSICS_FLAG_WAS_BELOW_FLOOR;
    } else {
        state->physicsFlags &= ~EXPLODED_PHYSICS_FLAG_WAS_BELOW_FLOOR;
    }

    obj->anim.rotX = (s16)(state->spin.x * timeDelta + (f32)(s32)obj->anim.rotX);
    obj->anim.rotY = (s16)(state->spin.y * timeDelta + (f32)(s32)obj->anim.rotY);
    obj->anim.rotZ = (s16)(state->spin.z * timeDelta + (f32)(s32)obj->anim.rotZ);
    Obj_TransformLocalPointByWorldMatrix(obj, &state->localCenter.x, worldAfter, 0);
    worldAfter[0] = worldBefore[0] - worldAfter[0];
    worldAfter[1] = worldBefore[1] - worldAfter[1];
    worldAfter[2] = worldBefore[2] - worldAfter[2];
    obj->anim.localPosX = obj->anim.localPosX + worldAfter[0];
    obj->anim.localPosY = obj->anim.localPosY + worldAfter[1];
    obj->anim.localPosZ = obj->anim.localPosZ + worldAfter[2];
    obj->anim.localPosX = obj->anim.velocityX * timeDelta + obj->anim.localPosX;
    obj->anim.localPosY = obj->anim.velocityY * timeDelta + obj->anim.localPosY;
    obj->anim.localPosZ = obj->anim.velocityZ * timeDelta + obj->anim.localPosZ;
    return stopped;
}

void exploded_update(GameObject* obj) {
    ExplodedState* state = obj->extra;
    u8 phase = state->phase;
    int expired;

    switch (phase) {
    case EXPLODED_PHASE_IDLE:
        break;
    case EXPLODED_PHASE_ACTIVE:
        if (exploded_stepDebrisPhysics(obj, state) != 0) {
            state->phase = EXPLODED_PHASE_IDLE;
        }
        break;
    case EXPLODED_PHASE_EXPIRED:
        break;
    }
    do {
        if (state->durationFrames != -1) {
            s32 elapsedFrames = state->elapsedFrames + framesThisStep;
            s32 durationFrames;
            state->elapsedFrames = elapsedFrames;
            durationFrames = state->durationFrames;
            if (elapsedFrames >= durationFrames) {
                state->durationFrames = -1;
                obj->anim.alpha = 0;
                obj->anim.flags = (s16)(obj->anim.flags | OBJANIM_FLAG_HIDDEN);
                expired = 1;
                break;
            } else {
                s32 remainingFrames = durationFrames - state->elapsedFrames;
                if (remainingFrames < EXPLODED_FULL_ALPHA) {
                    obj->anim.alpha = remainingFrames;
                }
            }
        }
        expired = 0;
    } while (0);

    if (expired != 0) {
        state->phase = EXPLODED_PHASE_EXPIRED;
    }
}

void exploded_init(GameObject* obj, ExplodedPlacement* placement, int usePresetCenter) {
    ExplodedState* state;
    obj->anim.bankIndex = placement->modelBankIndex;
    state = obj->extra;
    obj->anim.rootMotionScale = (obj->anim.modelInstance->rootMotionScaleBase * (f32)(s32)placement->scaleByte) / 20.0f;
    exploded_initDebrisState(obj, placement, usePresetCenter, state);
    if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialVelocity.x) != 0 ||
        ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialVelocity.y) != 0 ||
        ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialVelocity.z) != 0 ||
        ObjAnim_ReadPlacementS16(&obj->anim, &placement->acceleration.x) != 0 ||
        ObjAnim_ReadPlacementS16(&obj->anim, &placement->acceleration.y) != 0 ||
        ObjAnim_ReadPlacementS16(&obj->anim, &placement->acceleration.z) != 0) {
        state->phase = EXPLODED_PHASE_ACTIVE;
    } else {
        state->phase = EXPLODED_PHASE_IDLE;
    }
}

void exploded_release(void) {
}

void exploded_initialise(void) {
}

OBJECT_INIT_ADAPTER(gExplodedObjDescriptorInitAdapter, exploded_init, obj, placement, flags)
OBJECT_HIT_DETECT_ADAPTER(gExplodedObjDescriptorHitDetectAdapter, exploded_hitDetect)
OBJECT_FREE_ADAPTER(gExplodedObjDescriptorFreeAdapter, exploded_free)
OBJECT_TYPE_ID_ADAPTER(gExplodedObjDescriptorTypeIdAdapter, exploded_getObjectTypeId, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gExplodedObjDescriptorExtraSizeAdapter, exploded_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gExplodedObjDescriptorAcquire, exploded_initialise)

ExplodedDescriptor gExplodedObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_11_SLOTS,
        },
        gExplodedObjDescriptorAcquire,
        exploded_release,
    },
    {
        0,
        gExplodedObjDescriptorInitAdapter,
        exploded_update,
        gExplodedObjDescriptorHitDetectAdapter,
        exploded_render,
        gExplodedObjDescriptorFreeAdapter,
        gExplodedObjDescriptorTypeIdAdapter,
        gExplodedObjDescriptorExtraSizeAdapter,
        exploded_getPhase,
        0,
        0,
        0,
        0,
        0,
    },
};
