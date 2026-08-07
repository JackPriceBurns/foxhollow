#ifndef MAIN_DLL_DLL_007F_MODGFX_H_
#define MAIN_DLL_DLL_007F_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll7FSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll7FInterface {
    ResourceDescriptorCallback reserved;
    Dll7FSpawnFn spawn;
} Dll7FInterface;

STATIC_ASSERT(offsetof(Dll7FInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll7FInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll7FInterface) == 0x08);

typedef struct Dll7FResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll7FSpawnFn spawn;
    u32 opaqueTail;
} Dll7FResourceDescriptor;

STATIC_ASSERT(offsetof(Dll7FResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll7FResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll7FResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll7FResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll7FResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll7FResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(Dll7FResourceDescriptor) == 0x24);

extern Dll7FResourceDescriptor gDll7FResourceDescriptor;

void dll_7F_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_7F_release(void);
void dll_7F_initialise(void);

#endif /* MAIN_DLL_DLL_007F_MODGFX_H_ */
