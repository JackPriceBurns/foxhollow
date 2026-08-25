#ifndef DLLS_OBJECTS_599_DR_EARTHWAR_H_
#define DLLS_OBJECTS_599_DR_EARTHWAR_H_

#include "dlls/object_descriptor.h"
#include "dlls/objects/common/vehicle.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/byte_flags.h"
#include "main/dll/baddie_state.h"
#include "main/dll/dll_002E_moveLib.h"
#include "main/dll/dll_005A_staffcollision.h"
#include "main/dll/player_state.h"
#include "main/model.h"
#include "main/objprint_character.h"
#include "main/objprint_sound.h"
#include "main/objseq.h"

typedef struct EWSpeedRange {
    f32 minSpeed;
    f32 maxSpeed;
} EWSpeedRange;

typedef struct DREarthWarriorPlacement {
    ObjPlacement base;
    s8 spawnYaw;
    u8 setupVariant;
    s16 energyCapacity;
} DREarthWarriorPlacement;

/* EarthWarrior-specific runtime data at EarthWarriorState+0xB58. */
typedef struct EarthWarriorSub {
    PlayerState playerMotion;
    f32 riderPosX;
    f32 riderPosY;
    f32 riderPosZ;
    f32 maxSpeed;
    u8 pad8F0[0x90];
    int savedControlMode;
    u8 pad984[2];
    s16 turnThreshold;
    u8 pad988[2];
    s16 energy;
    u16 flags98C;
    u8 mountState;
    u8 pad98F;
    u8 setupVariant;
    u8 pad991;
    u8 dismountSide;
    u8 mountSide;
    ByteFlags flags994;
    u8 unk995;
    u8 pad996[2];
    f32 airMeterTimer;
    s8 talkSequenceId;
    u8 unk99D;
    u8 pad99E[2];
    ObjModelChain* modelChain;
} EarthWarriorSub;

typedef struct EarthWarriorState {
    BaddieState baddie;
    u8 pad35C[0x38C - 0x35C];
    CharacterEyeAnimState eyeAnimState;
    u8 pad3B4[0x3BC - 0x3B4];
    ObjSoundState modelSoundState;
    MoveLibState moveLib;
    u8 padA10[0xB18 - 0xA10];
    Vec3f pathPoints[4];
    u8 padB48[0xB54 - 0xB48];
    /* b58 */ GameObject* helperObj;
    EarthWarriorSub sub;
} EarthWarriorState;

typedef struct DREarthWarriorInitData {
    u8 unk0[0xC];
    f32 segmentLocalPoints[12];
    f32 segmentRadii[4];
    f32 localPointPositions[6];
    f32 localPointRadii[4];
    s32 unk74[4];
    EWSpeedRange configRow[6];
    u8 hitVolumeRowIndices[0x24];
    s16 moveTable[0x20];
    u8 paramCurve0Data[0xA4];
    u8 paramCurve1Data[0xA4];
    u8 paramCurve2Data[4];
} DREarthWarriorInitData;

typedef struct EWPathRange {
    s16 values[5];
} EWPathRange;

typedef struct EWColorTable {
    StaffCollisionColorArgs rows[4];
} EWColorTable;

STATIC_ASSERT(sizeof(EWColorTable) == 0x40);

STATIC_ASSERT(offsetof(DREarthWarriorPlacement, spawnYaw) == 0x18);
STATIC_ASSERT(offsetof(DREarthWarriorPlacement, setupVariant) == 0x19);
STATIC_ASSERT(offsetof(DREarthWarriorPlacement, energyCapacity) == 0x1A);
STATIC_ASSERT(sizeof(DREarthWarriorPlacement) == 0x1C);

STATIC_ASSERT(offsetof(DREarthWarriorInitData, localPointRadii) == 0x64);
STATIC_ASSERT(offsetof(DREarthWarriorInitData, unk74) == 0x74);
STATIC_ASSERT(offsetof(DREarthWarriorInitData, configRow) == 0x84);
STATIC_ASSERT(offsetof(DREarthWarriorInitData, hitVolumeRowIndices) == 0xB4);
STATIC_ASSERT(offsetof(DREarthWarriorInitData, moveTable) == 0xD8);
STATIC_ASSERT(offsetof(DREarthWarriorInitData, paramCurve0Data) == 0x118);
STATIC_ASSERT(offsetof(DREarthWarriorInitData, paramCurve1Data) == 0x1BC);
STATIC_ASSERT(offsetof(DREarthWarriorInitData, paramCurve2Data) == 0x260);
STATIC_ASSERT(sizeof(DREarthWarriorInitData) == 0x264);

STATIC_ASSERT(sizeof(EarthWarriorSub) == 0x9A4);
STATIC_ASSERT(offsetof(EarthWarriorSub, playerMotion.moveAnimIds) == 0x3F8);
STATIC_ASSERT(offsetof(EarthWarriorSub, playerMotion.moveParamValues) == 0x400);
STATIC_ASSERT(offsetof(EarthWarriorSub, playerMotion.paramCurve0) == 0x450);
STATIC_ASSERT(offsetof(EarthWarriorSub, playerMotion.targetYaw) == 0x478);
STATIC_ASSERT(offsetof(EarthWarriorSub, playerMotion.yaw) == 0x484);
STATIC_ASSERT(offsetof(EarthWarriorSub, playerMotion.lastInputHeading) == 0x494);
STATIC_ASSERT(offsetof(EarthWarriorSub, playerMotion.curveSpeedScale) == 0x7E0);
STATIC_ASSERT(offsetof(EarthWarriorSub, playerMotion.targetAnimSpeed) == 0x82C);
STATIC_ASSERT(offsetof(EarthWarriorSub, playerMotion.paramCurve0Count) == 0x8D0);
STATIC_ASSERT(offsetof(EarthWarriorSub, riderPosX) == 0x8E0);
STATIC_ASSERT(offsetof(EarthWarriorSub, maxSpeed) == 0x8EC);
STATIC_ASSERT(offsetof(EarthWarriorSub, turnThreshold) == 0x986);
STATIC_ASSERT(offsetof(EarthWarriorSub, energy) == 0x98A);
STATIC_ASSERT(offsetof(EarthWarriorSub, mountState) == 0x98E);
STATIC_ASSERT(offsetof(EarthWarriorSub, setupVariant) == 0x990);
STATIC_ASSERT(offsetof(EarthWarriorSub, flags994) == 0x994);
STATIC_ASSERT(offsetof(EarthWarriorSub, airMeterTimer) == 0x998);
STATIC_ASSERT(offsetof(EarthWarriorSub, talkSequenceId) == 0x99C);
STATIC_ASSERT(offsetof(EarthWarriorSub, modelChain) == 0x9A0);

STATIC_ASSERT(sizeof(EarthWarriorState) == 0x14FC);
STATIC_ASSERT(offsetof(EarthWarriorState, eyeAnimState) == 0x38C);
STATIC_ASSERT(offsetof(EarthWarriorState, modelSoundState) == 0x3BC);
STATIC_ASSERT(offsetof(EarthWarriorState, moveLib) == 0x3EC);
STATIC_ASSERT(offsetof(EarthWarriorState, pathPoints) == 0xB18);
STATIC_ASSERT(offsetof(EarthWarriorState, helperObj) == 0xB54);
STATIC_ASSERT(offsetof(EarthWarriorState, sub) == 0xB58);

void DR_EarthWarrior_feed(GameObject* obj, int mode);
int DR_EarthWarrior_updateLeap(GameObject* obj, EarthWarriorSub* warrior, BaddieState* baddie);
int DR_EarthWarrior_defaultStateHandler(void);
int DR_EarthWarrior_stateHandler03(GameObject* obj, BaddieState* baddie);
int DR_EarthWarrior_stateHandler02(GameObject* obj, EarthWarriorState* controllerState);
int DR_EarthWarrior_stateHandler01(GameObject* obj, BaddieState* baddie);
int DR_EarthWarrior_stateHandler00(GameObject* obj);
int DR_EarthWarrior_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
void DR_EarthWarrior_handleRiderScale(GameObject* obj, f32 scale);
void DR_EarthWarrior_resetToRomListPosition(void);
int DR_EarthWarrior_getRacePosition(void);
f32 DR_EarthWarrior_func19(GameObject* obj, f32* out);
void DR_EarthWarrior_getPlayerAnim(GameObject* obj, f32* steeringAngle, int* leanAngle);
void DR_EarthWarrior_setMountState(GameObject* obj, enum VehicleMountState mountState);
int DR_EarthWarrior_getMountState(void);
void DR_EarthWarrior_getCameraPosition(GameObject* obj, f32* x, f32* y, f32* z);
int DR_EarthWarrior_getDismountSide(GameObject* obj);
int DR_EarthWarrior_canDismount(void);
void DR_EarthWarrior_getRiderPosition(GameObject* obj, f32* x, f32* y, f32* z);
int DR_EarthWarrior_getMountSide(GameObject* obj);
int DR_EarthWarrior_canMount(void);
int DR_EarthWarrior_getExtraSize(void);
int DR_EarthWarrior_getObjectTypeId(void);
void DR_EarthWarrior_free(GameObject* obj);
void DR_EarthWarrior_render(GameObject* obj, int gdl, int mtxs, int vtxs, int pols, s8 visibility);
void DR_EarthWarrior_hitDetect(GameObject* obj);
void DR_EarthWarrior_runController(GameObject* obj, int updateRate, int frameIndex);
void DR_EarthWarrior_update(GameObject* obj);
void DR_EarthWarrior_init(GameObject* obj, DREarthWarriorPlacement* placement);
void DR_EarthWarrior_release(void);
void DR_EarthWarrior_initialise(void);

extern f32 gEarthWarriorMatrix[16];
extern void* gDREarthWarriorStateHandlers[4];
extern void* gDREarthWarriorDefaultStateHandler;
extern StaffCollisionInterface** gEarthWarriorResource;
extern u8 gDREarthWarriorInitData[132];
extern EWSpeedRange gDREarthWarriorSpeedRows[6];
extern u8 gDREarthWarriorRowIndices[36];
extern u16 lbl_803352D0[32];
extern f32 lbl_80335310[215];
extern const EWPathRange gDREarthWarriorLookInitData1;
extern const EWPathRange gDREarthWarriorLookInitData2;
extern const EWColorTable gDREarthWarriorColors;
extern s32 gEarthWarriorTailChainJointIndices[4];
extern ObjModelChainDesc gEarthWarriorTailChain;
extern ObjModelChainDesc* gEarthWarriorTailChainDesc;
extern f32 lbl_803DC76C;
extern VehicleDescriptorWithPadding gDR_EarthWarriorObjDescriptor;

#endif /* DLLS_OBJECTS_599_DR_EARTHWAR_H_ */
