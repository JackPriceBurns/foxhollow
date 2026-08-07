#ifndef DLLS_OBJECTS_448_DIMLOGFIRE_H_
#define DLLS_OBJECTS_448_DIMLOGFIRE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct ModelLightStruct ModelLightStruct;
typedef struct ObjSeqState ObjSeqState;

typedef struct DimLogFirePlacement {
    ObjPlacement base;
    s16 unknown18;
    s16 initialMode;
    s16 initialStrength;
    s16 douseGameBit;
    s16 unknown20;
    s16 unknown22;
} DimLogFirePlacement;

typedef struct DimLogFireState {
    ModelLightStruct* light;
    int subObject;
    u8 unknown08[0x10 - 0x08];
    f32 flickerTimerA;
    f32 flickerTimerB;
    u8 initialMode;
    u8 unknown19;
    u8 mode;
    u8 smokeEnabled;
    s8 remainingStrength;
    u8 transitionLatch;
    u8 initialStrength;
    u8 unknown1F;
    u8 unknown20;
    u8 unknown21[0x24 - 0x21];
} DimLogFireState;

STATIC_ASSERT(offsetof(DimLogFirePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimLogFirePlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(DimLogFirePlacement, initialMode) == 0x1A);
STATIC_ASSERT(offsetof(DimLogFirePlacement, initialStrength) == 0x1C);
STATIC_ASSERT(offsetof(DimLogFirePlacement, douseGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DimLogFirePlacement, unknown20) == 0x20);
STATIC_ASSERT(offsetof(DimLogFirePlacement, unknown22) == 0x22);
STATIC_ASSERT(sizeof(DimLogFirePlacement) == 0x24);

STATIC_ASSERT(offsetof(DimLogFireState, light) == 0x00);
STATIC_ASSERT(offsetof(DimLogFireState, subObject) == 0x04);
STATIC_ASSERT(offsetof(DimLogFireState, unknown08) == 0x08);
STATIC_ASSERT(offsetof(DimLogFireState, flickerTimerA) == 0x10);
STATIC_ASSERT(offsetof(DimLogFireState, flickerTimerB) == 0x14);
STATIC_ASSERT(offsetof(DimLogFireState, initialMode) == 0x18);
STATIC_ASSERT(offsetof(DimLogFireState, unknown19) == 0x19);
STATIC_ASSERT(offsetof(DimLogFireState, mode) == 0x1A);
STATIC_ASSERT(offsetof(DimLogFireState, smokeEnabled) == 0x1B);
STATIC_ASSERT(offsetof(DimLogFireState, remainingStrength) == 0x1C);
STATIC_ASSERT(offsetof(DimLogFireState, transitionLatch) == 0x1D);
STATIC_ASSERT(offsetof(DimLogFireState, initialStrength) == 0x1E);
STATIC_ASSERT(offsetof(DimLogFireState, unknown1F) == 0x1F);
STATIC_ASSERT(offsetof(DimLogFireState, unknown20) == 0x20);
STATIC_ASSERT(offsetof(DimLogFireState, unknown21) == 0x21);
STATIC_ASSERT(sizeof(DimLogFireState) == 0x24);

int DIMLogFire_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int dimlogfire_countdownCallback(GameObject* obj, int delta);
int DIMLogFire_getExtraSize(void);
int DIMLogFire_getObjectTypeId(void);
void DIMLogFire_free(GameObject* obj, int freeMode);
void DIMLogFire_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void DIMLogFire_update(GameObject* obj);
void DIMLogFire_init(GameObject* obj, const DimLogFirePlacement* placement);

extern ObjectDescriptor gDIMLogFireObjDescriptor;

#endif /* DLLS_OBJECTS_448_DIMLOGFIRE_H_ */
