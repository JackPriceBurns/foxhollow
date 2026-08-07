#ifndef MAIN_DLL_DLL_0075_MODGFX_H_
#define MAIN_DLL_DLL_0075_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef s16 (*Dll75SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                            int unusedArg4, void* unusedArg5);

typedef struct Dll75Interface {
    ResourceDescriptorCallback reserved;
    Dll75SpawnFn spawn;
} Dll75Interface;

STATIC_ASSERT(offsetof(Dll75Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll75Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll75Interface) == 0x08);

typedef struct Dll75ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll75SpawnFn spawn;
} Dll75ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll75ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll75ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll75ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll75ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll75ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll75ResourceDescriptor) == 0x20);

extern Dll75ResourceDescriptor gDll75ResourceDescriptor;

s16 dll_75_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                       int unusedArg4, void* unusedArg5);
void dll_75_release(void);
void dll_75_initialise(void);

#endif /* MAIN_DLL_DLL_0075_MODGFX_H_ */
