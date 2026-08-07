#ifndef MAIN_DLL_DLL_00B2_PROJROBOTFIRE_H_
#define MAIN_DLL_DLL_00B2_PROJROBOTFIRE_H_

#include "global.h"
#include "main/resource.h"

typedef int (*ProjrobotfireUnsupportedFn)(void);

typedef struct ProjrobotfireResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    ProjrobotfireUnsupportedFn unsupported;
} ProjrobotfireResourceDescriptor;

STATIC_ASSERT(offsetof(ProjrobotfireResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(ProjrobotfireResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(ProjrobotfireResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(ProjrobotfireResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(ProjrobotfireResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(ProjrobotfireResourceDescriptor) == 0x20);

extern ProjrobotfireResourceDescriptor gProjrobotfireResourceDescriptor;

int projrobotfire_doUnsupported(void);
void projrobotfire_release(void);
void projrobotfire_initialise(void);

#endif /* MAIN_DLL_DLL_00B2_PROJROBOTFIRE_H_ */
