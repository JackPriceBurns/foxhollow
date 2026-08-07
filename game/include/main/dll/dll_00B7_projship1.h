#ifndef MAIN_DLL_DLL_00B7_PROJSHIP1_H_
#define MAIN_DLL_DLL_00B7_PROJSHIP1_H_

#include "global.h"
#include "main/resource.h"

typedef int (*Projship1UnsupportedFn)(void);

typedef struct Projship1ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Projship1UnsupportedFn unsupported;
} Projship1ResourceDescriptor;

STATIC_ASSERT(offsetof(Projship1ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Projship1ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Projship1ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Projship1ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Projship1ResourceDescriptor, unsupported) == 0x1C);
STATIC_ASSERT(sizeof(Projship1ResourceDescriptor) == 0x20);

extern Projship1ResourceDescriptor gProjship1ResourceDescriptor;

int projship1_doUnsupported(void);
void projship1_release(void);
void projship1_initialise(void);

#endif /* MAIN_DLL_DLL_00B7_PROJSHIP1_H_ */
