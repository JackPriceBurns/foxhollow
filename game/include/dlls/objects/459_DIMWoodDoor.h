#ifndef DLLS_OBJECTS_459_DIMWOODDOOR_H_
#define DLLS_OBJECTS_459_DIMWOODDOOR_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* The active-target placement is a fixed 9-word (0x24-byte) record. */
typedef struct DimWoodDoorPlacement {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19[5];
    s16 openedGameBit;
    u8 unknown20[4];
} DimWoodDoorPlacement;

typedef struct DimWoodDoorState {
    s8 doorState;
    u8 unknown01[3];
    f32 animationSpeed;
    f32 riseSpeed;
} DimWoodDoorState;

STATIC_ASSERT(offsetof(DimWoodDoorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimWoodDoorPlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(DimWoodDoorPlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(DimWoodDoorPlacement, openedGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DimWoodDoorPlacement, unknown20) == 0x20);
STATIC_ASSERT(sizeof(DimWoodDoorPlacement) == 0x24);

STATIC_ASSERT(offsetof(DimWoodDoorState, doorState) == 0x00);
STATIC_ASSERT(offsetof(DimWoodDoorState, unknown01) == 0x01);
STATIC_ASSERT(offsetof(DimWoodDoorState, animationSpeed) == 0x04);
STATIC_ASSERT(offsetof(DimWoodDoorState, riseSpeed) == 0x08);
STATIC_ASSERT(sizeof(DimWoodDoorState) == 0x0C);

int dimwooddoor2_getExtraSize(void);
int dimwooddoor2_getObjectTypeId(void);
void dimwooddoor2_free(void);
void dimwooddoor2_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dimwooddoor2_hitDetect(void);
void dimwooddoor2_update(GameObject* obj);
void dimwooddoor2_init(GameObject* obj, const DimWoodDoorPlacement* placement);
void dimwooddoor2_release(void);
void dimwooddoor2_initialise(void);

extern ObjectDescriptor gDIMWoodDoor2ObjDescriptor;

#endif /* DLLS_OBJECTS_459_DIMWOODDOOR_H_ */
