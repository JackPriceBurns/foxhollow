#ifndef MAIN_DLL_DLL_008C_MODGFX_H_
#define MAIN_DLL_DLL_008C_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll8CSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll8CResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll8CSpawnFn spawn;
    u32 opaqueTail;
} Dll8CResourceDescriptor;

STATIC_ASSERT(offsetof(Dll8CResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll8CResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll8CResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll8CResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll8CResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll8CResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll8CResourceDescriptor) == 0x24);

extern Dll8CResourceDescriptor gDll8CResourceDescriptor;

void dll_8C_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_8C_release(void);
void dll_8C_initialise(void);

#endif /* MAIN_DLL_DLL_008C_MODGFX_H_ */
