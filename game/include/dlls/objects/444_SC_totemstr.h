#ifndef DLLS_OBJECTS_444_SC_TOTEMSTR_H_
#define DLLS_OBJECTS_444_SC_TOTEMSTR_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "global.h"
#include "main/objseq.h"

#define SC_TOTEM_STRENGTH_RECORD_GAME_BIT_COUNT 4

typedef struct ScTotemStrengthState {
    GameObject* linkedObject;
    f32 unknown04;
    f32 offsetVelocity;
    f32 savedPosX;
    f32 savedPosY;
    f32 savedPosZ;
    f32 playerSfxTimer;
    f32 platformSfxTimer;
    int currentTrackOffset;
    int sequenceIndex;
    int prevTrackOffset;
    u8 unknown2C[0x2E - 0x2C];
    s16 transitionStep;
    u8 flags;
    u8 unknown31[0x34 - 0x31];
} ScTotemStrengthState;

STATIC_ASSERT(offsetof(ScTotemStrengthState, linkedObject) == 0x00);
STATIC_ASSERT(offsetof(ScTotemStrengthState, unknown04) == 0x04);
STATIC_ASSERT(offsetof(ScTotemStrengthState, offsetVelocity) == 0x08);
STATIC_ASSERT(offsetof(ScTotemStrengthState, savedPosX) == 0x0C);
STATIC_ASSERT(offsetof(ScTotemStrengthState, savedPosY) == 0x10);
STATIC_ASSERT(offsetof(ScTotemStrengthState, savedPosZ) == 0x14);
STATIC_ASSERT(offsetof(ScTotemStrengthState, playerSfxTimer) == 0x18);
STATIC_ASSERT(offsetof(ScTotemStrengthState, platformSfxTimer) == 0x1C);
STATIC_ASSERT(offsetof(ScTotemStrengthState, currentTrackOffset) == 0x20);
STATIC_ASSERT(offsetof(ScTotemStrengthState, sequenceIndex) == 0x24);
STATIC_ASSERT(offsetof(ScTotemStrengthState, prevTrackOffset) == 0x28);
STATIC_ASSERT(offsetof(ScTotemStrengthState, unknown2C) == 0x2C);
STATIC_ASSERT(offsetof(ScTotemStrengthState, transitionStep) == 0x2E);
STATIC_ASSERT(offsetof(ScTotemStrengthState, flags) == 0x30);
STATIC_ASSERT(offsetof(ScTotemStrengthState, unknown31) == 0x31);
STATIC_ASSERT(sizeof(ScTotemStrengthState) == 0x34);

int sc_totemstrength_animEventCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int sc_totemstrength_getExtraSize(void);
int sc_totemstrength_getObjectTypeId(void);
void sc_totemstrength_free(void);
void sc_totemstrength_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible);
void sc_totemstrength_hitDetect(void);
void sc_totemstrength_update(GameObject* obj);
void sc_totemstrength_init(GameObject* obj);
void sc_totemstrength_release(void);
void sc_totemstrength_initialise(void);

extern u16 gScTotemStrengthRecordGameBits[SC_TOTEM_STRENGTH_RECORD_GAME_BIT_COUNT];
extern int gTotemStrengthDeactivateTimer;
extern ObjectDescriptor gSC_totemstrengthObjDescriptor;

#endif /* DLLS_OBJECTS_444_SC_TOTEMSTR_H_ */
