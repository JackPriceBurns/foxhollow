#ifndef DLLS_OBJECTS_203_H_
#define DLLS_OBJECTS_203_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/baddie_state.h"

struct ObjSeqState;

typedef struct DllCBPlacement {
    ObjPlacement base; /* 0x00: standard object placement */
    u8 pad18[0x24 - 0x18];
    s16 trackYieldId; /* 0x24: sequence yield after releasing the tracked target */
    u8 pad26;
    s8 rotZ; /* 0x27: initial Z rotation */
    s8 rotY; /* 0x28: initial Y rotation */
    u8 pad29[0x2B - 0x29];
    u8 flags;            /* 0x2B: ground-baddie initialization flags */
    s16 gameBitId;       /* 0x2C: sequence yield when gameBitC is set */
    s8 trackYieldEnable; /* 0x2E: -1 disables trackYieldId */
    u8 pad2F;
} DllCBPlacement;

typedef int (*DllCBStateHandler)(GameObject* obj, GroundBaddieState* state);
typedef int (*DllCBMoveHandler)(GameObject* obj, GroundBaddieState* state, f32 timeDelta);

STATIC_ASSERT(offsetof(DllCBPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(DllCBPlacement, trackYieldId) == 0x24);
STATIC_ASSERT(offsetof(DllCBPlacement, rotZ) == 0x27);
STATIC_ASSERT(offsetof(DllCBPlacement, rotY) == 0x28);
STATIC_ASSERT(offsetof(DllCBPlacement, flags) == 0x2B);
STATIC_ASSERT(offsetof(DllCBPlacement, gameBitId) == 0x2C);
STATIC_ASSERT(offsetof(DllCBPlacement, trackYieldEnable) == 0x2E);
STATIC_ASSERT(sizeof(DllCBPlacement) == 0x30);

int dll_CB_stateHandler5(GameObject* obj, GroundBaddieState* state);
int dll_CB_stateHandler4(GameObject* obj, GroundBaddieState* state);
int dll_CB_stateHandler3(GameObject* obj, GroundBaddieState* state);
int dll_CB_stateHandler2(GameObject* obj, GroundBaddieState* state);
int dll_CB_stateHandler1(GameObject* obj, GroundBaddieState* state);
int dll_CB_stateHandler0(GameObject* obj, GroundBaddieState* state);
int dll_CB_moveHandler3(GameObject* obj, GroundBaddieState* state, f32 timeDelta);
int dll_CB_moveHandler2(GameObject* obj, GroundBaddieState* state, f32 timeDelta);
int dll_CB_moveHandler1(GameObject* obj, GroundBaddieState* state, f32 timeDelta);
int dll_CB_moveHandler0(GameObject* obj, GroundBaddieState* state, f32 timeDelta);
void dll_CB_seekAndUpdate(GameObject* obj, struct ObjSeqState* sequenceState, GroundBaddieState* objectState,
                          GroundBaddieState* state);
void dll_CB_advanceAI(GameObject* obj, GroundBaddieState* objectState, GroundBaddieState* state);
int dll_CB_seqFn(GameObject* obj, int unused, struct ObjSeqState* sequenceState);

void dll_CB_initialise(void);
void dll_CB_release(void);
void dll_CB_init(GameObject* obj, DllCBPlacement* placement, int flags);
void dll_CB_update(GameObject* obj);
void dll_CB_hitDetect(GameObject* obj);
void dll_CB_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void dll_CB_free(GameObject* obj);
int dll_CB_getObjectTypeId(void);
int dll_CB_getExtraSize(void);
s16 dll_CB_getControlMode(GameObject* obj);
void dll_CB_handleMessage(GameObject* obj, int message);

extern DllCBMoveHandler gDllCBMoveHandlers[4];
extern DllCBStateHandler gDllCBStateHandlers[6];
extern const f32 gDllCBDefaultAnimSpeed[1];
extern int gDllCBHitReactionMoves[30];
extern u8 gDllCBHitReactionDamage[32];
extern ObjectDescriptor12 gDllCBObjDescriptor;

#endif /* DLLS_OBJECTS_203_H_ */
