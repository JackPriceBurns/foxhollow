#ifndef MAIN_DLL_CAM_CAMTALK_H_
#define MAIN_DLL_CAM_CAMTALK_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef struct CameraModeTalkInputs {
    f32 turn;
    f32 height;
    f32 roll;
    f32 pitch;
} CameraModeTalkInputs;

STATIC_ASSERT(offsetof(CameraModeTalkInputs, turn) == 0x00);
STATIC_ASSERT(offsetof(CameraModeTalkInputs, height) == 0x04);
STATIC_ASSERT(offsetof(CameraModeTalkInputs, roll) == 0x08);
STATIC_ASSERT(offsetof(CameraModeTalkInputs, pitch) == 0x0C);
STATIC_ASSERT(sizeof(CameraModeTalkInputs) == 0x10);

typedef struct CameraModeTalkState {
    f32 defaultFov;
    u8 unk04[0x14 - 0x04];
    f32 defaultScale;
    f32 entryFov;
    f32 turnInput;
    f32 smoothedYawOffset;
    f32 heightInput;
    f32 followDistance;
    f32 rollInput;
    f32 pitchTarget;
    u8 unk34[0x38 - 0x34];
} CameraModeTalkState;

STATIC_ASSERT(offsetof(CameraModeTalkState, defaultFov) == 0x00);
STATIC_ASSERT(offsetof(CameraModeTalkState, defaultScale) == 0x14);
STATIC_ASSERT(offsetof(CameraModeTalkState, entryFov) == 0x18);
STATIC_ASSERT(offsetof(CameraModeTalkState, turnInput) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeTalkState, smoothedYawOffset) == 0x20);
STATIC_ASSERT(offsetof(CameraModeTalkState, heightInput) == 0x24);
STATIC_ASSERT(offsetof(CameraModeTalkState, followDistance) == 0x28);
STATIC_ASSERT(offsetof(CameraModeTalkState, rollInput) == 0x2C);
STATIC_ASSERT(offsetof(CameraModeTalkState, pitchTarget) == 0x30);
STATIC_ASSERT(sizeof(CameraModeTalkState) == 0x38);

typedef struct CameraModeTalkDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(const CameraModeTalkInputs* inputs);
    ResourceDescriptorCallback reserved2C;
} CameraModeTalkDescriptor;

STATIC_ASSERT(offsetof(CameraModeTalkDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeTalkDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeTalkDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeTalkDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeTalkDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeTalkDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeTalkDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeTalkDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeTalkDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeTalkDescriptor) == 0x30);

extern CameraModeTalkDescriptor gCameraModeTalkDescriptor;

void CameraModeTalk_copyToCurrent(const CameraModeTalkInputs* inputs);
void CameraModeTalk_free(void);
void CameraModeTalk_update(CameraObject* camera);
void CameraModeTalk_init(CameraObject* camera);
void CameraModeTalk_release(void);
void CameraModeTalk_initialise(void);

#endif /* MAIN_DLL_CAM_CAMTALK_H_ */
