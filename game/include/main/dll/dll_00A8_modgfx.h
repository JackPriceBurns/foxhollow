#ifndef MAIN_DLL_DLL_00A8_MODGFX_H_
#define MAIN_DLL_DLL_00A8_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*DllA8SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags, int unused,
                             void* alternateStyle);

typedef struct DllA8ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    DllA8SpawnFn spawn;
} DllA8ResourceDescriptor;

STATIC_ASSERT(offsetof(DllA8ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(DllA8ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(DllA8ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(DllA8ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(DllA8ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(DllA8ResourceDescriptor) == 0x20);

extern DllA8ResourceDescriptor gDllA8ResourceDescriptor;

void dll_A8_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags, int unused,
                        void* alternateStyle);
void dll_A8_release(void);
void dll_A8_initialise(void);

#endif /* MAIN_DLL_DLL_00A8_MODGFX_H_ */
