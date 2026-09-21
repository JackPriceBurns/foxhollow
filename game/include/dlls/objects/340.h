#ifndef DLLS_OBJECTS_340_H_
#define DLLS_OBJECTS_340_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct CfPrisonCagePlacement {
    ObjPlacement base;
    s16 openedGameBit;
    u8 initialRotX;
    u8 pad1B;
    u8 pad1C[0x0C];
} CfPrisonCagePlacement;

STATIC_ASSERT(offsetof(CfPrisonCagePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CfPrisonCagePlacement, openedGameBit) == 0x18);
STATIC_ASSERT(offsetof(CfPrisonCagePlacement, initialRotX) == 0x1A);
STATIC_ASSERT(offsetof(CfPrisonCagePlacement, pad1B) == 0x1B);
STATIC_ASSERT(offsetof(CfPrisonCagePlacement, pad1C) == 0x1C);
STATIC_ASSERT(sizeof(CfPrisonCagePlacement) == 0x28);

int cfPrisonCage_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int cfPrisonCage_getExtraSize(void);
int cfPrisonCage_getObjectTypeId(GameObject* obj);
void cfPrisonCage_free(void);
void cfPrisonCage_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void cfPrisonCage_hitDetect(GameObject* obj);
void cfPrisonCage_update(GameObject* obj);
void cfPrisonCage_init(GameObject* obj, CfPrisonCagePlacement* placement);
void cfPrisonCage_release(void);
void cfPrisonCage_initialise(void);

extern ObjectDescriptor gCFPrisonCageObjDescriptor;

#endif /* DLLS_OBJECTS_340_H_ */
