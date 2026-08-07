#ifndef DLLS_OBJECTS_334_CFPRISONGUA_H_
#define DLLS_OBJECTS_334_CFPRISONGUA_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objprint_character_api.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef enum CfPrisonGuardStateId {
    CFPRISONGUARD_STATE_IDLE = 0,
    CFPRISONGUARD_STATE_WATCHING = 1,
    CFPRISONGUARD_STATE_WATCH_DELAY = 2,
    CFPRISONGUARD_STATE_IDLE_DELAY = 3,
    CFPRISONGUARD_STATE_ALERT = 4,
    CFPRISONGUARD_STATE_PLAYER_CAUGHT = 5,
    CFPRISONGUARD_STATE_SEQUENCE_COMPLETE = 6,
    CFPRISONGUARD_STATE_HIT = 7,
} CfPrisonGuardStateId;

typedef enum CfPrisonGuardSequenceFlag {
    CFPRISONGUARD_SEQUENCE_FLAG_INITIAL_PENDING = 0x01,
    CFPRISONGUARD_SEQUENCE_FLAG_WAITING_FOR_PERCH = 0x02,
    CFPRISONGUARD_SEQUENCE_FLAG_PERCH_ACTIVE = 0x04,
} CfPrisonGuardSequenceFlag;

typedef struct CfPrisonGuardPlacement {
    ObjPlacement base;
    s8 initialYaw;
    u8 pad19;
    s16 watchRadius;
    s16 unknown1C;
    s16 disableGameBit;
    u8 pad20[0x02];
    s16 unknown22;
    u8 pad24[0x04];
} CfPrisonGuardPlacement;

typedef struct CfPrisonGuardStatusFlags {
    u8 initialUpdate : 1;
    u8 unused : 7;
} CfPrisonGuardStatusFlags;

typedef struct CfPrisonGuardState {
    CharacterEyeAnimState eyeAnimState;
    u8 pad28[0x8];
    f32 alarmRamp;
    s16 stateTimer;
    s8 uncleFlewOffLatch;
    s8 stateId;
    u8 sequenceFlags;
    CfPrisonGuardStatusFlags statusFlags;
    u8 pad3A[0x02];
} CfPrisonGuardState;

STATIC_ASSERT(offsetof(CfPrisonGuardPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CfPrisonGuardPlacement, initialYaw) == 0x18);
STATIC_ASSERT(offsetof(CfPrisonGuardPlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(CfPrisonGuardPlacement, watchRadius) == 0x1A);
STATIC_ASSERT(offsetof(CfPrisonGuardPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(CfPrisonGuardPlacement, disableGameBit) == 0x1E);
STATIC_ASSERT(offsetof(CfPrisonGuardPlacement, pad20) == 0x20);
STATIC_ASSERT(offsetof(CfPrisonGuardPlacement, unknown22) == 0x22);
STATIC_ASSERT(offsetof(CfPrisonGuardPlacement, pad24) == 0x24);
STATIC_ASSERT(sizeof(CfPrisonGuardPlacement) == 0x28);

STATIC_ASSERT(sizeof(CfPrisonGuardStatusFlags) == 0x01);

STATIC_ASSERT(offsetof(CfPrisonGuardState, eyeAnimState) == 0x00);
STATIC_ASSERT(offsetof(CfPrisonGuardState, alarmRamp) == 0x30);
STATIC_ASSERT(offsetof(CfPrisonGuardState, stateTimer) == 0x34);
STATIC_ASSERT(offsetof(CfPrisonGuardState, uncleFlewOffLatch) == 0x36);
STATIC_ASSERT(offsetof(CfPrisonGuardState, stateId) == 0x37);
STATIC_ASSERT(offsetof(CfPrisonGuardState, sequenceFlags) == 0x38);
STATIC_ASSERT(offsetof(CfPrisonGuardState, statusFlags) == 0x39);
STATIC_ASSERT(offsetof(CfPrisonGuardState, pad3A) == 0x3A);
STATIC_ASSERT(sizeof(CfPrisonGuardState) == 0x3C);

int cfPrisonGuard_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int cfPrisonGuard_getExtraSize(void);
int cfPrisonGuard_getObjectTypeId(void);
void cfPrisonGuard_free(void);
void cfPrisonGuard_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void cfPrisonGuard_hitDetect(GameObject* obj);
void cfPrisonGuard_update(GameObject* obj);
void cfPrisonGuard_init(GameObject* obj, CfPrisonGuardPlacement* placement);
void cfPrisonGuard_release(void);
void cfPrisonGuard_initialise(void);

extern ObjectDescriptor gCFPrisonGuardObjDescriptor;

#endif /* DLLS_OBJECTS_334_CFPRISONGUA_H_ */
