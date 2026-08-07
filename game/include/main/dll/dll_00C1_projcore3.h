#ifndef MAIN_DLL_DLL_00C1_PROJCORE3_H_
#define MAIN_DLL_DLL_00C1_PROJCORE3_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projcore3UnsupportedFn)(void);

typedef struct Projcore3ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projcore3UnsupportedFn unsupported;
} Projcore3ResourceDescriptor;

STATIC_ASSERT(offsetof(Projcore3ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projcore3ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projcore3ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projcore3ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projcore3ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projcore3ResourceDescriptor) == 0x20);

extern Projcore3ResourceDescriptor gProjcore3ResourceDescriptor;

int projcore3_doUnsupported(void);
void projcore3_release(void);
void projcore3_initialise(void);

#endif /* MAIN_DLL_DLL_00C1_PROJCORE3_H_ */
