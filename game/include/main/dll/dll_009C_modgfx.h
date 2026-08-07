#ifndef MAIN_DLL_DLL_009C_MODGFX_H_
#define MAIN_DLL_DLL_009C_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll9CSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll9CResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll9CSpawnFn spawn;
    u32 opaqueTail;
} Dll9CResourceDescriptor;

STATIC_ASSERT(offsetof(Dll9CResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll9CResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll9CResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll9CResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll9CResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll9CResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll9CResourceDescriptor) == 0x24);

extern Dll9CResourceDescriptor gDll9CResourceDescriptor;

void dll_9C_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_9C_release(void);
void dll_9C_initialise(void);

#endif /* MAIN_DLL_DLL_009C_MODGFX_H_ */
