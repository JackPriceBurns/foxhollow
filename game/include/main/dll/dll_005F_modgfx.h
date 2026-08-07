#ifndef MAIN_DLL_DLL_005F_MODGFX_H_
#define MAIN_DLL_DLL_005F_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll5FSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll5FInterface {
    ResourceDescriptorCallback reserved;
    Dll5FSpawnFn spawn;
} Dll5FInterface;

STATIC_ASSERT(offsetof(Dll5FInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll5FInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll5FInterface) == 0x08);

typedef struct Dll5FResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll5FSpawnFn spawn;
} Dll5FResourceDescriptor;

STATIC_ASSERT(offsetof(Dll5FResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll5FResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll5FResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll5FResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll5FResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll5FResourceDescriptor) == 0x20);

extern Dll5FResourceDescriptor gDll5FResourceDescriptor;

void dll_5F_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_5F_release(void);
void dll_5F_initialise(void);

#endif /* MAIN_DLL_DLL_005F_MODGFX_H_ */
