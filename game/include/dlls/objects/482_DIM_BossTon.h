#ifndef DLLS_OBJECTS_482_DIM_BOSSTON_H_
#define DLLS_OBJECTS_482_DIM_BOSSTON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/baddie_state.h"
#include "main/model_light.h"
#include "main/objseq.h"

#define DIMBOSSTONSIL_HIT_GAMEBIT 0x20C

/*
 * Only the fields consumed by this DLL are modeled. The active-target retail
 * placement files are unavailable, so this does not claim a complete record
 * width.
 */
typedef struct DIMbosstonsilPlacementView {
    ObjPlacement base;
    u8 unknown18[0x14];
    s16 eventId;
    s8 animObjectId;
} DIMbosstonsilPlacementView;

typedef int (*DIMbosstonsilHitReactionCallback)(GameObject* obj, GroundBaddieState* state);
typedef int (*DIMbosstonsilUpdateHitReactionCallback)(GameObject* obj, GroundBaddieState* state, int unused);

typedef struct DIMbosstonsilStateHandlerTable {
    DIMbosstonsilHitReactionCallback startIdle;
    DIMbosstonsilHitReactionCallback choose;
} DIMbosstonsilStateHandlerTable;

typedef struct DIMbosstonsilSubstateHandlerTable {
    DIMbosstonsilHitReactionCallback enable;
    DIMbosstonsilUpdateHitReactionCallback update;
} DIMbosstonsilSubstateHandlerTable;

STATIC_ASSERT(offsetof(DIMbosstonsilPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(DIMbosstonsilPlacementView, eventId) == 0x2C);
STATIC_ASSERT(offsetof(DIMbosstonsilPlacementView, animObjectId) == 0x2E);

STATIC_ASSERT(sizeof(DIMbosstonsilStateHandlerTable) == 0x08);
STATIC_ASSERT(sizeof(DIMbosstonsilSubstateHandlerTable) == 0x08);

int DIMbosstonsil_updateHitReaction(GameObject* obj, GroundBaddieState* state, int unused);
int DIMbosstonsil_enableHitReaction(GameObject* obj, GroundBaddieState* state);
int DIMbosstonsil_chooseHitReaction(GameObject* obj, GroundBaddieState* state);
int DIMbosstonsil_startIdleHitReaction(GameObject* obj, GroundBaddieState* state);
void DIMbosstonsil_checkHit(GameObject* obj, GroundBaddieState* state);
void dimBossTonsil_newState_hitFightMain(GameObject* obj, ObjSeqState* animUpdate, GroundBaddieState* state,
                                         GroundBaddieState* updateState);
int DIMbosstonsil_SeqFn(GameObject* obj, u32 unused, ObjSeqState* animUpdate);
void DIMbosstonsil_func0B(void);
int DIMbosstonsil_getControlMode(GameObject* obj);
int DIMbosstonsil_getExtraSize(void);
int DIMbosstonsil_getObjectTypeId(void);
void DIMbosstonsil_free(GameObject* obj);
void DIMbosstonsil_render(GameObject* obj, u32 renderArg2, u32 renderArg3, u32 renderArg4, u32 renderArg5,
                          char visible);
void DIMbosstonsil_hitDetect(GameObject* obj);
void DIMbosstonsil_update(GameObject* obj);
void DIMbosstonsil_init(GameObject* obj, u8* placementAddress, int isAltVariant);
void DIMbosstonsil_release(void);
void DIMbosstonsil_initialise(void);

extern DIMbosstonsilStateHandlerTable gDIMbosstonsilStateHandlers;
extern DIMbosstonsilSubstateHandlerTable gDIMbosstonsilSubstateHandlers;
extern ModelLightStruct* gDIMbosstonsilLight;
extern s8 gDIMbosstonsilRoutePhase;
extern ObjectDescriptor12 gDIM_BossTonsilObjDescriptor;

#endif /* DLLS_OBJECTS_482_DIM_BOSSTON_H_ */
