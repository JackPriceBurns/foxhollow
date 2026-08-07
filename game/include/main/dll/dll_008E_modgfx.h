#ifndef MAIN_DLL_DLL_008E_MODGFX_H_
#define MAIN_DLL_DLL_008E_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll8ESpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll8EResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll8ESpawnFn spawn;
} Dll8EResourceDescriptor;

STATIC_ASSERT(offsetof(Dll8EResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll8EResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll8EResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll8EResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll8EResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll8EResourceDescriptor) == 0x20);

extern Dll8EResourceDescriptor gDll8EResourceDescriptor;

void dll_8E_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_8E_release(void);
void dll_8E_initialise(void);

#endif /* MAIN_DLL_DLL_008E_MODGFX_H_ */
