#ifndef DLLS_OBJECTS_281_COLD_WATER_CO_H_
#define DLLS_OBJECTS_281_COLD_WATER_CO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

#define COLD_WATER_CONTROL_STATE_SIZE 0x8

typedef struct ColdWaterControlState {
    f32 damageTimer;          /* 0x0 */
    GameObject* cachedPlayer; /* 0x4 */
} ColdWaterControlState;

STATIC_ASSERT(offsetof(ColdWaterControlState, damageTimer) == 0x0);
STATIC_ASSERT(offsetof(ColdWaterControlState, cachedPlayer) == 0x4);
STATIC_ASSERT(sizeof(ColdWaterControlState) == COLD_WATER_CONTROL_STATE_SIZE);

int ColdWaterControl_getExtraSize(void);
void ColdWaterControl_update(GameObject* obj);
void ColdWaterControl_init(GameObject* obj);

extern ObjectDescriptor gColdWaterControlObjDescriptor;

#endif /* DLLS_OBJECTS_281_COLD_WATER_CO_H_ */
