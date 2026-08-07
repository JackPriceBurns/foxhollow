#ifndef MAIN_DLL_DLL_006E_MODGFX_H_
#define MAIN_DLL_DLL_006E_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll6ESpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll6EInterface {
    ResourceDescriptorCallback reserved;
    Dll6ESpawnFn spawn;
} Dll6EInterface;

STATIC_ASSERT(offsetof(Dll6EInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll6EInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll6EInterface) == 0x08);

typedef struct Dll6EResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll6ESpawnFn spawn;
} Dll6EResourceDescriptor;

STATIC_ASSERT(offsetof(Dll6EResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll6EResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll6EResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll6EResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll6EResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll6EResourceDescriptor) == 0x20);

extern Dll6EResourceDescriptor gDll6EResourceDescriptor;

void dll_6E_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_6E_release(void);
void dll_6E_initialise(void);

#endif /* MAIN_DLL_DLL_006E_MODGFX_H_ */
