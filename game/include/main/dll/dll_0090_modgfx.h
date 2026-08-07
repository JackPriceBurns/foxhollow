#ifndef MAIN_DLL_DLL_0090_MODGFX_H_
#define MAIN_DLL_DLL_0090_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll90SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll90ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll90SpawnFn spawn;
    u32 opaqueTail;
} Dll90ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll90ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll90ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll90ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll90ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll90ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll90ResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll90ResourceDescriptor) == 0x24);

extern Dll90ResourceDescriptor gDll90ResourceDescriptor;

void dll_90_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_90_release(void);
void dll_90_initialise(void);

#endif /* MAIN_DLL_DLL_0090_MODGFX_H_ */
