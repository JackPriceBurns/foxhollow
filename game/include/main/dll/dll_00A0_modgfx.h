#ifndef MAIN_DLL_DLL_00A0_MODGFX_H_
#define MAIN_DLL_DLL_00A0_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*DllA0SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);

typedef struct DllA0ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    DllA0SpawnFn spawn;
} DllA0ResourceDescriptor;

STATIC_ASSERT(offsetof(DllA0ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(DllA0ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(DllA0ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(DllA0ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(DllA0ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(DllA0ResourceDescriptor) == 0x20);

extern DllA0ResourceDescriptor gDllA0ResourceDescriptor;

void dll_A0_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);
void dll_A0_release(void);
void dll_A0_initialise(void);

#endif /* MAIN_DLL_DLL_00A0_MODGFX_H_ */
