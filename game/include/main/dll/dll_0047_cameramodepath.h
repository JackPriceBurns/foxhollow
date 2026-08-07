#ifndef MAIN_DLL_DLL_0047_CAMERAMODEPATH_H_
#define MAIN_DLL_DLL_0047_CAMERAMODEPATH_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

enum CameraModePathResourceId {
    CAMERA_MODE_PATH_RESOURCE_ID = 0x47,
};

enum CameraModePathNodeFlags {
    CAMERA_MODE_PATH_TRACK_ROT_X = 1 << 0,
    CAMERA_MODE_PATH_TRACK_ROT_Y = 1 << 1,
    CAMERA_MODE_PATH_TRACK_ROT_Z = 1 << 2,
};

typedef struct CameraModePathSettings {
    int pathTag;
    u8 skipTransition;
    u8 pad05[3];
} CameraModePathSettings;

STATIC_ASSERT(offsetof(CameraModePathSettings, pathTag) == 0x00);
STATIC_ASSERT(offsetof(CameraModePathSettings, skipTransition) == 0x04);
STATIC_ASSERT(offsetof(CameraModePathSettings, pad05) == 0x05);
STATIC_ASSERT(sizeof(CameraModePathSettings) == 0x08);

typedef struct CameraModePathState {
    ObjAnimComponent* linkedTransform;
    int pathTag;
    int moveNodeId;
    int controlNodeId;
    f32 positionStartX;
    f32 positionEndX;
    f32 positionStartY;
    f32 positionEndY;
    f32 positionStartZ;
    f32 positionEndZ;
    f32 rotationXStart;
    f32 rotationXEnd;
    f32 rotationYStart;
    f32 rotationYEnd;
    f32 rotationZStart;
    f32 rotationZEnd;
    f32 fovStart;
    f32 fovEnd;
    f32 transitionCurve[4];
    f32 pathProgress;
    f32 transitionElapsed;
    f32 transitionDuration;
    u8 transitionComplete;
    u8 pathFailed;
    u8 pad66[2];
} CameraModePathState;

STATIC_ASSERT(offsetof(CameraModePathState, linkedTransform) == 0x00);
STATIC_ASSERT(offsetof(CameraModePathState, pathTag) == 0x04);
STATIC_ASSERT(offsetof(CameraModePathState, moveNodeId) == 0x08);
STATIC_ASSERT(offsetof(CameraModePathState, controlNodeId) == 0x0C);
STATIC_ASSERT(offsetof(CameraModePathState, positionStartX) == 0x10);
STATIC_ASSERT(offsetof(CameraModePathState, positionEndX) == 0x14);
STATIC_ASSERT(offsetof(CameraModePathState, positionStartY) == 0x18);
STATIC_ASSERT(offsetof(CameraModePathState, positionEndY) == 0x1C);
STATIC_ASSERT(offsetof(CameraModePathState, positionStartZ) == 0x20);
STATIC_ASSERT(offsetof(CameraModePathState, positionEndZ) == 0x24);
STATIC_ASSERT(offsetof(CameraModePathState, rotationXStart) == 0x28);
STATIC_ASSERT(offsetof(CameraModePathState, rotationXEnd) == 0x2C);
STATIC_ASSERT(offsetof(CameraModePathState, rotationYStart) == 0x30);
STATIC_ASSERT(offsetof(CameraModePathState, rotationYEnd) == 0x34);
STATIC_ASSERT(offsetof(CameraModePathState, rotationZStart) == 0x38);
STATIC_ASSERT(offsetof(CameraModePathState, rotationZEnd) == 0x3C);
STATIC_ASSERT(offsetof(CameraModePathState, fovStart) == 0x40);
STATIC_ASSERT(offsetof(CameraModePathState, fovEnd) == 0x44);
STATIC_ASSERT(offsetof(CameraModePathState, transitionCurve) == 0x48);
STATIC_ASSERT(offsetof(CameraModePathState, pathProgress) == 0x58);
STATIC_ASSERT(offsetof(CameraModePathState, transitionElapsed) == 0x5C);
STATIC_ASSERT(offsetof(CameraModePathState, transitionDuration) == 0x60);
STATIC_ASSERT(offsetof(CameraModePathState, transitionComplete) == 0x64);
STATIC_ASSERT(offsetof(CameraModePathState, pathFailed) == 0x65);
STATIC_ASSERT(offsetof(CameraModePathState, pad66) == 0x66);
STATIC_ASSERT(sizeof(CameraModePathState) == 0x68);

typedef struct CameraModePathDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, int mode, CameraModePathSettings* settings);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
} CameraModePathDescriptor;

STATIC_ASSERT(offsetof(CameraModePathDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModePathDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModePathDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModePathDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModePathDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModePathDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModePathDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModePathDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(sizeof(CameraModePathDescriptor) == 0x2C);

extern CameraModePathState* gCameraModePathState;
extern CameraModePathDescriptor gCameraModePathDescriptor;

void CameraModePath_copyToCurrent(void);
void CameraModePath_free(void);
void CameraModePath_update(CameraObject* camera);
void CameraModePath_init(CameraObject* camera, int mode, CameraModePathSettings* settings);
void CameraModePath_release(void);
void CameraModePath_initialise(void);

#endif /* MAIN_DLL_DLL_0047_CAMERAMODEPATH_H_ */
