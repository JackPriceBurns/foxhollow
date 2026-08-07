#ifndef MAIN_DLL_DLL_0065_MODGFX_H_
#define MAIN_DLL_DLL_0065_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/resource.h"

typedef void (*Dll65SpawnFn)(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);

typedef struct Dll65Interface {
    ResourceDescriptorCallback reserved;
    Dll65SpawnFn spawn;
} Dll65Interface;

STATIC_ASSERT(offsetof(Dll65Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll65Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll65Interface) == 0x08);

typedef struct Dll65ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll65SpawnFn spawn;
} Dll65ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll65ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll65ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll65ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll65ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll65ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll65ResourceDescriptor) == 0x20);

extern Dll65ResourceDescriptor gDll65ResourceDescriptor;

void dll_65_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);
void dll_65_release(void);
void dll_65_initialise(void);

#endif /* MAIN_DLL_DLL_0065_MODGFX_H_ */
