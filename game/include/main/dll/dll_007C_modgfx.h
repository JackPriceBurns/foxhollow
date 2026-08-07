#ifndef MAIN_DLL_DLL_007C_MODGFX_H_
#define MAIN_DLL_DLL_007C_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll7CSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll7CInterface {
    ResourceDescriptorCallback reserved;
    Dll7CSpawnFn spawn;
} Dll7CInterface;

STATIC_ASSERT(offsetof(Dll7CInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll7CInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll7CInterface) == 0x08);

typedef struct Dll7CResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll7CSpawnFn spawn;
} Dll7CResourceDescriptor;

STATIC_ASSERT(offsetof(Dll7CResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll7CResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll7CResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll7CResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll7CResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll7CResourceDescriptor) == 0x20);

extern Dll7CResourceDescriptor gDll7CResourceDescriptor;

void dll_7C_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_7C_release(void);
void dll_7C_initialise(void);

#endif /* MAIN_DLL_DLL_007C_MODGFX_H_ */
