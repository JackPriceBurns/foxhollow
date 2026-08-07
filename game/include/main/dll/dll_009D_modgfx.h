#ifndef MAIN_DLL_DLL_009D_MODGFX_H_
#define MAIN_DLL_DLL_009D_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll9DSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll9DResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll9DSpawnFn spawn;
} Dll9DResourceDescriptor;

STATIC_ASSERT(offsetof(Dll9DResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll9DResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll9DResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll9DResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll9DResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll9DResourceDescriptor) == 0x20);

extern Dll9DResourceDescriptor gDll9DResourceDescriptor;

void dll_9D_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_9D_release(void);
void dll_9D_initialise(void);

#endif /* MAIN_DLL_DLL_009D_MODGFX_H_ */
