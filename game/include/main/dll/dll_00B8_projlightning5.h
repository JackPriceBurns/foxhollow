#ifndef MAIN_DLL_DLL_00B8_PROJLIGHTNING5_H_
#define MAIN_DLL_DLL_00B8_PROJLIGHTNING5_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projlightning5UnsupportedFn)(void);

typedef struct Projlightning5ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projlightning5UnsupportedFn unsupported;
} Projlightning5ResourceDescriptor;

STATIC_ASSERT(offsetof(Projlightning5ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projlightning5ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projlightning5ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projlightning5ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projlightning5ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projlightning5ResourceDescriptor) == 0x20);

extern Projlightning5ResourceDescriptor gProjlightning5ResourceDescriptor;

int projlightning5_doUnsupported(void);
void projlightning5_release(void);
void projlightning5_initialise(void);

#endif /* MAIN_DLL_DLL_00B8_PROJLIGHTNING5_H_ */
