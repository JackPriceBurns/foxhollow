#ifndef MAIN_DLL_DLL_00BB_PROJWALLPOWER_H_
#define MAIN_DLL_DLL_00BB_PROJWALLPOWER_H_

#include "global.h"
#include "main/resource.h"

typedef int (*ProjwallpowerUnsupportedFn)(void);

typedef struct ProjwallpowerResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    ProjwallpowerUnsupportedFn unsupported;
} ProjwallpowerResourceDescriptor;

STATIC_ASSERT(offsetof(ProjwallpowerResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(ProjwallpowerResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(ProjwallpowerResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(ProjwallpowerResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(ProjwallpowerResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(ProjwallpowerResourceDescriptor) == 0x20);

extern ProjwallpowerResourceDescriptor gProjwallpowerResourceDescriptor;

int projwallpower_doUnsupported(void);
void projwallpower_release(void);
void projwallpower_initialise(void);

#endif /* MAIN_DLL_DLL_00BB_PROJWALLPOWER_H_ */
