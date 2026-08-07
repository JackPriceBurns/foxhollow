#ifndef DLLS_OBJECTS_480_DIM_BOSS_H_
#define DLLS_OBJECTS_480_DIM_BOSS_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/baddie_state.h"
#include "main/dll/dll_002E_moveLib.h"
#include "main/dll/partfx_interface.h"
#include "main/model_light.h"
#include "main/objseq.h"

#define DIMBOSS_RUNTIME_SIZE            0x4C8
#define DIMBOSS_GAMEBIT_ICICLE_DEFEATED 0x20E

typedef struct DIMbossSteamFlags {
    u8 sfxPending : 1;
    u8 rest : 7;
} DIMbossSteamFlags;

typedef struct DIMbossEffectMarker {
    u16 rotX;
    u16 rotY;
    u16 rotZ;
    u16 effectParam;
    f32 scale;
    f32 x;
    f32 y;
    f32 z;
} DIMbossEffectMarker;

/*
 * The boss and icicle combat modes reuse the same control-state storage at
 * +0xA0..+0xA8 and +0xB5. Both evidenced views are kept explicit.
 */
typedef struct DIMbossTopState {
    ModelLightStruct* effect;
    DIMbossEffectMarker blueWhiteEffectSource;
    DIMbossEffectMarker breathBurstSource;
    DIMbossEffectMarker tonsilDustSource;
    DIMbossEffectMarker liftGlowSource;
    f32 breathBurstMtx[12];
    f32 blueWhiteVelocity[3];
    union {
        struct {
            u8 unknownA0[0x04];
            f32 launchLift;
            f32 idleLift;
        };
        struct {
            f32 meltTimer;
            f32 lightTimer;
            f32 fadeTimer;
        } icicle;
    };
    f32 introSinkHeight;
    s32 defeatTimer;
    u8 stompDustDelay;
    union {
        u8 unknownB5;
        u8 meltEntryIndex;
    };
    DIMbossSteamFlags steamFlags;
} DIMbossTopState;

typedef struct DIMbossAnimHandlerTable {
    int (*selectTargetControlMode)(GameObject* obj);
    int (*returnToIdleWhenDone)(GameObject* obj, BaddieState* state);
    int (*hasMoveDone)(int unused, int* state);
    int (*finishDefeat)(GameObject* obj, BaddieState* state);
    int (*updatePlayerHitReaction)(GameObject* obj, BaddieState* state);
    int (*updateBossHitReaction)(GameObject* obj, BaddieState* state);
} DIMbossAnimHandlerTable;

typedef struct DIMbossHitDetectAnimHandlerTable {
    int (*resetIdleMove)(GameObject* obj, u8* state);
    int (*applyForwardMove)(int* obj, u8* state, f32 weight);
    int (*trackTargetMove)(GameObject* obj, BaddieState* state, f32 weight);
    int (*randomSwipe)(GameObject* obj, BaddieState* state, f32 weight);
    int (*blueWhiteEventCapture)(GameObject* obj, BaddieState* state, f32 weight);
    int (*blueWhiteCapture)(GameObject* obj, BaddieState* state, f32 weight);
    int (*breathBurst)(GameObject* obj, BaddieState* state, f32 weight);
    int (*lungeAttack)(GameObject* obj, BaddieState* state, f32 weight);
    int (*chooseIdleTaunt)(GameObject* obj, BaddieState* state);
    int (*liftImpact)(GameObject* obj, BaddieState* state);
    int (*liftSlam)(GameObject* obj, BaddieState* state);
    int (*tonsilSlam)(GameObject* obj, BaddieState* state);
} DIMbossHitDetectAnimHandlerTable;

/*
 * DIMboss_getExtraSize() allocates the complete 0x4C8-byte owner state.
 * The canonical ground-baddie view and the class-specific view describe the
 * same storage; topState is the boss role of GroundBaddieState.control.
 */
typedef struct DIMbossRuntime {
    union {
        GroundBaddieState groundBaddie;
        struct {
            u8 unknown000[0x25F];
            u8 physicsActive;
            u8 unknown260[0x270 - 0x260];
            s16 substate;
            u8 unknown272[0x274 - 0x272];
            s16 controlMode;
            u8 unknown276[0x2D0 - 0x276];
            int targetObj;
            u8 unknown2D4[0x314 - 0x2D4];
            s32 eventFlags;
            u8 unknown318[0x346 - 0x318];
            u8 moveDone;
            u8 unknown347[0x349 - 0x347];
            u8 hasTarget;
            u8 unknown34A[0x34F - 0x34A];
            s8 lastHitPriority;
            u8 unknown350[0x354 - 0x350];
            s8 hitPoints;
            u8 unknown355[0x35C - 0x355];
            u8 moveScratch[0x3E0 - 0x35C];
            u32 savedPendingParentObj;
            u8 unknown3E4[0x3F4 - 0x3E4];
            s16 gameBitB;
            s16 gameBitC;
            u8 unknown3F8[0x400 - 0x3F8];
            u16 flags400;
            s16 targetState;
            u8 unknown404;
            u8 subMode;
            u8 unknown406[0x40C - 0x406];
            DIMbossTopState* topState;
            u8 unknown410[DIMBOSS_RUNTIME_SIZE - 0x410];
        };
    };
} DIMbossRuntime;

/*
 * This type models only the placement prefix consumed by the callbacks; it
 * does not claim a complete retail placement width.
 */
typedef struct DIMbossPlacementView {
    ObjPlacement base;
    u8 unknown18[0x2C - 0x18];
    s16 eventId;
    s8 animObjectId;
} DIMbossPlacementView;

STATIC_ASSERT(sizeof(DIMbossEffectMarker) == 0x18);
STATIC_ASSERT(offsetof(DIMbossEffectMarker, scale) == 0x08);
STATIC_ASSERT(offsetof(DIMbossEffectMarker, x) == 0x0C);

STATIC_ASSERT(sizeof(DIMbossTopState) == 0xB8);
STATIC_ASSERT(offsetof(DIMbossTopState, effect) == 0x00);
STATIC_ASSERT(offsetof(DIMbossTopState, blueWhiteEffectSource) == 0x04);
STATIC_ASSERT(offsetof(DIMbossTopState, breathBurstSource) == 0x1C);
STATIC_ASSERT(offsetof(DIMbossTopState, tonsilDustSource) == 0x34);
STATIC_ASSERT(offsetof(DIMbossTopState, liftGlowSource) == 0x4C);
STATIC_ASSERT(offsetof(DIMbossTopState, breathBurstMtx) == 0x64);
STATIC_ASSERT(offsetof(DIMbossTopState, blueWhiteVelocity) == 0x94);
STATIC_ASSERT(offsetof(DIMbossTopState, unknownA0) == 0xA0);
STATIC_ASSERT(offsetof(DIMbossTopState, icicle.meltTimer) == 0xA0);
STATIC_ASSERT(offsetof(DIMbossTopState, launchLift) == 0xA4);
STATIC_ASSERT(offsetof(DIMbossTopState, icicle.lightTimer) == 0xA4);
STATIC_ASSERT(offsetof(DIMbossTopState, idleLift) == 0xA8);
STATIC_ASSERT(offsetof(DIMbossTopState, icicle.fadeTimer) == 0xA8);
STATIC_ASSERT(offsetof(DIMbossTopState, introSinkHeight) == 0xAC);
STATIC_ASSERT(offsetof(DIMbossTopState, defeatTimer) == 0xB0);
STATIC_ASSERT(offsetof(DIMbossTopState, stompDustDelay) == 0xB4);
STATIC_ASSERT(offsetof(DIMbossTopState, unknownB5) == 0xB5);
STATIC_ASSERT(offsetof(DIMbossTopState, meltEntryIndex) == 0xB5);
STATIC_ASSERT(offsetof(DIMbossTopState, steamFlags) == 0xB6);

STATIC_ASSERT(sizeof(DIMbossAnimHandlerTable) == 0x18);
STATIC_ASSERT(sizeof(DIMbossHitDetectAnimHandlerTable) == 0x30);

STATIC_ASSERT(offsetof(DIMbossRuntime, groundBaddie) == 0x000);
STATIC_ASSERT(offsetof(DIMbossRuntime, physicsActive) == 0x25F);
STATIC_ASSERT(offsetof(DIMbossRuntime, substate) == 0x270);
STATIC_ASSERT(offsetof(DIMbossRuntime, controlMode) == 0x274);
STATIC_ASSERT(offsetof(DIMbossRuntime, targetObj) == 0x2D0);
STATIC_ASSERT(offsetof(DIMbossRuntime, eventFlags) == 0x314);
STATIC_ASSERT(offsetof(DIMbossRuntime, moveDone) == 0x346);
STATIC_ASSERT(offsetof(DIMbossRuntime, hasTarget) == 0x349);
STATIC_ASSERT(offsetof(DIMbossRuntime, lastHitPriority) == 0x34F);
STATIC_ASSERT(offsetof(DIMbossRuntime, hitPoints) == 0x354);
STATIC_ASSERT(offsetof(DIMbossRuntime, moveScratch) == 0x35C);
STATIC_ASSERT(offsetof(DIMbossRuntime, savedPendingParentObj) == 0x3E0);
STATIC_ASSERT(offsetof(DIMbossRuntime, gameBitB) == 0x3F4);
STATIC_ASSERT(offsetof(DIMbossRuntime, gameBitC) == 0x3F6);
STATIC_ASSERT(offsetof(DIMbossRuntime, flags400) == 0x400);
STATIC_ASSERT(offsetof(DIMbossRuntime, targetState) == 0x402);
STATIC_ASSERT(offsetof(DIMbossRuntime, subMode) == 0x405);
STATIC_ASSERT(offsetof(DIMbossRuntime, topState) == 0x40C);
STATIC_ASSERT(offsetof(DIMbossRuntime, topState) == offsetof(DIMbossRuntime, groundBaddie.control));
STATIC_ASSERT(sizeof(DIMbossRuntime) == DIMBOSS_RUNTIME_SIZE);

STATIC_ASSERT(offsetof(DIMbossPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(DIMbossPlacementView, eventId) == 0x2C);
STATIC_ASSERT(offsetof(DIMbossPlacementView, animObjectId) == 0x2E);

int DIMbossAnim_updateBossHitReaction(GameObject* obj, BaddieState* state);
int DIMbossAnim_updatePlayerHitReaction(GameObject* obj, BaddieState* state);
int DIMbossAnim_finishDefeat(GameObject* obj, BaddieState* state);
int DIMbossAnim_hasMoveDone(int unused, int* state);
int DIMbossAnim_returnToIdleWhenDone(GameObject* obj, BaddieState* state);
int DIMbossAnim_selectTargetControlMode(GameObject* obj);

int DIMbossHitDetect_tonsilSlam(GameObject* obj, BaddieState* state);
int DIMbossHitDetect_liftSlam(GameObject* obj, BaddieState* state);
int DIMbossHitDetect_liftImpact(GameObject* obj, BaddieState* state);
int DIMbossHitDetect_chooseIdleTaunt(GameObject* obj, BaddieState* state);
int DIMbossHitDetect_lungeAttack(GameObject* obj, BaddieState* state, f32 weight);
int DIMbossHitDetect_breathBurst(GameObject* obj, BaddieState* state, f32 weight);
int DIMbossHitDetect_blueWhiteCapture(GameObject* obj, BaddieState* state, f32 weight);
int DIMbossHitDetect_blueWhiteEventCapture(GameObject* obj, BaddieState* state, f32 weight);
int DIMbossHitDetect_randomSwipe(GameObject* obj, BaddieState* state, f32 weight);
int DIMbossHitDetect_trackTargetMove(GameObject* obj, BaddieState* state, f32 weight);
int DIMbossHitDetect_applyForwardMove(int* obj, u8* state, f32 weight);
int DIMbossHitDetect_resetIdleMove(GameObject* obj, u8* state);

void DIMboss_spawnBlueWhiteEffect(DIMbossEffectMarker* source, f32* velocity);
void DIMboss_createStateLight(GameObject* obj, u8 isGreen);
void DIMboss_updateSequenceEffects(GameObject* obj, DIMbossRuntime* runtime);
void DIMboss_updateWarpAndEffects(GameObject* obj, DIMbossRuntime* runtime);
void DIMboss_updateHitResponse(GameObject* obj, BaddieState* playerState);
void DIMboss_updateCombatState(GameObject* obj, ObjSeqState* animUpdate, DIMbossRuntime* runtime,
                                  DIMbossRuntime* updateRuntime);

int DIMboss_updateState(GameObject* obj, u32 state, ObjSeqState* animUpdate);
void DIMboss_func0B(void);
int DIMboss_getControlMode(GameObject* obj);
int DIMboss_getExtraSize(void);
int DIMboss_getObjectTypeId(void);
void DIMboss_free(GameObject* obj);
void DIMboss_render(GameObject* obj, u32 renderArg2, u32 renderArg3, u32 renderArg4, u32 renderArg5, s8 visible);
void DIMboss_hitDetect(GameObject* obj);
void DIMboss_update(GameObject* obj);
void DIMboss_init(GameObject* obj, u32 params, int isAltVariant);
void DIMboss_release(void);
void DIMboss_initialise(void);
void DIMboss_initialiseAnimTables(void);

extern ObjectDescriptor12 gDIM_BossObjDescriptor;
extern DIMbossAnimHandlerTable gDIMbossAnimTable;
extern DIMbossHitDetectAnimHandlerTable gDIMbossHitDetectAnimTable;
extern MoveLibState gDIMbossAnimController;
extern f32 gDIMbossAnimScratchBase[3];
extern DIMbossEffectMarker gDIMbossHitFxBuffer;
extern PartFxSpawnParams gDIMbossDustFxSource;

#endif /* DLLS_OBJECTS_480_DIM_BOSS_H_ */
