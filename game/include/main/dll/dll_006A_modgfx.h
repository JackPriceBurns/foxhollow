#ifndef MAIN_DLL_DLL_006A_MODGFX_H_
#define MAIN_DLL_DLL_006A_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef s16 (*Dll6ASpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                            int unusedArg4, void* unusedArg5);

typedef struct Dll6AInterface {
    ResourceDescriptorCallback reserved;
    Dll6ASpawnFn spawn;
} Dll6AInterface;

STATIC_ASSERT(offsetof(Dll6AInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll6AInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll6AInterface) == 0x08);

typedef struct Dll6AResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll6ASpawnFn spawn;
    u32 padding;
} Dll6AResourceDescriptor;

STATIC_ASSERT(offsetof(Dll6AResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll6AResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll6AResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll6AResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll6AResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll6AResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(Dll6AResourceDescriptor) == 0x24);

extern Dll6AResourceDescriptor gDll6AResourceDescriptor;

s16 dll_6A_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                       int unusedArg4, void* unusedArg5);
void dll_6A_release(void);
void dll_6A_initialise(void);

#endif /* MAIN_DLL_DLL_006A_MODGFX_H_ */
