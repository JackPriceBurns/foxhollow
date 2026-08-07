#ifndef MAIN_DLL_DLL_0063_MODGFX_H_
#define MAIN_DLL_DLL_0063_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/resource.h"

typedef s16 (*Dll63SpawnFn)(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags, int unusedModelId,
                            void* unusedParams);

typedef struct Dll63Interface {
    ResourceDescriptorCallback reserved;
    Dll63SpawnFn spawn;
} Dll63Interface;

STATIC_ASSERT(offsetof(Dll63Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll63Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll63Interface) == 0x08);

typedef struct Dll63ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll63SpawnFn spawn;
} Dll63ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll63ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll63ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll63ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll63ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll63ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll63ResourceDescriptor) == 0x20);

extern Dll63ResourceDescriptor gDll63ResourceDescriptor;

s16 dll_63_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags, int unusedModelId,
                       void* unusedParams);
void dll_63_release(void);
void dll_63_initialise(void);

#endif /* MAIN_DLL_DLL_0063_MODGFX_H_ */
