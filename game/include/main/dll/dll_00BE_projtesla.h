#ifndef MAIN_DLL_DLL_00BE_PROJTESLA_H_
#define MAIN_DLL_DLL_00BE_PROJTESLA_H_

#include "global.h"
#include "main/resource.h"

typedef int (*ProjteslaUnsupportedFn)(void);

typedef struct ProjteslaResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    ProjteslaUnsupportedFn unsupported;
} ProjteslaResourceDescriptor;

STATIC_ASSERT(offsetof(ProjteslaResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(ProjteslaResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(ProjteslaResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(ProjteslaResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(ProjteslaResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(ProjteslaResourceDescriptor) == 0x20);

extern ProjteslaResourceDescriptor gProjteslaResourceDescriptor;

int projtesla_doUnsupported(void);
void projtesla_release(void);
void projtesla_initialise(void);

#endif /* MAIN_DLL_DLL_00BE_PROJTESLA_H_ */
