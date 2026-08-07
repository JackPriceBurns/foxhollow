#ifndef MAIN_DLL_DLL_0091_MODGFX_H_
#define MAIN_DLL_DLL_0091_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll91SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll91ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll91SpawnFn spawn;
    u32 opaqueTail;
} Dll91ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll91ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll91ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll91ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll91ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll91ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll91ResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll91ResourceDescriptor) == 0x24);

extern Dll91ResourceDescriptor gDll91ResourceDescriptor;

void dll_91_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_91_release(void);
void dll_91_initialise(void);

#endif /* MAIN_DLL_DLL_0091_MODGFX_H_ */
