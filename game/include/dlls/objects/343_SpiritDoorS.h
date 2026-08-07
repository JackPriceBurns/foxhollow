#ifndef DLLS_OBJECTS_343_SPIRITDOORS_H_
#define DLLS_OBJECTS_343_SPIRITDOORS_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define SPIRIT_DOOR_SPIRIT_OBJECT_GROUP 0x4E

typedef struct SpiritDoorSpiritState {
    u8 active;
} SpiritDoorSpiritState;

STATIC_ASSERT(offsetof(SpiritDoorSpiritState, active) == 0x00);
STATIC_ASSERT(sizeof(SpiritDoorSpiritState) == 0x01);

typedef struct SpiritDoorSpiritPlacement {
    ObjPlacement base;
    u8 pad18[0x06];
    s16 gateGameBit;
    u8 pad20[0x04];
} SpiritDoorSpiritPlacement;

STATIC_ASSERT(offsetof(SpiritDoorSpiritPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(SpiritDoorSpiritPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(SpiritDoorSpiritPlacement, gateGameBit) == 0x1E);
STATIC_ASSERT(offsetof(SpiritDoorSpiritPlacement, pad20) == 0x20);
STATIC_ASSERT(sizeof(SpiritDoorSpiritPlacement) == 0x24);

int spiritDoorSpirit_getExtraSize(void);
int spiritDoorSpirit_getObjectTypeId(void);
void spiritDoorSpirit_free(GameObject* obj);
void spiritDoorSpirit_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible);
void spiritDoorSpirit_hitDetect(void);
void spiritDoorSpirit_update(GameObject* obj);
void spiritDoorSpirit_init(GameObject* obj);
void spiritDoorSpirit_release(void);
void spiritDoorSpirit_initialise(void);

extern ObjectDescriptor gSpiritDoorSpiritObjDescriptor;

#endif /* DLLS_OBJECTS_343_SPIRITDOORS_H_ */
