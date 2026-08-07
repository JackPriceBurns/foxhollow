#ifndef DLLS_OBJECTS_455_DIMLAVASMAS_H_
#define DLLS_OBJECTS_455_DIMLAVASMAS_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "global.h"

typedef struct MapBlockData MapBlockData;
typedef struct ObjSeqState ObjSeqState;

/* All four active-target retail placements are fixed 9-word (0x24-byte) records. */
typedef struct DimLavaSmashPlacement {
    ObjPlacement base;
    s8 rotationXByte;
    s8 modelBankIndex;
    s16 surfaceLayerId;
    s16 sequenceSlot;
    s16 triggerGameBit;
    s16 gateGameBit;
    u8 unknown22[2];
} DimLavaSmashPlacement;

typedef struct DimLavaSmashState {
    s8 sequenceSlot;
    u8 surfaceLayerId;
    u8 phase;
} DimLavaSmashState;

STATIC_ASSERT(offsetof(DimLavaSmashPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimLavaSmashPlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(DimLavaSmashPlacement, modelBankIndex) == 0x19);
STATIC_ASSERT(offsetof(DimLavaSmashPlacement, surfaceLayerId) == 0x1A);
STATIC_ASSERT(offsetof(DimLavaSmashPlacement, sequenceSlot) == 0x1C);
STATIC_ASSERT(offsetof(DimLavaSmashPlacement, triggerGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DimLavaSmashPlacement, gateGameBit) == 0x20);
STATIC_ASSERT(offsetof(DimLavaSmashPlacement, unknown22) == 0x22);
STATIC_ASSERT(sizeof(DimLavaSmashPlacement) == 0x24);

STATIC_ASSERT(offsetof(DimLavaSmashState, sequenceSlot) == 0x00);
STATIC_ASSERT(offsetof(DimLavaSmashState, surfaceLayerId) == 0x01);
STATIC_ASSERT(offsetof(DimLavaSmashState, phase) == 0x02);
STATIC_ASSERT(sizeof(DimLavaSmashState) == 0x03);

void dimlavasmash_setBlockSurfaceFlags(MapBlockData* map, int disable, int surfaceType);
int dimlavasmash_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int dimlavasmash_getExtraSize(void);
int dimlavasmash_getObjectTypeId(void);
void dimlavasmash_free(void);
void dimlavasmash_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dimlavasmash_hitDetect(void);
void dimlavasmash_update(GameObject* obj);
void dimlavasmash_init(GameObject* obj, DimLavaSmashPlacement* placement);
void dimlavasmash_release(void);
void dimlavasmash_initialise(void);

extern ObjectDescriptor gDIMLavaSmashObjDescriptor;

#endif /* DLLS_OBJECTS_455_DIMLAVASMAS_H_ */
