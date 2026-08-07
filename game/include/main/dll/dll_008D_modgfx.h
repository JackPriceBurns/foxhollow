#ifndef MAIN_DLL_DLL_008D_MODGFX_H_
#define MAIN_DLL_DLL_008D_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef s16 (*Dll8DSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll8DResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll8DSpawnFn spawn;
} Dll8DResourceDescriptor;

STATIC_ASSERT(offsetof(Dll8DResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll8DResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll8DResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll8DResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll8DResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll8DResourceDescriptor) == 0x20);

extern Dll8DResourceDescriptor gDll8DResourceDescriptor;

s16 dll_8D_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_8D_release(void);
void dll_8D_initialise(void);

#endif /* MAIN_DLL_DLL_008D_MODGFX_H_ */
