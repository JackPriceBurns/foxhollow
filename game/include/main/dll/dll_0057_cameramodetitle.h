#ifndef MAIN_DLL_DLL_0057_CAMERAMODETITLE_H_
#define MAIN_DLL_DLL_0057_CAMERAMODETITLE_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef enum CameraModeTitleResourceId {
    CAMERA_MODE_TITLE_RESOURCE_ID = 0x57,
} CameraModeTitleResourceId;

typedef enum CameraModeTitlePoseIndex {
    CAMERA_MODE_TITLE_REST_POSE = 4,
    CAMERA_MODE_TITLE_POSE_COUNT = 5,
} CameraModeTitlePoseIndex;

typedef struct CameraModeTitlePose {
    f32 x;
    f32 y;
    f32 z;
    u16 yaw;
    u16 pitch;
    u16 roll;
} CameraModeTitlePose;

STATIC_ASSERT(offsetof(CameraModeTitlePose, x) == 0x00);
STATIC_ASSERT(offsetof(CameraModeTitlePose, y) == 0x04);
STATIC_ASSERT(offsetof(CameraModeTitlePose, z) == 0x08);
STATIC_ASSERT(offsetof(CameraModeTitlePose, yaw) == 0x0C);
STATIC_ASSERT(offsetof(CameraModeTitlePose, pitch) == 0x0E);
STATIC_ASSERT(offsetof(CameraModeTitlePose, roll) == 0x10);
STATIC_ASSERT(sizeof(CameraModeTitlePose) == 0x14);

typedef struct CameraModeTitleDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera);
    void (*update)(CameraObject* camera);
    void (*loadVolumes)(void);
    void (*moveCam)(u8 newPose);
} CameraModeTitleDescriptor;

STATIC_ASSERT(offsetof(CameraModeTitleDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeTitleDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeTitleDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeTitleDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeTitleDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeTitleDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeTitleDescriptor, loadVolumes) == 0x24);
STATIC_ASSERT(offsetof(CameraModeTitleDescriptor, moveCam) == 0x28);
STATIC_ASSERT(sizeof(CameraModeTitleDescriptor) == 0x2C);

extern CameraModeTitlePose gCameraModeTitlePoseTable[CAMERA_MODE_TITLE_POSE_COUNT];
extern CameraModeTitleDescriptor gCameraModeTitleDescriptor;

f32 titleScreenGetCamProgress(void);
void CameraModeTitle_moveCam(u8 newPose);
void CameraModeTitle_loadVolumes(void);
void CameraModeTitle_update(CameraObject* camera);
void CameraModeTitle_init(CameraObject* camera);
void CameraModeTitle_release(void);
void CameraModeTitle_initialise(void);

#endif /* MAIN_DLL_DLL_0057_CAMERAMODETITLE_H_ */
