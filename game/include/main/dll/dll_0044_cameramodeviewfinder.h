#ifndef MAIN_DLL_DLL_0044_CAMERAMODEVIEWFINDER_H_
#define MAIN_DLL_DLL_0044_CAMERAMODEVIEWFINDER_H_

#include "global.h"
#include "game/objects/object.h"
#include "main/camera_object.h"
#include "main/curve.h"
#include "main/resource.h"

enum CameraModeViewfinderResourceId {
    CAMERA_MODE_VIEWFINDER_RESOURCE_ID = 0x44,
};

enum CameraModeViewfinderPhase {
    CAMERA_MODE_VIEWFINDER_PHASE_ENTER_BLEND,
    CAMERA_MODE_VIEWFINDER_PHASE_YAW_SETTLE,
    CAMERA_MODE_VIEWFINDER_PHASE_ACTIVE,
    CAMERA_MODE_VIEWFINDER_PHASE_EXIT_BLEND,
    CAMERA_MODE_VIEWFINDER_PHASE_FADE_BACK,
    CAMERA_MODE_VIEWFINDER_PHASE_IDLE,
};

typedef struct CameraModeViewfinderSettings {
    f32 radius;
    f32 yOffset;
    u16 height;
    u8 pad0A[2];
} CameraModeViewfinderSettings;

STATIC_ASSERT(offsetof(CameraModeViewfinderSettings, radius) == 0x00);
STATIC_ASSERT(offsetof(CameraModeViewfinderSettings, yOffset) == 0x04);
STATIC_ASSERT(offsetof(CameraModeViewfinderSettings, height) == 0x08);
STATIC_ASSERT(offsetof(CameraModeViewfinderSettings, pad0A) == 0x0A);
STATIC_ASSERT(sizeof(CameraModeViewfinderSettings) == 0x0C);

typedef struct CameraModeViewfinderPose {
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    u8 pad06[2];
    f32 positionX;
    f32 positionY;
    f32 positionZ;
    f32 fov;
} CameraModeViewfinderPose;

STATIC_ASSERT(offsetof(CameraModeViewfinderPose, rotationX) == 0x00);
STATIC_ASSERT(offsetof(CameraModeViewfinderPose, rotationY) == 0x02);
STATIC_ASSERT(offsetof(CameraModeViewfinderPose, rotationZ) == 0x04);
STATIC_ASSERT(offsetof(CameraModeViewfinderPose, pad06) == 0x06);
STATIC_ASSERT(offsetof(CameraModeViewfinderPose, positionX) == 0x08);
STATIC_ASSERT(offsetof(CameraModeViewfinderPose, positionY) == 0x0C);
STATIC_ASSERT(offsetof(CameraModeViewfinderPose, positionZ) == 0x10);
STATIC_ASSERT(offsetof(CameraModeViewfinderPose, fov) == 0x14);
STATIC_ASSERT(sizeof(CameraModeViewfinderPose) == 0x18);

typedef struct CameraModeViewfinderHermiteChannel {
    f32 start;
    f32 end;
    f32 startTangent;
    f32 endTangent;
} CameraModeViewfinderHermiteChannel;

STATIC_ASSERT(offsetof(CameraModeViewfinderHermiteChannel, start) == 0x00);
STATIC_ASSERT(offsetof(CameraModeViewfinderHermiteChannel, end) == 0x04);
STATIC_ASSERT(offsetof(CameraModeViewfinderHermiteChannel, startTangent) == 0x08);
STATIC_ASSERT(offsetof(CameraModeViewfinderHermiteChannel, endTangent) == 0x0C);
STATIC_ASSERT(sizeof(CameraModeViewfinderHermiteChannel) == 0x10);

typedef struct CameraModeViewfinderFlags {
    u8 zoomHudEnabled : 1;
    u8 sfxEnabled : 1;
    u8 zoomSfxPlaying : 1;
    u8 rest : 5;
} CameraModeViewfinderFlags;

STATIC_ASSERT(sizeof(CameraModeViewfinderFlags) == 0x01);

typedef struct CameraModeViewfinderState {
    f32 radius;
    f32 yOffset;
    u8 unknown08[0x10 - 0x08];
    CameraModeViewfinderHermiteChannel positionXCurve;
    CameraModeViewfinderHermiteChannel positionYCurve;
    CameraModeViewfinderHermiteChannel positionZCurve;
    CameraModeViewfinderHermiteChannel yawCurve;
    CameraModeViewfinderHermiteChannel pitchCurve;
    u8 unknown60[0x78 - 0x60];
    Curve transitionCurve;
    f32 height;
    f32 exitDistance;
    f32 yawSpeed;
    f32 cameraPositionX;
    f32 cameraPositionY;
    f32 cameraPositionZ;
    u8 phase;
    CameraModeViewfinderFlags flags;
    u8 pad12E[2];
    f32 clampedPositionY;
} CameraModeViewfinderState;

STATIC_ASSERT(offsetof(CameraModeViewfinderState, radius) == 0x000);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, yOffset) == 0x004);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, unknown08) == 0x008);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, positionXCurve) == 0x010);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, positionYCurve) == 0x020);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, positionZCurve) == 0x030);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, yawCurve) == 0x040);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, pitchCurve) == 0x050);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, unknown60) == 0x060);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, transitionCurve) == 0x078);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, height) == 0x114);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, exitDistance) == 0x118);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, yawSpeed) == 0x11C);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, cameraPositionX) == 0x120);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, cameraPositionY) == 0x124);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, cameraPositionZ) == 0x128);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, phase) == 0x12C);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, flags) == 0x12D);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, pad12E) == 0x12E);
STATIC_ASSERT(offsetof(CameraModeViewfinderState, clampedPositionY) == 0x130);
STATIC_ASSERT(sizeof(CameraModeViewfinderState) == 0x134);

typedef struct CameraModeViewfinderDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, int mode, CameraModeViewfinderSettings* settings);
    void (*update)(CameraObject* camera);
    void (*free)(CameraObject* camera);
    void (*copyToCurrent)(const CameraModeViewfinderPose* pose);
    ResourceDescriptorCallback reserved2C;
} CameraModeViewfinderDescriptor;

STATIC_ASSERT(offsetof(CameraModeViewfinderDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeViewfinderDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeViewfinderDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeViewfinderDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeViewfinderDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeViewfinderDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeViewfinderDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeViewfinderDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeViewfinderDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeViewfinderDescriptor) == 0x30);

extern CameraModeViewfinderState* gCameraModeViewfinderState;
extern CameraModeViewfinderDescriptor gCameraModeViewfinderDescriptor;

void firstPersonPlaceCamera(GameObject* focus, int resetClamp);
void firstPersonExit(CameraObject* camera);
void firstPersonDoControls(CameraObject* camera);
int firstPersonEnter(CameraObject* camera, GameObject* focus);
void CameraModeViewfinder_copyToCurrent(const CameraModeViewfinderPose* pose);
void CameraModeViewfinder_free(CameraObject* camera);
void CameraModeViewfinder_update(CameraObject* camera);
void CameraModeViewfinder_init(CameraObject* camera, int mode, CameraModeViewfinderSettings* settings);
void CameraModeViewfinder_release(void);
void CameraModeViewfinder_initialise(void);

#endif /* MAIN_DLL_DLL_0044_CAMERAMODEVIEWFINDER_H_ */
