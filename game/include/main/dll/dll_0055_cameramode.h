#ifndef MAIN_DLL_DLL_0055_CAMERAMODE_H_
#define MAIN_DLL_DLL_0055_CAMERAMODE_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef enum CameraMode55ResourceId {
    CAMERA_MODE_55_RESOURCE_ID = 0x55,
} CameraMode55ResourceId;

typedef struct CameraMode55State {
    f32 timer;
    f32 cameraY;
} CameraMode55State;

STATIC_ASSERT(offsetof(CameraMode55State, timer) == 0x00);
STATIC_ASSERT(offsetof(CameraMode55State, cameraY) == 0x04);
STATIC_ASSERT(sizeof(CameraMode55State) == 0x08);

typedef struct CameraMode55Descriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraMode55Descriptor;

STATIC_ASSERT(offsetof(CameraMode55Descriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraMode55Descriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraMode55Descriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraMode55Descriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraMode55Descriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraMode55Descriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraMode55Descriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraMode55Descriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraMode55Descriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraMode55Descriptor) == 0x30);

extern CameraMode55Descriptor gCameraMode55Descriptor;

void CameraMode55_copyToCurrent(void);
void CameraMode55_free(void);
void CameraMode55_update(CameraObject* camera);
void CameraMode55_init(CameraObject* camera);
void CameraMode55_release(void);
void CameraMode55_initialise(void);

#endif /* MAIN_DLL_DLL_0055_CAMERAMODE_H_ */
