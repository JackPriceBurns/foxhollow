#ifndef MAIN_DLL_DLL_0080_MODGFX_H_
#define MAIN_DLL_DLL_0080_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll80SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll80Interface {
    ResourceDescriptorCallback reserved;
    Dll80SpawnFn spawn;
} Dll80Interface;

STATIC_ASSERT(offsetof(Dll80Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll80Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll80Interface) == 0x08);

typedef struct Dll80ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll80SpawnFn spawn;
} Dll80ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll80ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll80ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll80ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll80ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll80ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll80ResourceDescriptor) == 0x20);

extern Dll80ResourceDescriptor gDll80ResourceDescriptor;

void dll_80_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_80_release(void);
void dll_80_initialise(void);

#endif /* MAIN_DLL_DLL_0080_MODGFX_H_ */
