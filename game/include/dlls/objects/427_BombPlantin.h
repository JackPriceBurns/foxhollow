#ifndef DLLS_OBJECTS_427_BOMB_PLANTIN_H_
#define DLLS_OBJECTS_427_BOMB_PLANTIN_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct BombPlantingSpotPlacement {
    ObjPlacement base;
    s8 rotXByte;
    u8 unknown19[5];
    s16 plantedGameBit;
    s16 requiredGameBit;
} BombPlantingSpotPlacement;

STATIC_ASSERT(offsetof(BombPlantingSpotPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(BombPlantingSpotPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(BombPlantingSpotPlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(BombPlantingSpotPlacement, plantedGameBit) == 0x1E);
STATIC_ASSERT(offsetof(BombPlantingSpotPlacement, requiredGameBit) == 0x20);

void BombPlantingSpot_update(GameObject* obj);
void BombPlantingSpot_init(GameObject* obj, const BombPlantingSpotPlacement* placement);

extern ObjectDescriptor gBombPlantingSpotObjDescriptor;

#endif /* DLLS_OBJECTS_427_BOMB_PLANTIN_H_ */
