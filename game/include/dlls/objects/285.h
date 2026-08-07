#ifndef DLLS_OBJECTS_285_H_
#define DLLS_OBJECTS_285_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define TREASURE_CHEST_OBJECT_STATE_SIZE 0x1

typedef struct ObjSeqState ObjSeqState;

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct TreasureChestPlacement {
    ObjPlacement base;   /* 0x00 */
    s8 rotationX;        /* 0x18: high byte of the initial X rotation */
    u8 hitboxKind;       /* 0x19 */
    s16 triggerObjectId; /* 0x1A */
    s16 dialogueId;      /* 0x1C */
    s16 openedGameBit;   /* 0x1E */
} TreasureChestPlacement;

typedef struct TreasureChestObjectState {
    u8 opened : 1;           /* Set from the persistence bit or player activation. */
    u8 restoreOpenState : 1; /* Keeps a chest loaded as open at the end of its opening move. */
    u8 hitEffectEnabled : 1; /* Controlled by animation-sequence events. */
    u8 pad : 5;
} TreasureChestObjectState;

STATIC_ASSERT(offsetof(TreasureChestPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(TreasureChestPlacement, rotationX) == 0x18);
STATIC_ASSERT(offsetof(TreasureChestPlacement, hitboxKind) == 0x19);
STATIC_ASSERT(offsetof(TreasureChestPlacement, triggerObjectId) == 0x1A);
STATIC_ASSERT(offsetof(TreasureChestPlacement, dialogueId) == 0x1C);
STATIC_ASSERT(offsetof(TreasureChestPlacement, openedGameBit) == 0x1E);

STATIC_ASSERT(sizeof(TreasureChestObjectState) == TREASURE_CHEST_OBJECT_STATE_SIZE);

int TreasureChest_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int TreasureChest_getExtraSize(void);
int TreasureChest_getObjectTypeId(void);
void TreasureChest_free(void);
void TreasureChest_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void TreasureChest_hitDetect(GameObject* obj);
void TreasureChest_update(GameObject* obj);
void TreasureChest_init(GameObject* obj);
void TreasureChest_release(void);
void TreasureChest_initialise(void);

extern ObjectDescriptor gTreasureChestObjDescriptor;

#endif /* DLLS_OBJECTS_285_H_ */
