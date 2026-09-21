#ifndef DLLS_OBJECTS_299_FXEMIT_H_
#define DLLS_OBJECTS_299_FXEMIT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef enum FXEmitEffectBank {
    FXEMIT_EFFECT_BANK_PARTICLE = 0,
    FXEMIT_EFFECT_BANK_MODEL = 1,
    FXEMIT_EFFECT_BANK_PROJECTILE = 2,
} FXEmitEffectBank;

typedef enum FXEmitFlagPreset {
    FXEMIT_FLAG_PRESET_0 = 0,
    FXEMIT_FLAG_PRESET_1 = 1,
    FXEMIT_FLAG_PRESET_2 = 2,
    FXEMIT_FLAG_PRESET_NONE = 3,
} FXEmitFlagPreset;

#define FXEMIT_ROTATION_SPEED_AUTO 0x7F
#define FXEMIT_INTERVAL_DISABLED   0xFF

/* Retail EN romlists prove that all 189 placements use this complete 0x2C-byte layout. */
typedef struct FXEmitPlacement {
    ObjPlacement base;  /* 0x00 */
    s8 activationRange; /* 0x18: multiplied by four */
    s8 effectBank;      /* 0x19: FXEmitEffectBank */
    s16 effectId;       /* 0x1A: index within the selected bank */
    s16 emitRate;       /* 0x1C: positive count or negated cooldown */
    s16 toggleGameBit;  /* 0x1E: -1 leaves the emitter enabled */
    s16 disableGameBit; /* 0x20: -1 means there is no disable gate */
    s8 initialRoll;     /* 0x22: shifted left by eight */
    s8 initialPitch;    /* 0x23: shifted left by eight */
    s8 initialYaw;      /* 0x24: shifted left by eight */
    s8 rollSpeed;       /* 0x25: 0x7F selects the default speed */
    s8 pitchSpeed;      /* 0x26: 0x7F selects the default speed */
    s8 yawSpeed;        /* 0x27: 0x7F selects the default speed */
    u8 flagPreset;      /* 0x28: FXEmitFlagPreset */
    u8 interval;        /* 0x29: 0 skips; 0xFF disables emission */
    s16 intervalSfxId;  /* 0x2A */
} FXEmitPlacement;

/* FXEmit_getExtraSize proves the complete 0x20-byte runtime allocation. */
typedef struct FXEmitState {
    f32 activationRange;   /* 0x00 */
    f32 initialX;          /* 0x04: initialized but otherwise unused by this TU */
    s16 effectBank;        /* 0x08: FXEmitEffectBank */
    s16 effectId;          /* 0x0A */
    s16 alternateEffectId; /* 0x0C: used by the world-positioned fallback */
    s16 emitRate;          /* 0x0E */
    u8 pad10[2];           /* 0x10 */
    s16 randomDelay;       /* 0x12 */
    s16 toggleGameBit;     /* 0x14 */
    s16 disableGameBit;    /* 0x16 */
    s16 disabled;          /* 0x18 */
    s16 intervalTimer;     /* 0x1A */
    u8 sequenceRotate;     /* 0x1C: toggled by animation event 2 */
    u8 pad1D[3];           /* 0x1D */
} FXEmitState;

STATIC_ASSERT(offsetof(FXEmitPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(FXEmitPlacement, activationRange) == 0x18);
STATIC_ASSERT(offsetof(FXEmitPlacement, effectBank) == 0x19);
STATIC_ASSERT(offsetof(FXEmitPlacement, effectId) == 0x1A);
STATIC_ASSERT(offsetof(FXEmitPlacement, emitRate) == 0x1C);
STATIC_ASSERT(offsetof(FXEmitPlacement, toggleGameBit) == 0x1E);
STATIC_ASSERT(offsetof(FXEmitPlacement, disableGameBit) == 0x20);
STATIC_ASSERT(offsetof(FXEmitPlacement, initialRoll) == 0x22);
STATIC_ASSERT(offsetof(FXEmitPlacement, initialPitch) == 0x23);
STATIC_ASSERT(offsetof(FXEmitPlacement, initialYaw) == 0x24);
STATIC_ASSERT(offsetof(FXEmitPlacement, rollSpeed) == 0x25);
STATIC_ASSERT(offsetof(FXEmitPlacement, pitchSpeed) == 0x26);
STATIC_ASSERT(offsetof(FXEmitPlacement, yawSpeed) == 0x27);
STATIC_ASSERT(offsetof(FXEmitPlacement, flagPreset) == 0x28);
STATIC_ASSERT(offsetof(FXEmitPlacement, interval) == 0x29);
STATIC_ASSERT(offsetof(FXEmitPlacement, intervalSfxId) == 0x2A);
STATIC_ASSERT(sizeof(FXEmitPlacement) == 0x2C);

STATIC_ASSERT(offsetof(FXEmitState, activationRange) == 0x00);
STATIC_ASSERT(offsetof(FXEmitState, initialX) == 0x04);
STATIC_ASSERT(offsetof(FXEmitState, effectBank) == 0x08);
STATIC_ASSERT(offsetof(FXEmitState, effectId) == 0x0A);
STATIC_ASSERT(offsetof(FXEmitState, alternateEffectId) == 0x0C);
STATIC_ASSERT(offsetof(FXEmitState, emitRate) == 0x0E);
STATIC_ASSERT(offsetof(FXEmitState, pad10) == 0x10);
STATIC_ASSERT(offsetof(FXEmitState, randomDelay) == 0x12);
STATIC_ASSERT(offsetof(FXEmitState, toggleGameBit) == 0x14);
STATIC_ASSERT(offsetof(FXEmitState, disableGameBit) == 0x16);
STATIC_ASSERT(offsetof(FXEmitState, disabled) == 0x18);
STATIC_ASSERT(offsetof(FXEmitState, intervalTimer) == 0x1A);
STATIC_ASSERT(offsetof(FXEmitState, sequenceRotate) == 0x1C);
STATIC_ASSERT(offsetof(FXEmitState, pad1D) == 0x1D);
STATIC_ASSERT(sizeof(FXEmitState) == 0x20);

void FXEmit_emitEffect(GameObject* obj);
int FXEmit_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int FXEmit_getExtraSize(void);
int FXEmit_getObjectTypeId(void);
void FXEmit_free(GameObject* obj);
void FXEmit_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void FXEmit_hitDetect(void);
void FXEmit_update(GameObject* obj);
void FXEmit_init(GameObject* obj, FXEmitPlacement* placement);
void FXEmit_release(void);
void FXEmit_initialise(void);

extern ObjectDescriptor gFXEmitObjDescriptor;

#endif /* DLLS_OBJECTS_299_FXEMIT_H_ */
