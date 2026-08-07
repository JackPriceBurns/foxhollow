#ifndef DLLS_OBJECTS_403_GPSH_OBJCRE_H_
#define DLLS_OBJECTS_403_GPSH_OBJCRE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct GPSHObjCreatorPlacement {
    ObjPlacement base;
    s16 symbolSpawnActivationGameBit;
    s16 childTypeIndex;
    u8 unknown1C[0x1E - 0x1C];
    s8 initialYaw;
    u8 unknown1F[0x24 - 0x1F];
} GPSHObjCreatorPlacement;

STATIC_ASSERT(sizeof(GPSHObjCreatorPlacement) == 0x24);
STATIC_ASSERT(offsetof(GPSHObjCreatorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(GPSHObjCreatorPlacement, symbolSpawnActivationGameBit) == 0x18);
STATIC_ASSERT(offsetof(GPSHObjCreatorPlacement, childTypeIndex) == 0x1A);
STATIC_ASSERT(offsetof(GPSHObjCreatorPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(GPSHObjCreatorPlacement, initialYaw) == 0x1E);
STATIC_ASSERT(offsetof(GPSHObjCreatorPlacement, unknown1F) == 0x1F);

typedef struct GPSHObjCreatorFlags {
    u8 childSpawned : 1;
    u8 unknownBits : 7;
} GPSHObjCreatorFlags;

typedef struct GPSHObjCreatorState {
    f32 spawnTimer;
    u8 childTypeIndex;
    GPSHObjCreatorFlags flags;
    u8 unknown06[0x08 - 0x06];
} GPSHObjCreatorState;

STATIC_ASSERT(sizeof(GPSHObjCreatorFlags) == 0x01);
STATIC_ASSERT(sizeof(GPSHObjCreatorState) == 0x08);
STATIC_ASSERT(offsetof(GPSHObjCreatorState, spawnTimer) == 0x00);
STATIC_ASSERT(offsetof(GPSHObjCreatorState, childTypeIndex) == 0x04);
STATIC_ASSERT(offsetof(GPSHObjCreatorState, flags) == 0x05);
STATIC_ASSERT(offsetof(GPSHObjCreatorState, unknown06) == 0x06);

extern ObjectDescriptor10WithPadding gGPSHObjCreatorObjDescriptor;

int gpshObjCreator_getExtraSize(void);
int gpshObjCreator_getObjectTypeId(void);
void gpshObjCreator_free(void);
void gpshObjCreator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void gpshObjCreator_hitDetect(void);
void gpshObjCreator_update(GameObject* obj);
void gpshObjCreator_init(GameObject* obj, const GPSHObjCreatorPlacement* placement);
void gpshObjCreator_release(void);
void gpshObjCreator_initialise(void);

#endif /* DLLS_OBJECTS_403_GPSH_OBJCRE_H_ */
