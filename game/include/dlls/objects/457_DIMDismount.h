#ifndef DLLS_OBJECTS_457_DIMDISMOUNT_H_
#define DLLS_OBJECTS_457_DIMDISMOUNT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define DIM_DISMOUNT_POINT_OBJECT_GROUP 0x13

typedef struct DimDismountPlacement {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19[0xB];
} DimDismountPlacement;

typedef struct DimDismountState {
    f32 planeNormalX;
    f32 planeNormalY;
    f32 planeNormalZ;
    f32 planeConstant;
} DimDismountState;

STATIC_ASSERT(offsetof(DimDismountPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimDismountPlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(DimDismountPlacement, unknown19) == 0x19);
STATIC_ASSERT(sizeof(DimDismountPlacement) == 0x24);

STATIC_ASSERT(offsetof(DimDismountState, planeNormalX) == 0x00);
STATIC_ASSERT(offsetof(DimDismountState, planeNormalY) == 0x04);
STATIC_ASSERT(offsetof(DimDismountState, planeNormalZ) == 0x08);
STATIC_ASSERT(offsetof(DimDismountState, planeConstant) == 0x0C);
STATIC_ASSERT(sizeof(DimDismountState) == 0x10);

void DIMDismountPoint_func0B(GameObject* obj, int side);
int DIMDismountPoint_func0A(GameObject* obj);
int DIMDismountPoint_getExtraSize(void);
int DIMDismountPoint_getObjectTypeId(void);
void DIMDismountPoint_free(GameObject* obj);
void DIMDismountPoint_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible);
void DIMDismountPoint_hitDetect(void);
void DIMDismountPoint_update(GameObject* obj);
void DIMDismountPoint_init(GameObject* obj, DimDismountPlacement* placement);
void DIMDismountPoint_release(void);
void DIMDismountPoint_initialise(void);

extern ObjectDescriptor12 gDIMDismountPointObjDescriptor;

#endif /* DLLS_OBJECTS_457_DIMDISMOUNT_H_ */
