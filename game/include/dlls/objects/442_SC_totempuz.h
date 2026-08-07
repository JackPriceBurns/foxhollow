#ifndef DLLS_OBJECTS_442_SC_TOTEMPUZ_H_
#define DLLS_OBJECTS_442_SC_TOTEMPUZ_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

/* Retail placement records are 0x20 bytes; the puzzle index is byte 0x1B. */
typedef struct ScTotemPuzzlePlacement {
    ObjPlacement base;
    u8 unknown18[3];
    u8 puzzleIndex;
    u8 unknown1C[4];
} ScTotemPuzzlePlacement;

typedef struct ScTotemPuzzleState {
    f32 pulseTimer;
    f32 pulseTimerReset;
    f32 peerPhaseOffset;
    f32 angle;
    s16 stepIndex;
    s16 flags;
} ScTotemPuzzleState;

STATIC_ASSERT(offsetof(ScTotemPuzzlePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ScTotemPuzzlePlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(ScTotemPuzzlePlacement, puzzleIndex) == 0x1B);
STATIC_ASSERT(offsetof(ScTotemPuzzlePlacement, unknown1C) == 0x1C);
STATIC_ASSERT(sizeof(ScTotemPuzzlePlacement) == 0x20);

STATIC_ASSERT(offsetof(ScTotemPuzzleState, pulseTimer) == 0x00);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, pulseTimerReset) == 0x04);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, peerPhaseOffset) == 0x08);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, angle) == 0x0C);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, stepIndex) == 0x10);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, flags) == 0x12);
STATIC_ASSERT(sizeof(ScTotemPuzzleState) == 0x14);

int sc_totempuzzle_animEventCallback(GameObject* unusedObj, int unused, ObjSeqState* unusedAnimUpdate);
u8 sc_totempuzzle_checkSolvedSequence(GameObject* obj, ScTotemPuzzleState* state);
int sc_totempuzzle_getExtraSize(void);
int sc_totempuzzle_getObjectTypeId(void);
void sc_totempuzzle_free(void);
void sc_totempuzzle_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void sc_totempuzzle_hitDetect(void);
void sc_totempuzzle_update(GameObject* obj);
void sc_totempuzzle_init(GameObject* obj, const ScTotemPuzzlePlacement* placement);
void sc_totempuzzle_release(void);
void sc_totempuzzle_initialise(void);

extern s16 gTotemPuzzleStepAngles[6];
extern ObjectDescriptor gSC_totempuzzleObjDescriptor;

#endif /* DLLS_OBJECTS_442_SC_TOTEMPUZ_H_ */
