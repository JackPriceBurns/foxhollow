#ifndef DLLS_OBJECTS_368_IMSPACERING_H_
#define DLLS_OBJECTS_368_IMSPACERING_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct IMSpaceRingPlacement {
    ObjPlacement base;
    s8 initialRotX;
    u8 pad19;
    s16 spinSpeed;
    s16 tiltSpeed;
    u8 pad1E[0x06];
} IMSpaceRingPlacement;

STATIC_ASSERT(offsetof(IMSpaceRingPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(IMSpaceRingPlacement, initialRotX) == 0x18);
STATIC_ASSERT(offsetof(IMSpaceRingPlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(IMSpaceRingPlacement, spinSpeed) == 0x1A);
STATIC_ASSERT(offsetof(IMSpaceRingPlacement, tiltSpeed) == 0x1C);
STATIC_ASSERT(offsetof(IMSpaceRingPlacement, pad1E) == 0x1E);
STATIC_ASSERT(sizeof(IMSpaceRingPlacement) == 0x24);

int imSpaceRing_getExtraSize(void);
int imSpaceRing_getObjectTypeId(void);
void imSpaceRing_free(void);
void imSpaceRing_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void imSpaceRing_hitDetect(void);
void imSpaceRing_update(GameObject* obj);
void imSpaceRing_init(GameObject* obj, const IMSpaceRingPlacement* placement);
void imSpaceRing_release(void);
void imSpaceRing_initialise(void);

extern GameObject* gIMSpaceRingLeader;
extern ObjectDescriptor gIMSpaceRingObjDescriptor;

#endif /* DLLS_OBJECTS_368_IMSPACERING_H_ */
