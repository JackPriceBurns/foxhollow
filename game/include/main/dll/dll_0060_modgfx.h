#ifndef MAIN_DLL_DLL_0060_MODGFX_H_
#define MAIN_DLL_DLL_0060_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll60SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll60Interface {
    void* reserved;
    Dll60SpawnFn spawn;
} Dll60Interface;

STATIC_ASSERT(offsetof(Dll60Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll60Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll60Interface) == 0x08);

typedef struct Dll60ResourceDescriptor {
    ResourceDescriptorHeader header;
    void* reserved18;
    Dll60SpawnFn spawn;
    u32 padding;
} Dll60ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll60ResourceDescriptor, header.metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll60ResourceDescriptor, header.acquire) == 0x10);
STATIC_ASSERT(offsetof(Dll60ResourceDescriptor, header.release) == 0x14);
STATIC_ASSERT(offsetof(Dll60ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll60ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll60ResourceDescriptor, padding) == 0x20);
STATIC_ASSERT(sizeof(Dll60ResourceDescriptor) == 0x24);

extern Dll60ResourceDescriptor gDll60ResourceDescriptor;

void dll_60_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_60_release(void);
void dll_60_initialise(void);

#endif /* MAIN_DLL_DLL_0060_MODGFX_H_ */
