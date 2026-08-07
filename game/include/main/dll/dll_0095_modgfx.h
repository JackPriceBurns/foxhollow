#ifndef MAIN_DLL_DLL_0095_MODGFX_H_
#define MAIN_DLL_DLL_0095_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll95SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams);

typedef struct Dll95ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll95SpawnFn spawn;
} Dll95ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll95ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll95ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll95ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll95ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll95ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll95ResourceDescriptor) == 0x20);

extern Dll95ResourceDescriptor gDll95ResourceDescriptor;

void dll_95_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams);
void dll_95_release(void);
void dll_95_initialise(void);

#endif /* MAIN_DLL_DLL_0095_MODGFX_H_ */
