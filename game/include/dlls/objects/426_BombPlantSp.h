#ifndef DLLS_OBJECTS_426_BOMB_PLANT_SP_H_
#define DLLS_OBJECTS_426_BOMB_PLANT_SP_H_

#include "dlls/object_descriptor.h"
#include "main/dll/curves_collision_state.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/modellight_api.h"

#define BOMB_PLANT_SPORE_OBJECT_ID 0x198

typedef struct BombPlantSporeFlags {
    u8 hitSurface : 1;
    u8 waitingForDetonateAck : 1;
    u8 unknown : 6;
} BombPlantSporeFlags;

typedef struct BombPlantSporePlacement {
    ObjPlacement base;
    u8 unknown18[2];
    union {
        struct {
            s16 angleSpread;
            s16 baseAngle;
        } behavior;
        struct {
            s16 spawnYaw;
            s16 rotXSeed;
        } spawn;
    };
    u8 unknown1E[6];
} BombPlantSporePlacement;

typedef struct BombPlantSporeState {
    s16 pickupMsgBitId;
    s16 pickupMsgValue;
    f32 pickupMsgDelay;
    CurvesCollisionState path;
    ModelLightStruct* light;
    f32 fuseTimer;
    f32 driftAmplitude;
    f32 driftSpeed;
    f32 driftAmplitudeTarget;
    f32 driftTimer;
    f32 driftBaseX;
    f32 driftBaseZ;
    f32 driftSin;
    f32 driftCos;
    f32 spinTimer;
    f32 driftSpeedTarget;
    f32 spinChangeTimer;
    f32 detonateTimer;
    s16 currentSpinAngle;
    s16 burstDriftAngle;
    s16 spinAngle;
    s16 yawStep;
    BombPlantSporeFlags flags;
    u8 unknown2B1[3];
} BombPlantSporeState;

STATIC_ASSERT(sizeof(BombPlantSporePlacement) == 0x24);
STATIC_ASSERT(offsetof(BombPlantSporePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(BombPlantSporePlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(BombPlantSporePlacement, behavior.angleSpread) == 0x1A);
STATIC_ASSERT(offsetof(BombPlantSporePlacement, behavior.baseAngle) == 0x1C);
STATIC_ASSERT(offsetof(BombPlantSporePlacement, spawn.spawnYaw) == 0x1A);
STATIC_ASSERT(offsetof(BombPlantSporePlacement, spawn.rotXSeed) == 0x1C);
STATIC_ASSERT(offsetof(BombPlantSporePlacement, unknown1E) == 0x1E);



int BombPlantSpore_getExtraSize(void);
void BombPlantSpore_free(GameObject* obj);
void BombPlantSpore_update(GameObject* obj);
void BombPlantSpore_init(GameObject* obj, BombPlantSporePlacement* placement);

extern ObjectDescriptor10WithPadding gBombPlantSporeObjDescriptor;

#endif /* DLLS_OBJECTS_426_BOMB_PLANT_SP_H_ */
