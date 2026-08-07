#ifndef DLLS_OBJECTS_204_CHUKCHUK_H_
#define DLLS_OBJECTS_204_CHUKCHUK_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct ChukChukPlacement {
    ObjPlacement base;       /* 0x00 */
    s16 gameBit;             /* 0x18: set when the object is destroyed */
    u8 pad1A[0x22 - 0x1A];   /* 0x1A */
    s16 unk22;               /* 0x22 */
    u8 pad24[0x27 - 0x24];   /* 0x24 */
    u8 aimHeightY;           /* 0x27: vertical projectile-aim offset */
    s8 arcHalfAngleScale;    /* 0x28: attack arc half-angle, scaled by 182 */
    u8 triggerDistanceScale; /* 0x29: trigger distance, scaled by 8 */
    s8 rotX;                 /* 0x2A: initial X rotation, scaled by 256 */
    u8 pad2B[0x2F - 0x2B];   /* 0x2B */
    u8 attackChance;         /* 0x2F: attack probability percentage */
    u8 pad30[0x32 - 0x30];   /* 0x30 */
    u8 hitsLeft;             /* 0x32: priority hits remaining */
    u8 pad33[0x38 - 0x33];   /* 0x33 */
} ChukChukPlacement;

typedef struct ChukChukState {
    f32 glowPhase;       /* 0x00: glow-ramp phase or inactive delay */
    f32 steamTimer;      /* 0x04: post-destruction steam duration */
    s16 unk08;           /* 0x08: copied from placement offset 0x22 */
    s16 gameBit;         /* 0x0A: set when the object is destroyed */
    u16 triggerDistance; /* 0x0C: planar attack-trigger distance */
    u16 arcHalfAngle;    /* 0x0E: facing-arc half-angle */
    u16 prevDistance;    /* 0x10: previous planar player distance */
    u8 flags;            /* 0x12: primed, dead, and forced-attack flags */
    u8 hitsLeft;         /* 0x13 */
    u8 attackChance;     /* 0x14: attack probability percentage */
    u8 aimHeightY;       /* 0x15: vertical projectile-aim offset */
    u8 pad16[2];         /* 0x16 */
} ChukChukState;

STATIC_ASSERT(offsetof(ChukChukPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(ChukChukPlacement, gameBit) == 0x18);
STATIC_ASSERT(offsetof(ChukChukPlacement, unk22) == 0x22);
STATIC_ASSERT(offsetof(ChukChukPlacement, aimHeightY) == 0x27);
STATIC_ASSERT(offsetof(ChukChukPlacement, arcHalfAngleScale) == 0x28);
STATIC_ASSERT(offsetof(ChukChukPlacement, triggerDistanceScale) == 0x29);
STATIC_ASSERT(offsetof(ChukChukPlacement, rotX) == 0x2A);
STATIC_ASSERT(offsetof(ChukChukPlacement, attackChance) == 0x2F);
STATIC_ASSERT(offsetof(ChukChukPlacement, hitsLeft) == 0x32);
STATIC_ASSERT(sizeof(ChukChukPlacement) == 0x38);
STATIC_ASSERT(offsetof(ChukChukState, glowPhase) == 0x0);
STATIC_ASSERT(offsetof(ChukChukState, steamTimer) == 0x4);
STATIC_ASSERT(offsetof(ChukChukState, unk08) == 0x8);
STATIC_ASSERT(offsetof(ChukChukState, gameBit) == 0xA);
STATIC_ASSERT(offsetof(ChukChukState, triggerDistance) == 0xC);
STATIC_ASSERT(offsetof(ChukChukState, arcHalfAngle) == 0xE);
STATIC_ASSERT(offsetof(ChukChukState, prevDistance) == 0x10);
STATIC_ASSERT(offsetof(ChukChukState, flags) == 0x12);
STATIC_ASSERT(offsetof(ChukChukState, hitsLeft) == 0x13);
STATIC_ASSERT(offsetof(ChukChukState, attackChance) == 0x14);
STATIC_ASSERT(offsetof(ChukChukState, aimHeightY) == 0x15);
STATIC_ASSERT(sizeof(ChukChukState) == 0x18);

void chukChuk_spawnAimedIceBall(GameObject* obj);
void ChukChuk_handleMessage(GameObject* obj, int message);
int ChukChuk_getExtraSize(void);
int ChukChuk_getObjectTypeId(void);
void ChukChuk_free(GameObject* obj);
void ChukChuk_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void ChukChuk_hitDetect(GameObject* obj);
void ChukChuk_update(GameObject* obj);
void ChukChuk_init(GameObject* obj, ChukChukPlacement* placement);
void ChukChuk_release(void);
void ChukChuk_initialise(void);

extern u8 gChukChukGlowTextureRamp[16];
extern ObjectDescriptor11WithPadding gChukChukObjDescriptor;

#endif /* DLLS_OBJECTS_204_CHUKCHUK_H_ */
