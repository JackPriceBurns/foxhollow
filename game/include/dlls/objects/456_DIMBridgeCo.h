#ifndef DLLS_OBJECTS_456_DIMBRIDGECO_H_
#define DLLS_OBJECTS_456_DIMBRIDGECO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct ObjSeqState ObjSeqState;

/* Both active-target placements are fixed 10-word (0x28-byte) records. */
typedef struct DimBridgeCogPlacement {
    ObjPlacement base;
    s16 doneGameBit;
    s16 watchGameBit;
    u8 rotationAngle;
    u8 flags;
    s8 sequenceGate;
    u8 unknown1F[9];
} DimBridgeCogPlacement;

typedef struct DimBridgeCogState {
    u8 unknown00;
} DimBridgeCogState;

STATIC_ASSERT(offsetof(DimBridgeCogPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimBridgeCogPlacement, doneGameBit) == 0x18);
STATIC_ASSERT(offsetof(DimBridgeCogPlacement, watchGameBit) == 0x1A);
STATIC_ASSERT(offsetof(DimBridgeCogPlacement, rotationAngle) == 0x1C);
STATIC_ASSERT(offsetof(DimBridgeCogPlacement, flags) == 0x1D);
STATIC_ASSERT(offsetof(DimBridgeCogPlacement, sequenceGate) == 0x1E);
STATIC_ASSERT(offsetof(DimBridgeCogPlacement, unknown1F) == 0x1F);
STATIC_ASSERT(sizeof(DimBridgeCogPlacement) == 0x28);

STATIC_ASSERT(offsetof(DimBridgeCogState, unknown00) == 0x00);
STATIC_ASSERT(sizeof(DimBridgeCogState) == 0x01);

int dimbridgecogmai_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int dimbridgecogmai_getExtraSize(void);
int dimbridgecogmai_getObjectTypeId(void);
void dimbridgecogmai_free(GameObject* obj);
void dimbridgecogmai_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                            s8 visible);
void dimbridgecogmai_hitDetect(void);
void dimbridgecogmai_update(GameObject* obj);
void dimbridgecogmai_init(GameObject* obj, const DimBridgeCogPlacement* placement);
void dimbridgecogmai_release(void);
void dimbridgecogmai_initialise(void);

extern ObjectDescriptor gDIMBridgeCogMaiObjDescriptor;

#endif /* DLLS_OBJECTS_456_DIMBRIDGECO_H_ */
