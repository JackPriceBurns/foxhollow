#ifndef MAIN_DLL_DLL_006C_DUMMY6C_H_
#define MAIN_DLL_DLL_006C_DUMMY6C_H_

#include "global.h"
#include "main/resource.h"

typedef struct Dummy6CDescriptor {
    ResourceDescriptorHeader header;
    void* reserved18;
    int (*init)(void);
} Dummy6CDescriptor;

STATIC_ASSERT(offsetof(Dummy6CDescriptor, header.metadata) == 0x00);
STATIC_ASSERT(offsetof(Dummy6CDescriptor, header.acquire) == 0x10);
STATIC_ASSERT(offsetof(Dummy6CDescriptor, header.release) == 0x14);
STATIC_ASSERT(offsetof(Dummy6CDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dummy6CDescriptor, init) == 0x1C);
STATIC_ASSERT(sizeof(Dummy6CDescriptor) == 0x20);

extern Dummy6CDescriptor gDummy6CDescriptor;

int Dummy6C_init(void);
void Dummy6C_release(void);
void Dummy6C_initialise(void);

#endif /* MAIN_DLL_DLL_006C_DUMMY6C_H_ */
