#ifndef MAIN_DLL_DLL_00B6_PROJSQUIRT1_H_
#define MAIN_DLL_DLL_00B6_PROJSQUIRT1_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projsquirt1UnsupportedFn)(void);

typedef struct Projsquirt1ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projsquirt1UnsupportedFn unsupported;
} Projsquirt1ResourceDescriptor;

STATIC_ASSERT(offsetof(Projsquirt1ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projsquirt1ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projsquirt1ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projsquirt1ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projsquirt1ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projsquirt1ResourceDescriptor) == 0x20);

extern Projsquirt1ResourceDescriptor gProjsquirt1ResourceDescriptor;

int projsquirt1_doUnsupported(void);
void projsquirt1_release(void);
void projsquirt1_initialise(void);

#endif /* MAIN_DLL_DLL_00B6_PROJSQUIRT1_H_ */
