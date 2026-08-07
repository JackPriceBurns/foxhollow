#ifndef MAIN_DLL_DLL_0086_MODGFX_H_
#define MAIN_DLL_DLL_0086_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll86SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll86ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll86SpawnFn spawn;
} Dll86ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll86ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll86ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll86ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll86ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll86ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll86ResourceDescriptor) == 0x20);

extern Dll86ResourceDescriptor gDll86ResourceDescriptor;

void dll_86_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_86_release(void);
void dll_86_initialise(void);

#endif /* MAIN_DLL_DLL_0086_MODGFX_H_ */
