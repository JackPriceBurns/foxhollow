#ifndef MAIN_DLL_DLL_00B0_PROJLIGHTNING2_H_
#define MAIN_DLL_DLL_00B0_PROJLIGHTNING2_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projlightning2UnsupportedFn)(void);

typedef struct Projlightning2ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projlightning2UnsupportedFn unsupported;
} Projlightning2ResourceDescriptor;

STATIC_ASSERT(offsetof(Projlightning2ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projlightning2ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projlightning2ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projlightning2ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projlightning2ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projlightning2ResourceDescriptor) == 0x20);

extern Projlightning2ResourceDescriptor gProjlightning2ResourceDescriptor;

int projlightning2_doUnsupported(void);
void projlightning2_release(void);
void projlightning2_initialise(void);

#endif /* MAIN_DLL_DLL_00B0_PROJLIGHTNING2_H_ */
