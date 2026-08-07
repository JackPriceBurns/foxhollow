#ifndef MAIN_DLL_DLL_009E_MODGFX_H_
#define MAIN_DLL_DLL_009E_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll9ESpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);

typedef struct Dll9EResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll9ESpawnFn spawn;
} Dll9EResourceDescriptor;

STATIC_ASSERT(offsetof(Dll9EResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll9EResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll9EResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll9EResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll9EResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll9EResourceDescriptor) == 0x20);

extern Dll9EResourceDescriptor gDll9EResourceDescriptor;

void dll_9E_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);
void dll_9E_release(void);
void dll_9E_initialise(void);

#endif /* MAIN_DLL_DLL_009E_MODGFX_H_ */
