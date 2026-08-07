#ifndef DLLS_OBJECTS_290_CCTESTINFOT_H_
#define DLLS_OBJECTS_290_CCTESTINFOT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define CC_TEST_INFO_TEXT_STATE_SIZE 0x8

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct CCTestInfotPlacement {
    ObjPlacement base; /* 0x00 */
    u8 rotationZ;      /* 0x18: high byte of the initial Z rotation */
    u8 rotationY;      /* 0x19: high byte of the initial Y rotation */
    u8 rotationX;      /* 0x1A: high byte of the initial X rotation */
} CCTestInfotPlacement;

typedef struct CCTestInfotState {
    f32 displayTimer; /* 0x00 */
    u8 isDisguised;   /* 0x04: cached player state and model/help-text index */
    u8 pad05[3];      /* 0x05 */
} CCTestInfotState;

STATIC_ASSERT(offsetof(CCTestInfotPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(CCTestInfotPlacement, rotationZ) == 0x18);
STATIC_ASSERT(offsetof(CCTestInfotPlacement, rotationY) == 0x19);
STATIC_ASSERT(offsetof(CCTestInfotPlacement, rotationX) == 0x1A);

STATIC_ASSERT(offsetof(CCTestInfotState, displayTimer) == 0x0);
STATIC_ASSERT(offsetof(CCTestInfotState, isDisguised) == 0x4);
STATIC_ASSERT(offsetof(CCTestInfotState, pad05) == 0x5);
STATIC_ASSERT(sizeof(CCTestInfotState) == CC_TEST_INFO_TEXT_STATE_SIZE);

int CCTestInfot_getExtraSize(void);
void CCTestInfot_update(GameObject* obj);
void CCTestInfot_init(GameObject* obj, CCTestInfotPlacement* placement);

extern ObjectDescriptor gCCTestInfotObjDescriptor;

#endif /* DLLS_OBJECTS_290_CCTESTINFOT_H_ */
