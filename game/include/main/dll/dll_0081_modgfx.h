#ifndef MAIN_DLL_DLL_0081_MODGFX_H_
#define MAIN_DLL_DLL_0081_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef s16 (*Dll81SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                            int owner, int unused);

typedef struct Dll81Interface {
    void* reserved;
    Dll81SpawnFn spawn;
} Dll81Interface;

STATIC_ASSERT(offsetof(Dll81Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll81Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll81Interface) == 0x08);

typedef struct Dll81ResourceDescriptor {
    ResourceDescriptorHeader header;
    void* reserved18;
    Dll81SpawnFn spawn;
} Dll81ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll81ResourceDescriptor, header.metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll81ResourceDescriptor, header.acquire) == 0x10);
STATIC_ASSERT(offsetof(Dll81ResourceDescriptor, header.release) == 0x14);
STATIC_ASSERT(offsetof(Dll81ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll81ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll81ResourceDescriptor) == 0x20);

extern Dll81ResourceDescriptor gDll81ResourceDescriptor;

s16 dll_81_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int owner,
                       int unused);
void dll_81_release(void);
void dll_81_initialise(void);

#endif /* MAIN_DLL_DLL_0081_MODGFX_H_ */
