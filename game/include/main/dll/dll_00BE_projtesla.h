#ifndef MAIN_DLL_DLL_00BE_PROJTESLA_H_
#define MAIN_DLL_DLL_00BE_PROJTESLA_H_

#include "global.h"
#include "main/resource.h"

typedef int (*ProjteslaUnsupportedFn)(void);

typedef struct ProjteslaResourceDescriptor {
    ResourceDescriptorHeader header;
    void* reserved18;
    ProjteslaUnsupportedFn unsupported;
} ProjteslaResourceDescriptor;

STATIC_ASSERT(offsetof(ProjteslaResourceDescriptor, header.metadata) == 0x00);
STATIC_ASSERT(offsetof(ProjteslaResourceDescriptor, header.acquire) == 0x10);
STATIC_ASSERT(offsetof(ProjteslaResourceDescriptor, header.release) == 0x14);
STATIC_ASSERT(offsetof(ProjteslaResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(ProjteslaResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(ProjteslaResourceDescriptor) == 0x20);

extern ProjteslaResourceDescriptor gProjteslaResourceDescriptor;

int projtesla_doUnsupported(void);
void projtesla_release(void);
void projtesla_initialise(void);

#endif /* MAIN_DLL_DLL_00BE_PROJTESLA_H_ */
