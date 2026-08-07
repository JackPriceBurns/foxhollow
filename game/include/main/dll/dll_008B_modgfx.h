#ifndef MAIN_DLL_DLL_008B_MODGFX_H_
#define MAIN_DLL_DLL_008B_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll8BSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                             u32 unused, f32* scalePtr);

typedef struct Dll8BResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll8BSpawnFn spawn;
} Dll8BResourceDescriptor;

STATIC_ASSERT(offsetof(Dll8BResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll8BResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll8BResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll8BResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll8BResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll8BResourceDescriptor) == 0x20);

extern Dll8BResourceDescriptor gDll8BResourceDescriptor;

void dll_8B_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, u32 unused,
                        f32* scalePtr);
void dll_8B_release(void);
void dll_8B_initialise(void);

#endif /* MAIN_DLL_DLL_008B_MODGFX_H_ */
