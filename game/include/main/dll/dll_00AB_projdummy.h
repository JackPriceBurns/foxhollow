#ifndef MAIN_DLL_DLL_00AB_PROJDUMMY_H_
#define MAIN_DLL_DLL_00AB_PROJDUMMY_H_

#include "global.h"
#include "main/resource.h"

typedef int (*ProjdummyUnsupportedFn)(void);

typedef struct ProjdummyResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    ProjdummyUnsupportedFn unsupported;
} ProjdummyResourceDescriptor;

STATIC_ASSERT(offsetof(ProjdummyResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(ProjdummyResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(ProjdummyResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(ProjdummyResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(ProjdummyResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(ProjdummyResourceDescriptor) == 0x20);

extern ProjdummyResourceDescriptor gProjdummyResourceDescriptor;

int projdummy_doUnsupported(void);
void projdummy_release(void);
void projdummy_initialise(void);

#endif /* MAIN_DLL_DLL_00AB_PROJDUMMY_H_ */
