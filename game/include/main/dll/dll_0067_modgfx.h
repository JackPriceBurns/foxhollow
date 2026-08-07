#ifndef MAIN_DLL_DLL_0067_MODGFX_H_
#define MAIN_DLL_DLL_0067_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/resource.h"

typedef void (*Dll67SpawnFn)(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);

typedef struct Dll67Interface {
    ResourceDescriptorCallback reserved;
    Dll67SpawnFn spawn;
} Dll67Interface;

STATIC_ASSERT(offsetof(Dll67Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll67Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll67Interface) == 0x08);

typedef struct Dll67ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll67SpawnFn spawn;
    u32 padding;
} Dll67ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll67ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll67ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll67ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll67ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll67ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll67ResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(Dll67ResourceDescriptor) == 0x24);

extern Dll67ResourceDescriptor gDll67ResourceDescriptor;

void dll_67_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);
void dll_67_release(void);
void dll_67_initialise(void);

#endif /* MAIN_DLL_DLL_0067_MODGFX_H_ */
