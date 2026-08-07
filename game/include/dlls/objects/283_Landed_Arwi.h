#ifndef DLLS_OBJECTS_283_LANDED_ARWI_H_
#define DLLS_OBJECTS_283_LANDED_ARWI_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define LANDED_ARWING_OBJECT_STATE_SIZE       0x1C
#define LANDED_ARWING_HIT_REACTION_STATE_SIZE 0x24

typedef struct ObjSeqState ObjSeqState;

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct LandedArwingPlacement {
    ObjPlacement base;  /* 0x00 */
    u8 pad18[4];        /* 0x18 */
    s16 triggerGameBit; /* 0x1C */
} LandedArwingPlacement;

typedef struct LandedArwingObjectState {
    f32 sequenceHitCooldown; /* 0x00 */
    f32 path7EffectStrength; /* 0x04 */
    f32 path8EffectStrength; /* 0x08 */
    f32 path6EffectStrength; /* 0x0C */
    GameObject* childObject; /* 0x10 */
    u8 pad14[2];             /* 0x14 */
    u8 sequenceState;        /* 0x16 */
    u8 pad17[3];             /* 0x17 */
    u8 pathEffectsEnabled;   /* 0x1A */
    u8 pad1B;                /* 0x1B */
} LandedArwingObjectState;

typedef struct LandedArwingHitFlags {
    u8 damaged : 1;
    u8 impactHandled : 1;
    u8 damageStateGameBitSet : 1;
    u8 reactionDone : 1;
    u8 pad : 4;
} LandedArwingHitFlags;

/*
 * Shared hit-reaction overlay used with slot 284's 0x24-byte state. This is
 * not the state allocated by the Landed_Arwi object itself.
 */
typedef struct LandedArwingHitReactionState {
    u8 pad00[8];                /* 0x00 */
    f32 animationStepScale;     /* 0x08 */
    u8 pad0C[0x10];             /* 0x0C */
    u8 hitStarted;              /* 0x1C */
    LandedArwingHitFlags flags; /* 0x1D */
    u8 pad1E[2];                /* 0x1E */
    f32 hitEffectCooldown;      /* 0x20 */
} LandedArwingHitReactionState;

STATIC_ASSERT(offsetof(LandedArwingPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(LandedArwingPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(LandedArwingPlacement, triggerGameBit) == 0x1C);

STATIC_ASSERT(offsetof(LandedArwingObjectState, sequenceHitCooldown) == 0x0);
STATIC_ASSERT(offsetof(LandedArwingObjectState, path7EffectStrength) == 0x4);
STATIC_ASSERT(offsetof(LandedArwingObjectState, path8EffectStrength) == 0x8);
STATIC_ASSERT(offsetof(LandedArwingObjectState, path6EffectStrength) == 0xC);
STATIC_ASSERT(offsetof(LandedArwingObjectState, childObject) == 0x10);
STATIC_ASSERT(offsetof(LandedArwingObjectState, pad14) == 0x14);
STATIC_ASSERT(offsetof(LandedArwingObjectState, sequenceState) == 0x16);
STATIC_ASSERT(offsetof(LandedArwingObjectState, pad17) == 0x17);
STATIC_ASSERT(offsetof(LandedArwingObjectState, pathEffectsEnabled) == 0x1A);
STATIC_ASSERT(offsetof(LandedArwingObjectState, pad1B) == 0x1B);
STATIC_ASSERT(sizeof(LandedArwingObjectState) == LANDED_ARWING_OBJECT_STATE_SIZE);

STATIC_ASSERT(sizeof(LandedArwingHitFlags) == 0x1);
STATIC_ASSERT(offsetof(LandedArwingHitReactionState, pad00) == 0x0);
STATIC_ASSERT(offsetof(LandedArwingHitReactionState, animationStepScale) == 0x8);
STATIC_ASSERT(offsetof(LandedArwingHitReactionState, pad0C) == 0xC);
STATIC_ASSERT(offsetof(LandedArwingHitReactionState, hitStarted) == 0x1C);
STATIC_ASSERT(offsetof(LandedArwingHitReactionState, flags) == 0x1D);
STATIC_ASSERT(offsetof(LandedArwingHitReactionState, pad1E) == 0x1E);
STATIC_ASSERT(offsetof(LandedArwingHitReactionState, hitEffectCooldown) == 0x20);
STATIC_ASSERT(sizeof(LandedArwingHitReactionState) == LANDED_ARWING_HIT_REACTION_STATE_SIZE);

void landed_arwing_renderPathEffects(GameObject* obj);
int landed_arwing_getExtraSize(void);
void landed_arwing_free(GameObject* obj);
void landed_arwing_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
int Landed_Arwing_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
void landed_arwing_update(GameObject* obj);
void landed_arwing_init(GameObject* obj, LandedArwingPlacement* placement);
void landed_arwing_updateHitReaction(GameObject* obj, LandedArwingHitReactionState* state);
void landed_arwing_updateDamageTexture(GameObject* obj, LandedArwingHitReactionState* state);

extern ObjectDescriptor gLanded_ArwingObjDescriptor;

#endif /* DLLS_OBJECTS_283_LANDED_ARWI_H_ */
