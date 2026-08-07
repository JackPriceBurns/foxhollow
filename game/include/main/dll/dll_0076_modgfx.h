#ifndef MAIN_DLL_DLL_0076_MODGFX_H_
#define MAIN_DLL_DLL_0076_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll76SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll76Interface {
    ResourceDescriptorCallback reserved;
    Dll76SpawnFn spawn;
} Dll76Interface;

STATIC_ASSERT(offsetof(Dll76Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll76Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll76Interface) == 0x08);

typedef struct Dll76ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll76SpawnFn spawn;
} Dll76ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll76ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll76ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll76ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll76ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll76ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll76ResourceDescriptor) == 0x20);

extern Dll76ResourceDescriptor gDll76ResourceDescriptor;

void dll_76_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_76_release(void);
void dll_76_initialise(void);

#endif /* MAIN_DLL_DLL_0076_MODGFX_H_ */
