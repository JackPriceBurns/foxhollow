#ifndef DLLS_OBJECTS_318_H_
#define DLLS_OBJECTS_318_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/curves_collision_state.h"

typedef enum DimBossIceSmashPlacementFlag {
    DIM_BOSS_ICE_SMASH_PLACEMENT_HOMING = 0x01,
    DIM_BOSS_ICE_SMASH_PLACEMENT_PATH_CONTROL = 0x02,
    DIM_BOSS_ICE_SMASH_PLACEMENT_TRAIL_PARTICLES = 0x04,
} DimBossIceSmashPlacementFlag;

typedef enum DimBossIceSmashStateFlag {
    DIM_BOSS_ICE_SMASH_STATE_ACTIVE = 0x01,
    DIM_BOSS_ICE_SMASH_STATE_FINISHED = 0x02,
} DimBossIceSmashStateFlag;

typedef enum DimBossIceSmashDirectionFlag {
    DIM_BOSS_ICE_SMASH_POSITIVE_VELOCITY_X = 0x01,
    DIM_BOSS_ICE_SMASH_POSITIVE_VELOCITY_Z = 0x02,
    DIM_BOSS_ICE_SMASH_POSITIVE_ANGULAR_VELOCITY_X = 0x04,
    DIM_BOSS_ICE_SMASH_POSITIVE_ANGULAR_VELOCITY_Y = 0x08,
    DIM_BOSS_ICE_SMASH_POSITIVE_ANGULAR_VELOCITY_Z = 0x10,
} DimBossIceSmashDirectionFlag;

/*
 * The setup fields through homingTargetZ are evidenced by this TU. The
 * complete record extent after 0x47 is not yet proven.
 */
typedef struct DimBossIceSmashPlacement {
    ObjPlacement base;   /* 0x00 */
    u8 bankIndex;        /* 0x18 */
    u8 pad19;            /* 0x19 */
    s16 spawnRotX;       /* 0x1A */
    s16 spawnRotY;       /* 0x1C */
    s16 spawnRotZ;       /* 0x1E */
    s16 velocityX;       /* 0x20 */
    s16 velocityY;       /* 0x22 */
    s16 velocityZ;       /* 0x24 */
    s16 gravityX;        /* 0x26 */
    s16 gravityY;        /* 0x28 */
    s16 gravityZ;        /* 0x2A */
    s16 rotVelX;         /* 0x2C */
    s16 rotVelY;         /* 0x2E */
    s16 rotVelZ;         /* 0x30 */
    s16 rotGravityX;     /* 0x32 */
    s16 rotGravityY;     /* 0x34 */
    s16 rotGravityZ;     /* 0x36 */
    u16 lifetime;        /* 0x38 */
    u16 fadeStartFrame;  /* 0x3A */
    u8 flags;            /* 0x3C: DimBossIceSmashPlacementFlag */
    u8 pad3D;            /* 0x3D */
    s16 activateGameBit; /* 0x3E */
    s16 triggerGameBit;  /* 0x40 */
    s16 homingTargetX;   /* 0x42 */
    s16 homingTargetY;   /* 0x44 */
    s16 homingTargetZ;   /* 0x46 */
} DimBossIceSmashPlacement;

/* DIMBossIceSmash_getExtraSize proves the complete 0x2A0-byte allocation. */
typedef struct DimBossIceSmashState {
    CurvesCollisionState path;
    u8 pad00[4];
    f32 spawnScaleX;
    f32 spawnScaleY;
    f32 spawnScaleZ;
    f32 angVelX;
    f32 angVelY;
    f32 angVelZ;
    f32 angAccelX;
    f32 angAccelY;
    f32 angAccelZ;
    f32 accelX;
    f32 accelY;
    f32 accelZ;
    s16 timer;
    u8 stateFlags;
    u8 directionFlags;
} DimBossIceSmashState;

STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, bankIndex) == 0x18);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, spawnRotX) == 0x1A);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, spawnRotY) == 0x1C);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, spawnRotZ) == 0x1E);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, velocityX) == 0x20);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, velocityY) == 0x22);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, velocityZ) == 0x24);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, gravityX) == 0x26);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, gravityY) == 0x28);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, gravityZ) == 0x2A);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, rotVelX) == 0x2C);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, rotVelY) == 0x2E);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, rotVelZ) == 0x30);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, rotGravityX) == 0x32);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, rotGravityY) == 0x34);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, rotGravityZ) == 0x36);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, lifetime) == 0x38);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, fadeStartFrame) == 0x3A);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, flags) == 0x3C);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, pad3D) == 0x3D);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, activateGameBit) == 0x3E);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, triggerGameBit) == 0x40);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, homingTargetX) == 0x42);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, homingTargetY) == 0x44);
STATIC_ASSERT(offsetof(DimBossIceSmashPlacement, homingTargetZ) == 0x46);

STATIC_ASSERT(offsetof(DimBossIceSmashState, path) == 0x0);
STATIC_ASSERT(offsetof(DimBossIceSmashState, spawnScaleX) == sizeof(CurvesCollisionState) + 0x04);
STATIC_ASSERT(offsetof(DimBossIceSmashState, angVelX) == sizeof(CurvesCollisionState) + 0x10);
STATIC_ASSERT(offsetof(DimBossIceSmashState, angAccelX) == sizeof(CurvesCollisionState) + 0x1C);
STATIC_ASSERT(offsetof(DimBossIceSmashState, accelX) == sizeof(CurvesCollisionState) + 0x28);
STATIC_ASSERT(offsetof(DimBossIceSmashState, timer) == sizeof(CurvesCollisionState) + 0x34);
STATIC_ASSERT(sizeof(DimBossIceSmashState) == sizeof(CurvesCollisionState) + 0x38);
STATIC_ASSERT(offsetof(DimBossIceSmashState, timer) == 0x29C);
STATIC_ASSERT(offsetof(DimBossIceSmashState, stateFlags) == 0x29E);
STATIC_ASSERT(offsetof(DimBossIceSmashState, directionFlags) == 0x29F);
STATIC_ASSERT(sizeof(DimBossIceSmashState) == 0x2A0);

void DIMBossIceSmash_initLaunchState(GameObject* obj, DimBossIceSmashState* state, DimBossIceSmashPlacement* placement);
int DIMBossIceSmash_getExtraSize(void);
u32 DIMBossIceSmash_getObjectTypeId(GameObject* obj);
void DIMBossIceSmash_free(GameObject* obj);
void DIMBossIceSmash_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                            s8 visible);
void DIMBossIceSmash_hitDetect(void);
void DIMBossIceSmash_update(GameObject* obj);
void DIMBossIceSmash_init(GameObject* obj, DimBossIceSmashPlacement* placement);
void DIMBossIceSmash_release(void);
void DIMBossIceSmash_initialise(void);

extern ObjectDescriptor10WithPadding gDIMBossIceSmashObjDescriptor;

#endif /* DLLS_OBJECTS_318_H_ */
