#ifndef DLLS_OBJECTS_307_SFXPLAYER_H_
#define DLLS_OBJECTS_307_SFXPLAYER_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum SfxPlayerMode {
    SFXPLAYER_MODE_GAME_BIT = 0,
    SFXPLAYER_MODE_LOOPED = 1,
    SFXPLAYER_MODE_RANDOM_DELAY = 2,
} SfxPlayerMode;

typedef enum SfxPlayerPlacementFlag {
    SFXPLAYER_FLAG_FORCE_POINT = 0x01,
    SFXPLAYER_FLAG_TRIGGER_ON_SET = 0x02,
    SFXPLAYER_FLAG_TRIGGER_ON_CLEAR = 0x04,
    SFXPLAYER_FLAG_ROM_CURVE = 0x08,
    SFXPLAYER_FLAG_AT_OBJECT = 0x10,
} SfxPlayerPlacementFlag;

typedef enum SfxPlayerStateFlag {
    SFXPLAYER_STATE_FLAG_ACTIVE = 0x01,
} SfxPlayerStateFlag;

/*
 * All 72 active retail EN placements use this complete fixed-width
 * 0x24-byte record.
 */
typedef struct SfxPlayerPlacement {
    ObjPlacement base;  /* 0x00 */
    s16 gameBit;        /* 0x18: mode-specific trigger/gate; -1 means none */
    u16 primarySfxId;   /* 0x1A */
    u8 flags;           /* 0x1C: SfxPlayerPlacementFlag */
    u8 mode;            /* 0x1D: SfxPlayerMode */
    u8 randomDelayMin;  /* 0x1E: seconds */
    u8 randomDelayMax;  /* 0x1F: seconds */
    s8 romCurveChannel; /* 0x20 */
    u8 pad21;           /* 0x21 */
    u16 secondarySfxId; /* 0x22 */
} SfxPlayerPlacement;

/* SfxPlayer_getExtraSize proves the complete 0x08-byte allocation. */
typedef struct SfxPlayerState {
    union {
        int gameBitState; /* 0x00: SFXPLAYER_MODE_GAME_BIT */
        f32 delayTimer;   /* 0x00: SFXPLAYER_MODE_RANDOM_DELAY */
    };
    u8 flags;    /* 0x04: SfxPlayerStateFlag */
    u8 pad05[3]; /* 0x05 */
} SfxPlayerState;

STATIC_ASSERT(offsetof(SfxPlayerPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(SfxPlayerPlacement, gameBit) == 0x18);
STATIC_ASSERT(offsetof(SfxPlayerPlacement, primarySfxId) == 0x1A);
STATIC_ASSERT(offsetof(SfxPlayerPlacement, flags) == 0x1C);
STATIC_ASSERT(offsetof(SfxPlayerPlacement, mode) == 0x1D);
STATIC_ASSERT(offsetof(SfxPlayerPlacement, randomDelayMin) == 0x1E);
STATIC_ASSERT(offsetof(SfxPlayerPlacement, randomDelayMax) == 0x1F);
STATIC_ASSERT(offsetof(SfxPlayerPlacement, romCurveChannel) == 0x20);
STATIC_ASSERT(offsetof(SfxPlayerPlacement, pad21) == 0x21);
STATIC_ASSERT(offsetof(SfxPlayerPlacement, secondarySfxId) == 0x22);
STATIC_ASSERT(sizeof(SfxPlayerPlacement) == 0x24);

STATIC_ASSERT(offsetof(SfxPlayerState, gameBitState) == 0x00);
STATIC_ASSERT(offsetof(SfxPlayerState, delayTimer) == 0x00);
STATIC_ASSERT(offsetof(SfxPlayerState, flags) == 0x04);
STATIC_ASSERT(offsetof(SfxPlayerState, pad05) == 0x05);
STATIC_ASSERT(sizeof(SfxPlayerState) == 0x08);

int SfxPlayer_getExtraSize(void);
void SfxPlayer_free(GameObject* obj);
void SfxPlayer_update(GameObject* obj);
void SfxPlayer_init(GameObject* obj, SfxPlayerPlacement* placement);

extern ObjectDescriptor gSfxPlayerObjDescriptor;

#endif /* DLLS_OBJECTS_307_SFXPLAYER_H_ */
