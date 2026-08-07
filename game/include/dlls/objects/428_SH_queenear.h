#ifndef DLLS_OBJECTS_428_SH_QUEENEAR_H_
#define DLLS_OBJECTS_428_SH_QUEENEAR_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define QUEEN_EARTH_WALKER_EVENT_TABLE_SIZE          4
#define QUEEN_EARTH_WALKER_COMPLETE_EVENT_TABLE_SIZE 8
#define QUEEN_EARTH_WALKER_MOVE_COUNT                6
#define QUEEN_EARTH_WALKER_MOVE_SPEED_COUNT          5

typedef struct ObjSeqState ObjSeqState;

typedef struct QueenEarthWalkerPlacement {
    ObjPlacement base;
    s8 yawByte;
} QueenEarthWalkerPlacement;

typedef struct QueenEarthWalkerLookState {
    u8 enabled;
    u8 unknown01[3];
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    u8 unknown10[0x30 - 0x10];
} QueenEarthWalkerLookState;

typedef struct QueenEarthWalkerState {
    u8 stateIndex;
    u8 unknown01;
    u8 flags;
    u8 unknown03[5];
    QueenEarthWalkerLookState look;
    u8* eventTable;
    f32 attackTimer;
} QueenEarthWalkerState;

STATIC_ASSERT(offsetof(QueenEarthWalkerPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(QueenEarthWalkerPlacement, yawByte) == 0x18);

STATIC_ASSERT(sizeof(QueenEarthWalkerLookState) == 0x30);
STATIC_ASSERT(offsetof(QueenEarthWalkerLookState, enabled) == 0x00);
STATIC_ASSERT(offsetof(QueenEarthWalkerLookState, unknown01) == 0x01);
STATIC_ASSERT(offsetof(QueenEarthWalkerLookState, targetX) == 0x04);
STATIC_ASSERT(offsetof(QueenEarthWalkerLookState, targetY) == 0x08);
STATIC_ASSERT(offsetof(QueenEarthWalkerLookState, targetZ) == 0x0C);
STATIC_ASSERT(offsetof(QueenEarthWalkerLookState, unknown10) == 0x10);

STATIC_ASSERT(sizeof(QueenEarthWalkerState) == 0x40);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, stateIndex) == 0x00);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, unknown01) == 0x01);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, flags) == 0x02);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, unknown03) == 0x03);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, look) == 0x08);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, eventTable) == 0x38);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, attackTimer) == 0x3C);

extern u8 gQueenEarthWalkerEventTableAct1[QUEEN_EARTH_WALKER_EVENT_TABLE_SIZE];
extern u8 gQueenEarthWalkerEventTableAct2[QUEEN_EARTH_WALKER_EVENT_TABLE_SIZE];
extern u8 gQueenEarthWalkerEventTableFed[QUEEN_EARTH_WALKER_EVENT_TABLE_SIZE];
extern u8 gQueenEarthWalkerEventTableFeed[QUEEN_EARTH_WALKER_EVENT_TABLE_SIZE];
extern u8 gQueenEarthWalkerEventTablePortalDefault[QUEEN_EARTH_WALKER_EVENT_TABLE_SIZE];
extern u8 gQueenEarthWalkerEventTablePortalReady[QUEEN_EARTH_WALKER_EVENT_TABLE_SIZE];
extern u8 gQueenEarthWalkerEventTableSpell[QUEEN_EARTH_WALKER_EVENT_TABLE_SIZE];
extern u8 gQueenEarthWalkerEventTableBerry[QUEEN_EARTH_WALKER_EVENT_TABLE_SIZE];
extern u8 gQueenEarthWalkerEventTableDeparture[QUEEN_EARTH_WALKER_EVENT_TABLE_SIZE];
extern u8 gQueenEarthWalkerEventTableComplete[QUEEN_EARTH_WALKER_COMPLETE_EVENT_TABLE_SIZE];

extern s16 gQueenEarthWalkerMoveTable[QUEEN_EARTH_WALKER_MOVE_COUNT];
extern f32 gQueenEarthWalkerMoveSpeedTable[QUEEN_EARTH_WALKER_MOVE_SPEED_COUNT];
extern ObjectDescriptor gSH_queenearthwalkerObjDescriptor;

int sh_queenearthwalker_processAnimEvents(GameObject* obj, int unusedArg, ObjSeqState* animUpdate);
void sh_queenearthwalker_updatePortal(GameObject* obj, QueenEarthWalkerState* state);
void sh_queenearthwalker_updateFeeding(GameObject* obj, QueenEarthWalkerState* state);
int sh_queenearthwalker_getExtraSize(void);
void sh_queenearthwalker_update(GameObject* obj);
void sh_queenearthwalker_init(GameObject* obj, QueenEarthWalkerPlacement* placement);

#endif /* DLLS_OBJECTS_428_SH_QUEENEAR_H_ */
