#ifndef DLLS_OBJECTS_261_LARGECRATE_H_
#define DLLS_OBJECTS_261_LARGECRATE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define LARGECRATE_PLACEMENT_SIZE 0x24
#define LARGECRATE_STATE_SIZE     0x2C

typedef enum LargeCrateDropType {
    LARGECRATE_DROPTYPE_GREEN_SCARAB = 1,
    LARGECRATE_DROPTYPE_RED_SCARAB = 2,
    LARGECRATE_DROPTYPE_GOLD_SCARAB = 3,
    LARGECRATE_DROPTYPE_ENERGY_EGG = 5,
    LARGECRATE_DROPTYPE_APPLE = 6,
    LARGECRATE_DROPTYPE_NONE_A = 7,
    LARGECRATE_DROPTYPE_NONE_B = 8,
    LARGECRATE_DROPTYPE_PICKUP = 9,
} LargeCrateDropType;

/* Retail LargeCrate and LargeCrateL placements have a fixed 0x0C-byte parameter tail. */
typedef struct LargeCratePlacement {
    ObjPlacement base;  /* 0x00 */
    s8 rotXByte;        /* 0x18 */
    u8 dropType;        /* 0x19: remap-table index */
    s16 unk1A;          /* 0x1A: copied into state byte 0x12 */
    s16 respawnMinutes; /* 0x1C: 0 disables respawning; 0xFF never expires */
    s16 brokenGameBit;  /* 0x1E */
    u8 pad20[4];        /* 0x20 */
} LargeCratePlacement;

/* LargeCrate_getExtraSize allocates the complete 0x2C-byte state block. */
typedef struct LargeCrateState {
    s32 respawnDelay;   /* 0x00 */
    f32 hiddenTimer;    /* 0x04 */
    s16 breakTimer;     /* 0x08 */
    s16 idleTimer;      /* 0x0A */
    s16 unk0C;          /* 0x0C: initialized to 0x190 */
    s16 brokenGameBit;  /* 0x0E */
    u8 unk10;           /* 0x10 */
    u8 dropType;        /* 0x11: LargeCrateDropType */
    u8 unk12;           /* 0x12: copied from placement offset 0x1A */
    u8 damageTaken;     /* 0x13 */
    s16 hitSfxId;       /* 0x14 */
    s16 breakSfxId;     /* 0x16 */
    s16 spinSpeed;      /* 0x18 */
    u8 pad1A[2];        /* 0x1A */
    f32 slidePhase;     /* 0x1C */
    u16 slideOffset;    /* 0x20 */
    u8 pad22[2];        /* 0x22 */
    f32 homeX;          /* 0x24 */
    u8 damageThreshold; /* 0x28 */
    u8 pad29[3];        /* 0x29 */
} LargeCrateState;

STATIC_ASSERT(offsetof(LargeCratePlacement, base) == 0x0);
STATIC_ASSERT(offsetof(LargeCratePlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(LargeCratePlacement, dropType) == 0x19);
STATIC_ASSERT(offsetof(LargeCratePlacement, unk1A) == 0x1A);
STATIC_ASSERT(offsetof(LargeCratePlacement, respawnMinutes) == 0x1C);
STATIC_ASSERT(offsetof(LargeCratePlacement, brokenGameBit) == 0x1E);
STATIC_ASSERT(offsetof(LargeCratePlacement, pad20) == 0x20);
STATIC_ASSERT(sizeof(LargeCratePlacement) == LARGECRATE_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(LargeCrateState, respawnDelay) == 0x0);
STATIC_ASSERT(offsetof(LargeCrateState, hiddenTimer) == 0x4);
STATIC_ASSERT(offsetof(LargeCrateState, breakTimer) == 0x8);
STATIC_ASSERT(offsetof(LargeCrateState, idleTimer) == 0xA);
STATIC_ASSERT(offsetof(LargeCrateState, unk0C) == 0xC);
STATIC_ASSERT(offsetof(LargeCrateState, brokenGameBit) == 0xE);
STATIC_ASSERT(offsetof(LargeCrateState, unk10) == 0x10);
STATIC_ASSERT(offsetof(LargeCrateState, dropType) == 0x11);
STATIC_ASSERT(offsetof(LargeCrateState, unk12) == 0x12);
STATIC_ASSERT(offsetof(LargeCrateState, damageTaken) == 0x13);
STATIC_ASSERT(offsetof(LargeCrateState, hitSfxId) == 0x14);
STATIC_ASSERT(offsetof(LargeCrateState, breakSfxId) == 0x16);
STATIC_ASSERT(offsetof(LargeCrateState, spinSpeed) == 0x18);
STATIC_ASSERT(offsetof(LargeCrateState, pad1A) == 0x1A);
STATIC_ASSERT(offsetof(LargeCrateState, slidePhase) == 0x1C);
STATIC_ASSERT(offsetof(LargeCrateState, slideOffset) == 0x20);
STATIC_ASSERT(offsetof(LargeCrateState, pad22) == 0x22);
STATIC_ASSERT(offsetof(LargeCrateState, homeX) == 0x24);
STATIC_ASSERT(offsetof(LargeCrateState, damageThreshold) == 0x28);
STATIC_ASSERT(offsetof(LargeCrateState, pad29) == 0x29);
STATIC_ASSERT(sizeof(LargeCrateState) == LARGECRATE_STATE_SIZE);

f32 LargeCrate_getReticleDistance(GameObject* obj);
void LargeCrate_updateConveyorSlide(GameObject* obj, LargeCrateState* state);
int LargeCrate_spawnDropContents(GameObject* obj, GameObject* player, LargeCrateState* state);
int LargeCrate_seq(GameObject* obj);
int LargeCrate_getExtraSize(void);
int LargeCrate_getObjectTypeId(void);
void LargeCrate_free(GameObject* obj);
void LargeCrate_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void LargeCrate_hitDetect(GameObject* obj);
void LargeCrate_update(GameObject* obj);
void LargeCrate_init(GameObject* obj, LargeCratePlacement* placement);
void LargeCrate_release(void);
void LargeCrate_initialise(void);

extern ObjectDescriptor gLargeCrateObjDescriptor;

#endif /* DLLS_OBJECTS_261_LARGECRATE_H_ */
