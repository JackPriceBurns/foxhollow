#ifndef MAIN_DLL_DLL_00A9_MODGFX_H_
#define MAIN_DLL_DLL_00A9_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*DllA9SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags, int unused,
                             void* alternateStyle);

typedef struct DllA9ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    DllA9SpawnFn spawn;
} DllA9ResourceDescriptor;

STATIC_ASSERT(offsetof(DllA9ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(DllA9ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(DllA9ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(DllA9ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(DllA9ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(DllA9ResourceDescriptor) == 0x20);

extern DllA9ResourceDescriptor gDllA9ResourceDescriptor;

void dll_A9_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags, int unused,
                        void* alternateStyle);
void dll_A9_release(void);
void dll_A9_initialise(void);

#endif /* MAIN_DLL_DLL_00A9_MODGFX_H_ */
