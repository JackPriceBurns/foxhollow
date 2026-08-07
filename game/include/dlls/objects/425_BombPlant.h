#ifndef DLLS_OBJECTS_425_BOMB_PLANT_H_
#define DLLS_OBJECTS_425_BOMB_PLANT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define BOMB_PLANT_STATE_CONFIG_COUNT 5

typedef struct BombPlantPlacement {
    ObjPlacement base;
    s16 growTimer;
    s16 timerBase;
    s16 gameBit;    /* -1 selects timed regrowth. */
    s8 sporeYaw;    /* High byte of each spawned spore's yaw. */
    s8 initialRotX; /* High byte of the plant's initial X rotation. */
} BombPlantPlacement;

typedef struct BombPlantState {
    f32 growTimer;
    f32 growStartScale;
    f32 growDuration;
    f32 growTargetScale;
    f32 growRate;
    u8 stateIndex;
    u8 flags;
    u8 unknown16[2];
} BombPlantState;

typedef struct BombPlantStateConfig {
    s16 moveId;
    f32 moveStepScale;
    u8 flags;
} BombPlantStateConfig;

STATIC_ASSERT(sizeof(BombPlantPlacement) == 0x20);
STATIC_ASSERT(offsetof(BombPlantPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(BombPlantPlacement, growTimer) == 0x18);
STATIC_ASSERT(offsetof(BombPlantPlacement, timerBase) == 0x1A);
STATIC_ASSERT(offsetof(BombPlantPlacement, gameBit) == 0x1C);
STATIC_ASSERT(offsetof(BombPlantPlacement, sporeYaw) == 0x1E);
STATIC_ASSERT(offsetof(BombPlantPlacement, initialRotX) == 0x1F);

STATIC_ASSERT(sizeof(BombPlantState) == 0x18);
STATIC_ASSERT(offsetof(BombPlantState, growTimer) == 0x00);
STATIC_ASSERT(offsetof(BombPlantState, growStartScale) == 0x04);
STATIC_ASSERT(offsetof(BombPlantState, growDuration) == 0x08);
STATIC_ASSERT(offsetof(BombPlantState, growTargetScale) == 0x0C);
STATIC_ASSERT(offsetof(BombPlantState, growRate) == 0x10);
STATIC_ASSERT(offsetof(BombPlantState, stateIndex) == 0x14);
STATIC_ASSERT(offsetof(BombPlantState, flags) == 0x15);
STATIC_ASSERT(offsetof(BombPlantState, unknown16) == 0x16);

STATIC_ASSERT(sizeof(BombPlantStateConfig) == 0x0C);
STATIC_ASSERT(offsetof(BombPlantStateConfig, moveId) == 0x00);
STATIC_ASSERT(offsetof(BombPlantStateConfig, moveStepScale) == 0x04);
STATIC_ASSERT(offsetof(BombPlantStateConfig, flags) == 0x08);

extern BombPlantStateConfig gBombPlantStateConfigs[BOMB_PLANT_STATE_CONFIG_COUNT];
extern ObjectDescriptor10WithPadding gBombPlantObjDescriptor;

int BombPlant_animEventCallback(GameObject* obj);
void BombPlant_spawnSpore(GameObject* obj, BombPlantState* unusedState);
int BombPlant_getExtraSize(void);
int BombPlant_getObjectTypeId(void);
void BombPlant_free(void);
void BombPlant_render(GameObject* obj, int flags, int texData, int colorTable, int modelState, s8 unusedVisible);
void BombPlant_hitDetect(void);
void BombPlant_explode(GameObject* obj, BombPlantStateConfig* unusedConfig, BombPlantState* state);
void BombPlant_update(GameObject* obj);
void BombPlant_init(GameObject* obj, BombPlantPlacement* placement, int isReload);

#endif /* DLLS_OBJECTS_425_BOMB_PLANT_H_ */
