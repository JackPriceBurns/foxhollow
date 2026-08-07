#ifndef DLLS_OBJECTS_202_H_
#define DLLS_OBJECTS_202_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/baddie_state.h"

typedef int (*IceBaddieStateHandler)(GameObject* obj, GroundBaddieState* state);

void iceBaddie_installStateHandlers(void);

int iceBaddie_updateOpenHitState(GameObject* obj, GroundBaddieState* state);
int iceBaddie_updateOpenState(GameObject* obj, GroundBaddieState* state);
int iceBaddie_updateHideResetState(GameObject* obj, GroundBaddieState* state);
int iceBaddie_updateImpactHitState(GameObject* obj, GroundBaddieState* state);
int iceBaddie_updateSpinState(GameObject* obj, GroundBaddieState* state);
int iceBaddie_stateHandlerA05(GameObject* obj, GroundBaddieState* state);
int iceBaddie_stateHandlerA06(GameObject* obj, GroundBaddieState* state);
int iceBaddie_updateHeightBlendState(GameObject* obj, GroundBaddieState* state);
int iceBaddie_updateControlMove5State(GameObject* obj, GroundBaddieState* state);
int iceBaddie_updateCommDownState(GameObject* obj, GroundBaddieState* state);
int iceBaddie_updateDropState(GameObject* obj, GroundBaddieState* state);
int iceBaddie_stateHandlerA0B(GameObject* obj, GroundBaddieState* state);
int iceBaddie_updateContactHitState(GameObject* obj, GroundBaddieState* state);
int iceBaddie_updateLandingState(GameObject* obj, GroundBaddieState* state);

int iceBaddie_checkTargetState(GameObject* obj, GroundBaddieState* state);
int iceBaddie_stateHandlerB01(GameObject* obj, GroundBaddieState* state);
int iceBaddie_stateHandlerB02(GameObject* obj, GroundBaddieState* state);
int iceBaddie_stateHandlerB03(GameObject* obj, GroundBaddieState* state);
int iceBaddie_stateHandlerB04(GameObject* obj, GroundBaddieState* state);
int iceBaddie_stateHandlerB05(GameObject* obj, GroundBaddieState* state);
int iceBaddie_stateHandlerB06(GameObject* obj, GroundBaddieState* state);
int iceBaddie_stateHandlerB07(GameObject* obj, GroundBaddieState* state);

void iceBaddie_updateEffectAnchors(GameObject* obj, GroundBaddieState* state);
void iceBaddie_handleMessage(GameObject* obj, int message);
s16 iceBaddie_getControlMode(GameObject* obj);
int iceBaddie_getExtraSize(void);
int iceBaddie_getObjectTypeId(void);
void iceBaddie_free(GameObject* obj);
void iceBaddie_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void iceBaddie_hitDetect(GameObject* obj);
void iceBaddie_update(GameObject* obj, int unusedA, int unusedB);
void iceBaddie_init(GameObject* obj, GroundBaddiePlacement* placement, int flags);
void iceBaddie_release(void);
void iceBaddie_initialise(void);

extern IceBaddieStateHandler gIceBaddieStateHandlersB[];
extern IceBaddieStateHandler gIceBaddieStateHandlersA[];
extern u8 gIceBaddieA06MoveVariant;
extern u8 gIceBaddieA05MoveVariant;
extern s16 gIceBaddieAttackMoves[8];
extern s16 gIceBaddieAttackMovesAlt[8];
extern int gIceBaddieHitReactionMoves[30];
extern u8 gIceBaddieHitReactionDamage[32];
extern u8 gIceBaddieParticleArgsTable[16];
extern u8 gIceBaddiePaletteIndexTable[32];
extern ObjectDescriptor12 gIceBaddieObjDescriptor;

#endif /* DLLS_OBJECTS_202_H_ */
