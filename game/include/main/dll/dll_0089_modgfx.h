#ifndef MAIN_DLL_DLL_0089_MODGFX_H_
#define MAIN_DLL_DLL_0089_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll89SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);

typedef struct Dll89ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll89SpawnFn spawn;
} Dll89ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll89ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll89ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll89ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll89ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll89ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll89ResourceDescriptor) == 0x20);

extern Dll89ResourceDescriptor gDll89ResourceDescriptor;

void dll_89_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_89_release(void);
void dll_89_initialise(void);

#endif /* MAIN_DLL_DLL_0089_MODGFX_H_ */
