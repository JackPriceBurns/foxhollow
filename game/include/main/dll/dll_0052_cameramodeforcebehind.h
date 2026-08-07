#ifndef MAIN_DLL_DLL_0052_CAMERAMODEFORCEBEHIND_H_
#define MAIN_DLL_DLL_0052_CAMERAMODEFORCEBEHIND_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef enum CameraModeForceBehindResourceId {
    CAMERA_MODE_FORCE_BEHIND_RESOURCE_ID = 0x52,
} CameraModeForceBehindResourceId;

typedef struct CameraModeForceBehindInitParams {
    f32 orbitRadius;
    f32 heightOffset;
} CameraModeForceBehindInitParams;

STATIC_ASSERT(offsetof(CameraModeForceBehindInitParams, orbitRadius) == 0x00);
STATIC_ASSERT(offsetof(CameraModeForceBehindInitParams, heightOffset) == 0x04);
STATIC_ASSERT(sizeof(CameraModeForceBehindInitParams) == 0x08);

typedef struct CameraModeForceBehindDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, int unused, CameraModeForceBehindInitParams* params);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraModeForceBehindDescriptor;

STATIC_ASSERT(offsetof(CameraModeForceBehindDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeForceBehindDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeForceBehindDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeForceBehindDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeForceBehindDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeForceBehindDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeForceBehindDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeForceBehindDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeForceBehindDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeForceBehindDescriptor) == 0x30);

extern CameraModeForceBehindDescriptor gCameraModeForceBehindDescriptor;

void CameraModeForceBehind_copyToCurrent(void);
void CameraModeForceBehind_free(void);
void CameraModeForceBehind_update(CameraObject* camera);
void CameraModeForceBehind_init(CameraObject* camera, int unused, CameraModeForceBehindInitParams* params);
void CameraModeForceBehind_release(void);
void CameraModeForceBehind_initialise(void);

#endif /* MAIN_DLL_DLL_0052_CAMERAMODEFORCEBEHIND_H_ */
