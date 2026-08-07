#ifndef DLLS_OBJECTS_279_APPLE_ON_TREE_H_
#define DLLS_OBJECTS_279_APPLE_ON_TREE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define APPLE_ON_TREE_OBJECT_ID      0x210
#define APPLE_ON_TREE_PLACEMENT_SIZE 0x28
#define APPLE_ON_TREE_STATE_SIZE     0x64

typedef struct AppleOnTreePlacement {
    ObjPlacement base;      /* 0x00 */
    u32 unk18;              /* 0x18 */
    u16 phaseDuration;      /* 0x1C */
    u16 initialElapsedTime; /* 0x1E */
    u8 growthEndFraction;   /* 0x20 */
    u8 ripeEndFraction;     /* 0x21 */
    u8 fallEndFraction;     /* 0x22 */
    u8 landedEndFraction;   /* 0x23 */
    u8 fadeEndFraction;     /* 0x24 */
    s8 waterAccelerationPercent; /* 0x25 */
    s16 despawnGameBit;     /* 0x26: or -1 */
} AppleOnTreePlacement;

typedef struct AppleOnTreeState {
    u32 unk00;                 /* 0x00 */
    f32 phaseDuration;         /* 0x04 */
    f32 elapsedTime;           /* 0x08 */
    f32 flightTime;            /* 0x0C */
    f32 growthEnd;             /* 0x10 */
    f32 ripeEnd;               /* 0x14 */
    f32 fallEnd;               /* 0x18 */
    f32 landedEnd;             /* 0x1C */
    f32 fadeEnd;               /* 0x20 */
    f32 fallScale;             /* 0x24 */
    f32 waterAcceleration;     /* 0x28 */
    f32 positionY;             /* 0x2C */
    f32 dropHeight;            /* 0x30 */
    f32 splashPositionY;       /* 0x34 */
    u16 healthRestore;         /* 0x38 */
    u8 animState;              /* 0x3A: AppleOnTreeAnimState */
    u8 pad3B;                  /* 0x3B */
    f32 extraAcceleration;     /* 0x3C */
    f32 gravity;               /* 0x40 */
    f32 bounceVelocity;        /* 0x44 */
    s16 rotX;                  /* 0x48 */
    s16 rotY;                  /* 0x4A */
    s16 rotZ;                  /* 0x4C */
    u8 pad4E[2];               /* 0x4E */
    f32 totalFlightTime;       /* 0x50 */
    f32 fallBlendDivisor;      /* 0x54 */
    u8 pad58[2];               /* 0x58 */
    u8 flags;                  /* 0x5A: APPLE_ON_TREE_FLAG_* */
    u8 pad5B;                  /* 0x5B */
    s16 triggerGameBit;        /* 0x5C: head of the player pickup-message payload */
    s16 pickupMessageValue;    /* 0x5E */
    f32 pickupMessageArgument; /* 0x60 */
} AppleOnTreeState;

STATIC_ASSERT(offsetof(AppleOnTreePlacement, base) == 0x0);
STATIC_ASSERT(offsetof(AppleOnTreePlacement, unk18) == 0x18);
STATIC_ASSERT(offsetof(AppleOnTreePlacement, phaseDuration) == 0x1C);
STATIC_ASSERT(offsetof(AppleOnTreePlacement, initialElapsedTime) == 0x1E);
STATIC_ASSERT(offsetof(AppleOnTreePlacement, growthEndFraction) == 0x20);
STATIC_ASSERT(offsetof(AppleOnTreePlacement, ripeEndFraction) == 0x21);
STATIC_ASSERT(offsetof(AppleOnTreePlacement, fallEndFraction) == 0x22);
STATIC_ASSERT(offsetof(AppleOnTreePlacement, landedEndFraction) == 0x23);
STATIC_ASSERT(offsetof(AppleOnTreePlacement, fadeEndFraction) == 0x24);
STATIC_ASSERT(offsetof(AppleOnTreePlacement, waterAccelerationPercent) == 0x25);
STATIC_ASSERT(offsetof(AppleOnTreePlacement, despawnGameBit) == 0x26);
STATIC_ASSERT(sizeof(AppleOnTreePlacement) == APPLE_ON_TREE_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(AppleOnTreeState, unk00) == 0x0);
STATIC_ASSERT(offsetof(AppleOnTreeState, phaseDuration) == 0x4);
STATIC_ASSERT(offsetof(AppleOnTreeState, elapsedTime) == 0x8);
STATIC_ASSERT(offsetof(AppleOnTreeState, flightTime) == 0xC);
STATIC_ASSERT(offsetof(AppleOnTreeState, growthEnd) == 0x10);
STATIC_ASSERT(offsetof(AppleOnTreeState, ripeEnd) == 0x14);
STATIC_ASSERT(offsetof(AppleOnTreeState, fallEnd) == 0x18);
STATIC_ASSERT(offsetof(AppleOnTreeState, landedEnd) == 0x1C);
STATIC_ASSERT(offsetof(AppleOnTreeState, fadeEnd) == 0x20);
STATIC_ASSERT(offsetof(AppleOnTreeState, fallScale) == 0x24);
STATIC_ASSERT(offsetof(AppleOnTreeState, waterAcceleration) == 0x28);
STATIC_ASSERT(offsetof(AppleOnTreeState, positionY) == 0x2C);
STATIC_ASSERT(offsetof(AppleOnTreeState, dropHeight) == 0x30);
STATIC_ASSERT(offsetof(AppleOnTreeState, splashPositionY) == 0x34);
STATIC_ASSERT(offsetof(AppleOnTreeState, healthRestore) == 0x38);
STATIC_ASSERT(offsetof(AppleOnTreeState, animState) == 0x3A);
STATIC_ASSERT(offsetof(AppleOnTreeState, pad3B) == 0x3B);
STATIC_ASSERT(offsetof(AppleOnTreeState, extraAcceleration) == 0x3C);
STATIC_ASSERT(offsetof(AppleOnTreeState, gravity) == 0x40);
STATIC_ASSERT(offsetof(AppleOnTreeState, bounceVelocity) == 0x44);
STATIC_ASSERT(offsetof(AppleOnTreeState, rotX) == 0x48);
STATIC_ASSERT(offsetof(AppleOnTreeState, rotY) == 0x4A);
STATIC_ASSERT(offsetof(AppleOnTreeState, rotZ) == 0x4C);
STATIC_ASSERT(offsetof(AppleOnTreeState, pad4E) == 0x4E);
STATIC_ASSERT(offsetof(AppleOnTreeState, totalFlightTime) == 0x50);
STATIC_ASSERT(offsetof(AppleOnTreeState, fallBlendDivisor) == 0x54);
STATIC_ASSERT(offsetof(AppleOnTreeState, pad58) == 0x58);
STATIC_ASSERT(offsetof(AppleOnTreeState, flags) == 0x5A);
STATIC_ASSERT(offsetof(AppleOnTreeState, pad5B) == 0x5B);
STATIC_ASSERT(offsetof(AppleOnTreeState, triggerGameBit) == 0x5C);
STATIC_ASSERT(offsetof(AppleOnTreeState, pickupMessageValue) == 0x5E);
STATIC_ASSERT(offsetof(AppleOnTreeState, pickupMessageArgument) == 0x60);
STATIC_ASSERT(sizeof(AppleOnTreeState) == APPLE_ON_TREE_STATE_SIZE);

/* gAppleOnTreeObjDescriptor from slot02 onwards: the export table other
   objects reach through obj->anim.dll. */
typedef struct AppleOnTreeInterface {
    void* pad00[9];
    void (*setPosition)(GameObject* obj, f32* position);
    int (*getAnimState)(GameObject* obj);
} AppleOnTreeInterface;

#define APPLE_ON_TREE_INTERFACE(apple) ((AppleOnTreeInterface*)*((GameObject*)(apple))->anim.dll)

STATIC_ASSERT(offsetof(AppleOnTreeInterface, setPosition) == 0x24);
STATIC_ASSERT(offsetof(AppleOnTreeInterface, getAnimState) == 0x28);

void AppleOnTree_setPosition(GameObject* obj, f32* position);
void appleontree_knockLoose(GameObject* obj, int message);
void appleontree_handleCollectableHit(GameObject* obj);
u8 AppleOnTree_getAnimState(GameObject* obj);
void AppleOnTree_func0A(void);
int AppleOnTree_getExtraSize(void);
void AppleOnTree_free(GameObject* obj);
void AppleOnTree_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
int appleontree_bounceGroundStep(GameObject* obj, AppleOnTreeState* state, f32 positionY);
int appleontree_bounceWaterStep(GameObject* obj, AppleOnTreeState* state, f32 positionY);
void AppleOnTree_update(GameObject* obj);
void AppleOnTree_init(GameObject* obj, AppleOnTreePlacement* placement);

extern ObjectDescriptor13 gAppleOnTreeObjDescriptor;

#endif /* DLLS_OBJECTS_279_APPLE_ON_TREE_H_ */
