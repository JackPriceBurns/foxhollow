#ifndef MAIN_DLL_DLL_0058_DUMMY58_H_
#define MAIN_DLL_DLL_0058_DUMMY58_H_

#include "global.h"
#include "main/resource.h"

typedef struct Dummy58Descriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    int (*init)(void);
} Dummy58Descriptor;

STATIC_ASSERT(offsetof(Dummy58Descriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dummy58Descriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dummy58Descriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dummy58Descriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dummy58Descriptor, init) == 0x1C);
STATIC_ASSERT(sizeof(Dummy58Descriptor) == 0x20);

extern Dummy58Descriptor gDummy58Descriptor;

int Dummy58_init(void);
void Dummy58_release(void);
void Dummy58_initialise(void);

#endif /* MAIN_DLL_DLL_0058_DUMMY58_H_ */
