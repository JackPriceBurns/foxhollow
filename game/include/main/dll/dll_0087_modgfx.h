#ifndef MAIN_DLL_DLL_0087_MODGFX_H_
#define MAIN_DLL_DLL_0087_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll87SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll87ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll87SpawnFn spawn;
} Dll87ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll87ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll87ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll87ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll87ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll87ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll87ResourceDescriptor) == 0x20);

extern Dll87ResourceDescriptor gDll87ResourceDescriptor;

void dll_87_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_87_release(void);
void dll_87_initialise(void);

#endif /* MAIN_DLL_DLL_0087_MODGFX_H_ */
