#ifndef MAIN_DLL_DLL_00BD_PROJSUNSHOCK_H_
#define MAIN_DLL_DLL_00BD_PROJSUNSHOCK_H_

#include "global.h"
#include "main/resource.h"

typedef int (*ProjsunshockUnsupportedFn)(void);

typedef struct ProjsunshockResourceDescriptor {
    ResourceDescriptorHeader header;
    void* reserved18;
    ProjsunshockUnsupportedFn unsupported;
} ProjsunshockResourceDescriptor;

STATIC_ASSERT(offsetof(ProjsunshockResourceDescriptor, header.metadata) == 0x00);
STATIC_ASSERT(offsetof(ProjsunshockResourceDescriptor, header.acquire) == 0x10);
STATIC_ASSERT(offsetof(ProjsunshockResourceDescriptor, header.release) == 0x14);
STATIC_ASSERT(offsetof(ProjsunshockResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(ProjsunshockResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(ProjsunshockResourceDescriptor) == 0x20);

extern ProjsunshockResourceDescriptor gProjsunshockResourceDescriptor;

int projsunshock_doUnsupported(void);
void projsunshock_release(void);
void projsunshock_initialise(void);

#endif /* MAIN_DLL_DLL_00BD_PROJSUNSHOCK_H_ */
