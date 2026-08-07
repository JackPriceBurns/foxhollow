#ifndef DLLS_OBJECTS_314_VISANIMATOR_H_
#define DLLS_OBJECTS_314_VISANIMATOR_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum VisAnimatorStateFlag {
    VIS_ANIMATOR_STATE_REFRESH_PENDING = 0x01,
} VisAnimatorStateFlag;

typedef struct VisAnimatorPlacement {
    ObjPlacement base;       /* 0x00 */
    s16 gateGameBit;         /* 0x18 */
    u8 pad1A;                /* 0x1A */
    s8 initialVisibilityBit; /* 0x1B */
    u8 gateBitIndex;         /* 0x1C */
    u8 pad1D[0x0B];          /* 0x1D */
} VisAnimatorPlacement;

/* VisAnimator_getExtraSize proves the complete 0x05-byte allocation. */
typedef struct VisAnimatorState {
    u8 flags;             /* 0x00: VisAnimatorStateFlag */
    s8 visibilityBit;     /* 0x01 */
    u8 currentGateState;  /* 0x02 */
    u8 previousGateState; /* 0x03 */
    u8 gateMask;          /* 0x04 */
} VisAnimatorState;

STATIC_ASSERT(offsetof(VisAnimatorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(VisAnimatorPlacement, gateGameBit) == 0x18);
STATIC_ASSERT(offsetof(VisAnimatorPlacement, pad1A) == 0x1A);
STATIC_ASSERT(offsetof(VisAnimatorPlacement, initialVisibilityBit) == 0x1B);
STATIC_ASSERT(offsetof(VisAnimatorPlacement, gateBitIndex) == 0x1C);
STATIC_ASSERT(offsetof(VisAnimatorPlacement, pad1D) == 0x1D);
STATIC_ASSERT(sizeof(VisAnimatorPlacement) == 0x28);

STATIC_ASSERT(offsetof(VisAnimatorState, flags) == 0x00);
STATIC_ASSERT(offsetof(VisAnimatorState, visibilityBit) == 0x01);
STATIC_ASSERT(offsetof(VisAnimatorState, currentGateState) == 0x02);
STATIC_ASSERT(offsetof(VisAnimatorState, previousGateState) == 0x03);
STATIC_ASSERT(offsetof(VisAnimatorState, gateMask) == 0x04);
STATIC_ASSERT(sizeof(VisAnimatorState) == 0x05);

int VisAnimator_getExtraSize(void);
int VisAnimator_getObjectTypeId(void);
void VisAnimator_free(void);
void VisAnimator_render(void);
void VisAnimator_hitDetect(void);
void VisAnimator_update(GameObject* obj);
void VisAnimator_init(GameObject* obj, VisAnimatorPlacement* placement);
void VisAnimator_release(void);
void VisAnimator_initialise(void);

extern ObjectDescriptor gVisAnimatorObjDescriptor;

#endif /* DLLS_OBJECTS_314_VISANIMATOR_H_ */
