#ifndef DLLS_OBJECTS_272_H_
#define DLLS_OBJECTS_272_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define DOOR_PLACEMENT_SIZE 0x24
#define DOOR_STATE_SIZE     0x8

typedef struct ObjSeqState ObjSeqState;

/* Retail slot 272 placements share this fixed 0xC-byte parameter tail. */
typedef struct DoorPlacement {
    ObjPlacement base;       /* 0x00 */
    s16 closeRequestGameBit; /* 0x18: nonzero requests that the door close */
    s16 closedLatchGameBit;  /* 0x1A: set after closing and cleared after opening */
    s16 triggerSequenceId;   /* 0x1C: preempted when the door initializes closed */
    s8 runSequenceId;        /* 0x1E: sequence run once during update, or -1 */
    u8 rotXByte;             /* 0x1F: X rotation in 1/256 turns */
    u8 triggerArg;           /* 0x20: low seven bits passed to runSequence after preemption */
    u8 rootMotionScaleInput; /* 0x21: scale in 1/64 units */
    s16 closeReadyGameBit;   /* 0x22: closure waits for this bit, or -1 */
} DoorPlacement;

typedef struct DoorState {
    u16 movementSfx; /* 0x00: looping SFX played while opening or closing */
    u16 endpointSfx; /* 0x02: SFX played when fully opened or closed */
    u8 phase;        /* 0x04: DOOR_PHASE_* */
    u8 initPending;  /* 0x05: Door_update one-shot trigger flag */
    u8 closeFlags;   /* 0x06: DOOR_CLOSE_FLAG_* */
    u8 pad07;        /* 0x07 */
} DoorState;

STATIC_ASSERT(offsetof(DoorPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(DoorPlacement, closeRequestGameBit) == 0x18);
STATIC_ASSERT(offsetof(DoorPlacement, closedLatchGameBit) == 0x1A);
STATIC_ASSERT(offsetof(DoorPlacement, triggerSequenceId) == 0x1C);
STATIC_ASSERT(offsetof(DoorPlacement, runSequenceId) == 0x1E);
STATIC_ASSERT(offsetof(DoorPlacement, rotXByte) == 0x1F);
STATIC_ASSERT(offsetof(DoorPlacement, triggerArg) == 0x20);
STATIC_ASSERT(offsetof(DoorPlacement, rootMotionScaleInput) == 0x21);
STATIC_ASSERT(offsetof(DoorPlacement, closeReadyGameBit) == 0x22);
STATIC_ASSERT(sizeof(DoorPlacement) == DOOR_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(DoorState, movementSfx) == 0x0);
STATIC_ASSERT(offsetof(DoorState, endpointSfx) == 0x2);
STATIC_ASSERT(offsetof(DoorState, phase) == 0x4);
STATIC_ASSERT(offsetof(DoorState, initPending) == 0x5);
STATIC_ASSERT(offsetof(DoorState, closeFlags) == 0x6);
STATIC_ASSERT(offsetof(DoorState, pad07) == 0x7);
STATIC_ASSERT(sizeof(DoorState) == DOOR_STATE_SIZE);

int Door_animEventCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int Door_getExtraSize(void);
void Door_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void Door_update(GameObject* obj);
void Door_init(GameObject* obj, DoorPlacement* placement);

extern ObjectDescriptor gDoorObjDescriptor;

#endif /* DLLS_OBJECTS_272_H_ */
