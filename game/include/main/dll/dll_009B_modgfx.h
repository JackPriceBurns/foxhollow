#ifndef MAIN_DLL_DLL_009B_MODGFX_H_
#define MAIN_DLL_DLL_009B_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll9BSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll9BResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll9BSpawnFn spawn;
} Dll9BResourceDescriptor;

STATIC_ASSERT(offsetof(Dll9BResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll9BResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll9BResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll9BResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll9BResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll9BResourceDescriptor) == 0x20);

extern Dll9BResourceDescriptor gDll9BResourceDescriptor;

void dll_9B_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_9B_release(void);
void dll_9B_initialise(void);

#endif /* MAIN_DLL_DLL_009B_MODGFX_H_ */
