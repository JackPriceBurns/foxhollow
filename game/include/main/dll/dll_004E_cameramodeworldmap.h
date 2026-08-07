#ifndef MAIN_DLL_DLL_004E_CAMERAMODEWORLDMAP_H_
#define MAIN_DLL_DLL_004E_CAMERAMODEWORLDMAP_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef enum CameraModeWorldMapResourceId {
    CAMERA_MODE_WORLD_MAP_RESOURCE_ID = 0x4E,
} CameraModeWorldMapResourceId;

typedef enum CameraModeWorldMapViewMode {
    CAMERA_MODE_WORLD_MAP_FREE_OVERVIEW = 0,
    CAMERA_MODE_WORLD_MAP_LOCKED_PATH = 1,
} CameraModeWorldMapViewMode;

typedef enum CameraModeWorldMapAction {
    CAMERA_MODE_WORLD_MAP_ACTION_SET_MODE = 0,
    CAMERA_MODE_WORLD_MAP_ACTION_SET_FOCUS = 1,
    CAMERA_MODE_WORLD_MAP_ACTION_SET_FOCUS_IMMEDIATE = 2,
} CameraModeWorldMapAction;

typedef struct CameraModeWorldMapFlags {
    u8 transitionActive : 1;
    u8 unknown : 7;
} CameraModeWorldMapFlags;

STATIC_ASSERT(sizeof(CameraModeWorldMapFlags) == 0x01);

typedef struct CameraModeWorldMapState {
    f32 distance;
    f32 distanceVelocity;
    u8 mode;
    u8 previousMode;
    s16 settleFrames;
    s16 targetAngle;
    u8 unk0E[0x10 - 0x0E];
    s32 focusObjectId;
    u8 focusBlendTimer;
    CameraModeWorldMapFlags flags;
    u8 unk16[0x18 - 0x16];
} CameraModeWorldMapState;

STATIC_ASSERT(offsetof(CameraModeWorldMapState, distance) == 0x00);
STATIC_ASSERT(offsetof(CameraModeWorldMapState, distanceVelocity) == 0x04);
STATIC_ASSERT(offsetof(CameraModeWorldMapState, mode) == 0x08);
STATIC_ASSERT(offsetof(CameraModeWorldMapState, previousMode) == 0x09);
STATIC_ASSERT(offsetof(CameraModeWorldMapState, settleFrames) == 0x0A);
STATIC_ASSERT(offsetof(CameraModeWorldMapState, targetAngle) == 0x0C);
STATIC_ASSERT(offsetof(CameraModeWorldMapState, unk0E) == 0x0E);
STATIC_ASSERT(offsetof(CameraModeWorldMapState, focusObjectId) == 0x10);
STATIC_ASSERT(offsetof(CameraModeWorldMapState, focusBlendTimer) == 0x14);
STATIC_ASSERT(offsetof(CameraModeWorldMapState, flags) == 0x15);
STATIC_ASSERT(offsetof(CameraModeWorldMapState, unk16) == 0x16);
STATIC_ASSERT(sizeof(CameraModeWorldMapState) == 0x18);

typedef struct CameraModeWorldMapDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void* value, CameraModeWorldMapAction action);
    ResourceDescriptorCallback reserved2C;
} CameraModeWorldMapDescriptor;

STATIC_ASSERT(offsetof(CameraModeWorldMapDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeWorldMapDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeWorldMapDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeWorldMapDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeWorldMapDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeWorldMapDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeWorldMapDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeWorldMapDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeWorldMapDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeWorldMapDescriptor) == 0x30);

extern CameraModeWorldMapDescriptor gCameraModeWorldMapDescriptor;

void CameraModeWorldMap_copyToCurrent(void* value, CameraModeWorldMapAction action);
void CameraModeWorldMap_free(void);
void CameraModeWorldMap_update(CameraObject* camera);
void CameraModeWorldMap_init(CameraObject* camera);
void CameraModeWorldMap_release(void);
void CameraModeWorldMap_initialise(void);

#endif /* MAIN_DLL_DLL_004E_CAMERAMODEWORLDMAP_H_ */
