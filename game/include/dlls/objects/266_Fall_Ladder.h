#ifndef DLLS_OBJECTS_266_FALL_LADDER_H_
#define DLLS_OBJECTS_266_FALL_LADDER_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

#define FALL_LADDER_PLACEMENT_SIZE 0x24 /* 0x18-byte base plus fixed 0xC-byte retail parameter tail */
#define FALL_LADDER_STATE_SIZE     0xC

typedef enum FallLadderMotionState {
    FALL_LADDER_MOTION_WAITING = 0,
    FALL_LADDER_MOTION_FALLING = 1,
    FALL_LADDER_MOTION_SETTLED = 2,
} FallLadderMotionState;

typedef struct FallLadderPlacement {
    ObjPlacement base;       /* 0x00 */
    s8 rotXByte;             /* 0x18: X rotation in 1/256 turns */
    s8 modelIndex;           /* 0x19 */
    s16 initialHeightOffset; /* 0x1A: initial height above the resting Y position */
    s16 unk1C;               /* 0x1C */
    s16 baseGameBit;         /* 0x1E: selects trigger sequence 1 */
    s16 triggerGameBit;      /* 0x20: starts the fall and selects trigger sequence 0 */
    u8 pad22[2];             /* 0x22 */
} FallLadderPlacement;

typedef struct FallLadderState {
    f32 initialHeightOffset; /* 0x00 */
    s16 baseGameBit;         /* 0x04 */
    s16 triggerGameBit;      /* 0x06 */
    s8 motionState;          /* 0x08: FallLadderMotionState */
    u8 playFallSound;        /* 0x09 */
    s16 fallDelay;           /* 0x0A */
} FallLadderState;

STATIC_ASSERT(offsetof(FallLadderPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(FallLadderPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(FallLadderPlacement, modelIndex) == 0x19);
STATIC_ASSERT(offsetof(FallLadderPlacement, initialHeightOffset) == 0x1A);
STATIC_ASSERT(offsetof(FallLadderPlacement, unk1C) == 0x1C);
STATIC_ASSERT(offsetof(FallLadderPlacement, baseGameBit) == 0x1E);
STATIC_ASSERT(offsetof(FallLadderPlacement, triggerGameBit) == 0x20);
STATIC_ASSERT(offsetof(FallLadderPlacement, pad22) == 0x22);
STATIC_ASSERT(sizeof(FallLadderPlacement) == FALL_LADDER_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(FallLadderState, initialHeightOffset) == 0x0);
STATIC_ASSERT(offsetof(FallLadderState, baseGameBit) == 0x4);
STATIC_ASSERT(offsetof(FallLadderState, triggerGameBit) == 0x6);
STATIC_ASSERT(offsetof(FallLadderState, motionState) == 0x8);
STATIC_ASSERT(offsetof(FallLadderState, playFallSound) == 0x9);
STATIC_ASSERT(offsetof(FallLadderState, fallDelay) == 0xA);
STATIC_ASSERT(sizeof(FallLadderState) == FALL_LADDER_STATE_SIZE);

int Fall_Ladders_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int Fall_Ladders_getExtraSize(void);
int Fall_Ladders_getObjectTypeId(void);
void Fall_Ladders_free(GameObject* obj);
void Fall_Ladders_render(void);
void Fall_Ladders_hitDetect(void);
void Fall_Ladders_update(GameObject* obj);
void Fall_Ladders_init(GameObject* obj, FallLadderPlacement* placement);
void Fall_Ladders_release(void);
void Fall_Ladders_initialise(void);

extern ObjectDescriptor gFall_LaddersObjDescriptor;

#endif /* DLLS_OBJECTS_266_FALL_LADDER_H_ */
