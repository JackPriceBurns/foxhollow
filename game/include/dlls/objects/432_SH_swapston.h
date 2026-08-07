#ifndef DLLS_OBJECTS_432_SH_SWAPSTON_H_
#define DLLS_OBJECTS_432_SH_SWAPSTON_H_

#include "dlls/object_descriptor.h"
#include "main/objprint_character_api.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/objprint_sound_api.h"

typedef struct ObjSeqState ObjSeqState;

typedef struct WarpStonePlacement {
    ObjPlacement base;
    u8 unknown18[2];
    u8 rotXByte;
} WarpStonePlacement;

typedef struct WarpStoneFlags {
    u8 b7 : 1;
    u8 lookAtPlayer : 1;
    u8 b5 : 1;
    u8 sfxFired : 1;
    u8 lo : 4;
} WarpStoneFlags;

typedef struct WarpStoneState {
    GameObject* child;
    f32 dustEffectTimer;
    u8 pathPointIndex;
    u8 sequenceToggle;
    u8 sequenceFlags;
    u8 unknown0B;
    u8 activated;
    u8 unknown0D;
    s16 sequenceGameBit;
    s16 resetGameBit;
    u8 unknown12[0x14 - 0x12];
    ObjSoundState soundState;
    CharacterEyeAnimState eyeAnimState;
    u8 pad6C[0x8];
    u8 headAimState[0xD4 - 0x74];
    u8 dustEffectFlags;
    WarpStoneFlags behaviorFlags;
    u8 unknownD6[0xD8 - 0xD6];
} WarpStoneState;

STATIC_ASSERT(offsetof(WarpStonePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(WarpStonePlacement, rotXByte) == 0x1A);

STATIC_ASSERT(offsetof(WarpStoneState, child) == 0x00);
STATIC_ASSERT(offsetof(WarpStoneState, dustEffectTimer) == 0x04);
STATIC_ASSERT(offsetof(WarpStoneState, pathPointIndex) == 0x08);
STATIC_ASSERT(offsetof(WarpStoneState, sequenceToggle) == 0x09);
STATIC_ASSERT(offsetof(WarpStoneState, sequenceFlags) == 0x0A);
STATIC_ASSERT(offsetof(WarpStoneState, activated) == 0x0C);
STATIC_ASSERT(offsetof(WarpStoneState, sequenceGameBit) == 0x0E);
STATIC_ASSERT(offsetof(WarpStoneState, resetGameBit) == 0x10);
STATIC_ASSERT(offsetof(WarpStoneState, soundState) == 0x14);
STATIC_ASSERT(offsetof(WarpStoneState, eyeAnimState) == 0x44);
STATIC_ASSERT(offsetof(WarpStoneState, headAimState) == 0x74);
STATIC_ASSERT(offsetof(WarpStoneState, dustEffectFlags) == 0xD4);
STATIC_ASSERT(offsetof(WarpStoneState, behaviorFlags) == 0xD5);
STATIC_ASSERT(sizeof(WarpStoneState) == 0xD8);

void warpstone_updateDustEffects(GameObject* obj);
u32 warpstone_advanceAnimEvents(GameObject* lantern, f32 moveStepScale);
u32 warpstoneProbePlayerAnimState(void);
int warpstone_testEvent(u32 obj, u32 unused, int option);
void warpstone_loadBaseUi(void);
int warpstone_SeqFn(GameObject* obj, u32 unused, ObjSeqState* animObj);
int warpstone_getExtraSize(void);
int warpstone_getObjectTypeId(void);
void warpstone_free(GameObject* obj, int mode);
void warpstone_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void warpstone_hitDetect(GameObject* obj);
void warpstone_update(GameObject* obj);
void warpstone_init(GameObject* obj, const WarpStonePlacement* placement);
void warpstone_release(void);
void warpstone_initialise(void);

extern ObjectDescriptor gWarpStoneObjDescriptor;

#endif /* DLLS_OBJECTS_432_SH_SWAPSTON_H_ */
