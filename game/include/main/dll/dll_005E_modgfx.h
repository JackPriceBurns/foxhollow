#ifndef MAIN_DLL_DLL_005E_MODGFX_H_
#define MAIN_DLL_DLL_005E_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll5ESpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll5EInterface {
    ResourceDescriptorCallback reserved;
    Dll5ESpawnFn spawn;
} Dll5EInterface;

STATIC_ASSERT(offsetof(Dll5EInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll5EInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll5EInterface) == 0x08);

typedef struct Dll5EResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll5ESpawnFn spawn;
    u32 padding;
} Dll5EResourceDescriptor;

STATIC_ASSERT(offsetof(Dll5EResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll5EResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll5EResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll5EResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll5EResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll5EResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(Dll5EResourceDescriptor) == 0x24);

extern Dll5EResourceDescriptor gDll5EResourceDescriptor;

void dll_5E_spawnSequence(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_5E_release(void);
void dll_5E_initialise(void);

#endif /* MAIN_DLL_DLL_005E_MODGFX_H_ */
