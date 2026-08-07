#ifndef MAIN_DLL_DLL_00AE_PROJROOMBEAM_H_
#define MAIN_DLL_DLL_00AE_PROJROOMBEAM_H_

#include "global.h"
#include "main/resource.h"

typedef int (*ProjroombeamUnsupportedFn)(void);

typedef struct ProjroombeamResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    ProjroombeamUnsupportedFn unsupported;
} ProjroombeamResourceDescriptor;

STATIC_ASSERT(offsetof(ProjroombeamResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(ProjroombeamResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(ProjroombeamResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(ProjroombeamResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(ProjroombeamResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(ProjroombeamResourceDescriptor) == 0x20);

extern ProjroombeamResourceDescriptor gProjroombeamResourceDescriptor;

int projroombeam_doUnsupported(void);
void projroombeam_release(void);
void projroombeam_initialise(void);

#endif /* MAIN_DLL_DLL_00AE_PROJROOMBEAM_H_ */
