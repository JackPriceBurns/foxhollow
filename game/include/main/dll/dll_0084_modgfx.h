#ifndef MAIN_DLL_DLL_0084_MODGFX_H_
#define MAIN_DLL_DLL_0084_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef s16 (*Dll84SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                            int owner, void* unused);

typedef struct Dll84Interface {
    ResourceDescriptorCallback reserved;
    Dll84SpawnFn spawn;
} Dll84Interface;

STATIC_ASSERT(offsetof(Dll84Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll84Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll84Interface) == 0x08);

typedef struct Dll84ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll84SpawnFn spawn;
    u32 opaqueTail;
} Dll84ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll84ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll84ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll84ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll84ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll84ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll84ResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll84ResourceDescriptor) == 0x24);

extern Dll84ResourceDescriptor gDll84ResourceDescriptor;

s16 dll_84_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int owner,
                       void* unused);
void dll_84_release(void);
void dll_84_initialise(void);

#endif /* MAIN_DLL_DLL_0084_MODGFX_H_ */
