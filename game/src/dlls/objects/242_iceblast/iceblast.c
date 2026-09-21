#include "dlls/objects/242_iceblast.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "main/obj_path.h"
#include "main/objhits.h"

typedef enum IceblastHitType {
    ICEBLAST_HIT_TYPE_DEFAULT = 1,
    ICEBLAST_HIT_TYPE_ALTERNATE = 3
} IceblastHitType;

typedef struct IceblastState {
    f32 launchTimer;
} IceblastState;

int iceblast_getExtraSize(void) {
    return sizeof(IceblastState);
}

int iceblast_getObjectTypeId(void) {
    return 0;
}

void iceblast_free(GameObject* obj) {
}

void iceblast_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5) {
    objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
}

void iceblast_hitDetect(GameObject* obj) {
}

void iceblast_update(GameObject* obj) {
    GameObject* player = Obj_GetPlayerObject();
    IceblastState* state = obj->extra;
    const IceblastPlacement* placement = obj->anim.placementData;
    GameObject* pathObj;

    if (player != NULL && (pathObj = player->childObjs[0]) != NULL) {
        obj->anim.rotX = pathObj->anim.rotX;
        obj->anim.rotY = pathObj->anim.rotY;
        obj->anim.rotZ = pathObj->anim.rotZ;
    } else {
        return;
    }
    ObjHits_SetHitVolumeSlot(&obj->anim, 0x10,
                             placement->hitVolumeMode != 0 ? ICEBLAST_HIT_TYPE_ALTERNATE
                                                           : ICEBLAST_HIT_TYPE_DEFAULT,
                             0);

    state->launchTimer -= timeDelta;
    if (state->launchTimer <= 0.0f) {
        MatrixTransform rotationArg = {
            .rotX = pathObj->anim.rotX,
            .rotY = pathObj->anim.rotY,
            .rotZ = pathObj->anim.rotZ,
            .scale = 1.0f
        };

        state->launchTimer += 24.0f;
        obj->anim.velocityX = 0.0f;
        obj->anim.velocityZ = 0.0f;
        obj->anim.velocityY = -3.0f;
        vecRotateZXY(&rotationArg.rotX, &obj->anim.velocityX);
        ObjPath_GetPointWorldPosition(pathObj, 0, &obj->anim.localPosX, &obj->anim.localPosY, &obj->anim.localPosZ,
                                      0);
        ObjHits_EnableObject(obj);
    }
    obj->anim.previousLocalPosX = obj->anim.localPosX;
    obj->anim.previousLocalPosY = obj->anim.localPosY;
    obj->anim.previousLocalPosZ = obj->anim.localPosZ;
    obj->anim.localPosX = obj->anim.velocityX * timeDelta + obj->anim.localPosX;
    obj->anim.localPosY = obj->anim.velocityY * timeDelta + obj->anim.localPosY;
    obj->anim.localPosZ = obj->anim.velocityZ * timeDelta + obj->anim.localPosZ;
}

void iceblast_init(GameObject* obj, const IceblastPlacement* placement) {
    IceblastState* state = obj->extra;

    state->launchTimer = ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialLaunchTimer);
    ObjHits_SetTargetMask(obj, 1);
}

void iceblast_release(void) {
}

void iceblast_initialise(void) {
}

OBJECT_INIT_ADAPTER(gIceblastObjDescriptorInitAdapter, iceblast_init, obj, placement)
OBJECT_RENDER_ADAPTER(gIceblastObjDescriptorRenderAdapter, iceblast_render, obj, arg2, arg3, arg4, arg5)
OBJECT_FREE_ADAPTER(gIceblastObjDescriptorFreeAdapter, iceblast_free, obj)
OBJECT_TYPE_ID_ADAPTER(gIceblastObjDescriptorTypeIdAdapter, iceblast_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gIceblastObjDescriptorExtraSizeAdapter, iceblast_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gIceblastObjDescriptorAcquire, iceblast_initialise)

ObjectDescriptor gIceblastObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gIceblastObjDescriptorAcquire,
        iceblast_release,
    },
    0,
    gIceblastObjDescriptorInitAdapter,
    iceblast_update,
    iceblast_hitDetect,
    gIceblastObjDescriptorRenderAdapter,
    gIceblastObjDescriptorFreeAdapter,
    gIceblastObjDescriptorTypeIdAdapter,
    gIceblastObjDescriptorExtraSizeAdapter,
};
