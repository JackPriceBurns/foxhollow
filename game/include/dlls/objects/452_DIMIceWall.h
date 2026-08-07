#ifndef DLLS_OBJECTS_452_DIMICEWALL_H_
#define DLLS_OBJECTS_452_DIMICEWALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "global.h"

/* All nine active-target retail placements are fixed 9-word (0x24-byte) records. */
typedef struct DimIceWallPlacement {
    ObjPlacement base;
    s8 rotationXByte;
    s8 shatterScale;
    s16 hitPoints;
    u8 unknown1C[2];
    s16 shatterGameBit;
    u8 unknown20[4];
} DimIceWallPlacement;

typedef struct DimIceWallState {
    s8 hitPoints;
    u8 shattered;
} DimIceWallState;

STATIC_ASSERT(offsetof(DimIceWallPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimIceWallPlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(DimIceWallPlacement, shatterScale) == 0x19);
STATIC_ASSERT(offsetof(DimIceWallPlacement, hitPoints) == 0x1A);
STATIC_ASSERT(offsetof(DimIceWallPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(DimIceWallPlacement, shatterGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DimIceWallPlacement, unknown20) == 0x20);
STATIC_ASSERT(sizeof(DimIceWallPlacement) == 0x24);

STATIC_ASSERT(offsetof(DimIceWallState, hitPoints) == 0x00);
STATIC_ASSERT(offsetof(DimIceWallState, shattered) == 0x01);
STATIC_ASSERT(sizeof(DimIceWallState) == 0x02);

int dimicewall_countdownCallback(GameObject* obj, int delta);
int dimicewall_getExtraSize(void);
void dimicewall_update(GameObject* obj);
void dimicewall_init(GameObject* obj, DimIceWallPlacement* placement);

extern ObjectDescriptor gDIMIceWallObjDescriptor;

#endif /* DLLS_OBJECTS_452_DIMICEWALL_H_ */
