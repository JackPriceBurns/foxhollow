#ifndef MAIN_DLL_DLL_006F_MODGFX_H_
#define MAIN_DLL_DLL_006F_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll6FSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll6FInterface {
    ResourceDescriptorCallback reserved;
    Dll6FSpawnFn spawn;
} Dll6FInterface;

STATIC_ASSERT(offsetof(Dll6FInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll6FInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll6FInterface) == 0x08);

typedef struct Dll6FResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll6FSpawnFn spawn;
} Dll6FResourceDescriptor;

STATIC_ASSERT(offsetof(Dll6FResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll6FResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll6FResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll6FResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll6FResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll6FResourceDescriptor) == 0x20);

extern Dll6FResourceDescriptor gDll6FResourceDescriptor;

void dll_6F_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_6F_release(void);
void dll_6F_initialise(void);

#endif /* MAIN_DLL_DLL_006F_MODGFX_H_ */
