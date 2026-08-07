#ifndef MAIN_DLL_DLL_0072_MODGFX_H_
#define MAIN_DLL_DLL_0072_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll72SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll72Interface {
    ResourceDescriptorCallback reserved;
    Dll72SpawnFn spawn;
} Dll72Interface;

STATIC_ASSERT(offsetof(Dll72Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll72Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll72Interface) == 0x08);

typedef struct Dll72ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll72SpawnFn spawn;
} Dll72ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll72ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll72ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll72ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll72ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll72ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll72ResourceDescriptor) == 0x20);

extern Dll72ResourceDescriptor gDll72ResourceDescriptor;

void dll_72_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_72_release(void);
void dll_72_initialise(void);

#endif /* MAIN_DLL_DLL_0072_MODGFX_H_ */
