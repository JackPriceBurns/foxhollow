#ifndef MAIN_DLL_DLL_00B3_PROJLIGHTNING4_H_
#define MAIN_DLL_DLL_00B3_PROJLIGHTNING4_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projlightning4UnsupportedFn)(void);

typedef struct Projlightning4ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projlightning4UnsupportedFn unsupported;
} Projlightning4ResourceDescriptor;

STATIC_ASSERT(offsetof(Projlightning4ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projlightning4ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projlightning4ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projlightning4ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projlightning4ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projlightning4ResourceDescriptor) == 0x20);

extern Projlightning4ResourceDescriptor gProjlightning4ResourceDescriptor;

int projlightning4_doUnsupported(void);
void projlightning4_release(void);
void projlightning4_initialise(void);

#endif /* MAIN_DLL_DLL_00B3_PROJLIGHTNING4_H_ */
