#ifndef MAIN_DLL_DLL_0046_CAMERAMODEDEBUG_H_
#define MAIN_DLL_DLL_0046_CAMERAMODEDEBUG_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef struct CameraModeDebugState {
    f32 orbitRadius;
    f32 radiusVelocity;
} CameraModeDebugState;

STATIC_ASSERT(offsetof(CameraModeDebugState, orbitRadius) == 0x00);
STATIC_ASSERT(offsetof(CameraModeDebugState, radiusVelocity) == 0x04);
STATIC_ASSERT(sizeof(CameraModeDebugState) == 0x08);

typedef struct CameraModeDebugDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(void);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraModeDebugDescriptor;

STATIC_ASSERT(offsetof(CameraModeDebugDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeDebugDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeDebugDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeDebugDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeDebugDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeDebugDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeDebugDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeDebugDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeDebugDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeDebugDescriptor) == 0x30);

extern CameraModeDebugDescriptor gCameraModeDebugDescriptor;

void CameraModeDebug_copyToCurrent_nop(void);
void CameraModeDebug_free(void);
void CameraModeDebug_update(CameraObject* camera);
void CameraModeDebug_init(void);
void CameraModeDebug_release_nop(void);
void CameraModeDebug_initialise_nop(void);

#endif /* MAIN_DLL_DLL_0046_CAMERAMODEDEBUG_H_ */
