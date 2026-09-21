#ifndef DLLS_OBJECTS_250_INVISIBLEHI_H_
#define DLLS_OBJECTS_250_INVISIBLEHI_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct InvisibleHitSwitchPlacement {
    ObjPlacement base;        /* 0x00 */
    s16 gameBitId;            /* 0x18 */
    s16 autoResetDelayTenths; /* 0x1A */
    u8 pad1C;                 /* 0x1C */
    u8 radiusScale64;         /* 0x1D: radius/scale in 1/64 units */
    u8 mode;                  /* 0x1E */
    u8 pad1F;                 /* 0x1F */
    u8 pad20[3];              /* 0x20 */
    u8 hitPriorityType;       /* 0x23 */
} InvisibleHitSwitchPlacement;

typedef struct InvisibleHitSwitchState {
    u8 isOn;                  /* 0x00 */
    u8 hitPriority;           /* 0x01 */
    u8 pad02[2];              /* 0x02 */
    f32 autoResetTimerFrames; /* 0x04 */
    f32 delayedTriggerTimer;  /* 0x08 */
} InvisibleHitSwitchState;

enum {
    INVISIBLE_HIT_SWITCH_MODE_MASK = 3,
    INVISIBLE_HIT_SWITCH_MODE_LATCH = 0,
    INVISIBLE_HIT_SWITCH_MODE_TOGGLE = 1,
    INVISIBLE_HIT_SWITCH_MODE_TIMED_RESET = 2,
    INVISIBLE_HIT_SWITCH_MODE_DELAYED = 3
};

STATIC_ASSERT(offsetof(InvisibleHitSwitchPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(InvisibleHitSwitchPlacement, gameBitId) == 0x18);
STATIC_ASSERT(offsetof(InvisibleHitSwitchPlacement, autoResetDelayTenths) == 0x1A);
STATIC_ASSERT(offsetof(InvisibleHitSwitchPlacement, pad1C) == 0x1C);
STATIC_ASSERT(offsetof(InvisibleHitSwitchPlacement, radiusScale64) == 0x1D);
STATIC_ASSERT(offsetof(InvisibleHitSwitchPlacement, mode) == 0x1E);
STATIC_ASSERT(offsetof(InvisibleHitSwitchPlacement, pad1F) == 0x1F);
STATIC_ASSERT(offsetof(InvisibleHitSwitchPlacement, pad20) == 0x20);
STATIC_ASSERT(offsetof(InvisibleHitSwitchPlacement, hitPriorityType) == 0x23);
STATIC_ASSERT(sizeof(InvisibleHitSwitchPlacement) == 0x24);

STATIC_ASSERT(offsetof(InvisibleHitSwitchState, isOn) == 0x0);
STATIC_ASSERT(offsetof(InvisibleHitSwitchState, hitPriority) == 0x1);
STATIC_ASSERT(offsetof(InvisibleHitSwitchState, pad02) == 0x2);
STATIC_ASSERT(offsetof(InvisibleHitSwitchState, autoResetTimerFrames) == 0x4);
STATIC_ASSERT(offsetof(InvisibleHitSwitchState, delayedTriggerTimer) == 0x8);
STATIC_ASSERT(sizeof(InvisibleHitSwitchState) == 0xC);

int InvisibleHitSwitch_getExtraSize(void);
void InvisibleHitSwitch_update(GameObject* obj);
void InvisibleHitSwitch_init(GameObject* obj, InvisibleHitSwitchPlacement* placement);

extern ObjectDescriptor gInvisibleHitSwitchObjDescriptor;

#endif /* DLLS_OBJECTS_250_INVISIBLEHI_H_ */
