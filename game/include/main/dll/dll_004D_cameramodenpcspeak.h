#ifndef MAIN_DLL_DLL_004D_CAMERAMODENPCSPEAK_H_
#define MAIN_DLL_DLL_004D_CAMERAMODENPCSPEAK_H_

#include "game/objects/object.h"
#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef struct CameraModeNpcSpeakInitParams {
    f32 anchorX;
    f32 anchorY;
    f32 anchorZ;
    u8 mode;
    u8 unk0D[0x10 - 0x0D];
} CameraModeNpcSpeakInitParams;

STATIC_ASSERT(offsetof(CameraModeNpcSpeakInitParams, anchorX) == 0x00);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakInitParams, anchorY) == 0x04);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakInitParams, anchorZ) == 0x08);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakInitParams, mode) == 0x0C);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakInitParams, unk0D) == 0x0D);
STATIC_ASSERT(sizeof(CameraModeNpcSpeakInitParams) == 0x10);

typedef struct CameraModeNpcSpeakState {
    f32 anchorX;
    f32 anchorY;
    f32 anchorZ;
    u8 unk0C[0x10 - 0x0C];
    f32 distanceOffset;
    f32 unk14;
    s32 orbitAngleOffset;
    s32 mode;
    s16 unk20;
    s16 orbitAngleVelocity;
    f32 cameraX;
    f32 cameraY;
    f32 cameraZ;
    f32 targetHeightOffset;
    u8 unk34[0x38 - 0x34];
    f32 lookAtHeightOffset;
    f32 lookAtYScale;
    f32 minDistance;
    f32 anchorLerpScale;
    f32 lookAtXZScale;
} CameraModeNpcSpeakState;

STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, anchorX) == 0x00);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, anchorY) == 0x04);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, anchorZ) == 0x08);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, unk0C) == 0x0C);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, distanceOffset) == 0x10);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, unk14) == 0x14);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, orbitAngleOffset) == 0x18);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, mode) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, unk20) == 0x20);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, orbitAngleVelocity) == 0x22);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, cameraX) == 0x24);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, cameraY) == 0x28);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, cameraZ) == 0x2C);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, targetHeightOffset) == 0x30);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, unk34) == 0x34);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, lookAtHeightOffset) == 0x38);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, lookAtYScale) == 0x3C);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, minDistance) == 0x40);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, anchorLerpScale) == 0x44);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakState, lookAtXZScale) == 0x48);
STATIC_ASSERT(sizeof(CameraModeNpcSpeakState) == 0x4C);

typedef struct CameraModeNpcSpeakDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, int unused, CameraModeNpcSpeakInitParams* params);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
} CameraModeNpcSpeakDescriptor;

STATIC_ASSERT(offsetof(CameraModeNpcSpeakDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeNpcSpeakDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(sizeof(CameraModeNpcSpeakDescriptor) == 0x2C);

extern CameraModeNpcSpeakDescriptor gCameraModeNpcSpeakDescriptor;

void CameraModeNpcSpeak_copyToCurrent(void);
void CameraModeNpcSpeak_free(void);
void CameraModeNpcSpeak_release(void);
void CameraModeNpcSpeak_initialise(void);
void CameraModeNpcSpeak_init(CameraObject* camera, int unused, CameraModeNpcSpeakInitParams* params);
void CameraModeNpcSpeak_update(CameraObject* camera);
void CameraModeNpcSpeak_solveOrbitPosition(GameObject* target, f32* outX, f32* outY, f32* outZ);

#endif /* MAIN_DLL_DLL_004D_CAMERAMODENPCSPEAK_H_ */
