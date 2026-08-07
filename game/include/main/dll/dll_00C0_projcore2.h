#ifndef MAIN_DLL_DLL_00C0_PROJCORE2_H_
#define MAIN_DLL_DLL_00C0_PROJCORE2_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projcore2UnsupportedFn)(void);

typedef struct Projcore2ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projcore2UnsupportedFn unsupported;
} Projcore2ResourceDescriptor;

STATIC_ASSERT(offsetof(Projcore2ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projcore2ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projcore2ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projcore2ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projcore2ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projcore2ResourceDescriptor) == 0x20);

extern Projcore2ResourceDescriptor gProjcore2ResourceDescriptor;

int projcore2_doUnsupported(void);
void projcore2_release(void);
void projcore2_initialise(void);

#endif /* MAIN_DLL_DLL_00C0_PROJCORE2_H_ */
