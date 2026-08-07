#ifndef DLLS_OBJECTS_227_FIREBALL_H_
#define DLLS_OBJECTS_227_FIREBALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/modellight_api.h"
#include "main/objseq.h"

#define FIREBALL_ROTATION_COUNT 5

typedef struct FireballPlacement {
    ObjPlacement base;       /* 0x00 */
    u8 pad18;                /* 0x18 */
    s8 hitVolumeMode;        /* 0x19 */
    s16 startupDelayEnabled; /* 0x1A */
    s16 startDisabled;       /* 0x1C */
    s16 unk1E;               /* 0x1E */
    s16 unk20;               /* 0x20 */
    u8 pad22[0x2C - 0x22];   /* 0x22 */
    s16 unk2C;               /* 0x2C */
    u8 pad2E[0x30 - 0x2E];   /* 0x2E */
} FireballPlacement;

typedef struct FireballState {
    ModelLightStruct* light;                /* 0x00 */
    f32 unk4;                               /* 0x04 */
    f32 unk8;                               /* 0x08 */
    f32 unkC;                               /* 0x0C */
    s32 unk10;                              /* 0x10 */
    u8 pad14[0x18 - 0x14];                  /* 0x14 */
    u8 unk18;                               /* 0x18 */
    u8 pad19[0x24 - 0x19];                  /* 0x19 */
    f32 posX;                               /* 0x24 */
    f32 posY;                               /* 0x28 */
    f32 posZ;                               /* 0x2C */
    f32 flightDuration;                     /* 0x30 */
    f32 elapsedTime;                        /* 0x34 */
    f32 fadeoutTimer;                       /* 0x38 */
    f32 startupDelay;                       /* 0x3C */
    s16 unk40;                              /* 0x40 */
    s16 unk42;                              /* 0x42 */
    u8 pad44[0x46 - 0x44];                  /* 0x44 */
    u16 spiralPhase;                        /* 0x46 */
    u16 rotZBase[FIREBALL_ROTATION_COUNT];  /* 0x48 */
    u16 rotZDelta[FIREBALL_ROTATION_COUNT]; /* 0x52 */
    u16 rotYBase[FIREBALL_ROTATION_COUNT];  /* 0x5C */
    u16 rotYDelta[FIREBALL_ROTATION_COUNT]; /* 0x66 */
    u8 stateFlags;                          /* 0x70 */
    u8 colorIndex;                          /* 0x71 */
    u8 pad72[0x74 - 0x72];                  /* 0x72 */
} FireballState;

STATIC_ASSERT(offsetof(FireballPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(FireballPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(FireballPlacement, hitVolumeMode) == 0x19);
STATIC_ASSERT(offsetof(FireballPlacement, startupDelayEnabled) == 0x1A);
STATIC_ASSERT(offsetof(FireballPlacement, startDisabled) == 0x1C);
STATIC_ASSERT(offsetof(FireballPlacement, unk1E) == 0x1E);
STATIC_ASSERT(offsetof(FireballPlacement, unk20) == 0x20);
STATIC_ASSERT(offsetof(FireballPlacement, pad22) == 0x22);
STATIC_ASSERT(offsetof(FireballPlacement, unk2C) == 0x2C);
STATIC_ASSERT(offsetof(FireballPlacement, pad2E) == 0x2E);
STATIC_ASSERT(sizeof(FireballPlacement) == 0x30);

STATIC_ASSERT(offsetof(FireballState, light) == 0x0);
STATIC_ASSERT(offsetof(FireballState, unk4) == 0x4);
STATIC_ASSERT(offsetof(FireballState, unk8) == 0x8);
STATIC_ASSERT(offsetof(FireballState, unkC) == 0xC);
STATIC_ASSERT(offsetof(FireballState, unk10) == 0x10);
STATIC_ASSERT(offsetof(FireballState, pad14) == 0x14);
STATIC_ASSERT(offsetof(FireballState, unk18) == 0x18);
STATIC_ASSERT(offsetof(FireballState, pad19) == 0x19);
STATIC_ASSERT(offsetof(FireballState, posX) == 0x24);
STATIC_ASSERT(offsetof(FireballState, posY) == 0x28);
STATIC_ASSERT(offsetof(FireballState, posZ) == 0x2C);
STATIC_ASSERT(offsetof(FireballState, flightDuration) == 0x30);
STATIC_ASSERT(offsetof(FireballState, elapsedTime) == 0x34);
STATIC_ASSERT(offsetof(FireballState, fadeoutTimer) == 0x38);
STATIC_ASSERT(offsetof(FireballState, startupDelay) == 0x3C);
STATIC_ASSERT(offsetof(FireballState, unk40) == 0x40);
STATIC_ASSERT(offsetof(FireballState, unk42) == 0x42);
STATIC_ASSERT(offsetof(FireballState, pad44) == 0x44);
STATIC_ASSERT(offsetof(FireballState, spiralPhase) == 0x46);
STATIC_ASSERT(offsetof(FireballState, rotZBase) == 0x48);
STATIC_ASSERT(offsetof(FireballState, rotZDelta) == 0x52);
STATIC_ASSERT(offsetof(FireballState, rotYBase) == 0x5C);
STATIC_ASSERT(offsetof(FireballState, rotYDelta) == 0x66);
STATIC_ASSERT(offsetof(FireballState, stateFlags) == 0x70);
STATIC_ASSERT(offsetof(FireballState, colorIndex) == 0x71);
STATIC_ASSERT(offsetof(FireballState, pad72) == 0x72);
STATIC_ASSERT(sizeof(FireballState) == 0x74);

u8 Fireball_getColorIndex(GameObject* obj);
int Fireball_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
void Fireball_homeToTarget(GameObject* obj, FireballState* state, GameObject* target);
int Fireball_getExtraSize(void);
int Fireball_getObjectTypeId(void);
void Fireball_free(GameObject* obj);
void Fireball_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void Fireball_hitDetect(GameObject* obj);
void Fireball_update(GameObject* obj);
void Fireball_init(GameObject* obj);
void Fireball_release(void);
void Fireball_initialise(void);

extern ObjectDescriptor10WithPadding gFireballObjDescriptor;
extern u8 gFireballColorIndexTable[8];

#endif /* DLLS_OBJECTS_227_FIREBALL_H_ */
