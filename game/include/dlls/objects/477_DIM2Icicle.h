#ifndef DLLS_OBJECTS_477_DIM2ICICLE_H_
#define DLLS_OBJECTS_477_DIM2ICICLE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * This type models only the placement prefix read by the DLL's callbacks; it
 * does not claim a complete retail placement width.
 */
typedef struct Dim2IciclePlacementView {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19[5];
    s16 impactGameBit;
} Dim2IciclePlacementView;

/* dim2icicle_getExtraSize() allocates the complete 0x0C-byte state block. */
typedef struct Dim2IcicleState {
    f32 dropTargetY;
    s16 wobbleRotationY;
    u8 mode;
    u8 dropTargetFound;
    s16 timer;
    u8 unknown0A[2];
} Dim2IcicleState;

STATIC_ASSERT(offsetof(Dim2IciclePlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dim2IciclePlacementView, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(Dim2IciclePlacementView, unknown19) == 0x19);
STATIC_ASSERT(offsetof(Dim2IciclePlacementView, impactGameBit) == 0x1E);

STATIC_ASSERT(offsetof(Dim2IcicleState, dropTargetY) == 0x00);
STATIC_ASSERT(offsetof(Dim2IcicleState, wobbleRotationY) == 0x04);
STATIC_ASSERT(offsetof(Dim2IcicleState, mode) == 0x06);
STATIC_ASSERT(offsetof(Dim2IcicleState, dropTargetFound) == 0x07);
STATIC_ASSERT(offsetof(Dim2IcicleState, timer) == 0x08);
STATIC_ASSERT(offsetof(Dim2IcicleState, unknown0A) == 0x0A);
STATIC_ASSERT(sizeof(Dim2IcicleState) == 0x0C);

int dim2icicle_getExtraSize(void);
int dim2icicle_getObjectTypeId(void);
void dim2icicle_free(void);
void dim2icicle_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dim2icicle_hitDetect(void);
void dim2icicle_update(GameObject* obj);
void dim2icicle_init(GameObject* obj, const Dim2IciclePlacementView* placement);
void dim2icicle_release(void);
void dim2icicle_initialise(void);

extern ObjectDescriptor gDIM2IcicleObjDescriptor;

#endif /* DLLS_OBJECTS_477_DIM2ICICLE_H_ */
