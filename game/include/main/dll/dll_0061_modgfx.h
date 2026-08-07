#ifndef MAIN_DLL_DLL_0061_MODGFX_H_
#define MAIN_DLL_DLL_0061_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll61SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll61Interface {
    ResourceDescriptorCallback reserved;
    Dll61SpawnFn spawn;
} Dll61Interface;

STATIC_ASSERT(offsetof(Dll61Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll61Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll61Interface) == 0x08);

typedef struct Dll61ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll61SpawnFn spawn;
} Dll61ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll61ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll61ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll61ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll61ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll61ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll61ResourceDescriptor) == 0x20);

extern Dll61ResourceDescriptor gDll61ResourceDescriptor;

void dll_61_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_61_release(void);
void dll_61_initialise(void);

#endif /* MAIN_DLL_DLL_0061_MODGFX_H_ */
