#ifndef DLLS_OBJECTS_487_DIMBOSSFIRE_H_
#define DLLS_OBJECTS_487_DIMBOSSFIRE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/model_light.h"

#define DIMBOSSFIRE_FLAME_COUNT 10

/* dimbossfire_getExtraSize() allocates the complete 0x14-byte state block. */
typedef struct DimBossFireState {
    u8 flags;
    u8 durationIndex;
    u8 unknown02[2];
    f32 activeTimer;
    f32 initialActiveTimer;
    f32 cooldownTimer;
    ModelLightStruct* light;
} DimBossFireState;

/*
 * This type models only the placement prefix consumed by the callbacks; it
 * does not claim a complete retail placement width.
 */
typedef struct DimBossFirePlacementView {
    ObjPlacement base;
    u8 unknown18[2];
    s16 flameColor;
    u8 unknown1C[4];
    s16 triggerGameBit;
} DimBossFirePlacementView;

STATIC_ASSERT(offsetof(DimBossFireState, flags) == 0x00);
STATIC_ASSERT(offsetof(DimBossFireState, durationIndex) == 0x01);
STATIC_ASSERT(offsetof(DimBossFireState, unknown02) == 0x02);
STATIC_ASSERT(offsetof(DimBossFireState, activeTimer) == 0x04);
STATIC_ASSERT(offsetof(DimBossFireState, initialActiveTimer) == 0x08);
STATIC_ASSERT(offsetof(DimBossFireState, cooldownTimer) == 0x0C);
STATIC_ASSERT(offsetof(DimBossFireState, light) == 0x10);
STATIC_ASSERT(sizeof(DimBossFireState) == 0x14);

STATIC_ASSERT(offsetof(DimBossFirePlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(DimBossFirePlacementView, flameColor) == 0x1A);
STATIC_ASSERT(offsetof(DimBossFirePlacementView, triggerGameBit) == 0x20);

int dimbossfire_getExtraSize(void);
int dimbossfire_getObjectTypeId(void);
void dimbossfire_free(GameObject* obj);
void dimbossfire_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dimbossfire_hitDetect(void);
void dimbossfire_update(GameObject* obj);
void dimbossfire_init(GameObject* obj, u32 placementAddress, int isAltVariant);
void dimbossfire_release(void);
void dimbossfire_initialise(void);

extern f32 gDimbossfireActiveDurations[DIMBOSSFIRE_FLAME_COUNT];
extern ObjectDescriptor gDIMbossfireObjDescriptor;

#endif /* DLLS_OBJECTS_487_DIMBOSSFIRE_H_ */
