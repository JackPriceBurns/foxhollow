#ifndef MAIN_DLL_DLL_0071_MODGFX_H_
#define MAIN_DLL_DLL_0071_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll71SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll71Interface {
    ResourceDescriptorCallback reserved;
    Dll71SpawnFn spawn;
} Dll71Interface;

STATIC_ASSERT(offsetof(Dll71Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll71Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll71Interface) == 0x08);

typedef struct Dll71ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll71SpawnFn spawn;
} Dll71ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll71ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll71ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll71ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll71ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll71ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll71ResourceDescriptor) == 0x20);

extern Dll71ResourceDescriptor gDll71ResourceDescriptor;

void dll_71_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_71_release(void);
void dll_71_initialise(void);

#endif /* MAIN_DLL_DLL_0071_MODGFX_H_ */
