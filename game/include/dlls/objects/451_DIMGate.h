#ifndef DLLS_OBJECTS_451_DIMGATE_H_
#define DLLS_OBJECTS_451_DIMGATE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct ObjSeqState ObjSeqState;

typedef enum DimGateMode {
    DIM_GATE_MODE_CLOSED = 0,
    DIM_GATE_MODE_OPENING = 1,
    DIM_GATE_MODE_OPEN = 2,
} DimGateMode;

typedef struct DimGatePlacement {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19[0x1E - 0x19];
    s16 openGameBit;
    u8 unknown20[0x24 - 0x20];
} DimGatePlacement;

typedef struct DimGateState {
    s8 mode;
} DimGateState;

STATIC_ASSERT(offsetof(DimGatePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimGatePlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(DimGatePlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(DimGatePlacement, openGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DimGatePlacement, unknown20) == 0x20);
STATIC_ASSERT(sizeof(DimGatePlacement) == 0x24);

STATIC_ASSERT(offsetof(DimGateState, mode) == 0x00);
STATIC_ASSERT(sizeof(DimGateState) == 0x01);

int dimgate_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int dimgate_getExtraSize(void);
int dimgate_getObjectTypeId(void);
void dimgate_free(void);
void dimgate_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dimgate_hitDetect(void);
void dimgate_update(GameObject* obj);
void dimgate_init(GameObject* obj, DimGatePlacement* unusedPlacement);
void dimgate_release(void);
void dimgate_initialise(void);

extern ObjectDescriptor gDIMGateObjDescriptor;

#endif /* DLLS_OBJECTS_451_DIMGATE_H_ */
