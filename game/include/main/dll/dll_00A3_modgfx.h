#ifndef MAIN_DLL_DLL_00A3_MODGFX_H_
#define MAIN_DLL_DLL_00A3_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/resource.h"

typedef void (*DllA3SpawnFn)(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);

typedef struct DllA3Interface {
    ResourceDescriptorCallback reserved;
    DllA3SpawnFn spawn;
} DllA3Interface;

STATIC_ASSERT(offsetof(DllA3Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(DllA3Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(DllA3Interface) == 0x08);

typedef struct DllA3ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    DllA3SpawnFn spawn;
    u32 padding;
} DllA3ResourceDescriptor;

STATIC_ASSERT(offsetof(DllA3ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(DllA3ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(DllA3ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(DllA3ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(DllA3ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(DllA3ResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(DllA3ResourceDescriptor) == 0x24);

extern DllA3ResourceDescriptor gDllA3ResourceDescriptor;

void dll_A3_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);
void dll_A3_release(void);
void dll_A3_initialise(void);

#endif /* MAIN_DLL_DLL_00A3_MODGFX_H_ */
