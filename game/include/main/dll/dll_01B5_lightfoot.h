#ifndef MAIN_DLL_DLL_01B5_LIGHTFOOT_H_
#define MAIN_DLL_DLL_01B5_LIGHTFOOT_H_

#include "game/objects/object.h"

typedef struct PlayerLightfootScuffPattern
{
    s16 anims[8];
    f32 rates[8];
} PlayerLightfootScuffPattern;
STATIC_ASSERT(sizeof(PlayerLightfootScuffPattern) == 0x30);

typedef struct PlayerLightfootAnimTable
{
    PlayerLightfootScuffPattern lightScuff;
    PlayerLightfootScuffPattern heavyScuff;
    s16 challengeAnims[14];
    f32 challengeSpeeds[14];
} PlayerLightfootAnimTable;
STATIC_ASSERT(sizeof(PlayerLightfootAnimTable) == 0xB4);

extern PlayerLightfootAnimTable gPlayerLightfootAnimTable;

#endif /* MAIN_DLL_DLL_01B5_LIGHTFOOT_H_ */
