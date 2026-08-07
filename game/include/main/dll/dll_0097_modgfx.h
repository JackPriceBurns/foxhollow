#ifndef MAIN_DLL_DLL_0097_MODGFX_H_
#define MAIN_DLL_DLL_0097_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll97SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                             u32 unused, f32* scaleOverride);

typedef struct Dll97ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll97SpawnFn spawn;
    u32 opaqueTail;
} Dll97ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll97ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll97ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll97ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll97ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll97ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll97ResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll97ResourceDescriptor) == 0x24);

extern Dll97ResourceDescriptor gDll97ResourceDescriptor;

void dll_97_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, u32 unused,
                        f32* scaleOverride);
void dll_97_release(void);
void dll_97_initialise(void);

#endif /* MAIN_DLL_DLL_0097_MODGFX_H_ */
