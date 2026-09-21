/*
 * DLL 81 / 0x51 - cannon camera mode.
 */
#include "main/dll/dll_0051_cameramodecannon.h"
#include "dolphin/math.h"
#include "main/mm.h"
#include "main/objprint.h"

CameraModeCannonState* gCameraModeCannonState;

void CameraModeCannon_copyToCurrent(void) {
}

void CameraModeCannon_free(void) {
    mm_free(gCameraModeCannonState);
    gCameraModeCannonState = NULL;
}

void CameraModeCannon_update(CameraObject* camera) {
    s16* modelRotation = objFindJointPoseVector(gCameraModeCannonState->target, 0);
    if (gCameraModeCannonState->target == NULL) {
        return;
    }

    s16 yawDelta = 0x8000 - gCameraModeCannonState->target->anim.rotX - modelRotation[1] - camera->anim.rotX;
    camera->anim.rotX = camera->anim.rotX + yawDelta / 5.0f;
    camera->anim.localPosX =
        gCameraModeCannonState->target->anim.localPosX - 60.0f * mathSinf(3.1415927f * -camera->anim.rotX / 32768.0f);
    camera->anim.localPosY = 80.0f + gCameraModeCannonState->target->anim.localPosY;
    camera->anim.localPosZ =
        gCameraModeCannonState->target->anim.localPosZ - 60.0f * mathCosf(3.1415927f * -camera->anim.rotX / 32768.0f);
}

void CameraModeCannon_init(CameraObject* camera, int unused, CameraModeCannonInitParams* params) {
    if (gCameraModeCannonState == NULL) {
        gCameraModeCannonState = (CameraModeCannonState*)mmAlloc(sizeof(CameraModeCannonState), 15, 0);
    }
    if (params != NULL) {
        gCameraModeCannonState->target = params->target;
    } else {
        gCameraModeCannonState->target = NULL;
    }
    camera->anim.rotY = 2800;
}

void CameraModeCannon_release(void) {
}

void CameraModeCannon_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gCameraModeCannonDescriptorAcquire, CameraModeCannon_initialise)

CameraModeCannonDescriptor gCameraModeCannonDescriptor = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00060000}, gCameraModeCannonDescriptorAcquire, CameraModeCannon_release},
    NULL,
    CameraModeCannon_init,
    CameraModeCannon_update,
    CameraModeCannon_free,
    CameraModeCannon_copyToCurrent,
    NULL,
};
