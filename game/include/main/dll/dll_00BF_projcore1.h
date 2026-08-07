#ifndef MAIN_DLL_DLL_00BF_PROJCORE1_H_
#define MAIN_DLL_DLL_00BF_PROJCORE1_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projcore1UnsupportedFn)(void);

typedef struct Projcore1ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projcore1UnsupportedFn unsupported;
} Projcore1ResourceDescriptor;

STATIC_ASSERT(offsetof(Projcore1ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projcore1ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projcore1ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projcore1ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projcore1ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projcore1ResourceDescriptor) == 0x20);

extern Projcore1ResourceDescriptor gProjcore1ResourceDescriptor;

int projcore1_doUnsupported(void);
void projcore1_release(void);
void projcore1_initialise(void);

#endif /* MAIN_DLL_DLL_00BF_PROJCORE1_H_ */
