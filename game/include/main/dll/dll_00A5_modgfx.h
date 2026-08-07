#ifndef MAIN_DLL_DLL_00A5_MODGFX_H_
#define MAIN_DLL_DLL_00A5_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*DllA5SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);

typedef struct DllA5ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    DllA5SpawnFn spawn;
} DllA5ResourceDescriptor;

STATIC_ASSERT(offsetof(DllA5ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(DllA5ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(DllA5ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(DllA5ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(DllA5ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(DllA5ResourceDescriptor) == 0x20);

extern DllA5ResourceDescriptor gDllA5ResourceDescriptor;

void dll_A5_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);
void dll_A5_release(void);
void dll_A5_initialise(void);

#endif /* MAIN_DLL_DLL_00A5_MODGFX_H_ */
