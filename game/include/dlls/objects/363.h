#ifndef DLLS_OBJECTS_363_H_
#define DLLS_OBJECTS_363_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct MagicLightPlacement {
    ObjPlacement base;
    s8 initialRotX;
    u8 pad19;
    s16 subtype;
    u8 pad1C[0x08];
} MagicLightPlacement;

STATIC_ASSERT(offsetof(MagicLightPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(MagicLightPlacement, initialRotX) == 0x18);
STATIC_ASSERT(offsetof(MagicLightPlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(MagicLightPlacement, subtype) == 0x1A);
STATIC_ASSERT(offsetof(MagicLightPlacement, pad1C) == 0x1C);
STATIC_ASSERT(sizeof(MagicLightPlacement) == 0x24);

int MagicLight_sequenceCallback(GameObject* obj);
int MagicLight_getExtraSize(GameObject* obj);
int MagicLight_getObjectTypeId(void);
void MagicLight_free(GameObject* obj);
void MagicLight_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void MagicLight_hitDetect(void);
void MagicLight_update(GameObject* obj);
void MagicLight_init(GameObject* obj, const MagicLightPlacement* placement);
void MagicLight_release(void);
void MagicLight_initialise(void);

extern ObjectDescriptor gMagicLightObjDescriptor;

#endif /* DLLS_OBJECTS_363_H_ */
