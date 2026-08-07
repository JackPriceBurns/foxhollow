#ifndef DLLS_OBJECTS_389_CCGASVENT_H_
#define DLLS_OBJECTS_389_CCGASVENT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define CC_GAS_VENT_OBJECT_GROUP         0x3F
#define CC_GAS_VENT_BLOCKER_OBJECT_GROUP 5
#define CC_GAS_VENT_ACTIVE_GAMEBIT       0x1C0

typedef struct CCGasVentPlacement {
    ObjPlacement base;
    u8 unknown18[0x20 - 0x18];
} CCGasVentPlacement;

STATIC_ASSERT(sizeof(CCGasVentPlacement) == 0x20);
STATIC_ASSERT(offsetof(CCGasVentPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CCGasVentPlacement, unknown18) == 0x18);

typedef struct CCGasVentState {
    u8 phase; /* 0 with a nearby blocker; 1 while clear */
} CCGasVentState;

STATIC_ASSERT(sizeof(CCGasVentState) == 0x01);
STATIC_ASSERT(offsetof(CCGasVentState, phase) == 0x00);

extern ObjectDescriptor gCCGasVentObjDescriptor;

int ccGasVent_getExtraSize(void);
void ccGasVent_free(GameObject* obj);
void ccGasVent_render(void);
void ccGasVent_update(GameObject* obj);
void ccGasVent_init(GameObject* obj);

#endif /* DLLS_OBJECTS_389_CCGASVENT_H_ */
