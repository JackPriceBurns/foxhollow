#ifndef DLLS_OBJECTS_494_SB_CANNONBA_H_
#define DLLS_OBJECTS_494_SB_CANNONBA_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

#define SB_CANNONBALL_ALIAS_OBJECT_TYPE 0x0113

typedef struct ModelLightStruct ModelLightStruct;

/* SB_CannonBall_getExtraSize() allocates this complete 0x24-byte state. */
typedef struct SBCannonBallState {
    f32 velocityX;
    f32 velocityY;
    f32 velocityZ;
    f32 posX;
    f32 posY;
    f32 posZ;
    s16 lifetimeFrames;
    s8 flags;
    u8 unknown1B;
    f32 impactCooldown;
    ModelLightStruct* modelLight;
} SBCannonBallState;

STATIC_ASSERT(offsetof(SBCannonBallState, velocityX) == 0x00);
STATIC_ASSERT(offsetof(SBCannonBallState, velocityY) == 0x04);
STATIC_ASSERT(offsetof(SBCannonBallState, velocityZ) == 0x08);
STATIC_ASSERT(offsetof(SBCannonBallState, posX) == 0x0C);
STATIC_ASSERT(offsetof(SBCannonBallState, posY) == 0x10);
STATIC_ASSERT(offsetof(SBCannonBallState, posZ) == 0x14);
STATIC_ASSERT(offsetof(SBCannonBallState, lifetimeFrames) == 0x18);
STATIC_ASSERT(offsetof(SBCannonBallState, flags) == 0x1A);
STATIC_ASSERT(offsetof(SBCannonBallState, unknown1B) == 0x1B);
STATIC_ASSERT(offsetof(SBCannonBallState, impactCooldown) == 0x1C);
STATIC_ASSERT(offsetof(SBCannonBallState, modelLight) == 0x20);
STATIC_ASSERT(sizeof(SBCannonBallState) == 0x24);

int SB_CannonBall_getExtraSize(void);
int SB_CannonBall_getObjectTypeId(void);
void SB_CannonBall_free(GameObject* obj);
void SB_CannonBall_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SB_CannonBall_hitDetect(GameObject* obj);
void SB_CannonBall_update(GameObject* obj);
void SB_CannonBall_init(GameObject* obj);
void SB_CannonBall_release(void);
void SB_CannonBall_initialise(void);

extern ObjectDescriptor gSB_CannonBallObjDescriptor;

#endif /* DLLS_OBJECTS_494_SB_CANNONBA_H_ */
