#ifndef MAIN_DLL_DLL_0098_MODGFX_H_
#define MAIN_DLL_DLL_0098_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll98SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                             int unused, int invertY);

typedef struct Dll98ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll98SpawnFn spawn;
    u32 opaqueTail;
} Dll98ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll98ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll98ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll98ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll98ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll98ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll98ResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll98ResourceDescriptor) == 0x24);

extern Dll98ResourceDescriptor gDll98ResourceDescriptor;

void dll_98_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int unused,
                        int invertY);
void dll_98_release(void);
void dll_98_initialise(void);

#endif /* MAIN_DLL_DLL_0098_MODGFX_H_ */
