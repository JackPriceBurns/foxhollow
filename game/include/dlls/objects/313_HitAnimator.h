#ifndef DLLS_OBJECTS_313_HITANIMATOR_H_
#define DLLS_OBJECTS_313_HITANIMATOR_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct MapBlockData MapBlockData;

typedef enum HitAnimatorSetupFlag {
    HIT_ANIMATOR_SETUP_INITIAL_INVERT = 0x01,
    HIT_ANIMATOR_SETUP_AFFECT_SHADERS = 0x02,
    HIT_ANIMATOR_SETUP_BLOCK_UPDATE = 0x04,
    HIT_ANIMATOR_SETUP_HIT_LINES = 0x08,
    HIT_ANIMATOR_SETUP_SKIP_POLYGONS = 0x10,
} HitAnimatorSetupFlag;

typedef enum HitAnimatorStateFlag {
    HIT_ANIMATOR_STATE_TOGGLE_PENDING = 0x01,
    HIT_ANIMATOR_STATE_HIT_LINES_PENDING = 0x02,
    HIT_ANIMATOR_STATE_BLOCK_UPDATE_PENDING = 0x04,
} HitAnimatorStateFlag;

typedef struct HitAnimatorPlacement {
    ObjPlacement base; /* 0x00 */
    s16 gameBit;       /* 0x18 */
    u8 toggleMode;     /* 0x1A */
    u8 blockEffectId;  /* 0x1B */
    u8 setupFlags;     /* 0x1C: HitAnimatorSetupFlag */
    u8 hitLineParam;   /* 0x1D */
    u8 pad1E[2];       /* 0x1E */
} HitAnimatorPlacement;

/* HitAnimator_getExtraSize proves the complete 0x04-byte allocation. */
typedef struct HitAnimatorState {
    s8 active;               /* 0x00 */
    u8 flags;                /* 0x01: HitAnimatorStateFlag */
    u8 gameBitValue;         /* 0x02 */
    u8 previousGameBitValue; /* 0x03 */
} HitAnimatorState;

STATIC_ASSERT(offsetof(HitAnimatorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(HitAnimatorPlacement, gameBit) == 0x18);
STATIC_ASSERT(offsetof(HitAnimatorPlacement, toggleMode) == 0x1A);
STATIC_ASSERT(offsetof(HitAnimatorPlacement, blockEffectId) == 0x1B);
STATIC_ASSERT(offsetof(HitAnimatorPlacement, setupFlags) == 0x1C);
STATIC_ASSERT(offsetof(HitAnimatorPlacement, hitLineParam) == 0x1D);
STATIC_ASSERT(offsetof(HitAnimatorPlacement, pad1E) == 0x1E);
STATIC_ASSERT(sizeof(HitAnimatorPlacement) == 0x20);

STATIC_ASSERT(offsetof(HitAnimatorState, active) == 0x00);
STATIC_ASSERT(offsetof(HitAnimatorState, flags) == 0x01);
STATIC_ASSERT(offsetof(HitAnimatorState, gameBitValue) == 0x02);
STATIC_ASSERT(offsetof(HitAnimatorState, previousGameBitValue) == 0x03);
STATIC_ASSERT(sizeof(HitAnimatorState) == 0x04);

void HitAnimator_applyBlockState(MapBlockData* block, GameObject* obj, HitAnimatorState* state,
                                 HitAnimatorPlacement* placement);
int HitAnimator_getExtraSize(void);
void HitAnimator_update(GameObject* obj);
void HitAnimator_init(GameObject* obj, HitAnimatorPlacement* placement);

extern ObjectDescriptor gHitAnimatorObjDescriptor;

#endif /* DLLS_OBJECTS_313_HITANIMATOR_H_ */
