#ifndef MAIN_DLL_DLL_0066_MODGFX_H_
#define MAIN_DLL_DLL_0066_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/resource.h"

typedef void (*Dll66SpawnFn)(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);

typedef struct Dll66Interface {
    ResourceDescriptorCallback reserved;
    Dll66SpawnFn spawn;
} Dll66Interface;

STATIC_ASSERT(offsetof(Dll66Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll66Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll66Interface) == 0x08);

typedef struct Dll66ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll66SpawnFn spawn;
    u32 padding;
} Dll66ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll66ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll66ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll66ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll66ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll66ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll66ResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(Dll66ResourceDescriptor) == 0x24);

extern Dll66ResourceDescriptor gDll66ResourceDescriptor;

void dll_66_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);
void dll_66_release(void);
void dll_66_initialise(void);

#endif /* MAIN_DLL_DLL_0066_MODGFX_H_ */
