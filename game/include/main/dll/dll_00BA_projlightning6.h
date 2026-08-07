#ifndef MAIN_DLL_DLL_00BA_PROJLIGHTNING6_H_
#define MAIN_DLL_DLL_00BA_PROJLIGHTNING6_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projlightning6UnsupportedFn)(void);

typedef struct Projlightning6ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projlightning6UnsupportedFn unsupported;
} Projlightning6ResourceDescriptor;

STATIC_ASSERT(offsetof(Projlightning6ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projlightning6ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projlightning6ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projlightning6ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projlightning6ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projlightning6ResourceDescriptor) == 0x20);

extern Projlightning6ResourceDescriptor gProjlightning6ResourceDescriptor;

int projlightning6_doUnsupported(void);
void projlightning6_release(void);
void projlightning6_initialise(void);

#endif /* MAIN_DLL_DLL_00BA_PROJLIGHTNING6_H_ */
