#ifndef MAIN_DLL_DLL_00A1_MODGFX_H_
#define MAIN_DLL_DLL_00A1_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*DllA1SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);

typedef struct DllA1ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    DllA1SpawnFn spawn;
} DllA1ResourceDescriptor;

STATIC_ASSERT(offsetof(DllA1ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(DllA1ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(DllA1ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(DllA1ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(DllA1ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(DllA1ResourceDescriptor) == 0x20);

extern DllA1ResourceDescriptor gDllA1ResourceDescriptor;

void dll_A1_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);
void dll_A1_release(void);
void dll_A1_initialise(void);

#endif /* MAIN_DLL_DLL_00A1_MODGFX_H_ */
