#ifndef MAIN_DLL_DLL_00B4_PROJENERGISE1_H_
#define MAIN_DLL_DLL_00B4_PROJENERGISE1_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projenergise1UnsupportedFn)(void);

typedef struct Projenergise1ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projenergise1UnsupportedFn unsupported;
} Projenergise1ResourceDescriptor;

STATIC_ASSERT(offsetof(Projenergise1ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projenergise1ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projenergise1ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projenergise1ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projenergise1ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projenergise1ResourceDescriptor) == 0x20);

extern Projenergise1ResourceDescriptor gProjenergise1ResourceDescriptor;

int projenergise1_doUnsupported(void);
void projenergise1_release(void);
void projenergise1_initialise(void);

#endif /* MAIN_DLL_DLL_00B4_PROJENERGISE1_H_ */
