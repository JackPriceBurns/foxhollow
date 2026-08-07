#ifndef DLLS_OBJECTS_454_DIMCANNON_H_
#define DLLS_OBJECTS_454_DIMCANNON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct ObjSeqState ObjSeqState;

#define DIM_CANNON_BALL_SEQUENCE_ID 0x1D6

typedef enum DimCannonMode {
    DIM_CANNON_MODE_WAIT_FOR_ARM = 0,
    DIM_CANNON_MODE_AUTO_FIRE = 1,
    DIM_CANNON_MODE_PLAYER_CONTROLLED = 3,
    DIM_CANNON_MODE_ARMED = 4,
    DIM_CANNON_MODE_WAIT_FOR_RESET = 5,
} DimCannonMode;

typedef enum DimCannonBallMode {
    DIM_CANNON_BALL_MODE_FALLING = 0,
    DIM_CANNON_BALL_MODE_EXPLODED = 1,
} DimCannonBallMode;

typedef struct DimCannonPlacement {
    ObjPlacement base;
    u8 unknown18[2];
    s16 resetGameBit;
    s16 armGameBit;
    s16 holdGameBit;
    u8 unknown20[6];
    s16 triggerRange;
    s8 rotationXByte;
    u8 launchDelayMin;
    u8 launchDelayMax;
    u8 targetRadius;
} DimCannonPlacement;

typedef struct DimCannonBallPlacement {
    ObjPlacement base;
    u8 unknown18[0xC];
} DimCannonBallPlacement;

typedef struct DimCannonState {
    GameObject* targetPlayer;
    f32 aimTargetX;
    f32 aimTargetY;
    f32 aimTargetZ;
    f32 targetDistance;
    f32 aimHistoryX[10];
    f32 aimHistoryY[10];
    f32 aimHistoryZ[10];
    f32 launchOriginX;
    f32 launchOriginY;
    f32 launchOriginZ;
    f32 launchSpeed;
    u8 unknown9C[8];
    s16 launchDelay;
    s16 shotCooldown;
    s32 previousAimDelta;
    u8 mode;
    u8 shouldSpawnProjectile;
    u8 airMeterCharge;
    u8 aimRefreshTimer;
    s8 chargeTimer;
    u8 shutdownTimer;
    u8 hasActivated;
    u8 unknownB3;
} DimCannonState;

typedef struct DimCannonBallState {
    GameObject* parent;
    u8 variant;
    u8 hitboxRadius;
    u8 hitType;
    u8 clearLatch;
    u8 mode;
    s8 rotationZRate;
    s8 rotationYRate;
    s8 rotationXRate;
} DimCannonBallState;

STATIC_ASSERT(offsetof(DimCannonPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimCannonPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(DimCannonPlacement, resetGameBit) == 0x1A);
STATIC_ASSERT(offsetof(DimCannonPlacement, armGameBit) == 0x1C);
STATIC_ASSERT(offsetof(DimCannonPlacement, holdGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DimCannonPlacement, unknown20) == 0x20);
STATIC_ASSERT(offsetof(DimCannonPlacement, triggerRange) == 0x26);
STATIC_ASSERT(offsetof(DimCannonPlacement, rotationXByte) == 0x28);
STATIC_ASSERT(offsetof(DimCannonPlacement, launchDelayMin) == 0x29);
STATIC_ASSERT(offsetof(DimCannonPlacement, launchDelayMax) == 0x2A);
STATIC_ASSERT(offsetof(DimCannonPlacement, targetRadius) == 0x2B);
STATIC_ASSERT(sizeof(DimCannonPlacement) == 0x2C);

STATIC_ASSERT(offsetof(DimCannonBallPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimCannonBallPlacement, unknown18) == 0x18);
STATIC_ASSERT(sizeof(DimCannonBallPlacement) == 0x24);

STATIC_ASSERT(offsetof(DimCannonState, targetPlayer) == 0x00);
STATIC_ASSERT(offsetof(DimCannonState, aimTargetX) == 0x04);
STATIC_ASSERT(offsetof(DimCannonState, aimTargetY) == 0x08);
STATIC_ASSERT(offsetof(DimCannonState, aimTargetZ) == 0x0C);
STATIC_ASSERT(offsetof(DimCannonState, targetDistance) == 0x10);
STATIC_ASSERT(offsetof(DimCannonState, aimHistoryX) == 0x14);
STATIC_ASSERT(offsetof(DimCannonState, aimHistoryY) == 0x3C);
STATIC_ASSERT(offsetof(DimCannonState, aimHistoryZ) == 0x64);
STATIC_ASSERT(offsetof(DimCannonState, launchOriginX) == 0x8C);
STATIC_ASSERT(offsetof(DimCannonState, launchOriginY) == 0x90);
STATIC_ASSERT(offsetof(DimCannonState, launchOriginZ) == 0x94);
STATIC_ASSERT(offsetof(DimCannonState, launchSpeed) == 0x98);
STATIC_ASSERT(offsetof(DimCannonState, unknown9C) == 0x9C);
STATIC_ASSERT(offsetof(DimCannonState, launchDelay) == 0xA4);
STATIC_ASSERT(offsetof(DimCannonState, shotCooldown) == 0xA6);
STATIC_ASSERT(offsetof(DimCannonState, previousAimDelta) == 0xA8);
STATIC_ASSERT(offsetof(DimCannonState, mode) == 0xAC);
STATIC_ASSERT(offsetof(DimCannonState, shouldSpawnProjectile) == 0xAD);
STATIC_ASSERT(offsetof(DimCannonState, airMeterCharge) == 0xAE);
STATIC_ASSERT(offsetof(DimCannonState, aimRefreshTimer) == 0xAF);
STATIC_ASSERT(offsetof(DimCannonState, chargeTimer) == 0xB0);
STATIC_ASSERT(offsetof(DimCannonState, shutdownTimer) == 0xB1);
STATIC_ASSERT(offsetof(DimCannonState, hasActivated) == 0xB2);
STATIC_ASSERT(offsetof(DimCannonState, unknownB3) == 0xB3);
STATIC_ASSERT(sizeof(DimCannonState) == 0xB4);

STATIC_ASSERT(offsetof(DimCannonBallState, parent) == 0x00);
STATIC_ASSERT(offsetof(DimCannonBallState, variant) == 0x04);
STATIC_ASSERT(offsetof(DimCannonBallState, hitboxRadius) == 0x05);
STATIC_ASSERT(offsetof(DimCannonBallState, hitType) == 0x06);
STATIC_ASSERT(offsetof(DimCannonBallState, clearLatch) == 0x07);
STATIC_ASSERT(offsetof(DimCannonBallState, mode) == 0x08);
STATIC_ASSERT(offsetof(DimCannonBallState, rotationZRate) == 0x09);
STATIC_ASSERT(offsetof(DimCannonBallState, rotationYRate) == 0x0A);
STATIC_ASSERT(offsetof(DimCannonBallState, rotationXRate) == 0x0B);
STATIC_ASSERT(sizeof(DimCannonBallState) == 0x0C);

void DIMCannon_updateBall(GameObject* obj);
void DIMCannon_spawnBall(GameObject* obj, u8 variant);
void DIMCannon_updateAim(GameObject* obj, f32 targetX, f32 unusedTargetY, f32 targetZ, f32 unusedDistance);
int DIMCannon_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int DIMCannon_getExtraSize(GameObject* obj);
int DIMCannon_getObjectTypeId(GameObject* obj);
void DIMCannon_free(GameObject* obj);
void DIMCannon_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                      s8 unusedVisible);
void DIMCannon_hitDetect(void);
void DIMCannon_update(GameObject* obj);
void DIMCannon_init(GameObject* obj, DimCannonPlacement* placement);
void DIMCannon_release(void);
void DIMCannon_initialise(void);

extern ObjectDescriptor gDIMCannonObjDescriptor;

#endif /* DLLS_OBJECTS_454_DIMCANNON_H_ */
