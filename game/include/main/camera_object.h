#ifndef MAIN_CAMERA_OBJECT_H_
#define MAIN_CAMERA_OBJECT_H_

#include "game/objects/object_fwd.h"
#include "global.h"
#include "main/dll/DR/dr_types.h"
#include "main/objanim_internal.h"

/*
 * The camera controller and the camera-mode DLLs share this object. On the
 * original 32-bit target they described it with two separate structs whose
 * fields overlapped by offset. Keeping those structs separate on a native
 * host is unsafe: every pointer in ObjAnimComponent widens, while the manual
 * padding in the controller's copy does not.
 *
 * This is the canonical native layout. The anonymous views below retain the
 * intentional aliases used by the two APIs without relying on 32-bit byte
 * offsets:
 *
 *   localFrameObj == anim.parent
 *   focusObj      == anim.targetObj
 *   prevLocal     == savedLocalPos
 *   fovY          == fov
 *   prevWorld     == probePos
 *   overrideTarget == targetObj
 */
typedef struct CameraObject {
    union {
        ObjAnimComponent anim;
        struct {
            s16 yaw;
            s16 pitch;
            s16 roll;
            u8 padToLocalPosition[offsetof(ObjAnimComponent, localPosX) - 3 * sizeof(s16)];
            f32 localX;
            f32 localY;
            f32 localZ;
            f32 worldX;
            f32 worldY;
            f32 worldZ;
            u8 padToLocalFrame[offsetof(ObjAnimComponent, parent) -
                               (offsetof(ObjAnimComponent, worldPosZ) + sizeof(f32))];
            GameObject* localFrameObj;
            u8 padToCollisionSweepRadius[offsetof(ObjAnimComponent, hitVolumeTransforms) -
                                         (offsetof(ObjAnimComponent, parent) + sizeof(void*))];
            f32 collisionSweepRadius;
            u8 padToCollisionSweepState[offsetof(ObjAnimComponent, previousLocalPosY) -
                                        (offsetof(ObjAnimComponent, hitVolumeTransforms) + sizeof(f32))];
            s8 collisionSweepState;
            u8 padToCollisionSweepFlags[offsetof(ObjAnimComponent, previousLocalPosZ) -
                                        (offsetof(ObjAnimComponent, previousLocalPosY) + sizeof(s8))];
            s8 collisionSweepFlags;
            u8 padToFocus[offsetof(ObjAnimComponent, targetObj) -
                          (offsetof(ObjAnimComponent, previousLocalPosZ) + sizeof(s8))];
            ObjAnimComponent* focusObj;
            u8 padToAnimEnd[sizeof(ObjAnimComponent) -
                            (offsetof(ObjAnimComponent, targetObj) + sizeof(void*))];
        };
    };

    u16 objectFlags;
    u16 cameraObjectReserved;

    union {
        Vec3f savedLocalPos;
        struct {
            f32 prevLocalX;
            f32 prevLocalY;
            f32 prevLocalZ;
        };
    };
    union {
        f32 fov;
        f32 fovY;
    };
    union {
        struct {
            f32 probePosX;
            f32 probePosY;
            f32 probePosZ;
        };
        struct {
            f32 prevWorldX;
            f32 prevWorldY;
            f32 prevWorldZ;
        };
    };
    union {
        f32 unkC4;
        f32 focusMoveAverage;
    };
    f32 focusMoveHistory[5];
    union {
        struct {
            f32 overrideWorldX;
            f32 overrideWorldY;
            f32 overrideWorldZ;
        };
        Vec3f overrideWorldPos;
    };
    u8 cameraModeReserved[12];
    f32 blendProgress;
    f32 blendStep;
    u32 blendReserved;
    s16 blendDeltaYaw;
    s16 blendDeltaPitch;
    s16 blendDeltaRoll;
    s16 blendStartYaw;
    s16 blendStartPitch;
    s16 blendStartRoll;
    f32 blendStartX;
    f32 blendStartY;
    f32 blendStartZ;
    f32 blendStartFovY;
    union {
        void* targetObj;
        GameObject* overrideTarget;
    };
    GameObject* targetReticleOverride;
    GameObject* currentTarget;
    GameObject* targetReticleFocus;
    f32 boundHitZLower;
    f32 boundHitZUpper;
    f32 targetDistance;
    u8 targetKind;
    u8 blendCurveMode;
    u8 targetReserved;
    s8 letterboxTargetOffset;
    s8 letterboxStep;
    u8 overrideWorldPosPending;
    u8 unk13E;
    u8 queuedBlendFlags;
    u8 frameFlags;
    u8 targetFlags;
    u8 cameraCollisionActive;
    BitFlags8 smoothingFlags;
    u8 cameraTailReserved[4];
    int unk148;
} CameraObject;

/* These relationships, rather than legacy byte offsets, are the native ABI. */
STATIC_ASSERT(offsetof(CameraObject, anim.parent) == offsetof(CameraObject, localFrameObj));
STATIC_ASSERT(offsetof(CameraObject, anim.hitVolumeTransforms) == offsetof(CameraObject, collisionSweepRadius));
STATIC_ASSERT(offsetof(CameraObject, anim.previousLocalPosY) == offsetof(CameraObject, collisionSweepState));
STATIC_ASSERT(offsetof(CameraObject, anim.previousLocalPosZ) == offsetof(CameraObject, collisionSweepFlags));
STATIC_ASSERT(offsetof(CameraObject, anim.targetObj) == offsetof(CameraObject, focusObj));
STATIC_ASSERT(offsetof(CameraObject, savedLocalPos) == offsetof(CameraObject, prevLocalX));
STATIC_ASSERT(offsetof(CameraObject, fov) == offsetof(CameraObject, fovY));
STATIC_ASSERT(offsetof(CameraObject, probePosX) == offsetof(CameraObject, prevWorldX));
STATIC_ASSERT(offsetof(CameraObject, targetObj) == offsetof(CameraObject, overrideTarget));

#endif /* MAIN_CAMERA_OBJECT_H_ */
