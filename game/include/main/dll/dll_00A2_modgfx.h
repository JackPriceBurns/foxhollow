#ifndef MAIN_DLL_DLL_00A2_MODGFX_H_
#define MAIN_DLL_DLL_00A2_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*DllA2SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);

typedef struct DllA2ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    DllA2SpawnFn spawn;
} DllA2ResourceDescriptor;

STATIC_ASSERT(offsetof(DllA2ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(DllA2ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(DllA2ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(DllA2ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(DllA2ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(DllA2ResourceDescriptor) == 0x20);

extern DllA2ResourceDescriptor gDllA2ResourceDescriptor;

void dll_A2_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);
void dll_A2_release(void);
void dll_A2_initialise(void);

#endif /* MAIN_DLL_DLL_00A2_MODGFX_H_ */
