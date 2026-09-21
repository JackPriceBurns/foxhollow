#ifndef DLLS_OBJECTS_465_DIMTRUTHHOR_H_
#define DLLS_OBJECTS_465_DIMTRUTHHOR_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* The sole active-target retail placement is a fixed 12-word (0x30-byte) record. */
typedef struct DimTruthHornIcePlacement {
    ObjPlacement base;
    u8 unknown18[2];
    s16 hitCount;
    s16 unknown1C;
    s16 gameBit;
    u8 unknown20[0x10];
} DimTruthHornIcePlacement;

/* getExtraSize() allocates 0x08 bytes. */
typedef struct DimTruthHornIceState {
    s16 gameBit;
    s8 hitsLeft;
    s8 phase;
    f32 timer;
} DimTruthHornIceState;

STATIC_ASSERT(offsetof(DimTruthHornIcePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimTruthHornIcePlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(DimTruthHornIcePlacement, hitCount) == 0x1A);
STATIC_ASSERT(offsetof(DimTruthHornIcePlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(DimTruthHornIcePlacement, gameBit) == 0x1E);
STATIC_ASSERT(offsetof(DimTruthHornIcePlacement, unknown20) == 0x20);
STATIC_ASSERT(sizeof(DimTruthHornIcePlacement) == 0x30);

STATIC_ASSERT(offsetof(DimTruthHornIceState, gameBit) == 0x00);
STATIC_ASSERT(offsetof(DimTruthHornIceState, hitsLeft) == 0x02);
STATIC_ASSERT(offsetof(DimTruthHornIceState, phase) == 0x03);
STATIC_ASSERT(offsetof(DimTruthHornIceState, timer) == 0x04);
STATIC_ASSERT(sizeof(DimTruthHornIceState) == 0x08);

int dimtruthhornice_countdownCallback(GameObject* obj, int damage);
int dimtruthhornice_getExtraSize(void);
void dimtruthhornice_update(GameObject* obj);
void dimtruthhornice_init(GameObject* obj, const DimTruthHornIcePlacement* placement);

extern ObjectDescriptor gDIMTruthHornIceObjDescriptor;

#endif /* DLLS_OBJECTS_465_DIMTRUTHHOR_H_ */
