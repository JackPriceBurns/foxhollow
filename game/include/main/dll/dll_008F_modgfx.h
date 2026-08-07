#ifndef MAIN_DLL_DLL_008F_MODGFX_H_
#define MAIN_DLL_DLL_008F_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll8FSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll8FResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll8FSpawnFn spawn;
    u32 opaqueTail;
} Dll8FResourceDescriptor;

STATIC_ASSERT(offsetof(Dll8FResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll8FResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll8FResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll8FResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll8FResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll8FResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll8FResourceDescriptor) == 0x24);

extern Dll8FResourceDescriptor gDll8FResourceDescriptor;

void dll_8F_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_8F_release(void);
void dll_8F_initialise(void);

#endif /* MAIN_DLL_DLL_008F_MODGFX_H_ */
