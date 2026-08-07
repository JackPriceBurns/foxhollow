#ifndef MAIN_DLL_DLL_009F_MODGFX_H_
#define MAIN_DLL_DLL_009F_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*Dll9FSpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);

typedef struct Dll9FResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll9FSpawnFn spawn;
} Dll9FResourceDescriptor;

STATIC_ASSERT(offsetof(Dll9FResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll9FResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll9FResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll9FResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll9FResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll9FResourceDescriptor) == 0x20);

extern Dll9FResourceDescriptor gDll9FResourceDescriptor;

void dll_9F_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags);
void dll_9F_release(void);
void dll_9F_initialise(void);

#endif /* MAIN_DLL_DLL_009F_MODGFX_H_ */
