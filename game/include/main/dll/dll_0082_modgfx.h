#ifndef MAIN_DLL_DLL_0082_MODGFX_H_
#define MAIN_DLL_DLL_0082_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll82SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                             int modelId, void* extraArg);

typedef struct Dll82Interface {
    ResourceDescriptorCallback reserved;
    Dll82SpawnFn spawn;
} Dll82Interface;

STATIC_ASSERT(offsetof(Dll82Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll82Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll82Interface) == 0x08);

typedef struct Dll82ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll82SpawnFn spawn;
} Dll82ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll82ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll82ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll82ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll82ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll82ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll82ResourceDescriptor) == 0x20);

extern Dll82ResourceDescriptor gDll82ResourceDescriptor;

void dll_82_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int modelId,
                        void* extraArg);
void dll_82_release(void);
void dll_82_initialise(void);

#endif /* MAIN_DLL_DLL_0082_MODGFX_H_ */
