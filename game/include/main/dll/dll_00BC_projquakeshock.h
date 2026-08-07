#ifndef MAIN_DLL_DLL_00BC_PROJQUAKESHOCK_H_
#define MAIN_DLL_DLL_00BC_PROJQUAKESHOCK_H_

#include "global.h"
#include "main/resource.h"

typedef int (*ProjquakeshockUnsupportedFn)(void);

typedef struct ProjquakeshockResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    ProjquakeshockUnsupportedFn unsupported;
} ProjquakeshockResourceDescriptor;

STATIC_ASSERT(offsetof(ProjquakeshockResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(ProjquakeshockResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(ProjquakeshockResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(ProjquakeshockResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(ProjquakeshockResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(ProjquakeshockResourceDescriptor) == 0x20);

extern ProjquakeshockResourceDescriptor gProjquakeshockResourceDescriptor;

int projquakeshock_doUnsupported(void);
void projquakeshock_release(void);
void projquakeshock_initialise(void);

#endif /* MAIN_DLL_DLL_00BC_PROJQUAKESHOCK_H_ */
