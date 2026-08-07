#ifndef MAIN_DLL_DLL_00AC_PROJMAGICSTREAM_H_
#define MAIN_DLL_DLL_00AC_PROJMAGICSTREAM_H_

#include "global.h"
#include "main/resource.h"

typedef int (*ProjmagicstreamUnsupportedFn)(void);

typedef struct ProjmagicstreamResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    ProjmagicstreamUnsupportedFn unsupported;
} ProjmagicstreamResourceDescriptor;

STATIC_ASSERT(offsetof(ProjmagicstreamResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(ProjmagicstreamResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(ProjmagicstreamResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(ProjmagicstreamResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(ProjmagicstreamResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(ProjmagicstreamResourceDescriptor) == 0x20);

extern ProjmagicstreamResourceDescriptor gProjmagicstreamResourceDescriptor;

int projmagicstream_doUnsupported(void);
void projmagicstream_release(void);
void projmagicstream_initialise(void);

#endif /* MAIN_DLL_DLL_00AC_PROJMAGICSTREAM_H_ */
