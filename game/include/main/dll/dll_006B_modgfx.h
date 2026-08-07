#ifndef MAIN_DLL_DLL_006B_MODGFX_H_
#define MAIN_DLL_DLL_006B_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll6BSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll6BInterface {
    ResourceDescriptorCallback reserved;
    Dll6BSpawnFn spawn;
} Dll6BInterface;

STATIC_ASSERT(offsetof(Dll6BInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll6BInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll6BInterface) == 0x08);

typedef struct Dll6BResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll6BSpawnFn spawn;
} Dll6BResourceDescriptor;

STATIC_ASSERT(offsetof(Dll6BResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll6BResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll6BResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll6BResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll6BResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll6BResourceDescriptor) == 0x20);

extern Dll6BResourceDescriptor gDll6BResourceDescriptor;

void dll_6B_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_6B_release(void);
void dll_6B_initialise(void);

#endif /* MAIN_DLL_DLL_006B_MODGFX_H_ */
