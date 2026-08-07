#ifndef MAIN_DLL_DLL_004A_CAMERAMODESHIPBATTLE_H_
#define MAIN_DLL_DLL_004A_CAMERAMODESHIPBATTLE_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef struct CameraModeShipBattleState {
    f32 smoothedZOffset;
    f32 smoothedYOffset;
    f32 lateralOffset;
    f32 targetLateralOffset;
    f32 startLateralOffset;
    f32 blendProgress;
    f32 lateralDelta;
    f32 verticalOffset;
    f32 startVerticalOffset;
    f32 verticalDelta;
    u8 targetMode;
    u8 unk29[0x2C - 0x29];
} CameraModeShipBattleState;

STATIC_ASSERT(offsetof(CameraModeShipBattleState, smoothedZOffset) == 0x00);
STATIC_ASSERT(offsetof(CameraModeShipBattleState, smoothedYOffset) == 0x04);
STATIC_ASSERT(offsetof(CameraModeShipBattleState, lateralOffset) == 0x08);
STATIC_ASSERT(offsetof(CameraModeShipBattleState, targetLateralOffset) == 0x0C);
STATIC_ASSERT(offsetof(CameraModeShipBattleState, startLateralOffset) == 0x10);
STATIC_ASSERT(offsetof(CameraModeShipBattleState, blendProgress) == 0x14);
STATIC_ASSERT(offsetof(CameraModeShipBattleState, lateralDelta) == 0x18);
STATIC_ASSERT(offsetof(CameraModeShipBattleState, verticalOffset) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeShipBattleState, startVerticalOffset) == 0x20);
STATIC_ASSERT(offsetof(CameraModeShipBattleState, verticalDelta) == 0x24);
STATIC_ASSERT(offsetof(CameraModeShipBattleState, targetMode) == 0x28);
STATIC_ASSERT(sizeof(CameraModeShipBattleState) == 0x2C);

typedef struct CameraModeShipBattleDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(void);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraModeShipBattleDescriptor;

STATIC_ASSERT(offsetof(CameraModeShipBattleDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeShipBattleDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeShipBattleDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeShipBattleDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeShipBattleDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeShipBattleDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeShipBattleDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeShipBattleDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeShipBattleDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeShipBattleDescriptor) == 0x30);

extern CameraModeShipBattleDescriptor gCameraModeShipBattleDescriptor;

void CameraModeShipBattle_copyToCurrent(void);
void CameraModeShipBattle_free(void);
void CameraModeShipBattle_update(CameraObject* camera);
void CameraModeShipBattle_init(void);
void CameraModeShipBattle_release(void);
void CameraModeShipBattle_initialise(void);

#endif /* MAIN_DLL_DLL_004A_CAMERAMODESHIPBATTLE_H_ */
