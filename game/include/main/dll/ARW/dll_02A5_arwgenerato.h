#ifndef MAIN_DLL_ARW_DLL_02A5_ARWGENERATO_H
#define MAIN_DLL_ARW_DLL_02A5_ARWGENERATO_H

#include "global.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"

typedef struct ARWGeneratorState
{
    f32 spawnTimer;
} ARWGeneratorState;

typedef struct ARWGeneratorSetup
{
    ObjPlacement base;
    u16 spawnInterval;
    u16 projectileSpeed;
    s8 velocityX;
    s8 velocityY;
    s8 velocityZ;
    u8 pad1F[3];
    s8 spreadX;
    s8 spreadY;
    s8 spreadZ;
    u8 spawnMode;
} ARWGeneratorSetup;

STATIC_ASSERT(sizeof(ARWGeneratorState) == 0x4);
STATIC_ASSERT(offsetof(ARWGeneratorSetup, spawnInterval) == 0x18);
STATIC_ASSERT(offsetof(ARWGeneratorSetup, projectileSpeed) == 0x1A);
STATIC_ASSERT(offsetof(ARWGeneratorSetup, velocityX) == 0x1C);
STATIC_ASSERT(offsetof(ARWGeneratorSetup, spreadX) == 0x22);
STATIC_ASSERT(offsetof(ARWGeneratorSetup, spawnMode) == 0x25);

int arwgenerato_getExtraSize(void);
int arwgenerato_getObjectTypeId(void);
void arwgenerato_free(void);
void arwgenerato_render(GameObject* obj, int p2, int p3, int p4, int p5, f32 scale);
void arwgenerato_hitDetect(void);
void arwgenerato_update(GameObject* obj);
void arwgenerato_init(GameObject* obj, ARWGeneratorSetup* setup);
void arwgenerato_release(void);
void arwgenerato_initialise(void);

void arwgenerato_spawnSquadronShipA(GameObject* obj, ARWGeneratorState* state, ARWGeneratorSetup* setup);
void arwgenerato_spawnSquadronShipB(GameObject* obj, ARWGeneratorState* state, ARWGeneratorSetup* setup);

#endif
