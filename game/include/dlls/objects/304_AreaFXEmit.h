#ifndef DLLS_OBJECTS_304_AREAFXEMIT_H_
#define DLLS_OBJECTS_304_AREAFXEMIT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

/*
 * All 56 retail EN placements across the AreaFXEmit and TAreaFXEmit
 * families use this complete fixed-width 0x28-byte record.
 */
typedef struct AreaFXEmitPlacement {
    ObjPlacement base; /* 0x00 */
    s8 triggerRadius;  /* 0x18: multiplied by four; zero always emits */
    s8 initialRotZ;    /* 0x19: shifted left by eight */
    s8 initialRotY;    /* 0x1A: shifted left by eight */
    s8 initialRotX;    /* 0x1B: shifted left by eight */
    u8 extentX;        /* 0x1C: multiplied by four */
    u8 extentZ;        /* 0x1D: multiplied by four */
    u8 extentY;        /* 0x1E: multiplied by four */
    u8 emitType;       /* 0x1F */
    u16 effectId;      /* 0x20 */
    s16 emitCount;     /* 0x22: positive count or negated cooldown */
    s16 enableGameBit; /* 0x24: -1 means no enable gate */
    s16 stopGameBit;   /* 0x26: -1 bypasses the initial stop check */
} AreaFXEmitPlacement;

/* AreaFXEmit_getExtraSize proves the complete 0x20-byte allocation. */
typedef struct AreaFXEmitState {
    f32 triggerRadius; /* 0x00: zero always emits */
    f32 lastDistance;  /* 0x04 */
    u8 emitType;       /* 0x08 */
    u8 pad09;          /* 0x09 */
    u16 effectId;      /* 0x0A */
    s16 emitCount;     /* 0x0C: positive count or negated cooldown */
    s16 enableGameBit; /* 0x0E: -1 means no enable gate */
    s16 stopGameBit;   /* 0x10: -1 bypasses the initial stop check */
    s16 suppressed;    /* 0x12 */
    u16 extentX;       /* 0x14 */
    u16 extentZ;       /* 0x16 */
    u16 extentY;       /* 0x18 */
    s16 emitAngles[3]; /* 0x1A: X, Y, Z */
} AreaFXEmitState;

STATIC_ASSERT(offsetof(AreaFXEmitPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, triggerRadius) == 0x18);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, initialRotZ) == 0x19);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, initialRotY) == 0x1A);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, initialRotX) == 0x1B);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, extentX) == 0x1C);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, extentZ) == 0x1D);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, extentY) == 0x1E);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, emitType) == 0x1F);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, effectId) == 0x20);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, emitCount) == 0x22);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, enableGameBit) == 0x24);
STATIC_ASSERT(offsetof(AreaFXEmitPlacement, stopGameBit) == 0x26);
STATIC_ASSERT(sizeof(AreaFXEmitPlacement) == 0x28);

STATIC_ASSERT(offsetof(AreaFXEmitState, triggerRadius) == 0x00);
STATIC_ASSERT(offsetof(AreaFXEmitState, lastDistance) == 0x04);
STATIC_ASSERT(offsetof(AreaFXEmitState, emitType) == 0x08);
STATIC_ASSERT(offsetof(AreaFXEmitState, pad09) == 0x09);
STATIC_ASSERT(offsetof(AreaFXEmitState, effectId) == 0x0A);
STATIC_ASSERT(offsetof(AreaFXEmitState, emitCount) == 0x0C);
STATIC_ASSERT(offsetof(AreaFXEmitState, enableGameBit) == 0x0E);
STATIC_ASSERT(offsetof(AreaFXEmitState, stopGameBit) == 0x10);
STATIC_ASSERT(offsetof(AreaFXEmitState, suppressed) == 0x12);
STATIC_ASSERT(offsetof(AreaFXEmitState, extentX) == 0x14);
STATIC_ASSERT(offsetof(AreaFXEmitState, extentZ) == 0x16);
STATIC_ASSERT(offsetof(AreaFXEmitState, extentY) == 0x18);
STATIC_ASSERT(offsetof(AreaFXEmitState, emitAngles) == 0x1A);
STATIC_ASSERT(sizeof(AreaFXEmitState) == 0x20);

void AreaFXEmit_emitBurst(GameObject* obj, int count);
void AreaFXEmit_emitEffect(GameObject* obj);
int AreaFXEmit_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int AreaFXEmit_getExtraSize(void);
int AreaFXEmit_getObjectTypeId(void);
void AreaFXEmit_free(GameObject* obj);
void AreaFXEmit_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void AreaFXEmit_hitDetect(void);
void AreaFXEmit_update(GameObject* obj);
void AreaFXEmit_init(GameObject* obj, AreaFXEmitPlacement* placement);
void AreaFXEmit_release(void);
void AreaFXEmit_initialise(void);

extern ObjectDescriptor gAreaFXEmitObjDescriptor;

#endif /* DLLS_OBJECTS_304_AREAFXEMIT_H_ */
