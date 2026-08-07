#ifndef DLLS_OBJECTS_413_H_
#define DLLS_OBJECTS_413_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct Dll19DPlacement {
    ObjPlacement base;
    u8 unknown18;
    s8 variant;
} Dll19DPlacement;

typedef struct Dll19DState {
    u8 unknown00[0x08];
    f32 positionX;
    f32 positionY;
    f32 positionZ;
    u8 unknown14[0x2C - 0x14];
    s16 angularVelocityZ;
    s16 angularVelocityX;
    s16 effectTimer;
    s16 despawnTimer;
    u16 spinPhase;
    u8 flags;
    u8 unknown37;
} Dll19DState;

STATIC_ASSERT(offsetof(Dll19DPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(Dll19DPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(Dll19DPlacement, variant) == 0x19);

STATIC_ASSERT(sizeof(Dll19DState) == 0x38);
STATIC_ASSERT(offsetof(Dll19DState, unknown00) == 0x00);
STATIC_ASSERT(offsetof(Dll19DState, positionX) == 0x08);
STATIC_ASSERT(offsetof(Dll19DState, positionY) == 0x0C);
STATIC_ASSERT(offsetof(Dll19DState, positionZ) == 0x10);
STATIC_ASSERT(offsetof(Dll19DState, unknown14) == 0x14);
STATIC_ASSERT(offsetof(Dll19DState, angularVelocityZ) == 0x2C);
STATIC_ASSERT(offsetof(Dll19DState, angularVelocityX) == 0x2E);
STATIC_ASSERT(offsetof(Dll19DState, effectTimer) == 0x30);
STATIC_ASSERT(offsetof(Dll19DState, despawnTimer) == 0x32);
STATIC_ASSERT(offsetof(Dll19DState, spinPhase) == 0x34);
STATIC_ASSERT(offsetof(Dll19DState, flags) == 0x36);
STATIC_ASSERT(offsetof(Dll19DState, unknown37) == 0x37);

extern ObjectDescriptor gDll19DObjDescriptor;

int dll413_getExtraSize(void);
int dll413_getObjectTypeId(void);
void dll413_free(GameObject* obj);
void dll413_render(void);
void dll413_hitDetect(GameObject* obj);
void dll413_update(GameObject* obj);
void dll413_init(GameObject* obj);
void dll413_release(void);
void dll413_initialise(void);

#endif /* DLLS_OBJECTS_413_H_ */
