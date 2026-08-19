#include "dlls/objects/354_CFMagicWall.h"
#include "main/camera.h"
#include "main/gamebits_api.h"
#include "main/obj_query.h"
#include "main/object_render.h"
#include "sys/objects.h"

#define CFMAGICWALL_MAX_VISIBLE_YAW 0x4000

int cfmagicwall_getExtraSize(void) {
    return 0;
}

int cfmagicwall_getObjectTypeId(void) {
    return 0;
}

void cfmagicwall_free(void) {
}

void cfmagicwall_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void cfmagicwall_hitDetect(void) {
}

void cfmagicwall_update(GameObject* obj) {
    CfMagicWallPlacement* placement = (CfMagicWallPlacement*)obj->anim.placementData;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->visibleGameBit)) == 0) {
        return;
    }

    GameObject* player = Obj_GetPlayerObject();
    int yaw = Obj_GetYawDeltaToObject(obj, player, NULL);
    yaw = yaw >= 0 ? yaw : -yaw;

    if (yaw > CFMAGICWALL_MAX_VISIBLE_YAW) {
        obj->anim.alpha = 0;
        return;
    }

    f32 range = ObjAnim_ReadPlacementS16(&obj->anim, &placement->fadeRange);
    f32 playerDistance = Vec_distance(&obj->anim.worldPosX, &player->anim.worldPosX);
    f32 fadeDistance =
        Camera_DistanceToCurrentViewPosition(obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ);

    if (fadeDistance < playerDistance) {
        fadeDistance =
            Camera_DistanceToCurrentViewPosition(obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ);
    } else {
        fadeDistance = playerDistance;
    }

    obj->anim.alpha = fadeDistance < range ? 255.0f * (fadeDistance / range) : 255.0f;
}

void cfmagicwall_init(GameObject* obj, CfMagicWallPlacement* placement) {
    obj->anim.rotX = placement->rotXByte << 8;
}

void cfmagicwall_release(void) {
}

void cfmagicwall_initialise(void) {
}

ObjectDescriptor gCFMagicWallObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)cfmagicwall_initialise,
    (ObjectDescriptorCallback)cfmagicwall_release,
    0,
    (ObjectDescriptorCallback)cfmagicwall_init,
    (ObjectDescriptorCallback)cfmagicwall_update,
    (ObjectDescriptorCallback)cfmagicwall_hitDetect,
    (ObjectDescriptorCallback)cfmagicwall_render,
    (ObjectDescriptorCallback)cfmagicwall_free,
    (ObjectDescriptorCallback)cfmagicwall_getObjectTypeId,
    cfmagicwall_getExtraSize,
};
