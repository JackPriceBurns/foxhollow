#ifndef MAIN_DLL_SAVEGAME_ENV_API_H_
#define MAIN_DLL_SAVEGAME_ENV_API_H_

#include "global.h"

/*
 * Persisted environment-effect state (SaveGameData + 0x6A8). Each ENVFXACT
 * kind handler saves its active 0-based act index here (-1 = none); the
 * map-setup path replays them via getEnvfxActImmediately.
 */
typedef struct SaveGameEnvState {
    f32 unk00;
    s16 skyEnvfxActIds[2];     /* 0x04: kind-5 sky slots A/B */
    u8 unk08[2];
    s16 cloudActionEnvfxActId; /* 0x0A: kind 6 */
    s16 sky2EnvfxActId;        /* 0x0C: kind 3 */
    s16 cloudEnvfxActIds[3];   /* 0x0E: kind 0/1/2/4 per cloud slot */
    s32 cloudPos[3][3];        /* 0x14: saved spawn position per cloud slot */
    u8 unk38[8];
    u8 envFlags;               /* 0x40: bit0 drawClouds, bit1/2 sky slot flag80, bit3 drawLights, bit4 lightIndex, bit5 heatEffect */
    s8 cloudStationary[3];     /* 0x41: -1 = slot empty */
} SaveGameEnvState;

STATIC_ASSERT(offsetof(SaveGameEnvState, skyEnvfxActIds) == 0x04);
STATIC_ASSERT(offsetof(SaveGameEnvState, cloudActionEnvfxActId) == 0x0A);
STATIC_ASSERT(offsetof(SaveGameEnvState, cloudEnvfxActIds) == 0x0E);
STATIC_ASSERT(offsetof(SaveGameEnvState, cloudPos) == 0x14);
STATIC_ASSERT(offsetof(SaveGameEnvState, envFlags) == 0x40);
STATIC_ASSERT(sizeof(SaveGameEnvState) == 0x44);

SaveGameEnvState* saveGameGetEnvState(void);

#endif /* MAIN_DLL_SAVEGAME_ENV_API_H_ */
