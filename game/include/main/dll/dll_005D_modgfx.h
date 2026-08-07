#ifndef MAIN_DLL_DLL_005D_MODGFX_H_
#define MAIN_DLL_DLL_005D_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll5DSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll5DInterface {
    ResourceDescriptorCallback reserved;
    Dll5DSpawnFn spawn;
} Dll5DInterface;

STATIC_ASSERT(offsetof(Dll5DInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll5DInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll5DInterface) == 0x08);

typedef struct Dll5DResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll5DSpawnFn spawn;
    u32 padding;
} Dll5DResourceDescriptor;

STATIC_ASSERT(offsetof(Dll5DResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll5DResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll5DResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll5DResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll5DResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll5DResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(Dll5DResourceDescriptor) == 0x24);

extern Dll5DResourceDescriptor gDll5DResourceDescriptor;

void dll_5D_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_5D_release(void);
void dll_5D_initialise(void);

#endif /* MAIN_DLL_DLL_005D_MODGFX_H_ */
