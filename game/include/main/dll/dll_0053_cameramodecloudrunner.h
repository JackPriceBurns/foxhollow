#ifndef MAIN_DLL_DLL_0053_CAMERAMODECLOUDRUNNER_H_
#define MAIN_DLL_DLL_0053_CAMERAMODECLOUDRUNNER_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef enum CameraModeCloudRunnerResourceId {
    CAMERA_MODE_CLOUDRUNNER_RESOURCE_ID = 0x53,
} CameraModeCloudRunnerResourceId;

typedef struct CameraModeCloudRunnerInitParams {
    f32 focusX;
    f32 focusY;
    f32 focusZ;
    f32 radius;
} CameraModeCloudRunnerInitParams;

STATIC_ASSERT(offsetof(CameraModeCloudRunnerInitParams, focusX) == 0x00);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerInitParams, focusY) == 0x04);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerInitParams, focusZ) == 0x08);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerInitParams, radius) == 0x0C);
STATIC_ASSERT(sizeof(CameraModeCloudRunnerInitParams) == 0x10);

typedef struct CameraModeCloudRunnerState {
    f32 focusX;
    f32 focusY;
    f32 focusZ;
    f32 radius;
} CameraModeCloudRunnerState;

STATIC_ASSERT(offsetof(CameraModeCloudRunnerState, focusX) == 0x00);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerState, focusY) == 0x04);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerState, focusZ) == 0x08);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerState, radius) == 0x0C);
STATIC_ASSERT(sizeof(CameraModeCloudRunnerState) == 0x10);

typedef struct CameraModeCloudRunnerDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, int fallbackRadius, CameraModeCloudRunnerInitParams* params);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraModeCloudRunnerDescriptor;

STATIC_ASSERT(offsetof(CameraModeCloudRunnerDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeCloudRunnerDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeCloudRunnerDescriptor) == 0x30);

extern CameraModeCloudRunnerDescriptor gCameraModeCloudRunnerDescriptor;

void CameraModeCloudRunner_copyToCurrent(void);
void CameraModeCloudRunner_free(void);
void CameraModeCloudRunner_update(CameraObject* camera);
void CameraModeCloudRunner_init(CameraObject* camera, int fallbackRadius, CameraModeCloudRunnerInitParams* params);
void CameraModeCloudRunner_release(void);
void CameraModeCloudRunner_initialise(void);

#endif /* MAIN_DLL_DLL_0053_CAMERAMODECLOUDRUNNER_H_ */
