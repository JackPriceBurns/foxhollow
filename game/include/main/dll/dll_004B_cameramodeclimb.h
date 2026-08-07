#ifndef MAIN_DLL_DLL_004B_CAMERAMODECLIMB_H_
#define MAIN_DLL_DLL_004B_CAMERAMODECLIMB_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef struct CameraModeClimbTransition {
    s8 unk00;
    s8 duration;
    s8 distance;
    s8 relativePosition;
    s8 maxHeight;
    s8 minHeight;
} CameraModeClimbTransition;

STATIC_ASSERT(offsetof(CameraModeClimbTransition, unk00) == 0x00);
STATIC_ASSERT(offsetof(CameraModeClimbTransition, duration) == 0x01);
STATIC_ASSERT(offsetof(CameraModeClimbTransition, distance) == 0x02);
STATIC_ASSERT(offsetof(CameraModeClimbTransition, relativePosition) == 0x03);
STATIC_ASSERT(offsetof(CameraModeClimbTransition, maxHeight) == 0x04);
STATIC_ASSERT(offsetof(CameraModeClimbTransition, minHeight) == 0x05);
STATIC_ASSERT(sizeof(CameraModeClimbTransition) == 0x06);

typedef struct CameraModeClimbState {
    f32 targetDistance;
    f32 smoothedDistance;
    f32 heightAdjustRate;
    f32 minHeight;
    f32 maxHeight;
    f32 startDistance;
    f32 endDistance;
    f32 startMinHeight;
    f32 endMinHeight;
    f32 startMaxHeight;
    f32 endMaxHeight;
    s16 transitionTimer;
    s16 transitionDuration;
    u16 relativePosition;
    u16 startRelativePosition;
    u16 targetRelativePosition;
    u8 unk36[0x38 - 0x36];
} CameraModeClimbState;

STATIC_ASSERT(offsetof(CameraModeClimbState, targetDistance) == 0x00);
STATIC_ASSERT(offsetof(CameraModeClimbState, smoothedDistance) == 0x04);
STATIC_ASSERT(offsetof(CameraModeClimbState, heightAdjustRate) == 0x08);
STATIC_ASSERT(offsetof(CameraModeClimbState, minHeight) == 0x0C);
STATIC_ASSERT(offsetof(CameraModeClimbState, maxHeight) == 0x10);
STATIC_ASSERT(offsetof(CameraModeClimbState, startDistance) == 0x14);
STATIC_ASSERT(offsetof(CameraModeClimbState, endDistance) == 0x18);
STATIC_ASSERT(offsetof(CameraModeClimbState, startMinHeight) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeClimbState, endMinHeight) == 0x20);
STATIC_ASSERT(offsetof(CameraModeClimbState, startMaxHeight) == 0x24);
STATIC_ASSERT(offsetof(CameraModeClimbState, endMaxHeight) == 0x28);
STATIC_ASSERT(offsetof(CameraModeClimbState, transitionTimer) == 0x2C);
STATIC_ASSERT(offsetof(CameraModeClimbState, transitionDuration) == 0x2E);
STATIC_ASSERT(offsetof(CameraModeClimbState, relativePosition) == 0x30);
STATIC_ASSERT(offsetof(CameraModeClimbState, startRelativePosition) == 0x32);
STATIC_ASSERT(offsetof(CameraModeClimbState, targetRelativePosition) == 0x34);
STATIC_ASSERT(offsetof(CameraModeClimbState, unk36) == 0x36);
STATIC_ASSERT(sizeof(CameraModeClimbState) == 0x38);

typedef struct CameraModeClimbDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, int mode, CameraModeClimbTransition* transition);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraModeClimbDescriptor;

STATIC_ASSERT(offsetof(CameraModeClimbDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeClimbDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeClimbDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeClimbDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeClimbDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeClimbDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeClimbDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeClimbDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeClimbDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeClimbDescriptor) == 0x30);

extern CameraModeClimbDescriptor gCameraModeClimbDescriptor;

void CameraModeClimb_copyToCurrent(void);
void CameraModeClimb_free(void);
void CameraModeClimb_update(CameraObject* camera);
void CameraModeClimb_init(CameraObject* camera, int mode, CameraModeClimbTransition* transition);
void CameraModeClimb_release(void);
void CameraModeClimb_initialise(void);

#endif /* MAIN_DLL_DLL_004B_CAMERAMODECLIMB_H_ */
