#ifndef MAIN_DLL_DLL_00A7_MODGFX_H_
#define MAIN_DLL_DLL_00A7_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef struct DllA7CommandParams {
    u32 valueX;
    u32 valueY;
    u32 valueZ;
    s32 flags;
} DllA7CommandParams;

STATIC_ASSERT(offsetof(DllA7CommandParams, valueX) == 0x00);
STATIC_ASSERT(offsetof(DllA7CommandParams, valueY) == 0x04);
STATIC_ASSERT(offsetof(DllA7CommandParams, valueZ) == 0x08);
STATIC_ASSERT(offsetof(DllA7CommandParams, flags) == 0x0C);
STATIC_ASSERT(sizeof(DllA7CommandParams) == 0x10);

typedef void (*DllA7SpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags, int unused,
                             DllA7CommandParams* commandParams);

typedef struct DllA7ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    DllA7SpawnFn spawn;
} DllA7ResourceDescriptor;

STATIC_ASSERT(offsetof(DllA7ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(DllA7ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(DllA7ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(DllA7ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(DllA7ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(DllA7ResourceDescriptor) == 0x20);

extern DllA7ResourceDescriptor gDllA7ResourceDescriptor;

void dll_A7_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags, int unused,
                        DllA7CommandParams* commandParams);
void dll_A7_release(void);
void dll_A7_initialise(void);

#endif /* MAIN_DLL_DLL_00A7_MODGFX_H_ */
