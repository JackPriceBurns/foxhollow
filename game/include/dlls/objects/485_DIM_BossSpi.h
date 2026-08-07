#ifndef DLLS_OBJECTS_485_DIM_BOSSSPI_H_
#define DLLS_OBJECTS_485_DIM_BOSSSPI_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/model_light.h"

typedef struct DIMbossSpitState {
    union {
        s16 phase;
        s16 burstTimer;
    };
    s16 unknown02;
    ModelLightStruct* light;
} DIMbossSpitState;

STATIC_ASSERT(offsetof(DIMbossSpitState, phase) == 0x00);
STATIC_ASSERT(offsetof(DIMbossSpitState, burstTimer) == 0x00);
STATIC_ASSERT(offsetof(DIMbossSpitState, unknown02) == 0x02);
STATIC_ASSERT(offsetof(DIMbossSpitState, light) == 0x04);
STATIC_ASSERT(sizeof(DIMbossSpitState) == 0x08);

void DIMbossspit_updateBurst(GameObject* obj);
int DIMbossspit_getExtraSize(void);
int DIMbossspit_getObjectTypeId(void);
void DIMbossspit_free(GameObject* obj);
void DIMbossspit_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void DIMbossspit_hitDetect(void);
void DIMbossspit_update(GameObject* obj);
void DIMbossspit_init(GameObject* obj);
void DIMbossspit_release(void);
void DIMbossspit_initialise(void);

extern ObjectDescriptor gDIM_BossSpitObjDescriptor;

#endif /* DLLS_OBJECTS_485_DIM_BOSSSPI_H_ */
