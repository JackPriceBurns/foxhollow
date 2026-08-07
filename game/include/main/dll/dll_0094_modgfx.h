#ifndef MAIN_DLL_DLL_0094_MODGFX_H_
#define MAIN_DLL_DLL_0094_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll94SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                             int unused, f32* scaleOverride);

typedef struct Dll94ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll94SpawnFn spawn;
    u32 opaqueTail;
} Dll94ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll94ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll94ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll94ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll94ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll94ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll94ResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll94ResourceDescriptor) == 0x24);

extern Dll94ResourceDescriptor gDll94ResourceDescriptor;

void dll_94_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int unused,
                        f32* scaleOverride);
void dll_94_release(void);
void dll_94_initialise(void);

#endif /* MAIN_DLL_DLL_0094_MODGFX_H_ */
