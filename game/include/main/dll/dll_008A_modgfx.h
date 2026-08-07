#ifndef MAIN_DLL_DLL_008A_MODGFX_H_
#define MAIN_DLL_DLL_008A_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll8ASpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll8AResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll8ASpawnFn spawn;
} Dll8AResourceDescriptor;

STATIC_ASSERT(offsetof(Dll8AResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll8AResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll8AResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll8AResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll8AResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll8AResourceDescriptor) == 0x20);

extern Dll8AResourceDescriptor gDll8AResourceDescriptor;

void dll_8A_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_8A_release(void);
void dll_8A_initialise(void);

#endif /* MAIN_DLL_DLL_008A_MODGFX_H_ */
