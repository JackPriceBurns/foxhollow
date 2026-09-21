#ifndef MAIN_DLL_DLL_0266_KYTESMUM_H_
#define MAIN_DLL_DLL_0266_KYTESMUM_H_

#include "game/objects/object.h"
#include "main/objprint_character.h"
#include "global.h"
#include "main/objanim_internal.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"
#include "main/objprint_sound.h"
#include "main/dll/dll_002E_moveLib.h"

typedef int (*KytesMumUpdateCallback)(GameObject* obj);

typedef struct KytesMumQuestTriple {
    int values[3];
} KytesMumQuestTriple;

typedef struct KytesMumMoveSet {
    s16 moves[6];
} KytesMumMoveSet;

typedef struct KytesMumSetup {
    ObjPlacement base;
    s8 yaw;
    s8 mode;
    s16 interactionRange;
    u8 pad1C[0x1e - 0x1c];
    s16 completionGameBit;
    u8 pad20[0x24 - 0x20];
} KytesMumSetup;

typedef struct KytesMumRuntime {
    MoveLibState moveLib;
    u8 pad624[0x654 - 0x624];
    CharacterEyeAnimState eyeAnimState;
    u8 pad67C[0x8];
    ObjSoundState modelSoundState;
    u8 animEvents[0x6d0 - 0x6b4];
    ObjSoundDef* idleSfxTable;
    KytesMumUpdateCallback updateCallback;
    s16* eventSfxTable;
    KytesMumMoveSet* moveSet;
    f32 animSpeed;
    s16 idleSfxTimer;
    u8 questComplete;
} KytesMumRuntime;

STATIC_ASSERT(sizeof(KytesMumSetup) == 0x24);
STATIC_ASSERT(offsetof(KytesMumSetup, yaw) == 0x18);
STATIC_ASSERT(offsetof(KytesMumSetup, mode) == 0x19);
STATIC_ASSERT(offsetof(KytesMumSetup, interactionRange) == 0x1A);
STATIC_ASSERT(offsetof(KytesMumSetup, completionGameBit) == 0x1E);
STATIC_ASSERT(sizeof(KytesMumRuntime) == 0x6EC);
STATIC_ASSERT(offsetof(KytesMumRuntime, moveLib) == 0x0);
STATIC_ASSERT(offsetof(KytesMumRuntime, eyeAnimState) == 0x654);
STATIC_ASSERT(offsetof(KytesMumRuntime, modelSoundState) == 0x684);
STATIC_ASSERT(offsetof(KytesMumRuntime, idleSfxTable) == 0x6D0);

int kytesmum_getExtraSize(void);
int kytesmum_getObjectTypeId(void);
void kytesmum_hitDetect(void);
void kytesmum_initialise(void);
void kytesmum_release(void);
void kytesmum_update(GameObject* obj);
int kytesmum_idleCallback(void);
void kytesmum_render(GameObject* obj, int p2, int p3, int p4, int p5, char visible);
void kytesmum_free(GameObject* obj);
int kytesmum_spawnInteractionCallback(GameObject* obj);
int kytesmum_updateInteractionRangeCallback(GameObject* obj, int unused, u8* arg);
int kytesmum_animEventCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
void kytesmum_init(GameObject* obj, KytesMumSetup* setup);
int kytesmum_updateNearPlayerCallback(GameObject* obj, int unused, u8* arg);
int kytesmum_updateQuestStateCallback(GameObject* obj, int unused, u8* arg);
void kytesmum_playAnimationEventSfx(GameObject* obj, u8* arg, s16* sfxData);

extern char sKytesMumYawDiffMessage[];
extern const s32 gKytesMumQuestBits[3];
extern const s32 gKytesMumTriggerIds[3];
extern int gKytesMumQuestIdleSfxTable[];
extern u8 gKytesMumMoveSets[];
extern s16 gKytesMumRoamEventSfxTable[4];
extern s16 gKytesMumQuestEventSfxTable[4];

#endif /* MAIN_DLL_DLL_0266_KYTESMUM_H_ */
