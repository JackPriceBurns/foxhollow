#ifndef MAIN_DLL_DLL_0085_MODGFX_H_
#define MAIN_DLL_DLL_0085_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll85SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll85ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll85SpawnFn spawn;
} Dll85ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll85ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll85ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll85ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll85ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll85ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll85ResourceDescriptor) == 0x20);

extern Dll85ResourceDescriptor gDll85ResourceDescriptor;

void dll_85_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_85_release(void);
void dll_85_initialise(void);

#endif /* MAIN_DLL_DLL_0085_MODGFX_H_ */
