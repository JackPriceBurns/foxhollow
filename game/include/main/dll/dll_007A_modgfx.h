#ifndef MAIN_DLL_DLL_007A_MODGFX_H_
#define MAIN_DLL_DLL_007A_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef s16 (*Dll7ASpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll7AInterface {
    ResourceDescriptorCallback reserved;
    Dll7ASpawnFn spawn;
} Dll7AInterface;

STATIC_ASSERT(offsetof(Dll7AInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll7AInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll7AInterface) == 0x08);

typedef struct Dll7AResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll7ASpawnFn spawn;
} Dll7AResourceDescriptor;

STATIC_ASSERT(offsetof(Dll7AResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll7AResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll7AResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll7AResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll7AResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll7AResourceDescriptor) == 0x20);

extern Dll7AResourceDescriptor gDll7AResourceDescriptor;

s16 dll_7A_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_7A_release(void);
void dll_7A_initialise(void);

#endif /* MAIN_DLL_DLL_007A_MODGFX_H_ */
