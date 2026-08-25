#ifndef MAIN_DLL_FIREFLY_FLIGHT_STATE_H
#define MAIN_DLL_FIREFLY_FLIGHT_STATE_H

#include "global.h"
#include "game/objects/object.h"

typedef struct FireFlyActiveBits
{
    u8 active : 1;
} FireFlyActiveBits;

typedef struct FireFlyFlightState
{
    f32 splineX[4];
    f32 splineY[4];
    f32 splineZ[4];
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    f32 splineT;
    f32 splineSpeed;
    f32 proximityAlpha;
    f32 playerRadius;
    f32 radius;
    f32 posX;
    f32 posY;
    f32 posZ;
    s16 angle;
    s16 angleStep;
    s16 ampMax;
    u8 kind;
    u8 unk67;
    u8 pathAge;
    u8 pad65[2];
    u8 firstFrame;
    FireFlyActiveBits activeFlags;
    u8 pad69[3];
    f32 despawnTimer;
    f32 lifeTimer;
    f32 unk74;
} FireFlyFlightState;

STATIC_ASSERT(sizeof(FireFlyFlightState) == 0x78);
STATIC_ASSERT(offsetof(FireFlyFlightState, splineX) == 0x00);
STATIC_ASSERT(offsetof(FireFlyFlightState, targetX) == 0x30);
STATIC_ASSERT(offsetof(FireFlyFlightState, splineT) == 0x3C);
STATIC_ASSERT(offsetof(FireFlyFlightState, pathAge) == 0x64);
STATIC_ASSERT(offsetof(FireFlyFlightState, activeFlags) == 0x68);
STATIC_ASSERT(offsetof(FireFlyFlightState, despawnTimer) == 0x6C);
STATIC_ASSERT(offsetof(FireFlyFlightState, unk74) == 0x74);

void firefly_initFlightRec(GameObject* obj, FireFlyFlightState* state);
void firefly_pickWanderTarget(GameObject* obj, FireFlyFlightState* state);
void firefly_shiftPathHistory(GameObject* obj, FireFlyFlightState* state);

#endif
