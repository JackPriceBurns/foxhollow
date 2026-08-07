#ifndef DLLS_OBJECTS_229_SHIELD_H_
#define DLLS_OBJECTS_229_SHIELD_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/modellight_api.h"

#define SHIELD_SEGMENT_COUNT       4
#define SHIELD_SEGMENT_TABLE_COUNT 16

typedef enum ShieldMode {
    SHIELD_MODE_STANDARD_FADE_OUT_SHORT = 0,
    SHIELD_MODE_STANDARD_FADE_IN_SHORT = 1,
    SHIELD_MODE_STANDARD_FADE_OUT_LONG = 2,
    SHIELD_MODE_STANDARD_FADE_IN_LONG = 3,
    SHIELD_MODE_OMNI_ACTIVE = 4,
    SHIELD_MODE_INIT_OMNI = 5,
    SHIELD_MODE_OMNI_HIT = 6,
    SHIELD_MODE_INIT_STANDARD = 7,
} ShieldMode;

typedef struct ShieldState {
    ModelLightStruct* light;                /* 0x00 */
    f32 fadeValue;                          /* 0x04 */
    f32 fadeTarget;                         /* 0x08 */
    f32 fadeRate;                           /* 0x0C */
    f32 fadeMax;                            /* 0x10 */
    f32 segmentAlpha[SHIELD_SEGMENT_COUNT]; /* 0x14 */
    f32 segmentScale[SHIELD_SEGMENT_COUNT]; /* 0x24 */
    s16 segmentPhase[SHIELD_SEGMENT_COUNT]; /* 0x34 */
    s16 segmentRate[SHIELD_SEGMENT_COUNT];  /* 0x3C */
    s16 segmentRotX[SHIELD_SEGMENT_COUNT];  /* 0x44 */
    s16 segmentRotY[SHIELD_SEGMENT_COUNT];  /* 0x4C */
    s16 segmentRotZ[SHIELD_SEGMENT_COUNT];  /* 0x54 */
    u8 segmentFlags[SHIELD_SEGMENT_COUNT];  /* 0x5C */
} ShieldState;

STATIC_ASSERT(offsetof(ShieldState, light) == 0x0);
STATIC_ASSERT(offsetof(ShieldState, fadeValue) == 0x4);
STATIC_ASSERT(offsetof(ShieldState, fadeTarget) == 0x8);
STATIC_ASSERT(offsetof(ShieldState, fadeRate) == 0xC);
STATIC_ASSERT(offsetof(ShieldState, fadeMax) == 0x10);
STATIC_ASSERT(offsetof(ShieldState, segmentAlpha) == 0x14);
STATIC_ASSERT(offsetof(ShieldState, segmentScale) == 0x24);
STATIC_ASSERT(offsetof(ShieldState, segmentPhase) == 0x34);
STATIC_ASSERT(offsetof(ShieldState, segmentRate) == 0x3C);
STATIC_ASSERT(offsetof(ShieldState, segmentRotX) == 0x44);
STATIC_ASSERT(offsetof(ShieldState, segmentRotY) == 0x4C);
STATIC_ASSERT(offsetof(ShieldState, segmentRotZ) == 0x54);
STATIC_ASSERT(offsetof(ShieldState, segmentFlags) == 0x5C);
STATIC_ASSERT(sizeof(ShieldState) == 0x60);

GameObject* Shield_spawnOmniShield(GameObject* obj, f32 rootMotionScale);
void Shield_setMode(GameObject* obj, u8 mode);
int Shield_getExtraSize(void);
int Shield_getObjectTypeId(void);
void Shield_free(GameObject* obj);
void Shield_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void Shield_hitDetect(GameObject* obj);
void Shield_update(GameObject* obj);
void Shield_init(GameObject* obj, void* unused);
void Shield_release(void);
void Shield_initialise(void);

extern ObjectDescriptor gShieldObjDescriptor;
extern f32 gShieldSegmentTable[SHIELD_SEGMENT_TABLE_COUNT];
extern s16 gShieldRotXRates[SHIELD_SEGMENT_COUNT];
extern s16 gOmniShieldRotXRates[SHIELD_SEGMENT_COUNT];
extern s16 gOmniShieldRotYRates[SHIELD_SEGMENT_COUNT];
extern s16 gOmniShieldRotZRates[SHIELD_SEGMENT_COUNT];

#endif /* DLLS_OBJECTS_229_SHIELD_H_ */
