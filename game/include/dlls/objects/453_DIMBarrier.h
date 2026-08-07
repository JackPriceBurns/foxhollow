#ifndef DLLS_OBJECTS_453_DIMBARRIER_H_
#define DLLS_OBJECTS_453_DIMBARRIER_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* All three active-target placements are fixed 9-word (0x24-byte) records. */
typedef struct DimBarrierPlacement {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19[5];
    s16 barrierGameBit;
    u8 unknown20[4];
} DimBarrierPlacement;

typedef struct DimBarrierState {
    s16 fadeTimer;
    u8 phase;
    s8 triggerCountdown;
} DimBarrierState;

STATIC_ASSERT(offsetof(DimBarrierPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimBarrierPlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(DimBarrierPlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(DimBarrierPlacement, barrierGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DimBarrierPlacement, unknown20) == 0x20);
STATIC_ASSERT(sizeof(DimBarrierPlacement) == 0x24);

STATIC_ASSERT(offsetof(DimBarrierState, fadeTimer) == 0x00);
STATIC_ASSERT(offsetof(DimBarrierState, phase) == 0x02);
STATIC_ASSERT(offsetof(DimBarrierState, triggerCountdown) == 0x03);
STATIC_ASSERT(sizeof(DimBarrierState) == 0x04);

int dimbarrier_getExtraSize(void);
int dimbarrier_getObjectTypeId(void);
void dimbarrier_free(void);
void dimbarrier_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dimbarrier_hitDetect(void);
void dimbarrier_update(GameObject* obj);
void dimbarrier_init(GameObject* obj, const DimBarrierPlacement* placement);
void dimbarrier_release(void);
void dimbarrier_initialise(void);

extern ObjectDescriptor gDIMBarrierObjDescriptor;

#endif /* DLLS_OBJECTS_453_DIMBARRIER_H_ */
