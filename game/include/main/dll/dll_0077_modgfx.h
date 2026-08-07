#ifndef MAIN_DLL_DLL_0077_MODGFX_H_
#define MAIN_DLL_DLL_0077_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll77SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll77Interface {
    ResourceDescriptorCallback reserved;
    Dll77SpawnFn spawn;
} Dll77Interface;

STATIC_ASSERT(offsetof(Dll77Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll77Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll77Interface) == 0x08);

typedef struct Dll77ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll77SpawnFn spawn;
} Dll77ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll77ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll77ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll77ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll77ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll77ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll77ResourceDescriptor) == 0x20);

extern Dll77ResourceDescriptor gDll77ResourceDescriptor;

void dll_77_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_77_release(void);
void dll_77_initialise(void);

#endif /* MAIN_DLL_DLL_0077_MODGFX_H_ */
