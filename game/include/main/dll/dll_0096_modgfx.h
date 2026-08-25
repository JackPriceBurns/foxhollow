#ifndef MAIN_DLL_DLL_0096_MODGFX_H_
#define MAIN_DLL_DLL_0096_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef s16 (*Dll96SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll96ResourceDescriptor {
    ResourceDescriptorHeader header;
    void* reserved18;
    Dll96SpawnFn spawn;
} Dll96ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll96ResourceDescriptor, header.metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll96ResourceDescriptor, header.acquire) == 0x10);
STATIC_ASSERT(offsetof(Dll96ResourceDescriptor, header.release) == 0x14);
STATIC_ASSERT(offsetof(Dll96ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll96ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll96ResourceDescriptor) == 0x20);

extern Dll96ResourceDescriptor gDll96ResourceDescriptor;

s16 dll_96_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_96_release(void);
void dll_96_initialise(void);

#endif /* MAIN_DLL_DLL_0096_MODGFX_H_ */
