#ifndef MAIN_DLL_DLL_0093_MODGFX_H_
#define MAIN_DLL_DLL_0093_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll93SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll93ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll93SpawnFn spawn;
} Dll93ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll93ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll93ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll93ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll93ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll93ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll93ResourceDescriptor) == 0x20);

extern Dll93ResourceDescriptor gDll93ResourceDescriptor;

void dll_93_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_93_release(void);
void dll_93_initialise(void);

#endif /* MAIN_DLL_DLL_0093_MODGFX_H_ */
