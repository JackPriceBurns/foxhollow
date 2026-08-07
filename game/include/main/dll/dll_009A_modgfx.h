#ifndef MAIN_DLL_DLL_009A_MODGFX_H_
#define MAIN_DLL_DLL_009A_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll9ASpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll9AResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll9ASpawnFn spawn;
} Dll9AResourceDescriptor;

STATIC_ASSERT(offsetof(Dll9AResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll9AResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll9AResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll9AResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll9AResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll9AResourceDescriptor) == 0x20);

extern Dll9AResourceDescriptor gDll9AResourceDescriptor;

void dll_9A_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_9A_release(void);
void dll_9A_initialise(void);

#endif /* MAIN_DLL_DLL_009A_MODGFX_H_ */
