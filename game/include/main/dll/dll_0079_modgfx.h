#ifndef MAIN_DLL_DLL_0079_MODGFX_H_
#define MAIN_DLL_DLL_0079_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef s16 (*Dll79SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll79Interface {
    ResourceDescriptorCallback reserved;
    Dll79SpawnFn spawn;
} Dll79Interface;

STATIC_ASSERT(offsetof(Dll79Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll79Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll79Interface) == 0x08);

typedef struct Dll79ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll79SpawnFn spawn;
} Dll79ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll79ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll79ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll79ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll79ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll79ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll79ResourceDescriptor) == 0x20);

extern Dll79ResourceDescriptor gDll79ResourceDescriptor;

s16 dll_79_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_79_release(void);
void dll_79_initialise(void);

#endif /* MAIN_DLL_DLL_0079_MODGFX_H_ */
