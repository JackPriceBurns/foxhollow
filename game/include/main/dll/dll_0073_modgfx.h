#ifndef MAIN_DLL_DLL_0073_MODGFX_H_
#define MAIN_DLL_DLL_0073_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll73SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll73Interface {
    ResourceDescriptorCallback reserved;
    Dll73SpawnFn spawn;
} Dll73Interface;

STATIC_ASSERT(offsetof(Dll73Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll73Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll73Interface) == 0x08);

typedef struct Dll73ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll73SpawnFn spawn;
} Dll73ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll73ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll73ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll73ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll73ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll73ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll73ResourceDescriptor) == 0x20);

extern Dll73ResourceDescriptor gDll73ResourceDescriptor;

void dll_73_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_73_release(void);
void dll_73_initialise(void);

#endif /* MAIN_DLL_DLL_0073_MODGFX_H_ */
