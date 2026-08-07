#ifndef MAIN_DLL_DLL_00AF_PROJLIGHTNING1_H_
#define MAIN_DLL_DLL_00AF_PROJLIGHTNING1_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projlightning1UnsupportedFn)(void);

typedef struct Projlightning1ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projlightning1UnsupportedFn unsupported;
} Projlightning1ResourceDescriptor;

STATIC_ASSERT(offsetof(Projlightning1ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projlightning1ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projlightning1ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projlightning1ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projlightning1ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projlightning1ResourceDescriptor) == 0x20);

extern Projlightning1ResourceDescriptor gProjlightning1ResourceDescriptor;

int projlightning1_doUnsupported(void);
void projlightning1_release(void);
void projlightning1_initialise(void);

#endif /* MAIN_DLL_DLL_00AF_PROJLIGHTNING1_H_ */
