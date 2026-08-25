#ifndef MAIN_DLL_DLL_0070_MODGFX_H_
#define MAIN_DLL_DLL_0070_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll70SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll70Interface {
    void* reserved;
    Dll70SpawnFn spawn;
} Dll70Interface;

STATIC_ASSERT(offsetof(Dll70Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll70Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll70Interface) == 0x08);

typedef struct Dll70ResourceDescriptor {
    ResourceDescriptorHeader header;
    void* reserved18;
    Dll70SpawnFn spawn;
    u32 padding;
} Dll70ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll70ResourceDescriptor, header.metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll70ResourceDescriptor, header.acquire) == 0x10);
STATIC_ASSERT(offsetof(Dll70ResourceDescriptor, header.release) == 0x14);
STATIC_ASSERT(offsetof(Dll70ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll70ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll70ResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(Dll70ResourceDescriptor) == 0x24);

extern Dll70ResourceDescriptor gDll70ResourceDescriptor;

void dll_70_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_70_release(void);
void dll_70_initialise(void);

#endif /* MAIN_DLL_DLL_0070_MODGFX_H_ */
