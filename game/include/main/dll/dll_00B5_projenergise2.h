#ifndef MAIN_DLL_DLL_00B5_PROJENERGISE2_H_
#define MAIN_DLL_DLL_00B5_PROJENERGISE2_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projenergise2UnsupportedFn)(void);

typedef struct Projenergise2ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projenergise2UnsupportedFn unsupported;
} Projenergise2ResourceDescriptor;

STATIC_ASSERT(offsetof(Projenergise2ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projenergise2ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projenergise2ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projenergise2ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projenergise2ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projenergise2ResourceDescriptor) == 0x20);

extern Projenergise2ResourceDescriptor gProjenergise2ResourceDescriptor;

int projenergise2_doUnsupported(void);
void projenergise2_release(void);
void projenergise2_initialise(void);

#endif /* MAIN_DLL_DLL_00B5_PROJENERGISE2_H_ */
