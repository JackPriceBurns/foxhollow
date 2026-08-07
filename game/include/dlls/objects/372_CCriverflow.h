#ifndef DLLS_OBJECTS_372_CCRIVERFLOW_H_
#define DLLS_OBJECTS_372_CCRIVERFLOW_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct CCRiverFlowPlacement {
    ObjPlacement base;
    u8 angle;
    u8 heightOffset;
    u8 speed;
    u8 pad1B;
    s16 gameBit;
    u8 pad1E[0x02];
} CCRiverFlowPlacement;

typedef struct CCRiverFlowState {
    u8 active;
} CCRiverFlowState;

STATIC_ASSERT(offsetof(CCRiverFlowPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CCRiverFlowPlacement, angle) == 0x18);
STATIC_ASSERT(offsetof(CCRiverFlowPlacement, heightOffset) == 0x19);
STATIC_ASSERT(offsetof(CCRiverFlowPlacement, speed) == 0x1A);
STATIC_ASSERT(offsetof(CCRiverFlowPlacement, pad1B) == 0x1B);
STATIC_ASSERT(offsetof(CCRiverFlowPlacement, gameBit) == 0x1C);
STATIC_ASSERT(offsetof(CCRiverFlowPlacement, pad1E) == 0x1E);
STATIC_ASSERT(sizeof(CCRiverFlowPlacement) == 0x20);

STATIC_ASSERT(offsetof(CCRiverFlowState, active) == 0x00);
STATIC_ASSERT(sizeof(CCRiverFlowState) == 0x01);

int ccRiverFlow_getExtraSize(void);
void ccRiverFlow_free(GameObject* obj);
void ccRiverFlow_render(void);
void ccRiverFlow_update(GameObject* obj);
void ccRiverFlow_init(GameObject* obj, CCRiverFlowPlacement* placement);

extern ObjectDescriptor gCCRiverFlowObjDescriptor;

#endif /* DLLS_OBJECTS_372_CCRIVERFLOW_H_ */
