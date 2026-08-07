#ifndef DLLS_OBJECTS_277_H_
#define DLLS_OBJECTS_277_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define DLL_115_STATE_SIZE 0x2
#define DLL_115_STEP_COUNT 8

typedef struct ObjSeqState ObjSeqState;

typedef enum Dll115Step {
    DLL_115_STEP_IDLE = DLL_115_STEP_COUNT,
    DLL_115_STEP_FINISH,
    DLL_115_STEP_DONE,
} Dll115Step;

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct Dll115Placement {
    ObjPlacement base;                          /* 0x00 */
    s16 completionGameBits[DLL_115_STEP_COUNT]; /* 0x18: set when each step completes, or -1 */
    s16 activeGameBits[DLL_115_STEP_COUNT];     /* 0x28: gates each step, or -1 */
    u8 initialYaw;                              /* 0x38: rotation in 1/256 turns */
    u8 flags;                                   /* 0x39 */
    u8 finishSequenceId;                        /* 0x3A */
    u8 finishSequenceParam;                     /* 0x3B */
    s16 finishPreemptId;                        /* 0x3C */
    u8 pad3E[2];                                /* 0x3E */
    s8 sequenceIds[DLL_115_STEP_COUNT];         /* 0x40: or -1 */
} Dll115Placement;

typedef struct Dll115State {
    u8 step;  /* 0x00: Dll115Step */
    u8 flags; /* 0x01 */
} Dll115State;

STATIC_ASSERT(offsetof(Dll115Placement, base) == 0x0);
STATIC_ASSERT(offsetof(Dll115Placement, completionGameBits) == 0x18);
STATIC_ASSERT(offsetof(Dll115Placement, activeGameBits) == 0x28);
STATIC_ASSERT(offsetof(Dll115Placement, initialYaw) == 0x38);
STATIC_ASSERT(offsetof(Dll115Placement, flags) == 0x39);
STATIC_ASSERT(offsetof(Dll115Placement, finishSequenceId) == 0x3A);
STATIC_ASSERT(offsetof(Dll115Placement, finishSequenceParam) == 0x3B);
STATIC_ASSERT(offsetof(Dll115Placement, finishPreemptId) == 0x3C);
STATIC_ASSERT(offsetof(Dll115Placement, pad3E) == 0x3E);
STATIC_ASSERT(offsetof(Dll115Placement, sequenceIds) == 0x40);

STATIC_ASSERT(offsetof(Dll115State, step) == 0x0);
STATIC_ASSERT(offsetof(Dll115State, flags) == 0x1);
STATIC_ASSERT(sizeof(Dll115State) == DLL_115_STATE_SIZE);

int dll_115_animEventCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int dll_115_getExtraSize(void);
int dll_115_getObjectTypeId(void);
void dll_115_free(GameObject* obj);
void dll_115_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_115_hitDetect(void);
void dll_115_update(GameObject* obj);
void dll_115_init(GameObject* obj, Dll115Placement* placement);
void dll_115_release(void);
void dll_115_initialise(void);

extern ObjectDescriptor gDll115ObjDescriptor;

#endif /* DLLS_OBJECTS_277_H_ */
