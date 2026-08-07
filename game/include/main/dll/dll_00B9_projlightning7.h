#ifndef MAIN_DLL_DLL_00B9_PROJLIGHTNING7_H_
#define MAIN_DLL_DLL_00B9_PROJLIGHTNING7_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projlightning7UnsupportedFn)(void);

typedef struct Projlightning7ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projlightning7UnsupportedFn unsupported;
} Projlightning7ResourceDescriptor;

STATIC_ASSERT(offsetof(Projlightning7ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projlightning7ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projlightning7ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projlightning7ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projlightning7ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projlightning7ResourceDescriptor) == 0x20);

extern Projlightning7ResourceDescriptor gProjlightning7ResourceDescriptor;

int projlightning7_doUnsupported(void);
void projlightning7_release(void);
void projlightning7_initialise(void);

#endif /* MAIN_DLL_DLL_00B9_PROJLIGHTNING7_H_ */
