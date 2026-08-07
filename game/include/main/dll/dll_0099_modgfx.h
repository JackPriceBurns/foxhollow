#ifndef MAIN_DLL_DLL_0099_MODGFX_H_
#define MAIN_DLL_DLL_0099_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll99SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                             int unused, f32* scaleOverride);

typedef struct Dll99ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll99SpawnFn spawn;
    u32 opaqueTail;
} Dll99ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll99ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll99ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll99ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll99ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll99ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll99ResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll99ResourceDescriptor) == 0x24);

extern Dll99ResourceDescriptor gDll99ResourceDescriptor;

void dll_99_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int unused,
                        f32* scaleOverride);
void dll_99_release(void);
void dll_99_initialise(void);

#endif /* MAIN_DLL_DLL_0099_MODGFX_H_ */
