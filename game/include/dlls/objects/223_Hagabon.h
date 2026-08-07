#ifndef DLLS_OBJECTS_223_HAGABON_H_
#define DLLS_OBJECTS_223_HAGABON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/curve_walker.h"

typedef struct HagabonPlacement {
    ObjPlacement base;     /* 0x00; ident is the map-event ID */
    u8 pad18;              /* 0x18 */
    s8 chaseRadiusScale;   /* 0x19 */
    s16 curveStepRaw;      /* 0x1A */
    s16 timeReward;        /* 0x1C */
    s16 startInactive;     /* 0x1E */
    s16 armGameBit;        /* 0x20 */
    u8 pad22[0x28 - 0x22]; /* 0x22 */
} HagabonPlacement;

typedef struct HagabonState {
    RomCurveWalker* curve; /* 0x00 */
    GameObject* player;    /* 0x04 */
    f32 curveStep;         /* 0x08 */
    f32 animSpeed;         /* 0x0C */
    f32 playerDistance;    /* 0x10 */
    f32 pathDistance;      /* 0x14 */
    f32 chaseRadius;       /* 0x18 */
    u8 pad1C[4];           /* 0x1C */
    u16 wavePhaseA;        /* 0x20 */
    u16 wavePhaseB;        /* 0x22 */
    u16 wavePhaseC;        /* 0x24 */
    u8 flags;              /* 0x26 */
    u8 pad27;              /* 0x27 */
} HagabonState;

STATIC_ASSERT(offsetof(HagabonPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(HagabonPlacement, chaseRadiusScale) == 0x19);
STATIC_ASSERT(offsetof(HagabonPlacement, curveStepRaw) == 0x1A);
STATIC_ASSERT(offsetof(HagabonPlacement, timeReward) == 0x1C);
STATIC_ASSERT(offsetof(HagabonPlacement, startInactive) == 0x1E);
STATIC_ASSERT(offsetof(HagabonPlacement, armGameBit) == 0x20);
STATIC_ASSERT(sizeof(HagabonPlacement) == 0x28);

STATIC_ASSERT(offsetof(HagabonState, curve) == 0x0);
STATIC_ASSERT(offsetof(HagabonState, player) == 0x4);
STATIC_ASSERT(offsetof(HagabonState, curveStep) == 0x8);
STATIC_ASSERT(offsetof(HagabonState, animSpeed) == 0xC);
STATIC_ASSERT(offsetof(HagabonState, playerDistance) == 0x10);
STATIC_ASSERT(offsetof(HagabonState, pathDistance) == 0x14);
STATIC_ASSERT(offsetof(HagabonState, chaseRadius) == 0x18);
STATIC_ASSERT(offsetof(HagabonState, wavePhaseA) == 0x20);
STATIC_ASSERT(offsetof(HagabonState, wavePhaseB) == 0x22);
STATIC_ASSERT(offsetof(HagabonState, wavePhaseC) == 0x24);
STATIC_ASSERT(offsetof(HagabonState, flags) == 0x26);
STATIC_ASSERT(sizeof(HagabonState) == 0x28);

void Hagabon_updateMovement(GameObject* obj, HagabonState* state);
int Hagabon_getExtraSize(void);
int Hagabon_getObjectTypeId(void);
void Hagabon_free(GameObject* objAddress);
void Hagabon_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void Hagabon_hitDetect(GameObject* obj);
void Hagabon_update(GameObject* obj);
void Hagabon_init(GameObject* obj, HagabonPlacement* placement, int skipAlloc);
void Hagabon_release(void);
void Hagabon_initialise(void);

extern ObjectDescriptor gHagabonObjDescriptor;
extern int gHagabonCurveInitData[2];
extern int gHagabonLastCurvePoint;

#endif /* DLLS_OBJECTS_223_HAGABON_H_ */
