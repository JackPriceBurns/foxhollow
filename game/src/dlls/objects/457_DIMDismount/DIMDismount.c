#include "dlls/objects/457_DIMDismount.h"

#include "dolphin/MSL_C/PPCEABI/bare/H/math_trig_api.h"
#include "game/objects/object_setup.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/objprint_render_api.h"
#include "main/objseq.h"
#include "main/objtype.h"
#include "main/object_render.h"
#include "sys/objects.h"

typedef enum DimDismountSide {
    DIM_DISMOUNT_SIDE_POSITIVE,
    DIM_DISMOUNT_SIDE_NEGATIVE
} DimDismountSide;

struct DimDismountPlacement {
    ObjPlacement base;
    s8 rotationX;
    u8 pad19[11];
};

typedef struct DimDismountState {
    Vec3f planeNormal;
    f32 planeConstant;
} DimDismountState;

typedef int (*DimDismountCanUsePointFn)(GameObject* mount, GameObject* dismountPoint);

STATIC_ASSERT(offsetof(DimDismountPlacement, rotationX) == 0x18);
STATIC_ASSERT(sizeof(DimDismountPlacement) == 0x24);
STATIC_ASSERT(offsetof(DimDismountState, planeConstant) == 0x0C);
STATIC_ASSERT(sizeof(DimDismountState) == 0x10);

void DIMDismountPoint_runOppositeSideSequence(GameObject* obj, int side) {
    (*gObjectTriggerInterface)->runSequence((side ^ 1) + 2, obj, -1);
}

int DIMDismountPoint_getPlayerSide(GameObject* obj) {
    GameObject* player = Obj_GetPlayerObject();
    DimDismountState* state = obj->extra;
    f32 signedDistance;
    int playerSide;

    signedDistance = state->planeConstant +
                     (state->planeNormal.z * player->anim.localPosZ +
                      (state->planeNormal.x * player->anim.localPosX +
                       state->planeNormal.y * player->anim.localPosY));

    if (signedDistance >= 0.0f) {
        playerSide = DIM_DISMOUNT_SIDE_POSITIVE;
    } else {
        playerSide = DIM_DISMOUNT_SIDE_NEGATIVE;
    }
    (*gObjectTriggerInterface)->runSequence(playerSide, obj, -1);
    return playerSide;
}

int DIMDismountPoint_getExtraSize(void) {
    return sizeof(DimDismountState);
}

int DIMDismountPoint_getObjectTypeId(void) {
    return 0;
}

void DIMDismountPoint_free(GameObject* obj) {
    objFreeObjectType(obj, DIM_DISMOUNT_POINT_OBJECT_GROUP);
}

void DIMDismountPoint_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible) {
    if (visible == 0 || obj->userData2 != 0) {
        if (obj->userData2 != 0) {
            objUpdateHitVolumeTransforms(obj);
        }
    } else {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void DIMDismountPoint_hitDetect(void) {
}

void DIMDismountPoint_update(GameObject* obj) {
    GameObject* nearestNeighbor;
    f32 searchRadius;

    searchRadius = 500.0f;
    nearestNeighbor = objGetNearestTypeTo(DIM_DISMOUNT_MOUNT_OBJECT_GROUP, obj, &searchRadius);
    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    if (mainGetBit(GAMEBIT_NW_SnowHorn03E3) != 0) {
        obj->hitVolumeIndex = 1;
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_PROMPT_SUPPRESSED;
    } else {
        obj->hitVolumeIndex = 0;
        if (nearestNeighbor != NULL && ((DimDismountCanUsePointFn)nearestNeighbor->anim.dll[0][8])(
                                           nearestNeighbor, obj) != 0) {
            obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_PROMPT_SUPPRESSED;
        } else {
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
        }
    }
    if ((obj->anim.modelInstance->flags & OBJDEF_FLAG_HAS_MODELS) != 0 && obj->anim.hitVolumeTransforms != NULL) {
        objUpdateHitVolumeTransforms(obj);
    }
}

void DIMDismountPoint_init(GameObject* obj, const DimDismountPlacement* placement) {
    DimDismountState* state;

    objAddObjectType(obj, DIM_DISMOUNT_POINT_OBJECT_GROUP);
    obj->anim.rotX = placement->rotationX * 256;
    state = obj->extra;
    state->planeNormal.x = mathSinf(3.1415927f * (f32)(s32)obj->anim.rotX / 32768.0f);
    state->planeNormal.y = 0.0f;
    state->planeNormal.z = mathCosf(3.1415927f * (f32)(s32)obj->anim.rotX / 32768.0f);
    state->planeConstant =
        -(state->planeNormal.x * obj->anim.localPosX + state->planeNormal.y * obj->anim.localPosY +
          state->planeNormal.z * obj->anim.localPosZ);
    obj->userData2 = 1;
}

void DIMDismountPoint_release(void) {
}

void DIMDismountPoint_initialise(void) {
}

ObjectDescriptor12 gDIMDismountPointObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_12_SLOTS,
    (ObjectDescriptorCallback)DIMDismountPoint_initialise,
    (ObjectDescriptorCallback)DIMDismountPoint_release,
    0,
    (ObjectDescriptorCallback)DIMDismountPoint_init,
    (ObjectDescriptorCallback)DIMDismountPoint_update,
    (ObjectDescriptorCallback)DIMDismountPoint_hitDetect,
    (ObjectDescriptorCallback)DIMDismountPoint_render,
    (ObjectDescriptorCallback)DIMDismountPoint_free,
    (ObjectDescriptorCallback)DIMDismountPoint_getObjectTypeId,
    DIMDismountPoint_getExtraSize,
    (ObjectDescriptorCallback)DIMDismountPoint_getPlayerSide,
    (ObjectDescriptorCallback)DIMDismountPoint_runOppositeSideSequence,
};
