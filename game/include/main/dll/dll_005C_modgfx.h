#ifndef MAIN_DLL_DLL_005C_MODGFX_H_
#define MAIN_DLL_DLL_005C_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll5CSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll5CInterface {
    ResourceDescriptorCallback reserved;
    Dll5CSpawnFn spawn;
} Dll5CInterface;

STATIC_ASSERT(offsetof(Dll5CInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll5CInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll5CInterface) == 0x08);

typedef struct Dll5CResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll5CSpawnFn spawn;
    u32 padding;
} Dll5CResourceDescriptor;

STATIC_ASSERT(offsetof(Dll5CResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll5CResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll5CResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll5CResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll5CResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll5CResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(Dll5CResourceDescriptor) == 0x24);

extern Dll5CResourceDescriptor gDll5CResourceDescriptor;

void dll_5C_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_5C_release(void);
void dll_5C_initialise(void);

#endif /* MAIN_DLL_DLL_005C_MODGFX_H_ */
