#ifndef DLLS_OBJECTS_245_SIDEKICKBAL_H_
#define DLLS_OBJECTS_245_SIDEKICKBAL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/dll/curves_collision_state.h"

typedef enum SidekickBallMode {
    SIDEKICK_BALL_IDLE = 0,
    SIDEKICK_BALL_MOVING = 1,
    SIDEKICK_BALL_HELD = 2,
    SIDEKICK_BALL_THROWN = 3,
    SIDEKICK_BALL_FADING = 5
} SidekickBallMode;

typedef struct SidekickBallState {
    CurvesCollisionState path;
    f32 primaryRadius;
    f32 fadeTimer;
    u8 pad08[4];
    u8 ballMode;
    u8 onPathPoint;
    u8 pad0E[0x3A];
    f32 previousPosX;
    f32 previousPosY;
    f32 previousPosZ;
    u8 pad54[4];
    f32 floorY;
    f32 floorDepth;
    u8 triggerArmed;
    u8 triggerHit;
    u8 sendHoldMessage;
    u8 pad63;
} SidekickBallState;

STATIC_ASSERT(offsetof(SidekickBallState, path) == 0x0);
STATIC_ASSERT(offsetof(SidekickBallState, primaryRadius) == sizeof(CurvesCollisionState));
STATIC_ASSERT(offsetof(SidekickBallState, ballMode) == sizeof(CurvesCollisionState) + 0xC);
STATIC_ASSERT(offsetof(SidekickBallState, previousPosX) == sizeof(CurvesCollisionState) + 0x48);
STATIC_ASSERT(offsetof(SidekickBallState, floorY) == sizeof(CurvesCollisionState) + 0x58);
STATIC_ASSERT(offsetof(SidekickBallState, triggerArmed) == sizeof(CurvesCollisionState) + 0x60);
STATIC_ASSERT(offsetof(SidekickBallState, pad63) + sizeof(u8) == sizeof(CurvesCollisionState) + 0x64);

int sidekickBall_isIdle(GameObject* obj);
void sidekickBall_handlePlayerInteraction(GameObject* obj, SidekickBallState* state);
void sidekickBall_keepAlive(GameObject* obj);
int sidekickBall_isHeldOrMoving(GameObject* obj);
void sidekickBall_setIdle(GameObject* obj, GameObject* source);
void sidekickBall_launch(GameObject* obj, GameObject* source, f32 velocityX, f32 velocityY, f32 velocityZ);
int SidekickBall_getExtraSize(void);
void SidekickBall_free(GameObject* obj);
void SidekickBall_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void SidekickBall_update(GameObject* obj);
u8 trickyBallMove(GameObject* obj);
void SidekickBall_init(GameObject* obj);

extern ObjectDescriptor gSidekickBallObjDescriptor;

#endif /* DLLS_OBJECTS_245_SIDEKICKBAL_H_ */
