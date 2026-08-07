#ifndef MAIN_DLL_DLL_0092_MODGFX_H_
#define MAIN_DLL_DLL_0092_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll92SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                             u32 unused, f32* scaleOverride);

typedef struct Dll92ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll92SpawnFn spawn;
    u32 opaqueTail;
} Dll92ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll92ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll92ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll92ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll92ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll92ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll92ResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll92ResourceDescriptor) == 0x24);

extern Dll92ResourceDescriptor gDll92ResourceDescriptor;

void dll_92_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, u32 unused,
                        f32* scaleOverride);
void dll_92_release(void);
void dll_92_initialise(void);

#endif /* MAIN_DLL_DLL_0092_MODGFX_H_ */
