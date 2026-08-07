#ifndef MAIN_DLL_DLL_025A_STATICCAMERA_H_
#define MAIN_DLL_DLL_025A_STATICCAMERA_H_

#include "global.h"
#include "dlls/object_descriptor.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"

enum StaticCameraObjectContract {
    STATIC_CAMERA_OBJECT_GROUP = 7,
    STATIC_CAMERA_CLASS_ID = 18,
};

typedef struct StaticCameraState {
    u8 setupParam; /* 0x00: from placement byte 0x19 */
    u8 unk1;       /* 0x01: cleared at init */
    u8 pad2[2];
    f32 fov;
} StaticCameraState;

STATIC_ASSERT(offsetof(StaticCameraState, setupParam) == 0x00);
STATIC_ASSERT(offsetof(StaticCameraState, unk1) == 0x01);
STATIC_ASSERT(offsetof(StaticCameraState, fov) == 0x04);
STATIC_ASSERT(sizeof(StaticCameraState) == 0x08);

typedef struct StaticCameraPlacement {
    ObjPlacement base;
    u8 anchorId;
    u8 setupParam; /* 0x19 */
    u8 fov;
    u8 modeFlags;
    union {
        struct {
            s16 rotX;
            s16 rotY;
            s16 rotZ;
        } objectRotation;
        struct {
            s16 yaw;
            s16 pitch;
            s16 roll;
        } cameraModeRotation;
    };
} StaticCameraPlacement;

STATIC_ASSERT(offsetof(StaticCameraPlacement, anchorId) == 0x18);
STATIC_ASSERT(offsetof(StaticCameraPlacement, setupParam) == 0x19);
STATIC_ASSERT(offsetof(StaticCameraPlacement, fov) == 0x1A);
STATIC_ASSERT(offsetof(StaticCameraPlacement, modeFlags) == 0x1B);
STATIC_ASSERT(offsetof(StaticCameraPlacement, objectRotation.rotX) == 0x1C);
STATIC_ASSERT(offsetof(StaticCameraPlacement, objectRotation.rotY) == 0x1E);
STATIC_ASSERT(offsetof(StaticCameraPlacement, objectRotation.rotZ) == 0x20);
STATIC_ASSERT(offsetof(StaticCameraPlacement, cameraModeRotation.yaw) == 0x1C);
STATIC_ASSERT(offsetof(StaticCameraPlacement, cameraModeRotation.pitch) == 0x1E);
STATIC_ASSERT(offsetof(StaticCameraPlacement, cameraModeRotation.roll) == 0x20);

extern ObjectDescriptor gStaticCameraObjDescriptor;

int StaticCamera_getExtraSize(void);
int StaticCamera_getObjectTypeId(void);
void StaticCamera_free(GameObject* obj);
void StaticCamera_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void StaticCamera_hitDetect(void);
void StaticCamera_update(void);
void StaticCamera_init(GameObject* obj, StaticCameraPlacement* params, int deferAdd);
void StaticCamera_release(void);
void StaticCamera_initialise(void);

#endif /* MAIN_DLL_DLL_025A_STATICCAMERA_H_ */
