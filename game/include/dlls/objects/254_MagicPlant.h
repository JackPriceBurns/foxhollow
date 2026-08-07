#ifndef DLLS_OBJECTS_254_MAGICPLANT_H_
#define DLLS_OBJECTS_254_MAGICPLANT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

#define MAGICPLANT_PLACEMENT_SIZE 0x20
#define MAGICPLANT_STATE_SIZE     0x10

/* Retail MagicPlant placements consistently use a fixed 0x20-byte record. */
typedef struct MagicPlantPlacement {
    u8 pad00[0x14];    /* 0x00 */
    s32 eventId;       /* 0x14 */
    u16 eventDuration; /* 0x18 */
    u8 pad1A;          /* 0x1A */
    u8 gemColor;       /* 0x1B: MAGICPLANT_GEM_* */
    u8 modelIndex;     /* 0x1C */
    u8 yawByte;        /* 0x1D */
    u8 pad1E[2];       /* 0x1E */
} MagicPlantPlacement;

typedef struct MagicPlantState {
    GameObject* childObject; /* 0x00 */
    f32 animProgress;        /* 0x04 */
    f32 animStepScale;       /* 0x08 */
    s16 idleTimer;           /* 0x0C */
    u8 pad0E;                /* 0x0E */
    s8 mode;                 /* 0x0F: MAGICPLANT_MODE_* */
} MagicPlantState;

#define MAGICPLANT_MODE_WAIT_FOR_EVENT 0
#define MAGICPLANT_MODE_ACTIVE         1
#define MAGICPLANT_MODE_FADE_OUT       2
#define MAGICPLANT_MODE_FADE_IN        3
#define MAGICPLANT_MODE_HIT_REACT      4

#define MAGICPLANT_GEM_GREEN  0
#define MAGICPLANT_GEM_RED    1
#define MAGICPLANT_GEM_YELLOW 2
#define MAGICPLANT_GEM_BLUE   3

#define MAGICPLANT_MOVE_CLOSED    0
#define MAGICPLANT_MOVE_SWAY_FAST 1
#define MAGICPLANT_MOVE_BURST     2
#define MAGICPLANT_MOVE_HIT       3
#define MAGICPLANT_MOVE_IDLE      4

STATIC_ASSERT(offsetof(MagicPlantPlacement, pad00) == 0x0);
STATIC_ASSERT(offsetof(MagicPlantPlacement, eventId) == 0x14);
STATIC_ASSERT(offsetof(MagicPlantPlacement, eventDuration) == 0x18);
STATIC_ASSERT(offsetof(MagicPlantPlacement, pad1A) == 0x1A);
STATIC_ASSERT(offsetof(MagicPlantPlacement, gemColor) == 0x1B);
STATIC_ASSERT(offsetof(MagicPlantPlacement, modelIndex) == 0x1C);
STATIC_ASSERT(offsetof(MagicPlantPlacement, yawByte) == 0x1D);
STATIC_ASSERT(offsetof(MagicPlantPlacement, pad1E) == 0x1E);
STATIC_ASSERT(sizeof(MagicPlantPlacement) == MAGICPLANT_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(MagicPlantState, childObject) == 0x0);
STATIC_ASSERT(offsetof(MagicPlantState, animProgress) == 0x4);
STATIC_ASSERT(offsetof(MagicPlantState, animStepScale) == 0x8);
STATIC_ASSERT(offsetof(MagicPlantState, idleTimer) == 0xC);
STATIC_ASSERT(offsetof(MagicPlantState, pad0E) == 0xE);
STATIC_ASSERT(offsetof(MagicPlantState, mode) == 0xF);
STATIC_ASSERT(sizeof(MagicPlantState) == MAGICPLANT_STATE_SIZE);

void magicPlantDropGem(GameObject* obj, MagicPlantPlacement* unusedPlacement, MagicPlantState* state);
void MagicPlant_updateActive(GameObject* obj, MagicPlantPlacement* unusedPlacement, MagicPlantState* state);
void MagicPlant_spawnChild(GameObject* obj, int objectId);
int MagicPlant_SeqFn(GameObject* obj);
int MagicPlant_getExtraSize(void);
u32 MagicPlant_getObjectTypeId(GameObject* obj);
void MagicPlant_free(GameObject* obj, int keepChildren);
void MagicPlant_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void MagicPlant_update(GameObject* obj);
void MagicPlant_init(GameObject* obj, MagicPlantPlacement* placement);

extern s16 gMagicPlantGemDefIds[4];
extern ObjectDescriptor gMagicPlantObjDescriptor;

#endif /* DLLS_OBJECTS_254_MAGICPLANT_H_ */
