#ifndef MAIN_DLL_DLL_0064_MODGFX_H_
#define MAIN_DLL_DLL_0064_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/resource.h"

typedef void (*Dll64SpawnFn)(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);

typedef struct Dll64Interface {
    ResourceDescriptorCallback reserved;
    Dll64SpawnFn spawn;
} Dll64Interface;

STATIC_ASSERT(offsetof(Dll64Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll64Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll64Interface) == 0x08);

typedef struct Dll64ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll64SpawnFn spawn;
} Dll64ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll64ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll64ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll64ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll64ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll64ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll64ResourceDescriptor) == 0x20);

extern Dll64ResourceDescriptor gDll64ResourceDescriptor;

void dll_64_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);
void dll_64_release(void);
void dll_64_initialise(void);

#endif /* MAIN_DLL_DLL_0064_MODGFX_H_ */
