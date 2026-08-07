#ifndef MAIN_DLL_DLL_006D_MODGFX_H_
#define MAIN_DLL_DLL_006D_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll6DSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll6DInterface {
    ResourceDescriptorCallback reserved;
    Dll6DSpawnFn spawn;
} Dll6DInterface;

STATIC_ASSERT(offsetof(Dll6DInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll6DInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll6DInterface) == 0x08);

typedef struct Dll6DResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll6DSpawnFn spawn;
} Dll6DResourceDescriptor;

STATIC_ASSERT(offsetof(Dll6DResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll6DResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll6DResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll6DResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll6DResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll6DResourceDescriptor) == 0x20);

extern Dll6DResourceDescriptor gDll6DResourceDescriptor;

void dll_6D_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_6D_release(void);
void dll_6D_initialise(void);

#endif /* MAIN_DLL_DLL_006D_MODGFX_H_ */
