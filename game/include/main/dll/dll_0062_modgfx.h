#ifndef MAIN_DLL_DLL_0062_MODGFX_H_
#define MAIN_DLL_DLL_0062_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll62SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll62Interface {
    ResourceDescriptorCallback reserved;
    Dll62SpawnFn spawn;
} Dll62Interface;

STATIC_ASSERT(offsetof(Dll62Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll62Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll62Interface) == 0x08);

typedef struct Dll62ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll62SpawnFn spawn;
    u32 padding;
} Dll62ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll62ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll62ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll62ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll62ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll62ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll62ResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(Dll62ResourceDescriptor) == 0x24);

extern Dll62ResourceDescriptor gDll62ResourceDescriptor;

void dll_62_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_62_release(void);
void dll_62_initialise(void);

#endif /* MAIN_DLL_DLL_0062_MODGFX_H_ */
