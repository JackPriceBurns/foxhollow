#ifndef DLLS_OBJECTS_282_H_
#define DLLS_OBJECTS_282_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/vec_types.h"

#define DECORATION11A_STATE_SIZE 0x1C

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct Decoration11APlacement {
    ObjPlacement base; /* 0x00 */
    u8 rotationZ;      /* 0x18: high byte of the initial Z rotation */
    u8 rotationY;      /* 0x19: high byte of the initial Y rotation */
    u8 rotationX;      /* 0x1A: high byte of the initial X rotation */
    u8 scale;          /* 0x1B: root-motion scale fraction, or 0 for the model default */
} Decoration11APlacement;

typedef struct Decoration11AState {
    Vec3f boundsMax;    /* 0x00 */
    Vec3f boundsMin;    /* 0x0C */
    f32 boundingRadius; /* 0x18 */
} Decoration11AState;

STATIC_ASSERT(offsetof(Decoration11APlacement, base) == 0x0);
STATIC_ASSERT(offsetof(Decoration11APlacement, rotationZ) == 0x18);
STATIC_ASSERT(offsetof(Decoration11APlacement, rotationY) == 0x19);
STATIC_ASSERT(offsetof(Decoration11APlacement, rotationX) == 0x1A);
STATIC_ASSERT(offsetof(Decoration11APlacement, scale) == 0x1B);

STATIC_ASSERT(offsetof(Decoration11AState, boundsMax) == 0x0);
STATIC_ASSERT(offsetof(Decoration11AState, boundsMin) == 0xC);
STATIC_ASSERT(offsetof(Decoration11AState, boundingRadius) == 0x18);
STATIC_ASSERT(sizeof(Decoration11AState) == DECORATION11A_STATE_SIZE);

int decoration11a_getExtraSize(void);
void decoration11a_free(void);
void decoration11a_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void decoration11a_hitDetect(GameObject* obj);
void decoration11a_update(void);
void decoration11a_expandBoundsWithVertex(const Vec3f* vertex, Vec3f* boundsMax, Vec3f* boundsMin);
void decoration11a_init(GameObject* obj, Decoration11APlacement* placement);

extern ObjectDescriptor gDecoration11AObjDescriptor;

#endif /* DLLS_OBJECTS_282_H_ */
