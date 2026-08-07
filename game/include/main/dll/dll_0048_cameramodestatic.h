#ifndef MAIN_DLL_DLL_0048_CAMERAMODESTATIC_H_
#define MAIN_DLL_DLL_0048_CAMERAMODESTATIC_H_

#include "global.h"
#include "game/objects/object.h"
#include "main/camera_object.h"
#include "main/resource.h"

enum CameraModeStaticPlacementFlags {
    CAMERA_MODE_STATIC_TRACK_YAW = 1 << 0,
    CAMERA_MODE_STATIC_TRACK_PITCH = 1 << 1,
    CAMERA_MODE_STATIC_TRACK_ROLL = 1 << 2,
};

typedef struct CameraModeStaticState {
    GameObject* anchor;
    u8 unk04[0xF4 - 0x04];
    u8 active;
    u8 missingAnchor;
    u8 unkF6[0xF8 - 0xF6];
} CameraModeStaticState;

STATIC_ASSERT(offsetof(CameraModeStaticState, anchor) == 0x00);
STATIC_ASSERT(offsetof(CameraModeStaticState, active) == 0xF4);
STATIC_ASSERT(offsetof(CameraModeStaticState, missingAnchor) == 0xF5);
STATIC_ASSERT(sizeof(CameraModeStaticState) == 0xF8);

typedef struct CameraModeStaticDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(CameraObject* camera, int unused, const int* anchorId);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void);
    ResourceDescriptorCallback reserved2C;
} CameraModeStaticDescriptor;

STATIC_ASSERT(offsetof(CameraModeStaticDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeStaticDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeStaticDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeStaticDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeStaticDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeStaticDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeStaticDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeStaticDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeStaticDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeStaticDescriptor) == 0x30);

extern CameraModeStaticDescriptor gCameraModeStaticDescriptor;

GameObject* camStaticFindNearestAnchor(f32 x, f32 y, f32 z, int anchorId, int classId);
void CameraModeStatic_copyToCurrent(void);
void CameraModeStatic_free(void);
void CameraModeStatic_update(CameraObject* camera);
void CameraModeStatic_init(CameraObject* camera, int unused, const int* anchorId);
void CameraModeStatic_release(void);
void CameraModeStatic_initialise(void);

#endif /* MAIN_DLL_DLL_0048_CAMERAMODESTATIC_H_ */
