#ifndef DLLS_OBJECTS_457_DIMDISMOUNT_H_
#define DLLS_OBJECTS_457_DIMDISMOUNT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef enum DimDismountObjectGroup {
    DIM_DISMOUNT_MOUNT_OBJECT_GROUP = 0xA,
    DIM_DISMOUNT_POINT_OBJECT_GROUP = 0x13
} DimDismountObjectGroup;

typedef struct DimDismountPlacement DimDismountPlacement;

void DIMDismountPoint_runOppositeSideSequence(GameObject* obj, int side);
int DIMDismountPoint_getPlayerSide(GameObject* obj);
int DIMDismountPoint_getExtraSize(void);
int DIMDismountPoint_getObjectTypeId(void);
void DIMDismountPoint_free(GameObject* obj);
void DIMDismountPoint_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible);
void DIMDismountPoint_hitDetect(void);
void DIMDismountPoint_update(GameObject* obj);
void DIMDismountPoint_init(GameObject* obj, const DimDismountPlacement* placement);
void DIMDismountPoint_release(void);
void DIMDismountPoint_initialise(void);

struct DIMDismountPointObjDescriptorType;
extern struct DIMDismountPointObjDescriptorType gDIMDismountPointObjDescriptor;

#endif
