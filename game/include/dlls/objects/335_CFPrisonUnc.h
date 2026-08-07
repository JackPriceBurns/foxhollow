#ifndef DLLS_OBJECTS_335_CFPRISONUNC_H_
#define DLLS_OBJECTS_335_CFPRISONUNC_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objprint_character_api.h"
#include "main/objseq.h"
#include "main/objprint_sound_api.h"

typedef struct CfPrisonUncleState {
    GameObject* companion;
    CharacterEyeAnimState eyeAnimState;
    u8 pad2C[0x8];
    ObjSoundState soundState;
    int unknown64;
    int unknown68;
    u8 pad6C[0x04];
    s16 unknown70;
    u8 pad72;
    s8 cageOpen;
    s8 magicGranted;
    u8 pad75[0x33];
} CfPrisonUncleState;

STATIC_ASSERT(offsetof(CfPrisonUncleState, companion) == 0x00);
STATIC_ASSERT(offsetof(CfPrisonUncleState, eyeAnimState) == 0x04);
STATIC_ASSERT(offsetof(CfPrisonUncleState, soundState) == 0x34);
STATIC_ASSERT(offsetof(CfPrisonUncleState, unknown64) == 0x64);
STATIC_ASSERT(offsetof(CfPrisonUncleState, unknown68) == 0x68);
STATIC_ASSERT(offsetof(CfPrisonUncleState, pad6C) == 0x6C);
STATIC_ASSERT(offsetof(CfPrisonUncleState, unknown70) == 0x70);
STATIC_ASSERT(offsetof(CfPrisonUncleState, pad72) == 0x72);
STATIC_ASSERT(offsetof(CfPrisonUncleState, cageOpen) == 0x73);
STATIC_ASSERT(offsetof(CfPrisonUncleState, magicGranted) == 0x74);
STATIC_ASSERT(offsetof(CfPrisonUncleState, pad75) == 0x75);
STATIC_ASSERT(sizeof(CfPrisonUncleState) == 0xA8);

int cfPrisonUncle_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int cfPrisonUncle_getExtraSize(void);
int cfPrisonUncle_getObjectTypeId(void);
void cfPrisonUncle_free(void);
void cfPrisonUncle_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void cfPrisonUncle_hitDetect(void);
void cfPrisonUncle_update(GameObject* obj);
void cfPrisonUncle_init(GameObject* obj);
void cfPrisonUncle_release(void);
void cfPrisonUncle_initialise(void);

extern ObjectDescriptor gCFPrisonUncleObjDescriptor;

#endif /* DLLS_OBJECTS_335_CFPRISONUNC_H_ */
