#ifndef DLLS_OBJECTS_273_H_
#define DLLS_OBJECTS_273_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define DOOR_LOCK_STATE_SIZE 0x1

typedef struct ObjSeqState ObjSeqState;

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct DoorLockPlacement {
    ObjPlacement base;    /* 0x00 */
    u8 rotXByte;          /* 0x18: X rotation in 1/256 turns */
    u8 rotYByte;          /* 0x19: Y rotation in 1/256 turns */
    u8 rotZByte;          /* 0x1A: Z rotation in 1/256 turns */
    u8 flags;             /* 0x1B: DOOR_LOCK_FLAG_* */
    s16 unlockedGameBit;  /* 0x1C */
    s16 triggerGameBit;   /* 0x1E */
    s8 unlockSequenceId;  /* 0x20: or -1 */
    u8 modelBankIndex;    /* 0x21 */
    s16 requiredGameBit;  /* 0x22 */
    s16 queuedSequenceId; /* 0x24: zero means none */
    s16 modeFlags;        /* 0x26: DOOR_LOCK_MODE_* */
} DoorLockPlacement;

typedef struct DoorLockState {
    u8 unlocked;
} DoorLockState;

STATIC_ASSERT(offsetof(DoorLockPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(DoorLockPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(DoorLockPlacement, rotYByte) == 0x19);
STATIC_ASSERT(offsetof(DoorLockPlacement, rotZByte) == 0x1A);
STATIC_ASSERT(offsetof(DoorLockPlacement, flags) == 0x1B);
STATIC_ASSERT(offsetof(DoorLockPlacement, unlockedGameBit) == 0x1C);
STATIC_ASSERT(offsetof(DoorLockPlacement, triggerGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DoorLockPlacement, unlockSequenceId) == 0x20);
STATIC_ASSERT(offsetof(DoorLockPlacement, modelBankIndex) == 0x21);
STATIC_ASSERT(offsetof(DoorLockPlacement, requiredGameBit) == 0x22);
STATIC_ASSERT(offsetof(DoorLockPlacement, queuedSequenceId) == 0x24);
STATIC_ASSERT(offsetof(DoorLockPlacement, modeFlags) == 0x26);

STATIC_ASSERT(offsetof(DoorLockState, unlocked) == 0x0);
STATIC_ASSERT(sizeof(DoorLockState) == DOOR_LOCK_STATE_SIZE);

int DoorLock_getExtraSize(void);
void DoorLock_free(GameObject* obj);
void DoorLock_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void DoorLock_update(GameObject* obj);
void DoorLock_init(GameObject* obj, DoorLockPlacement* placement);

extern ObjectDescriptor gDoorLockObjDescriptor;

#endif /* DLLS_OBJECTS_273_H_ */
