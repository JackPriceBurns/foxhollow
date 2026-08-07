#ifndef DLLS_OBJECTS_424_SH_KILLERMU_H_
#define DLLS_OBJECTS_424_SH_KILLERMU_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define ENEMY_MUSHROOM_STATE_ANIM_MOVE_COUNT 12
#define ENEMY_MUSHROOM_STATE_ANIM_RATE_COUNT 11

#define ENEMY_MUSHROOM_STATE_FLAG_HIT_PLAYER 0x01
#define ENEMY_MUSHROOM_STATE_FLAG_ANIM_DONE  0x02
#define ENEMY_MUSHROOM_STATE_FLAG_ACTIVE     0x04

typedef enum EnemyMushroomStateId {
    ENEMY_MUSHROOM_STATE_IDLE = 0,
    ENEMY_MUSHROOM_STATE_REGROWING = 1,
    ENEMY_MUSHROOM_STATE_FADING = 2,
    ENEMY_MUSHROOM_STATE_STARTLED = 3,
    ENEMY_MUSHROOM_STATE_POISONING = 4,
    ENEMY_MUSHROOM_STATE_SETTLING = 5,
    ENEMY_MUSHROOM_STATE_DEFLATING = 6,
    ENEMY_MUSHROOM_STATE_STUNNED = 9,
    ENEMY_MUSHROOM_STATE_RESPAWN_WAIT = 10,
} EnemyMushroomStateId;

typedef struct EnemyMushroomPlacement {
    ObjPlacement base;
    u16 regrowDelay;
    u16 respawnFrameLimit;
    s16 popGameBit;
    u8 detectRange;
    u8 objectTypeParam;
    u8 unknown20[0x24 - 0x20];
} EnemyMushroomPlacement;

typedef struct EnemyMushroomState {
    f32 timer;
    f32 heightTarget;
    f32 riseDuration;
    f32 baseScale;
    f32 riseStep;
    u8 unknown14[0x20 - 0x14];
    f32 hitEffectX;
    f32 hitEffectY;
    f32 hitEffectZ;
    f32 hitRadius;
    f32 effectTimer;
    s16 respawnFrameLimit;
    u8 stateId;
    u8 stateFlags;
    u8 unknown38[0x3C - 0x38];
} EnemyMushroomState;

STATIC_ASSERT(sizeof(EnemyMushroomPlacement) == 0x24);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, regrowDelay) == 0x18);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, respawnFrameLimit) == 0x1A);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, popGameBit) == 0x1C);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, detectRange) == 0x1E);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, objectTypeParam) == 0x1F);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, unknown20) == 0x20);

STATIC_ASSERT(sizeof(EnemyMushroomState) == 0x3C);
STATIC_ASSERT(offsetof(EnemyMushroomState, timer) == 0x00);
STATIC_ASSERT(offsetof(EnemyMushroomState, heightTarget) == 0x04);
STATIC_ASSERT(offsetof(EnemyMushroomState, riseDuration) == 0x08);
STATIC_ASSERT(offsetof(EnemyMushroomState, baseScale) == 0x0C);
STATIC_ASSERT(offsetof(EnemyMushroomState, riseStep) == 0x10);
STATIC_ASSERT(offsetof(EnemyMushroomState, unknown14) == 0x14);
STATIC_ASSERT(offsetof(EnemyMushroomState, hitEffectX) == 0x20);
STATIC_ASSERT(offsetof(EnemyMushroomState, hitEffectY) == 0x24);
STATIC_ASSERT(offsetof(EnemyMushroomState, hitEffectZ) == 0x28);
STATIC_ASSERT(offsetof(EnemyMushroomState, hitRadius) == 0x2C);
STATIC_ASSERT(offsetof(EnemyMushroomState, effectTimer) == 0x30);
STATIC_ASSERT(offsetof(EnemyMushroomState, respawnFrameLimit) == 0x34);
STATIC_ASSERT(offsetof(EnemyMushroomState, stateId) == 0x36);
STATIC_ASSERT(offsetof(EnemyMushroomState, stateFlags) == 0x37);
STATIC_ASSERT(offsetof(EnemyMushroomState, unknown38) == 0x38);

extern s16 gEnemyMushroomStateAnimMoves[ENEMY_MUSHROOM_STATE_ANIM_MOVE_COUNT];
extern f32 gEnemyMushroomStateAnimRates[ENEMY_MUSHROOM_STATE_ANIM_RATE_COUNT];
extern ObjectDescriptor gEnemyMushroomObjDescriptor;

void EnemyMushroom_resetToSpawn(GameObject* obj, EnemyMushroomState* state, int enableTimer);
int EnemyMushroom_getExtraSize(void);
int EnemyMushroom_getObjectTypeId(GameObject* obj);
void EnemyMushroom_free(GameObject* obj);
void EnemyMushroom_render(GameObject* obj, u32 flags, u32 texData, u32 colorTable, u32 modelState, char visible);
void EnemyMushroom_hitDetect(void);
void EnemyMushroom_update(GameObject* obj);
void EnemyMushroom_init(GameObject* obj, EnemyMushroomPlacement* placement, int flags);
void EnemyMushroom_release(void);
void EnemyMushroom_initialise(void);

#endif /* DLLS_OBJECTS_424_SH_KILLERMU_H_ */
