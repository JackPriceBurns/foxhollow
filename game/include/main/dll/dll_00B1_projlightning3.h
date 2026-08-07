#ifndef MAIN_DLL_DLL_00B1_PROJLIGHTNING3_H_
#define MAIN_DLL_DLL_00B1_PROJLIGHTNING3_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projlightning3UnsupportedFn)(void);

typedef struct Projlightning3ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projlightning3UnsupportedFn unsupported;
} Projlightning3ResourceDescriptor;

STATIC_ASSERT(offsetof(Projlightning3ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projlightning3ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projlightning3ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projlightning3ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projlightning3ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projlightning3ResourceDescriptor) == 0x20);

extern Projlightning3ResourceDescriptor gProjlightning3ResourceDescriptor;

int projlightning3_doUnsupported(void);
void projlightning3_release(void);
void projlightning3_initialise(void);

#endif /* MAIN_DLL_DLL_00B1_PROJLIGHTNING3_H_ */
