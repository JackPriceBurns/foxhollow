#ifndef MAIN_DLL_DLL_004F_CAMERAMODE_H_
#define MAIN_DLL_DLL_004F_CAMERAMODE_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef struct CameraMode4FState {
    u8 unk00[0x04];
    f32 blendProgress;
} CameraMode4FState;

STATIC_ASSERT(offsetof(CameraMode4FState, unk00) == 0x00);
STATIC_ASSERT(offsetof(CameraMode4FState, blendProgress) == 0x04);
STATIC_ASSERT(sizeof(CameraMode4FState) == 0x08);

typedef struct CameraMode4FDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(void);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraMode4FDescriptor;

STATIC_ASSERT(offsetof(CameraMode4FDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraMode4FDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraMode4FDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraMode4FDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraMode4FDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraMode4FDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraMode4FDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraMode4FDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraMode4FDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraMode4FDescriptor) == 0x30);

extern CameraMode4FDescriptor gCameraMode4FDescriptor;

void CameraMode4F_copyToCurrent(void);
void CameraMode4F_free(void);
void CameraMode4F_update(CameraObject* camera);
void CameraMode4F_init(void);
void CameraMode4F_release(void);
void CameraMode4F_initialise(void);

#endif /* MAIN_DLL_DLL_004F_CAMERAMODE_H_ */
