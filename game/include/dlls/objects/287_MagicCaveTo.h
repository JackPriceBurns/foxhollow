#ifndef DLLS_OBJECTS_287_MAGICCAVETO_H_
#define DLLS_OBJECTS_287_MAGICCAVETO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define MAGIC_CAVE_TOP_PLACEMENT_SIZE 0x28
#define MAGIC_CAVE_TOP_STATE_SIZE     0xC

typedef enum MagicCaveTopPhase {
    MAGIC_CAVE_TOP_PHASE_IDLE = 0,
    MAGIC_CAVE_TOP_PHASE_LOADED = 1,
    MAGIC_CAVE_TOP_PHASE_WARPING = 2,
    MAGIC_CAVE_TOP_PHASE_WARP_DONE = 3,
} MagicCaveTopPhase;

typedef enum MagicCaveTopStateFlag {
    MAGIC_CAVE_TOP_FLAG_RUMBLE_ACTIVE = 0x1,
    MAGIC_CAVE_TOP_FLAG_RUMBLE_COMPLETE = 0x2,
    MAGIC_CAVE_TOP_FLAG_RUMBLE_DISABLED = 0x4,
    MAGIC_CAVE_TOP_FLAG_ALT_EFFECT = 0x8,
} MagicCaveTopStateFlag;

typedef enum MagicCaveTopRumbleState {
    MAGIC_CAVE_TOP_RUMBLE_STATE_STOPPED = 0,
    MAGIC_CAVE_TOP_RUMBLE_STATE_PULSING = 1,
} MagicCaveTopRumbleState;

/*
 * The complete 0x10-byte parameter tail is preserved by the retail EN
 * moonpass#188 placement, establishing the 0x28 total width.
 */
typedef struct MagicCaveTopPlacement {
    ObjPlacement base;      /* 0x00 */
    u8 outerRange;          /* 0x18 */
    u8 innerRange;          /* 0x19 */
    u8 objectGroup;         /* 0x1A */
    u8 mapAct;              /* 0x1B */
    s16 visibleGameBit;     /* 0x1C */
    u8 lockDirId;           /* 0x1E */
    u8 mapId;               /* 0x1F */
    s8 warpMapId;           /* 0x20: direct WARPTAB index */
    s8 exitWarpId;          /* 0x21: stored in GAMEBIT_MagicCaveExitWarp */
    u8 skipMapLoad;         /* 0x22 */
    u8 rotationX;           /* 0x23: high byte of the initial X rotation */
    s16 textureSwapGameBit; /* 0x24 */
    u8 pad26[2];            /* 0x26 */
} MagicCaveTopPlacement;

typedef struct MagicCaveTopState {
    u8 phase;        /* 0x00: MagicCaveTopPhase */
    u8 flags;        /* 0x01: MagicCaveTopStateFlag */
    u8 rumbleState;  /* 0x02: MagicCaveTopRumbleState */
    u8 pad03;        /* 0x03 */
    f32 fadeTimer;   /* 0x04 */
    f32 rumbleTimer; /* 0x08 */
} MagicCaveTopState;

STATIC_ASSERT(offsetof(MagicCaveTopPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, outerRange) == 0x18);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, innerRange) == 0x19);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, objectGroup) == 0x1A);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, mapAct) == 0x1B);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, visibleGameBit) == 0x1C);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, lockDirId) == 0x1E);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, mapId) == 0x1F);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, warpMapId) == 0x20);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, exitWarpId) == 0x21);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, skipMapLoad) == 0x22);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, rotationX) == 0x23);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, textureSwapGameBit) == 0x24);
STATIC_ASSERT(offsetof(MagicCaveTopPlacement, pad26) == 0x26);
STATIC_ASSERT(sizeof(MagicCaveTopPlacement) == MAGIC_CAVE_TOP_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(MagicCaveTopState, phase) == 0x0);
STATIC_ASSERT(offsetof(MagicCaveTopState, flags) == 0x1);
STATIC_ASSERT(offsetof(MagicCaveTopState, rumbleState) == 0x2);
STATIC_ASSERT(offsetof(MagicCaveTopState, pad03) == 0x3);
STATIC_ASSERT(offsetof(MagicCaveTopState, fadeTimer) == 0x4);
STATIC_ASSERT(offsetof(MagicCaveTopState, rumbleTimer) == 0x8);
STATIC_ASSERT(sizeof(MagicCaveTopState) == MAGIC_CAVE_TOP_STATE_SIZE);

int MagicCaveTop_getExtraSize(void);
void MagicCaveTop_free(GameObject* obj);
void MagicCaveTop_update(GameObject* obj);
void MagicCaveTop_init(GameObject* obj, MagicCaveTopPlacement* placement);

extern ObjectDescriptor gMagicCaveTopObjDescriptor;

#endif /* DLLS_OBJECTS_287_MAGICCAVETO_H_ */
