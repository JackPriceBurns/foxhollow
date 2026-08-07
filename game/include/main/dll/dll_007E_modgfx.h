#ifndef MAIN_DLL_DLL_007E_MODGFX_H_
#define MAIN_DLL_DLL_007E_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll7ESpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                             u32 unused, f32* scaleOverride);

typedef struct Dll7EInterface {
    ResourceDescriptorCallback reserved;
    Dll7ESpawnFn spawn;
} Dll7EInterface;

STATIC_ASSERT(offsetof(Dll7EInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll7EInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll7EInterface) == 0x08);

typedef struct Dll7EResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll7ESpawnFn spawn;
    u32 opaqueTail;
} Dll7EResourceDescriptor;

STATIC_ASSERT(offsetof(Dll7EResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll7EResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll7EResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll7EResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll7EResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll7EResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll7EResourceDescriptor) == 0x24);

extern Dll7EResourceDescriptor gDll7EResourceDescriptor;

void dll_7E_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, u32 unused,
                        f32* scaleOverride);
void dll_7E_release(void);
void dll_7E_initialise(void);

#endif /* MAIN_DLL_DLL_007E_MODGFX_H_ */
