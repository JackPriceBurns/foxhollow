#ifndef DLLS_OBJECTS_376_DFSH_SHRINE_H_
#define DLLS_OBJECTS_376_DFSH_SHRINE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "dlls/objects/430_SH_LevelCon.h"
#include "main/model_light.h"
#include "main/objseq.h"

typedef struct DFSHShrineFlags {
    u8 openedBySequence : 1;
    u8 success : 1;
    u8 unknown2 : 1;
    u8 unknown3 : 1;
    u8 unknown4 : 1;
    u8 unknown5 : 1;
    u8 unknown6 : 1;
    u8 unknown7 : 1;
} DFSHShrineFlags;

typedef struct DFSHShrineHoverState {
    ModelLightStruct* light;
    u8 unknown04[0x14 - 0x04];
    s16 hoverPhase;
    s16 rollPhase;
    s16 yawPhase;
    u8 unknown1A[0x1C - 0x1A];
    DFSHShrineFlags flags;
    u8 unknown1D[0x20 - 0x1D];
} DFSHShrineHoverState;

typedef struct DFSHShrineState {
    ModelLightStruct* light;
    f32 rewardTimer;
    f32 idleChimeTimer;
    GameBitLatchState musicLatch;
    s16 startDelayFrames;
    s16 transitionTimer;
    u8 unknown14[0x1A - 0x14];
    u8 mode;
    u8 rewardIndex;
    DFSHShrineFlags flags;
    u8 unknown1D[0x20 - 0x1D];
} DFSHShrineState;

typedef struct DFSHShrinePlacement {
    ObjPlacement base;
    s8 initialYaw;
    u8 unknown19;
    s16 startDelay;
    u8 unknown1C[0x24 - 0x1C];
} DFSHShrinePlacement;

STATIC_ASSERT(sizeof(DFSHShrineHoverState) == 0x20);
STATIC_ASSERT(offsetof(DFSHShrineHoverState, light) == 0x00);
STATIC_ASSERT(offsetof(DFSHShrineHoverState, unknown04) == 0x04);
STATIC_ASSERT(offsetof(DFSHShrineHoverState, hoverPhase) == 0x14);
STATIC_ASSERT(offsetof(DFSHShrineHoverState, rollPhase) == 0x16);
STATIC_ASSERT(offsetof(DFSHShrineHoverState, yawPhase) == 0x18);
STATIC_ASSERT(offsetof(DFSHShrineHoverState, unknown1A) == 0x1A);
STATIC_ASSERT(offsetof(DFSHShrineHoverState, flags) == 0x1C);
STATIC_ASSERT(offsetof(DFSHShrineHoverState, unknown1D) == 0x1D);

STATIC_ASSERT(sizeof(DFSHShrineState) == 0x20);
STATIC_ASSERT(offsetof(DFSHShrineState, light) == 0x00);
STATIC_ASSERT(offsetof(DFSHShrineState, rewardTimer) == 0x04);
STATIC_ASSERT(offsetof(DFSHShrineState, idleChimeTimer) == 0x08);
STATIC_ASSERT(offsetof(DFSHShrineState, musicLatch) == 0x0C);
STATIC_ASSERT(offsetof(DFSHShrineState, startDelayFrames) == 0x10);
STATIC_ASSERT(offsetof(DFSHShrineState, transitionTimer) == 0x12);
STATIC_ASSERT(offsetof(DFSHShrineState, unknown14) == 0x14);
STATIC_ASSERT(offsetof(DFSHShrineState, mode) == 0x1A);
STATIC_ASSERT(offsetof(DFSHShrineState, rewardIndex) == 0x1B);
STATIC_ASSERT(offsetof(DFSHShrineState, flags) == 0x1C);
STATIC_ASSERT(offsetof(DFSHShrineState, unknown1D) == 0x1D);

STATIC_ASSERT(sizeof(DFSHShrinePlacement) == 0x24);
STATIC_ASSERT(offsetof(DFSHShrinePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DFSHShrinePlacement, initialYaw) == 0x18);
STATIC_ASSERT(offsetof(DFSHShrinePlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(DFSHShrinePlacement, startDelay) == 0x1A);
STATIC_ASSERT(offsetof(DFSHShrinePlacement, unknown1C) == 0x1C);

extern u8 gDFSHShrinePendingReward;
extern u16 gDFSHShrineRewardTable[50];
extern ObjectDescriptor gDFSHShrineObjDescriptor;

void dfshShrine_updateHoverMotion(int objArg);
int dfshShrine_processAnimEvents(GameObject* obj, int unusedArg2, ObjSeqState* animUpdate);
int dfshShrine_getExtraSize(void);
int dfshShrine_getObjectTypeId(void);
void dfshShrine_free(GameObject* obj);
void dfshShrine_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dfshShrine_hitDetect(void);
void dfshShrine_update(GameObject* obj);
void dfshShrine_init(GameObject* obj, const DFSHShrinePlacement* placement);
void dfshShrine_release(void);
void dfshShrine_initialise(void);

#endif /* DLLS_OBJECTS_376_DFSH_SHRINE_H_ */
