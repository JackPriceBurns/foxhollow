#ifndef DLLS_OBJECTS_495_SB_CLOUDBAL_H_
#define DLLS_OBJECTS_495_SB_CLOUDBAL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef struct ModelLightStruct ModelLightStruct;

/* SB_CloudBall_getExtraSize() allocates this complete 0x24-byte state. */
typedef struct SBCloudBallState {
    f32 velocityX;
    f32 velocityY;
    f32 velocityZ;
    f32 positionX;
    f32 positionY;
    f32 positionZ;
    ModelLightStruct* light;
    s8 launched;
    u8 padding1D[3];
    f32 fadeTimer;
} SBCloudBallState;

STATIC_ASSERT(offsetof(SBCloudBallState, velocityX) == 0x00);
STATIC_ASSERT(offsetof(SBCloudBallState, velocityY) == 0x04);
STATIC_ASSERT(offsetof(SBCloudBallState, velocityZ) == 0x08);
STATIC_ASSERT(offsetof(SBCloudBallState, positionX) == 0x0C);
STATIC_ASSERT(offsetof(SBCloudBallState, positionY) == 0x10);
STATIC_ASSERT(offsetof(SBCloudBallState, positionZ) == 0x14);
STATIC_ASSERT(offsetof(SBCloudBallState, light) == 0x18);
STATIC_ASSERT(offsetof(SBCloudBallState, launched) == 0x1C);
STATIC_ASSERT(offsetof(SBCloudBallState, padding1D) == 0x1D);
STATIC_ASSERT(offsetof(SBCloudBallState, fadeTimer) == 0x20);
STATIC_ASSERT(sizeof(SBCloudBallState) == 0x24);

int SB_CloudBall_getExtraSize(void);
int SB_CloudBall_getObjectTypeId(void);
void SB_CloudBall_free(GameObject* obj);
void SB_CloudBall_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SB_CloudBall_hitDetect(GameObject* obj);
void SB_CloudBall_update(GameObject* obj);
void SB_CloudBall_init(GameObject* obj);
void SB_CloudBall_release(void);
void SB_CloudBall_initialise(void);

extern ObjectDescriptor gSB_CloudBallObjDescriptor;

#endif /* DLLS_OBJECTS_495_SB_CLOUDBAL_H_ */
