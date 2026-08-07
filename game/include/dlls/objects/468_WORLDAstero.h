#ifndef DLLS_OBJECTS_468_WORLDASTERO_H_
#define DLLS_OBJECTS_468_WORLDASTERO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

/* worldasteroids_getExtraSize() allocates 0x0C bytes. */
typedef struct WorldAsteroidsState {
    s16 rotStepZ;
    s16 rotStepY;
    s16 rotStepX;
    s16 orbitAngle;
    s16 orbitRadius;
    s16 heightOffset;
} WorldAsteroidsState;

STATIC_ASSERT(offsetof(WorldAsteroidsState, rotStepZ) == 0x00);
STATIC_ASSERT(offsetof(WorldAsteroidsState, rotStepY) == 0x02);
STATIC_ASSERT(offsetof(WorldAsteroidsState, rotStepX) == 0x04);
STATIC_ASSERT(offsetof(WorldAsteroidsState, orbitAngle) == 0x06);
STATIC_ASSERT(offsetof(WorldAsteroidsState, orbitRadius) == 0x08);
STATIC_ASSERT(offsetof(WorldAsteroidsState, heightOffset) == 0x0A);
STATIC_ASSERT(sizeof(WorldAsteroidsState) == 0x0C);

int worldasteroids_getExtraSize(void);
int worldasteroids_getObjectTypeId(void);
void worldasteroids_free(void);
void worldasteroids_render(GameObject* obj, u32 renderArg2, u32 renderArg3, u32 renderArg4, u32 renderArg5, s8 visible);
void worldasteroids_hitDetect(void);
void worldasteroids_update(GameObject* obj);
void worldasteroids_init(GameObject* obj);
void worldasteroids_release(void);
void worldasteroids_initialise(void);

extern ObjectDescriptor gWorldAsteroidsObjDescriptor;

#endif /* DLLS_OBJECTS_468_WORLDASTERO_H_ */
