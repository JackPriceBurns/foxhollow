#ifndef MAIN_DLL_DLL_00AD_PROJMAGICEMMIT1_H_
#define MAIN_DLL_DLL_00AD_PROJMAGICEMMIT1_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projmagicemmit1UnsupportedFn)(void);

typedef struct Projmagicemmit1ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projmagicemmit1UnsupportedFn unsupported;
} Projmagicemmit1ResourceDescriptor;

STATIC_ASSERT(offsetof(Projmagicemmit1ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projmagicemmit1ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projmagicemmit1ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projmagicemmit1ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projmagicemmit1ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projmagicemmit1ResourceDescriptor) == 0x20);

extern Projmagicemmit1ResourceDescriptor gProjmagicemmit1ResourceDescriptor;

int projmagicemmit1_doUnsupported(void);
void projmagicemmit1_release(void);
void projmagicemmit1_initialise(void);

#endif /* MAIN_DLL_DLL_00AD_PROJMAGICEMMIT1_H_ */
