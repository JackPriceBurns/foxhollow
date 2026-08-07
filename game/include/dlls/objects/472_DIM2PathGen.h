#ifndef DLLS_OBJECTS_472_DIM2PATHGEN_H_
#define DLLS_OBJECTS_472_DIM2PATHGEN_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define DIM2_PATH_GENERATOR_POINT_CAPACITY     200
#define DIM2_PATH_GENERATOR_NODE_DATA_CAPACITY (DIM2_PATH_GENERATOR_POINT_CAPACITY / 4)

/*
 * Only the prefix consumed by this DLL is modeled. The active-target retail
 * placement files are unavailable, so this does not claim a complete record
 * width.
 */
typedef struct Dim2PathGeneratorPlacementView {
    ObjPlacement base;
    s16 spawnPeriod;
    u8 childUnknown1A;
    u8 childUnknown1C;
    u8 childRotationXByte;
    u8 initialSpawnDelay;
    u16 primarySpawnType;
    s16 secondarySpawnType;
    s16 activeGameBit;
} Dim2PathGeneratorPlacementView;

/*
 * DIM2PathGenerator_getExtraSize() allocates the complete 0x9A8-byte block.
 * The three 0x320-byte coordinate buffers hold 200 samples each; the paired
 * snowball consumer indexes pathNodeData with the sample index divided by four.
 */
typedef struct Dim2PathGeneratorState {
    f32 originX;
    f32 originY;
    f32 originZ;
    f32 pathX[DIM2_PATH_GENERATOR_POINT_CAPACITY];
    f32 pathY[DIM2_PATH_GENERATOR_POINT_CAPACITY];
    f32 pathZ[DIM2_PATH_GENERATOR_POINT_CAPACITY];
    s8 pathNodeData[DIM2_PATH_GENERATOR_NODE_DATA_CAPACITY];
    s16 spawnTimer;
    s16 spawnPeriod;
    s16 spawnTypes[2];
    u8 pointCount;
    u8 flags;
} Dim2PathGeneratorState;

STATIC_ASSERT(offsetof(Dim2PathGeneratorPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dim2PathGeneratorPlacementView, spawnPeriod) == 0x18);
STATIC_ASSERT(offsetof(Dim2PathGeneratorPlacementView, childUnknown1A) == 0x1A);
STATIC_ASSERT(offsetof(Dim2PathGeneratorPlacementView, childUnknown1C) == 0x1B);
STATIC_ASSERT(offsetof(Dim2PathGeneratorPlacementView, childRotationXByte) == 0x1C);
STATIC_ASSERT(offsetof(Dim2PathGeneratorPlacementView, initialSpawnDelay) == 0x1D);
STATIC_ASSERT(offsetof(Dim2PathGeneratorPlacementView, primarySpawnType) == 0x1E);
STATIC_ASSERT(offsetof(Dim2PathGeneratorPlacementView, secondarySpawnType) == 0x20);
STATIC_ASSERT(offsetof(Dim2PathGeneratorPlacementView, activeGameBit) == 0x22);

STATIC_ASSERT(offsetof(Dim2PathGeneratorState, originX) == 0x000);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, originY) == 0x004);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, originZ) == 0x008);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, pathX) == 0x00C);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, pathY) == 0x32C);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, pathZ) == 0x64C);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, pathNodeData) == 0x96C);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, spawnTimer) == 0x99E);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, spawnPeriod) == 0x9A0);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, spawnTypes) == 0x9A2);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, pointCount) == 0x9A6);
STATIC_ASSERT(offsetof(Dim2PathGeneratorState, flags) == 0x9A7);
STATIC_ASSERT(sizeof(Dim2PathGeneratorState) == 0x9A8);

/* gDIM2PathGeneratorObjDescriptor from slot02 onwards: the export table other
   objects reach through obj->anim.dll. */
typedef struct Dim2PathGeneratorInterface {
    void* pad00[8];
    int (*getCurveVals)(GameObject* generator, f32** outPathX, f32** outPathY, f32** outPathZ, u8** outPathNodeData);
} Dim2PathGeneratorInterface;

#define DIM2_PATH_GENERATOR_INTERFACE(generator) \
    ((Dim2PathGeneratorInterface*)*((GameObject*)(generator))->anim.dll)

STATIC_ASSERT(offsetof(Dim2PathGeneratorInterface, getCurveVals) == 0x20);

u8 DIM2PathGenerator_getCurveVals(GameObject* obj, int** outPathX, int** outPathY, int** outPathZ,
                                  int** outPathNodeData);
int DIM2PathGenerator_getExtraSize(void);
int DIM2PathGenerator_getObjectTypeId(void);
void DIM2PathGenerator_free(GameObject* obj);
void DIM2PathGenerator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                              s8 visible);
void DIM2PathGenerator_hitDetect(void);
void DIM2PathGenerator_update(GameObject* obj);
/* The raw callback parameter preserves the exact primary-type reload in init. */
void DIM2PathGenerator_init(GameObject* obj, int* placementData);
void DIM2PathGenerator_release(void);
void DIM2PathGenerator_initialise(void);

extern ObjectDescriptor11WithPadding gDIM2PathGeneratorObjDescriptor;

#endif /* DLLS_OBJECTS_472_DIM2PATHGEN_H_ */
