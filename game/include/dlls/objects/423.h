#ifndef DLLS_OBJECTS_423_H_
#define DLLS_OBJECTS_423_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/dll/curve_walker.h"

typedef struct EdibleMushroomPlacement {
    ObjPlacement base;
    u8 objectType;
    u8 lungeTriggerDistance;
    s16 gameBitId;
    u8 scaleParam;
    u8 unknown1D[2];
    u8 retreatTriggerDistance;
} EdibleMushroomPlacement;

typedef struct EdibleMushroomState {
    RomCurveWalker curve;
    f32 currentTargetDistance;
    f32 previousTargetDistance;
    f32 lungeRootSpeedScale;
    f32 mapParamScale;
    f32 lungeRange;
    f32 retreatRange;
    f32 curveAdvanceStep;
    f32 burrowAttackTimer;
    f32 sporePuffTimer;
    f32 tailSwingFxTimer;
    s16 moveAngle;
    u8 unknown132[2];
    s16 collectedGameBitId;
    u8 animState;
    u8 flags;
    u8 unknown138;
    u8 seqResetPending;
    u8 unknown13A[2];
    s16 pickupMsgBitId;
    s16 pickupMsgValue;
    f32 pickupMsgDelay;
} EdibleMushroomState;

STATIC_ASSERT(sizeof(EdibleMushroomPlacement) == 0x20);
STATIC_ASSERT(offsetof(EdibleMushroomPlacement, objectType) == 0x18);
STATIC_ASSERT(offsetof(EdibleMushroomPlacement, lungeTriggerDistance) == 0x19);
STATIC_ASSERT(offsetof(EdibleMushroomPlacement, gameBitId) == 0x1A);
STATIC_ASSERT(offsetof(EdibleMushroomPlacement, scaleParam) == 0x1C);
STATIC_ASSERT(offsetof(EdibleMushroomPlacement, retreatTriggerDistance) == 0x1F);

STATIC_ASSERT(sizeof(EdibleMushroomState) == 0x144);
STATIC_ASSERT(offsetof(EdibleMushroomState, curve) == 0x000);
STATIC_ASSERT(offsetof(EdibleMushroomState, currentTargetDistance) == 0x108);
STATIC_ASSERT(offsetof(EdibleMushroomState, previousTargetDistance) == 0x10C);
STATIC_ASSERT(offsetof(EdibleMushroomState, lungeRootSpeedScale) == 0x110);
STATIC_ASSERT(offsetof(EdibleMushroomState, mapParamScale) == 0x114);
STATIC_ASSERT(offsetof(EdibleMushroomState, lungeRange) == 0x118);
STATIC_ASSERT(offsetof(EdibleMushroomState, retreatRange) == 0x11C);
STATIC_ASSERT(offsetof(EdibleMushroomState, curveAdvanceStep) == 0x120);
STATIC_ASSERT(offsetof(EdibleMushroomState, burrowAttackTimer) == 0x124);
STATIC_ASSERT(offsetof(EdibleMushroomState, sporePuffTimer) == 0x128);
STATIC_ASSERT(offsetof(EdibleMushroomState, tailSwingFxTimer) == 0x12C);
STATIC_ASSERT(offsetof(EdibleMushroomState, moveAngle) == 0x130);
STATIC_ASSERT(offsetof(EdibleMushroomState, collectedGameBitId) == 0x134);
STATIC_ASSERT(offsetof(EdibleMushroomState, animState) == 0x136);
STATIC_ASSERT(offsetof(EdibleMushroomState, flags) == 0x137);
STATIC_ASSERT(offsetof(EdibleMushroomState, seqResetPending) == 0x139);
STATIC_ASSERT(offsetof(EdibleMushroomState, pickupMsgBitId) == 0x13C);
STATIC_ASSERT(offsetof(EdibleMushroomState, pickupMsgValue) == 0x13E);
STATIC_ASSERT(offsetof(EdibleMushroomState, pickupMsgDelay) == 0x140);

int EdibleMushroom_animEventCallback(GameObject* obj);
void EdibleMushroom_updateBehavior(GameObject* obj, EdibleMushroomState* state, EdibleMushroomPlacement* placement);
s16 EdibleMushroom_findClearApproachAngle(GameObject* obj, GameObject* player, EdibleMushroomState* state,
                                          f32 distance);
int EdibleMushroom_getExtraSize(void);
void EdibleMushroom_free(GameObject* obj);
void EdibleMushroom_hitDetect(GameObject* obj);
void EdibleMushroom_update(GameObject* obj);
void EdibleMushroom_init(GameObject* obj, EdibleMushroomPlacement* placement);

extern ObjectDescriptor gEdibleMushroomObjDescriptor;

#endif /* DLLS_OBJECTS_423_H_ */
