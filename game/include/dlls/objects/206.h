#ifndef DLLS_OBJECTS_206_H_
#define DLLS_OBJECTS_206_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/baddie_state.h"
#include "main/dll/partfx_interface.h"

typedef struct DllCEPlacement {
    ObjPlacement base;     /* 0x00 */
    u8 pad18[0x2B - 0x18]; /* 0x18 */
    u8 flags;              /* 0x2B: ground-baddie initialization flags */
    u8 pad2C[0x2E - 0x2C]; /* 0x2C */
    s8 sequenceId;         /* 0x2E: object sequence run on first update */
    u8 pad2F;              /* 0x2F */
} DllCEPlacement;

typedef struct DllCEControl {
    f32 soundTimer;       /* 0x0 */
    f32 nextSoundTime;    /* 0x4 */
    u8 effectFlags;       /* 0x8 */
    u8 coordinationFlags; /* 0x9 */
    u8 padA[2];           /* 0xA */
} DllCEControl;

typedef int (*DllCEStateHandler)(GameObject* obj, GroundBaddieState* state);

STATIC_ASSERT(offsetof(DllCEPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(DllCEPlacement, flags) == 0x2B);
STATIC_ASSERT(offsetof(DllCEPlacement, sequenceId) == 0x2E);
STATIC_ASSERT(sizeof(DllCEPlacement) == 0x30);
STATIC_ASSERT(offsetof(DllCEControl, soundTimer) == 0x0);
STATIC_ASSERT(offsetof(DllCEControl, nextSoundTime) == 0x4);
STATIC_ASSERT(offsetof(DllCEControl, effectFlags) == 0x8);
STATIC_ASSERT(offsetof(DllCEControl, coordinationFlags) == 0x9);
STATIC_ASSERT(sizeof(DllCEControl) == 0xC);

int dll_CE_checkChooseAttackState(GameObject* obj, GroundBaddieState* state);
int dll_CE_checkSubmergeState(GameObject* obj, GroundBaddieState* state);
int dll_CE_checkYieldState(GameObject* obj, GroundBaddieState* state);
int dll_CE_checkDeathState(GameObject* obj, GroundBaddieState* state);
int dll_CE_checkHealthState(GameObject* obj, GroundBaddieState* state);
int dll_CE_checkTargetState(GameObject* obj, GroundBaddieState* state);
int dll_CE_updateWindupState(GameObject* obj, GroundBaddieState* state);
int dll_CE_updateAlertState(GameObject* obj, GroundBaddieState* state);
int dll_CE_updateSpitState(GameObject* obj, GroundBaddieState* state);
int dll_CE_updateState3(GameObject* obj, GroundBaddieState* state);
int dll_CE_updateAttackState(GameObject* obj, GroundBaddieState* state);
int dll_CE_updateSubmergeState(GameObject* obj, GroundBaddieState* state);
int dll_CE_updateEmergeState(GameObject* obj, GroundBaddieState* state);
void dll_CE_spawnIceBall(GameObject* obj, GroundBaddieState* state);
void dll_CE_acquireTarget(GameObject* obj, GroundBaddieState* objectState, GroundBaddieState* state);
void dll_CE_updateTargeting(GameObject* obj, GroundBaddieState* objectStateAddress, GroundBaddieState* stateAddress);

void dll_CE_handleMessage(GameObject* obj, int message);
s16 dll_CE_getControlMode(GameObject* obj);
int dll_CE_getExtraSize(void);
int dll_CE_getObjectTypeId(void);
void dll_CE_free(GameObject* obj);
void dll_CE_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void dll_CE_hitDetect(GameObject* obj);
void dll_CE_update(GameObject* obj, int unusedA, int unusedB);
void dll_CE_init(GameObject* obj, DllCEPlacement* placement, int flags);
void dll_CE_release(void);
void dll_CE_initialise(void);

extern DllCEStateHandler gDllCEMoveHandlers[8];
extern DllCEStateHandler gDllCECheckHandlers[6];
extern int gDllCEHitReactionMoves[30];
extern u8 gDllCEHitReactionDamage[32];
extern PartFxSpawnParams gDllCEHitReactionScratch;
extern ObjectDescriptor12 gDllCEObjDescriptor;

#endif /* DLLS_OBJECTS_206_H_ */
