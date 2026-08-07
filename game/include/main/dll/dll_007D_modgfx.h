#ifndef MAIN_DLL_DLL_007D_MODGFX_H_
#define MAIN_DLL_DLL_007D_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef s16 (*Dll7DSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                            u32 unused, f32* scaleOverride);

typedef struct Dll7DInterface {
    ResourceDescriptorCallback reserved;
    Dll7DSpawnFn spawn;
} Dll7DInterface;

STATIC_ASSERT(offsetof(Dll7DInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll7DInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll7DInterface) == 0x08);

typedef struct Dll7DResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll7DSpawnFn spawn;
} Dll7DResourceDescriptor;

STATIC_ASSERT(offsetof(Dll7DResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll7DResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll7DResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll7DResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll7DResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll7DResourceDescriptor) == 0x20);

extern Dll7DResourceDescriptor gDll7DResourceDescriptor;

s16 dll_7D_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, u32 unused,
                       f32* scaleOverride);
void dll_7D_release(void);
void dll_7D_initialise(void);

#endif /* MAIN_DLL_DLL_007D_MODGFX_H_ */
