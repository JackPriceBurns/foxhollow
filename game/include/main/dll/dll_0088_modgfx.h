#ifndef MAIN_DLL_DLL_0088_MODGFX_H_
#define MAIN_DLL_DLL_0088_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll88SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll88ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll88SpawnFn spawn;
} Dll88ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll88ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll88ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll88ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll88ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll88ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll88ResourceDescriptor) == 0x20);

extern Dll88ResourceDescriptor gDll88ResourceDescriptor;

void dll_88_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_88_release(void);
void dll_88_initialise(void);

#endif /* MAIN_DLL_DLL_0088_MODGFX_H_ */
