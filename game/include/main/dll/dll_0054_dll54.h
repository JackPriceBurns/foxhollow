#ifndef MAIN_DLL_DLL_0054_DLL54_H_
#define MAIN_DLL_DLL_0054_DLL54_H_

#include "global.h"
#include "game/objects/object.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef enum CameraMode54ResourceId {
    CAMERA_MODE_54_RESOURCE_ID = 0x54,
} CameraMode54ResourceId;

typedef struct CameraMode54State {
    GameObject* originObj;
    GameObject* lookAtObj;
    GameObject* playerObj;
    u8 transitionDone;
    u8 exitRequested;
    u8 reserved0E[2];
    f32 transitionTimer;
    f32 startX;
    f32 startY;
    f32 startZ;
    s16 startYaw;
    s16 startPitch;
    s16 startRoll;
    u8 reserved26[2];
} CameraMode54State;

STATIC_ASSERT(offsetof(CameraMode54State, originObj) == 0x00);
STATIC_ASSERT(offsetof(CameraMode54State, lookAtObj) == 0x04);
STATIC_ASSERT(offsetof(CameraMode54State, playerObj) == 0x08);
STATIC_ASSERT(offsetof(CameraMode54State, transitionDone) == 0x0C);
STATIC_ASSERT(offsetof(CameraMode54State, exitRequested) == 0x0D);
STATIC_ASSERT(offsetof(CameraMode54State, reserved0E) == 0x0E);
STATIC_ASSERT(offsetof(CameraMode54State, transitionTimer) == 0x10);
STATIC_ASSERT(offsetof(CameraMode54State, startX) == 0x14);
STATIC_ASSERT(offsetof(CameraMode54State, startY) == 0x18);
STATIC_ASSERT(offsetof(CameraMode54State, startZ) == 0x1C);
STATIC_ASSERT(offsetof(CameraMode54State, startYaw) == 0x20);
STATIC_ASSERT(offsetof(CameraMode54State, startPitch) == 0x22);
STATIC_ASSERT(offsetof(CameraMode54State, startRoll) == 0x24);
STATIC_ASSERT(offsetof(CameraMode54State, reserved26) == 0x26);
STATIC_ASSERT(sizeof(CameraMode54State) == 0x28);

typedef struct CameraMode54Descriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, int unusedArg, CameraObject* source);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraMode54Descriptor;

STATIC_ASSERT(offsetof(CameraMode54Descriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraMode54Descriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraMode54Descriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraMode54Descriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraMode54Descriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraMode54Descriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraMode54Descriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraMode54Descriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraMode54Descriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraMode54Descriptor) == 0x30);

extern CameraMode54Descriptor gCameraMode54Descriptor;

void CameraMode54_copyToCurrent(void);
void CameraMode54_free(void);
void CameraMode54_update(CameraObject* camera);
void CameraMode54_init(CameraObject* camera, int unusedArg, CameraObject* source);
void CameraMode54_release(void);
void CameraMode54_initialise(void);

#endif /* MAIN_DLL_DLL_0054_DLL54_H_ */
