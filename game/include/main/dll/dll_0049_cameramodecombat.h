#ifndef MAIN_DLL_DLL_0049_CAMERAMODECOMBAT_H_
#define MAIN_DLL_DLL_0049_CAMERAMODECOMBAT_H_

#include "global.h"
#include "game/objects/object.h"
#include "main/camera_object.h"
#include "main/resource.h"

enum CameraModeCombatResourceId {
    CAMERA_MODE_COMBAT_RESOURCE_ID = 0x49,
};

typedef struct CameraModeCombatState {
    f32 followDistance;
    f32 heightOffset;
    f32 zoomOffset;
    u8 unk0C[4];
    u8 unk10;
    u8 unk11;
    u8 invalidTarget;
    u8 hitVolumeBlendStartIndex;
    u8 hitVolumeBlendTargetIndex;
    u8 pad15[3];
    f32 hitVolumeBlendWeight;
} CameraModeCombatState;

STATIC_ASSERT(offsetof(CameraModeCombatState, followDistance) == 0x00);
STATIC_ASSERT(offsetof(CameraModeCombatState, heightOffset) == 0x04);
STATIC_ASSERT(offsetof(CameraModeCombatState, zoomOffset) == 0x08);
STATIC_ASSERT(offsetof(CameraModeCombatState, unk0C) == 0x0C);
STATIC_ASSERT(offsetof(CameraModeCombatState, unk10) == 0x10);
STATIC_ASSERT(offsetof(CameraModeCombatState, unk11) == 0x11);
STATIC_ASSERT(offsetof(CameraModeCombatState, invalidTarget) == 0x12);
STATIC_ASSERT(offsetof(CameraModeCombatState, hitVolumeBlendStartIndex) == 0x13);
STATIC_ASSERT(offsetof(CameraModeCombatState, hitVolumeBlendTargetIndex) == 0x14);
STATIC_ASSERT(offsetof(CameraModeCombatState, pad15) == 0x15);
STATIC_ASSERT(offsetof(CameraModeCombatState, hitVolumeBlendWeight) == 0x18);
STATIC_ASSERT(sizeof(CameraModeCombatState) == 0x1C);

typedef struct CameraModeCombatDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, u32 unused, GameObject** targetPtr);
    void (*update)(CameraObject* camera);
    void (*free)(CameraObject* camera);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraModeCombatDescriptor;

STATIC_ASSERT(offsetof(CameraModeCombatDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeCombatDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeCombatDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeCombatDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeCombatDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeCombatDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeCombatDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeCombatDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeCombatDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeCombatDescriptor) == 0x30);

extern CameraModeCombatState* gCameraModeCombatState;
extern CameraModeCombatDescriptor gCameraModeCombatDescriptor;

void CameraModeCombat_copyToCurrent(void);
void CameraModeCombat_evaluateTargetPosition(CameraObject* camera, f32* outX, f32* outY, f32* outZ, f32* targetY);
void CameraModeCombat_free(CameraObject* camera);
void CameraModeCombat_update(CameraObject* camera);
void CameraModeCombat_init(CameraObject* camera, u32 unused, GameObject** targetPtr);
void CameraModeCombat_release(void);
void CameraModeCombat_initialise(void);

#endif /* MAIN_DLL_DLL_0049_CAMERAMODECOMBAT_H_ */
