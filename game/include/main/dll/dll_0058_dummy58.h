#ifndef MAIN_DLL_DLL_0058_DUMMY58_H_
#define MAIN_DLL_DLL_0058_DUMMY58_H_

#include "global.h"
#include "main/resource.h"

typedef struct Dummy58Descriptor {
    ResourceDescriptorHeader header;
    void* reserved18;
    int (*init)(void);
} Dummy58Descriptor;

STATIC_ASSERT(offsetof(Dummy58Descriptor, header.metadata) == 0x00);
STATIC_ASSERT(offsetof(Dummy58Descriptor, header.acquire) == 0x10);
STATIC_ASSERT(offsetof(Dummy58Descriptor, header.release) == 0x14);
STATIC_ASSERT(offsetof(Dummy58Descriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dummy58Descriptor, init) == 0x1C);
STATIC_ASSERT(sizeof(Dummy58Descriptor) == 0x20);

extern Dummy58Descriptor gDummy58Descriptor;

int Dummy58_init(void);
void Dummy58_release(void);
void Dummy58_initialise(void);

#endif /* MAIN_DLL_DLL_0058_DUMMY58_H_ */
