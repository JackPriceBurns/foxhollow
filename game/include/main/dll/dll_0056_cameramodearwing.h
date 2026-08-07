#ifndef MAIN_DLL_DLL_0056_CAMERAMODEARWING_H_
#define MAIN_DLL_DLL_0056_CAMERAMODEARWING_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef enum CameraModeArwingResourceId {
    CAMERA_MODE_ARWING_RESOURCE_ID = 0x56,
} CameraModeArwingResourceId;

typedef struct CameraModeArwingPositionInput {
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
} CameraModeArwingPositionInput;

STATIC_ASSERT(offsetof(CameraModeArwingPositionInput, offsetX) == 0x00);
STATIC_ASSERT(offsetof(CameraModeArwingPositionInput, offsetY) == 0x04);
STATIC_ASSERT(offsetof(CameraModeArwingPositionInput, offsetZ) == 0x08);
STATIC_ASSERT(sizeof(CameraModeArwingPositionInput) == 0x0C);

typedef struct CameraModeArwingRotationInput {
    s16 yaw;
    s16 pitch;
    s16 roll;
} CameraModeArwingRotationInput;

STATIC_ASSERT(offsetof(CameraModeArwingRotationInput, yaw) == 0x00);
STATIC_ASSERT(offsetof(CameraModeArwingRotationInput, pitch) == 0x02);
STATIC_ASSERT(offsetof(CameraModeArwingRotationInput, roll) == 0x04);
STATIC_ASSERT(sizeof(CameraModeArwingRotationInput) == 0x06);

typedef struct CameraModeArwingZOffsetInput {
    f32 offset;
} CameraModeArwingZOffsetInput;

STATIC_ASSERT(offsetof(CameraModeArwingZOffsetInput, offset) == 0x00);
STATIC_ASSERT(sizeof(CameraModeArwingZOffsetInput) == 0x04);

typedef struct CameraModeArwingZEaseInput {
    f32 denominator;
    f32 numerator;
} CameraModeArwingZEaseInput;

STATIC_ASSERT(offsetof(CameraModeArwingZEaseInput, denominator) == 0x00);
STATIC_ASSERT(offsetof(CameraModeArwingZEaseInput, numerator) == 0x04);
STATIC_ASSERT(sizeof(CameraModeArwingZEaseInput) == 0x08);

typedef struct CameraModeArwingState {
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    f32 basePosX;
    f32 basePosY;
    f32 basePosZ;
    u8 reserved18[0x0C];
    f32 xScale;
    f32 yScale;
    f32 unk2C;
    f32 initialOffsetX;
    f32 initialOffsetY;
    f32 posZOffset;
    f32 zEaseDenominator;
    f32 zEaseNumerator;
    f32 yawScale;
    f32 pitchScale;
    f32 rollScale;
    f32 rollRate;
    s16 inputYaw;
    s16 inputPitch;
    s16 inputRoll;
    u8 zScaleNear;
    u8 zScaleFar;
    u8 reserved5C[2];
    u8 active;
    u8 reserved5F;
} CameraModeArwingState;

STATIC_ASSERT(offsetof(CameraModeArwingState, offsetX) == 0x00);
STATIC_ASSERT(offsetof(CameraModeArwingState, offsetY) == 0x04);
STATIC_ASSERT(offsetof(CameraModeArwingState, offsetZ) == 0x08);
STATIC_ASSERT(offsetof(CameraModeArwingState, basePosX) == 0x0C);
STATIC_ASSERT(offsetof(CameraModeArwingState, basePosY) == 0x10);
STATIC_ASSERT(offsetof(CameraModeArwingState, basePosZ) == 0x14);
STATIC_ASSERT(offsetof(CameraModeArwingState, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeArwingState, xScale) == 0x24);
STATIC_ASSERT(offsetof(CameraModeArwingState, yScale) == 0x28);
STATIC_ASSERT(offsetof(CameraModeArwingState, unk2C) == 0x2C);
STATIC_ASSERT(offsetof(CameraModeArwingState, initialOffsetX) == 0x30);
STATIC_ASSERT(offsetof(CameraModeArwingState, initialOffsetY) == 0x34);
STATIC_ASSERT(offsetof(CameraModeArwingState, posZOffset) == 0x38);
STATIC_ASSERT(offsetof(CameraModeArwingState, zEaseDenominator) == 0x3C);
STATIC_ASSERT(offsetof(CameraModeArwingState, zEaseNumerator) == 0x40);
STATIC_ASSERT(offsetof(CameraModeArwingState, yawScale) == 0x44);
STATIC_ASSERT(offsetof(CameraModeArwingState, pitchScale) == 0x48);
STATIC_ASSERT(offsetof(CameraModeArwingState, rollScale) == 0x4C);
STATIC_ASSERT(offsetof(CameraModeArwingState, rollRate) == 0x50);
STATIC_ASSERT(offsetof(CameraModeArwingState, inputYaw) == 0x54);
STATIC_ASSERT(offsetof(CameraModeArwingState, inputPitch) == 0x56);
STATIC_ASSERT(offsetof(CameraModeArwingState, inputRoll) == 0x58);
STATIC_ASSERT(offsetof(CameraModeArwingState, zScaleNear) == 0x5A);
STATIC_ASSERT(offsetof(CameraModeArwingState, zScaleFar) == 0x5B);
STATIC_ASSERT(offsetof(CameraModeArwingState, reserved5C) == 0x5C);
STATIC_ASSERT(offsetof(CameraModeArwingState, active) == 0x5E);
STATIC_ASSERT(offsetof(CameraModeArwingState, reserved5F) == 0x5F);
STATIC_ASSERT(sizeof(CameraModeArwingState) == 0x60);

typedef struct CameraModeArwingDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, int mode, int unusedArg);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void* actionData, u32 recordSize);
    ResourceDescriptorCallback reserved2C;
} CameraModeArwingDescriptor;

STATIC_ASSERT(offsetof(CameraModeArwingDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeArwingDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeArwingDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeArwingDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeArwingDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeArwingDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeArwingDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeArwingDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeArwingDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeArwingDescriptor) == 0x30);

extern CameraModeArwingDescriptor gCameraModeArwingDescriptor;

void CameraModeArwing_copyToCurrent(void* actionData, u32 recordSize);
void CameraModeArwing_free(void);
void CameraModeArwing_update(CameraObject* camera);
void CameraModeArwing_init(CameraObject* camera, int mode, int unusedArg);
void CameraModeArwing_release(void);
void CameraModeArwing_initialise(void);

#endif /* MAIN_DLL_DLL_0056_CAMERAMODEARWING_H_ */
