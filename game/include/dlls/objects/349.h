#ifndef DLLS_OBJECTS_349_H_
#define DLLS_OBJECTS_349_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef enum SlidingDoorMode {
    SLIDING_DOOR_MODE_CLOSED = 0,
    SLIDING_DOOR_MODE_OPEN = 1,
    SLIDING_DOOR_MODE_OPENING = 2,
    SLIDING_DOOR_MODE_CLOSING = 3,
} SlidingDoorMode;

typedef struct SlidingDoorPlacement {
    ObjPlacement base;
    s16 openGameBit;
    s16 openedGameBit;
    s16 preemptTriggerId;
    s8 startupSequenceId;
    u8 rotXByte;
    u8 pad20;
    u8 scaleByte;
    s16 gateGameBit;
} SlidingDoorPlacement;

typedef struct SlidingDoorState {
    u8 mode : 3;
    u8 unused : 5;
} SlidingDoorState;

STATIC_ASSERT(offsetof(SlidingDoorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(SlidingDoorPlacement, openGameBit) == 0x18);
STATIC_ASSERT(offsetof(SlidingDoorPlacement, openedGameBit) == 0x1A);
STATIC_ASSERT(offsetof(SlidingDoorPlacement, preemptTriggerId) == 0x1C);
STATIC_ASSERT(offsetof(SlidingDoorPlacement, startupSequenceId) == 0x1E);
STATIC_ASSERT(offsetof(SlidingDoorPlacement, rotXByte) == 0x1F);
STATIC_ASSERT(offsetof(SlidingDoorPlacement, pad20) == 0x20);
STATIC_ASSERT(offsetof(SlidingDoorPlacement, scaleByte) == 0x21);
STATIC_ASSERT(offsetof(SlidingDoorPlacement, gateGameBit) == 0x22);

STATIC_ASSERT(sizeof(SlidingDoorState) == 0x01);

int slidingDoor_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int slidingDoor_getExtraSize(void);
int slidingDoor_getObjectTypeId(void);
void slidingDoor_free(void);
void slidingDoor_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void slidingDoor_hitDetect(void);
void slidingDoor_update(GameObject* obj);
void slidingDoor_init(GameObject* obj, SlidingDoorPlacement* placement);
void slidingDoor_release(void);
void slidingDoor_initialise(void);

extern ObjectDescriptor gSlidingDoorObjDescriptor;

#endif /* DLLS_OBJECTS_349_H_ */
