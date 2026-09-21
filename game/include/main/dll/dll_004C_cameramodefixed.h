#ifndef MAIN_DLL_DLL_004C_CAMERAMODEFIXED_H_
#define MAIN_DLL_DLL_004C_CAMERAMODEFIXED_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef struct CameraModeFixedPose {
    s16 rotX;
    s16 rotY;
    s16 rotZ;
    u8 unk06[0x0C - 0x06];
    Vec3f savedWorldPosition;
    Vec3f worldPosition;
    u8 unk24[0xB4 - 0x24];
    f32 fov;
    u8 unkB8[0x144 - 0xB8];
} CameraModeFixedPose;

STATIC_ASSERT(offsetof(CameraModeFixedPose, rotX) == 0x000);
STATIC_ASSERT(offsetof(CameraModeFixedPose, rotY) == 0x002);
STATIC_ASSERT(offsetof(CameraModeFixedPose, rotZ) == 0x004);
STATIC_ASSERT(offsetof(CameraModeFixedPose, savedWorldPosition) == 0x00C);
STATIC_ASSERT(offsetof(CameraModeFixedPose, worldPosition) == 0x018);
STATIC_ASSERT(offsetof(CameraModeFixedPose, fov) == 0x0B4);
STATIC_ASSERT(sizeof(CameraModeFixedPose) == 0x144);

typedef struct CameraModeFixedDescriptor {
    ResourceDescriptorHeader header;
    void* reserved18;
    void (*init)(CameraObject* camera, int unused, const CameraModeFixedPose* pose);
    void (*update)(void);
    void (*free)(void);
    void (*copyToCurrent)(void);
    void* reserved2C;
} CameraModeFixedDescriptor;

STATIC_ASSERT(offsetof(CameraModeFixedDescriptor, header.metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeFixedDescriptor, header.acquire) == 0x10);
STATIC_ASSERT(offsetof(CameraModeFixedDescriptor, header.release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeFixedDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeFixedDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeFixedDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeFixedDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeFixedDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeFixedDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeFixedDescriptor) == 0x30);

extern CameraModeFixedDescriptor gCameraModeFixedDescriptor;

void CameraModeFixed_copyToCurrent(void);
void CameraModeFixed_free(void);
void CameraModeFixed_update(void);
void CameraModeFixed_init(CameraObject* camera, int unused, const CameraModeFixedPose* pose);
void CameraModeFixed_release(void);
void CameraModeFixed_initialise(void);

#endif /* MAIN_DLL_DLL_004C_CAMERAMODEFIXED_H_ */
