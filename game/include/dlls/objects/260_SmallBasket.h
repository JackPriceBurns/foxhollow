#ifndef DLLS_OBJECTS_260_SMALLBASKET_H_
#define DLLS_OBJECTS_260_SMALLBASKET_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define SMALLBASKET_PLACEMENT_SIZE 0x24
#define SMALLBASKET_STATE_SIZE     0x24

#define SMALLBASKET_SEQUENCE_VARIANT_A      0x3CF
#define SMALLBASKET_SEQUENCE_DISGUISE_GATED 0x662

/* Retail SmallBasket placements have a fixed 0x0C-byte parameter tail. */
typedef struct SmallBasketPlacement {
    ObjPlacement base;  /* 0x00 */
    s8 rotXByte;        /* 0x18 */
    u8 subtype;         /* 0x19: SmallBasketSubtype */
    s16 unk1A;          /* 0x1A */
    s16 respawnMinutes; /* 0x1C */
    s16 enableGameBit;  /* 0x1E: starts hidden when set */
    s16 leashRange;     /* 0x20 */
    u8 pad22[2];        /* 0x22 */
} SmallBasketPlacement;

typedef enum SmallBasketSubtype {
    SMALLBASKET_SUBTYPE_GREEN_SCARAB = 1,
    SMALLBASKET_SUBTYPE_RED_SCARAB = 2,
    SMALLBASKET_SUBTYPE_GOLD_SCARAB = 3,
    SMALLBASKET_SUBTYPE_ENERGY_EGG = 5,
    SMALLBASKET_SUBTYPE_APPLE = 6,
    SMALLBASKET_SUBTYPE_RANDOM = 7,
} SmallBasketSubtype;

typedef enum SmallBasketCarryState {
    SMALLBASKET_CARRY_IDLE = 0,
    SMALLBASKET_CARRY_GRABBED = 1,
    SMALLBASKET_CARRY_HELD = 2,
} SmallBasketCarryState;

typedef enum SmallBasketThrowState {
    SMALLBASKET_THROW_NONE = 0,
    SMALLBASKET_THROW_LAUNCHED = 1,
    SMALLBASKET_THROW_DROPPED = 2,
} SmallBasketThrowState;

/* SmallBasket_getExtraSize allocates the complete 0x24-byte state block. */
typedef struct SmallBasketState {
    s16 carryAngle;      /* 0x00: low half of the player carry message */
    s16 carryParam;      /* 0x02: high half of the player carry message */
    u8 pad04;            /* 0x04 */
    s8 carryState;       /* 0x05: SmallBasketCarryState */
    s8 carryAttached;    /* 0x06 */
    u8 pad07[2];         /* 0x07 */
    s8 throwState;       /* 0x09: SmallBasketThrowState */
    s16 disableTimer;    /* 0x0A */
    s16 leashRange;      /* 0x0C */
    s16 ambientSfxTimer; /* 0x0E */
    s16 hitSfxId;        /* 0x10 */
    s16 flightTimer;     /* 0x12 */
    s32 hiddenTimer;     /* 0x14 */
    s32 respawnDelay;    /* 0x18 */
    s16 enableGameBit;   /* 0x1C */
    u8 subtype;          /* 0x1E */
    u8 unk1F;            /* 0x1F */
    u8 disguiseGated;    /* 0x20 */
    u8 pad21[3];         /* 0x21 */
} SmallBasketState;

STATIC_ASSERT(offsetof(SmallBasketPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(SmallBasketPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(SmallBasketPlacement, subtype) == 0x19);
STATIC_ASSERT(offsetof(SmallBasketPlacement, unk1A) == 0x1A);
STATIC_ASSERT(offsetof(SmallBasketPlacement, respawnMinutes) == 0x1C);
STATIC_ASSERT(offsetof(SmallBasketPlacement, enableGameBit) == 0x1E);
STATIC_ASSERT(offsetof(SmallBasketPlacement, leashRange) == 0x20);
STATIC_ASSERT(offsetof(SmallBasketPlacement, pad22) == 0x22);
STATIC_ASSERT(sizeof(SmallBasketPlacement) == SMALLBASKET_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(SmallBasketState, carryAngle) == 0x0);
STATIC_ASSERT(offsetof(SmallBasketState, carryParam) == 0x2);
STATIC_ASSERT(offsetof(SmallBasketState, pad04) == 0x4);
STATIC_ASSERT(offsetof(SmallBasketState, carryState) == 0x5);
STATIC_ASSERT(offsetof(SmallBasketState, carryAttached) == 0x6);
STATIC_ASSERT(offsetof(SmallBasketState, pad07) == 0x7);
STATIC_ASSERT(offsetof(SmallBasketState, throwState) == 0x9);
STATIC_ASSERT(offsetof(SmallBasketState, disableTimer) == 0xA);
STATIC_ASSERT(offsetof(SmallBasketState, leashRange) == 0xC);
STATIC_ASSERT(offsetof(SmallBasketState, ambientSfxTimer) == 0xE);
STATIC_ASSERT(offsetof(SmallBasketState, hitSfxId) == 0x10);
STATIC_ASSERT(offsetof(SmallBasketState, flightTimer) == 0x12);
STATIC_ASSERT(offsetof(SmallBasketState, hiddenTimer) == 0x14);
STATIC_ASSERT(offsetof(SmallBasketState, respawnDelay) == 0x18);
STATIC_ASSERT(offsetof(SmallBasketState, enableGameBit) == 0x1C);
STATIC_ASSERT(offsetof(SmallBasketState, subtype) == 0x1E);
STATIC_ASSERT(offsetof(SmallBasketState, unk1F) == 0x1F);
STATIC_ASSERT(offsetof(SmallBasketState, disguiseGated) == 0x20);
STATIC_ASSERT(offsetof(SmallBasketState, pad21) == 0x21);
STATIC_ASSERT(sizeof(SmallBasketState) == SMALLBASKET_STATE_SIZE);

void SmallBasket_handleHit(GameObject* obj, GameObject* player, SmallBasketState* state);
int SmallBasket_spawnContents(GameObject* obj, GameObject* player, SmallBasketState* state);
int SmallBasket_resolveCollision(GameObject* obj);
void SmallBasket_throw(GameObject* obj);
int SmallBasket_getExtraSize(void);
void SmallBasket_free(GameObject* obj);
void SmallBasket_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SmallBasket_update(GameObject* obj);
void SmallBasket_init(GameObject* obj, SmallBasketPlacement* placement);

extern ObjectDescriptor gSmallBasketObjDescriptor;

#endif /* DLLS_OBJECTS_260_SMALLBASKET_H_ */
