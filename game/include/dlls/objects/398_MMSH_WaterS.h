#ifndef DLLS_OBJECTS_398_MMSH_WATERS_H_
#define DLLS_OBJECTS_398_MMSH_WATERS_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct MMSHWaterSpikePlacement {
    ObjPlacement base;
    u8 unknown18[0x1A - 0x18];
    s16 xyzAnimatorObjectIdLow;
    s16 xyzAnimatorObjectIdHigh;
    u8 unknown1E[0x24 - 0x1E];
} MMSHWaterSpikePlacement;

STATIC_ASSERT(sizeof(MMSHWaterSpikePlacement) == 0x24);
STATIC_ASSERT(offsetof(MMSHWaterSpikePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(MMSHWaterSpikePlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(MMSHWaterSpikePlacement, xyzAnimatorObjectIdLow) == 0x1A);
STATIC_ASSERT(offsetof(MMSHWaterSpikePlacement, xyzAnimatorObjectIdHigh) == 0x1C);
STATIC_ASSERT(offsetof(MMSHWaterSpikePlacement, unknown1E) == 0x1E);

extern ObjectDescriptor gMMSHWaterSpikeObjDescriptor;

int mmshWaterSpike_getExtraSize(void);
int mmshWaterSpike_getObjectTypeId(void);
void mmshWaterSpike_free(GameObject* obj);
void mmshWaterSpike_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void mmshWaterSpike_hitDetect(void);
void mmshWaterSpike_update(GameObject* obj);
void mmshWaterSpike_init(GameObject* obj, const MMSHWaterSpikePlacement* placement);
void mmshWaterSpike_release(void);
void mmshWaterSpike_initialise(void);

#endif /* DLLS_OBJECTS_398_MMSH_WATERS_H_ */
