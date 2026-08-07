#ifndef MAIN_DLL_DLL_0068_MODGFX_H_
#define MAIN_DLL_DLL_0068_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/resource.h"

typedef void (*Dll68SpawnFn)(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);

typedef struct Dll68Interface {
    ResourceDescriptorCallback reserved;
    Dll68SpawnFn spawn;
} Dll68Interface;

STATIC_ASSERT(offsetof(Dll68Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll68Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll68Interface) == 0x08);

typedef struct Dll68ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll68SpawnFn spawn;
    u32 padding;
} Dll68ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll68ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll68ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll68ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll68ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll68ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll68ResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(Dll68ResourceDescriptor) == 0x24);

extern Dll68ResourceDescriptor gDll68ResourceDescriptor;

void dll_68_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags);
void dll_68_release(void);
void dll_68_initialise(void);

#endif /* MAIN_DLL_DLL_0068_MODGFX_H_ */
