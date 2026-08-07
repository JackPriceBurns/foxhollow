#ifndef MAIN_DLL_DLL_007B_MODGFX_H_
#define MAIN_DLL_DLL_007B_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll7BSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll7BInterface {
    ResourceDescriptorCallback reserved;
    Dll7BSpawnFn spawn;
} Dll7BInterface;

STATIC_ASSERT(offsetof(Dll7BInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll7BInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll7BInterface) == 0x08);

typedef struct Dll7BResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll7BSpawnFn spawn;
    u32 opaqueTail;
} Dll7BResourceDescriptor;

STATIC_ASSERT(offsetof(Dll7BResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll7BResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll7BResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll7BResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll7BResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll7BResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll7BResourceDescriptor) == 0x24);

extern Dll7BResourceDescriptor gDll7BResourceDescriptor;

void dll_7B_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_7B_release(void);
void dll_7B_initialise(void);

#endif /* MAIN_DLL_DLL_007B_MODGFX_H_ */
