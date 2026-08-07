#ifndef DLLS_OBJECTS_301_LFXEMITTER_H_
#define DLLS_OBJECTS_301_LFXEMITTER_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/curve_walker.h"

typedef struct LFXEmitterLightAction LFXEmitterLightAction;

typedef enum LFXEmitterFlag {
    LFXEMITTER_FLAG_FOLLOW_CURVE = 0x01,
    LFXEMITTER_FLAG_DAMP_Y_VELOCITY = 0x02,
} LFXEmitterFlag;

#define LFXEMITTER_GAME_BIT_NONE -1

/*
 * Both retail EN placements use the complete fixed-width 0x28-byte record.
 */
typedef struct LFXEmitterPlacement {
    ObjPlacement base; /* 0x00 */
    s16 spinRoll;      /* 0x18 */
    s16 spinPitch;     /* 0x1A */
    s16 spinYaw;       /* 0x1C */
    s16 actionIndex;   /* 0x1E: LACTIONS.bin row */
    s16 lifeTimer;     /* 0x20: frames; zero disables the timer */
    s16 enableGameBit; /* 0x22: LFXEMITTER_GAME_BIT_NONE means no gate */
    u8 followCurve;    /* 0x24 */
    s8 curveSpeed;     /* 0x25: divided by ten */
    u8 pad26[2];       /* 0x26 */
} LFXEmitterPlacement;

/* LFXEmitter_getExtraSize proves the complete 0x124-byte runtime allocation. */
typedef struct LFXEmitterState {
    RomCurveWalker curve;               /* 0x000 */
    LFXEmitterLightAction* lightAction; /* 0x108: allocated LACTIONS.bin row */
    f32 curveSpeed;                     /* 0x10C */
    s16 lifeTimer;                      /* 0x110 */
    s16 actionIndex;                    /* 0x112 */
    s16 unk114;                         /* 0x114: initialized to -2 */
    s16 enableGameBit;                  /* 0x116 */
    s16 spinRoll;                       /* 0x118 */
    s16 spinPitch;                      /* 0x11A */
    s16 spinYaw;                        /* 0x11C */
    u8 lifeTimerActive;                 /* 0x11E */
    u8 lightActionLoaded;               /* 0x11F */
    u8 flags;                           /* 0x120: LFXEmitterFlag */
    u8 pad121[3];                       /* 0x121 */
} LFXEmitterState;

STATIC_ASSERT(offsetof(LFXEmitterPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(LFXEmitterPlacement, spinRoll) == 0x18);
STATIC_ASSERT(offsetof(LFXEmitterPlacement, spinPitch) == 0x1A);
STATIC_ASSERT(offsetof(LFXEmitterPlacement, spinYaw) == 0x1C);
STATIC_ASSERT(offsetof(LFXEmitterPlacement, actionIndex) == 0x1E);
STATIC_ASSERT(offsetof(LFXEmitterPlacement, lifeTimer) == 0x20);
STATIC_ASSERT(offsetof(LFXEmitterPlacement, enableGameBit) == 0x22);
STATIC_ASSERT(offsetof(LFXEmitterPlacement, followCurve) == 0x24);
STATIC_ASSERT(offsetof(LFXEmitterPlacement, curveSpeed) == 0x25);
STATIC_ASSERT(offsetof(LFXEmitterPlacement, pad26) == 0x26);
STATIC_ASSERT(sizeof(LFXEmitterPlacement) == 0x28);

STATIC_ASSERT(offsetof(LFXEmitterState, curve) == 0x000);
STATIC_ASSERT(offsetof(LFXEmitterState, lightAction) == 0x108);
STATIC_ASSERT(offsetof(LFXEmitterState, curveSpeed) == 0x10C);
STATIC_ASSERT(offsetof(LFXEmitterState, lifeTimer) == 0x110);
STATIC_ASSERT(offsetof(LFXEmitterState, actionIndex) == 0x112);
STATIC_ASSERT(offsetof(LFXEmitterState, unk114) == 0x114);
STATIC_ASSERT(offsetof(LFXEmitterState, enableGameBit) == 0x116);
STATIC_ASSERT(offsetof(LFXEmitterState, spinRoll) == 0x118);
STATIC_ASSERT(offsetof(LFXEmitterState, spinPitch) == 0x11A);
STATIC_ASSERT(offsetof(LFXEmitterState, spinYaw) == 0x11C);
STATIC_ASSERT(offsetof(LFXEmitterState, lifeTimerActive) == 0x11E);
STATIC_ASSERT(offsetof(LFXEmitterState, lightActionLoaded) == 0x11F);
STATIC_ASSERT(offsetof(LFXEmitterState, flags) == 0x120);
STATIC_ASSERT(offsetof(LFXEmitterState, pad121) == 0x121);
STATIC_ASSERT(sizeof(LFXEmitterState) == 0x124);

int LFXEmitter_isLightActionLoaded(GameObject* obj);
int LFXEmitter_func0A(void);
void LFXEmitter_copyLightAction(const LFXEmitterLightAction* source, LFXEmitterLightAction* destination);
int LFXEmitter_getExtraSize(void);
int LFXEmitter_getObjectTypeId(void);
void LFXEmitter_free(GameObject* obj);
void LFXEmitter_render(void);
void LFXEmitter_hitDetect(void);
void LFXEmitter_update(GameObject* obj);
void LFXEmitter_init(GameObject* obj, LFXEmitterPlacement* placement);
void LFXEmitter_release(void);
void LFXEmitter_initialise(void);

extern ObjectDescriptor12 gLFXEmitterObjDescriptor;

#endif /* DLLS_OBJECTS_301_LFXEMITTER_H_ */
