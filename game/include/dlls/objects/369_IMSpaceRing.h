#ifndef DLLS_OBJECTS_369_IMSPACERING_H_
#define DLLS_OBJECTS_369_IMSPACERING_H_

#include "dlls/object_descriptor.h"
#include "dlls/objects/368_IMSpaceRing.h"
#include "game/objects/object_fwd.h"

#define IM_SPACE_RING_GENERATOR_CHILD_COUNT 10

typedef struct IMSpaceRingGeneratorState {
    GameObject* ringA;
    GameObject* ringB;
    u8 visible;
    u8 pad09[0x03];
} IMSpaceRingGeneratorState;

typedef struct IMSpaceRingInterface {
    void* standardSlots[9];
    int (*isVisible)(GameObject* ring);
} IMSpaceRingInterface;

STATIC_ASSERT(offsetof(IMSpaceRingGeneratorState, ringA) == 0x00);
STATIC_ASSERT(offsetof(IMSpaceRingGeneratorState, ringB) == 0x04);
STATIC_ASSERT(offsetof(IMSpaceRingGeneratorState, visible) == 0x08);
STATIC_ASSERT(offsetof(IMSpaceRingGeneratorState, pad09) == 0x09);
STATIC_ASSERT(sizeof(IMSpaceRingGeneratorState) == 0x0C);

STATIC_ASSERT(offsetof(IMSpaceRingInterface, standardSlots) == 0x00);
STATIC_ASSERT(offsetof(IMSpaceRingInterface, isVisible) == 0x24);
STATIC_ASSERT(sizeof(IMSpaceRingInterface) == 0x28);

#define IM_SPACE_RING_INTERFACE(obj) ((IMSpaceRingInterface*)*((obj)->anim.dll))

int imSpaceRingGenerator_getExtraSize(void);
int imSpaceRingGenerator_getObjectTypeId(void);
void imSpaceRingGenerator_free(void);
void imSpaceRingGenerator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                 s8 visible);
void imSpaceRingGenerator_hitDetect(void);
void imSpaceRingGenerator_update(GameObject* obj);
void imSpaceRingGenerator_init(GameObject* obj);
void imSpaceRingGenerator_release(void);
void imSpaceRingGenerator_initialise(void);

extern ObjectDescriptor gIMSpaceRingGeneratorObjDescriptor;

#endif /* DLLS_OBJECTS_369_IMSPACERING_H_ */
