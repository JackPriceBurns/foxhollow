#ifndef MAIN_DLL_DLL_0083_MODGFX_H_
#define MAIN_DLL_DLL_0083_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef s16 (*Dll83SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                            int owner, void* unused);

typedef struct Dll83Interface {
    ResourceDescriptorCallback reserved;
    Dll83SpawnFn spawn;
} Dll83Interface;

STATIC_ASSERT(offsetof(Dll83Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll83Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll83Interface) == 0x08);

typedef struct Dll83ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll83SpawnFn spawn;
    u32 opaqueTail;
} Dll83ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll83ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll83ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll83ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll83ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll83ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll83ResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll83ResourceDescriptor) == 0x24);

extern Dll83ResourceDescriptor gDll83ResourceDescriptor;

s16 dll_83_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int owner,
                       void* unused);
void dll_83_release(void);
void dll_83_initialise(void);

#endif /* MAIN_DLL_DLL_0083_MODGFX_H_ */
