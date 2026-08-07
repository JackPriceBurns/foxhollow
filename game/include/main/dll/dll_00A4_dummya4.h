#ifndef MAIN_DLL_DLL_00A4_DUMMYA4_H_
#define MAIN_DLL_DLL_00A4_DUMMYA4_H_

#include "global.h"
#include "main/resource.h"

typedef struct DummyA4ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    int (*returnZero)(void);
} DummyA4ResourceDescriptor;

STATIC_ASSERT(offsetof(DummyA4ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(DummyA4ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(DummyA4ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(DummyA4ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(DummyA4ResourceDescriptor, returnZero) == 0x1C);
STATIC_ASSERT(sizeof(DummyA4ResourceDescriptor) == 0x20);

extern DummyA4ResourceDescriptor gDummyA4ResourceDescriptor;

int DummyA4_returnZero(void);
void DummyA4_release(void);
void DummyA4_initialise(void);

#endif /* MAIN_DLL_DLL_00A4_DUMMYA4_H_ */
