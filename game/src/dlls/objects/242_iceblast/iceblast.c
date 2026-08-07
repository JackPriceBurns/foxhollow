/*
 * iceblast (DLL 0xF2) - path-following ice projectiles cast by the player.
 *
 * Each blast follows the player's first child path object. Its launch timer
 * staggers repeated velocity resets along that path, producing the seven-part
 * stream created by playerCastIceSpell.
 */
#include "dlls/objects/242_iceblast.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "main/obj_path.h"
#include "main/objhits.h"

#define ICEBLAST_OBJECT_TYPE_ID 0
#define ICEBLAST_RENDER_SCALE   1.0f
#define ICEBLAST_TARGET_MASK    1

#define ICEBLAST_HIT_VOLUME_SLOT    0x10
#define ICEBLAST_DEFAULT_HIT_TYPE   1
#define ICEBLAST_ALTERNATE_HIT_TYPE 3

#define ICEBLAST_LAUNCH_PERIOD     24.0f
#define ICEBLAST_VERTICAL_VELOCITY -3.0f

int iceblast_getExtraSize(void) {
    return sizeof(IceblastState);
}

int iceblast_getObjectTypeId(void) {
    return ICEBLAST_OBJECT_TYPE_ID;
}

void iceblast_free(GameObject* obj) {
}

void iceblast_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5) {
    objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, ICEBLAST_RENDER_SCALE);
}

void iceblast_hitDetect(GameObject* obj) {
}

void iceblast_update(GameObject* obj) {
    GameObject* pathObj;
    GameObject* player = Obj_GetPlayerObject();
    IceblastState* state = obj->extra;
    IceblastPlacement* placement = (IceblastPlacement*)obj->anim.placementData;
    MatrixTransform rotationArg;

    if (player != NULL && (pathObj = player->childObjs[0]) != NULL) {
        obj->anim.rotZ = pathObj->anim.rotZ;
        obj->anim.rotY = pathObj->anim.rotY;
        obj->anim.rotX = pathObj->anim.rotX;
    } else {
        return;
    }
    ObjHits_SetHitVolumeSlot((ObjAnimComponent*)obj, ICEBLAST_HIT_VOLUME_SLOT,
                             placement->hitVolumeMode != 0 ? ICEBLAST_ALTERNATE_HIT_TYPE
                                                           : ICEBLAST_DEFAULT_HIT_TYPE,
                             0);

    state->launchTimer -= timeDelta;
    if (state->launchTimer <= 0.0f) {
        state->launchTimer += ICEBLAST_LAUNCH_PERIOD;
        obj->anim.velocityX = 0.0f;
        obj->anim.velocityZ = 0.0f;
        obj->anim.velocityY = ICEBLAST_VERTICAL_VELOCITY;
        rotationArg.x = 0.0f;
        rotationArg.y = 0.0f;
        rotationArg.z = 0.0f;
        rotationArg.scale = 1.0f;
        rotationArg.rotZ = pathObj->anim.rotZ;
        rotationArg.rotY = pathObj->anim.rotY;
        rotationArg.rotX = pathObj->anim.rotX;
        vecRotateZXY(&rotationArg.rotX, &obj->anim.velocity.x);
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

void iceblast_init(GameObject* obj, IceblastPlacement* placement) {
    IceblastState* state = obj->extra;

    state->launchTimer = placement->initialLaunchTimer;
    ObjHits_SetTargetMask(obj, ICEBLAST_TARGET_MASK);
}

void iceblast_release(void) {
}

void iceblast_initialise(void) {
}

ObjectDescriptor gIceblastObjDescriptor = {
    0,                                                  /* reserved0 */
    0,                                                  /* reserved1 */
    0,                                                  /* reserved2 */
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,                   /* slotCountAndFlags */
    (ObjectDescriptorCallback)iceblast_initialise,      /* initialise */
    (ObjectDescriptorCallback)iceblast_release,         /* release */
    0,                                                  /* slot02 */
    (ObjectDescriptorCallback)iceblast_init,            /* init */
    (ObjectDescriptorCallback)iceblast_update,          /* update */
    (ObjectDescriptorCallback)iceblast_hitDetect,       /* hitDetect */
    (ObjectDescriptorCallback)iceblast_render,          /* render */
    (ObjectDescriptorCallback)iceblast_free,            /* free */
    (ObjectDescriptorCallback)iceblast_getObjectTypeId, /* getObjectTypeId */
    iceblast_getExtraSize,                              /* getExtraSize */
};
