#ifndef MAIN_DLL_DLL_0074_MODGFX_H_
#define MAIN_DLL_DLL_0074_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll74SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll74Interface {
    ResourceDescriptorCallback reserved;
    Dll74SpawnFn spawn;
} Dll74Interface;

STATIC_ASSERT(offsetof(Dll74Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll74Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll74Interface) == 0x08);

typedef struct Dll74ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll74SpawnFn spawn;
    u32 opaqueTail;
} Dll74ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll74ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll74ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll74ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll74ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll74ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll74ResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll74ResourceDescriptor) == 0x24);

extern Dll74ResourceDescriptor gDll74ResourceDescriptor;

void dll_74_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_74_release(void);
void dll_74_initialise(void);

#endif /* MAIN_DLL_DLL_0074_MODGFX_H_ */
