#ifndef MAIN_DLL_DLL_0051_CAMERAMODECANNON_H_
#define MAIN_DLL_DLL_0051_CAMERAMODECANNON_H_

#include "game/objects/object.h"
#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef enum CameraModeCannonResourceId {
    CAMERA_MODE_CANNON_RESOURCE_ID = 0x51,
} CameraModeCannonResourceId;

typedef struct CameraModeCannonInitParams {
    GameObject* target;
} CameraModeCannonInitParams;

STATIC_ASSERT(offsetof(CameraModeCannonInitParams, target) == 0x00);
STATIC_ASSERT(sizeof(CameraModeCannonInitParams) == 0x04);

typedef struct CameraModeCannonState {
    GameObject* target;
} CameraModeCannonState;

STATIC_ASSERT(offsetof(CameraModeCannonState, target) == 0x00);
STATIC_ASSERT(sizeof(CameraModeCannonState) == 0x04);

typedef struct CameraModeCannonDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, int unused, CameraModeCannonInitParams* params);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraModeCannonDescriptor;

STATIC_ASSERT(offsetof(CameraModeCannonDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeCannonDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeCannonDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeCannonDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeCannonDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeCannonDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeCannonDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeCannonDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeCannonDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeCannonDescriptor) == 0x30);

extern CameraModeCannonDescriptor gCameraModeCannonDescriptor;

void CameraModeCannon_copyToCurrent(void);
void CameraModeCannon_free(void);
void CameraModeCannon_update(CameraObject* camera);
void CameraModeCannon_init(CameraObject* camera, int unused, CameraModeCannonInitParams* params);
void CameraModeCannon_release(void);
void CameraModeCannon_initialise(void);

#endif /* MAIN_DLL_DLL_0051_CAMERAMODECANNON_H_ */
