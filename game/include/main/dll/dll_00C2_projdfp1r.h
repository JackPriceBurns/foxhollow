#ifndef MAIN_DLL_DLL_00C2_PROJDFP1R_H_
#define MAIN_DLL_DLL_00C2_PROJDFP1R_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projdfp1rUnsupportedFn)(void);

typedef struct Projdfp1rResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projdfp1rUnsupportedFn unsupported;
} Projdfp1rResourceDescriptor;

STATIC_ASSERT(offsetof(Projdfp1rResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projdfp1rResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projdfp1rResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projdfp1rResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projdfp1rResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projdfp1rResourceDescriptor) == 0x20);

extern Projdfp1rResourceDescriptor gProjdfp1rResourceDescriptor;

int projdfp1r_doUnsupported(void);
void projdfp1r_release(void);
void projdfp1r_initialise(void);

#endif /* MAIN_DLL_DLL_00C2_PROJDFP1R_H_ */
