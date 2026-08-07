#ifndef MAIN_DLL_DLL_0078_MODGFX_H_
#define MAIN_DLL_DLL_0078_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll78SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll78Interface {
    ResourceDescriptorCallback reserved;
    Dll78SpawnFn spawn;
} Dll78Interface;

STATIC_ASSERT(offsetof(Dll78Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll78Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll78Interface) == 0x08);

typedef struct Dll78ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll78SpawnFn spawn;
} Dll78ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll78ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll78ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll78ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll78ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll78ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll78ResourceDescriptor) == 0x20);

extern Dll78ResourceDescriptor gDll78ResourceDescriptor;

void dll_78_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_78_release(void);
void dll_78_initialise(void);

#endif /* MAIN_DLL_DLL_0078_MODGFX_H_ */
