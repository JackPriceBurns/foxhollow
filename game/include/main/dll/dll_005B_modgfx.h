#ifndef MAIN_DLL_DLL_005B_MODGFX_H_
#define MAIN_DLL_DLL_005B_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef struct Dll5BSpawnCountRange {
    s16 min;
    s16 max;
} Dll5BSpawnCountRange;

STATIC_ASSERT(offsetof(Dll5BSpawnCountRange, min) == 0x00);
STATIC_ASSERT(offsetof(Dll5BSpawnCountRange, max) == 0x02);
STATIC_ASSERT(sizeof(Dll5BSpawnCountRange) == 0x04);

typedef s16 (*Dll5BSpawnFn)(GameObject* sourceObj, int effectId, PartFxSpawnParams* unusedSpawnParams, u32 spawnFlags,
                            int unusedModelId, const Dll5BSpawnCountRange* countRange);

typedef struct Dll5BInterface {
    ResourceDescriptorCallback reserved;
    Dll5BSpawnFn spawn;
} Dll5BInterface;

STATIC_ASSERT(offsetof(Dll5BInterface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll5BInterface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll5BInterface) == 0x08);

typedef struct Dll5BResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll5BSpawnFn spawn;
    char missingOwnerMessage[0x28];
} Dll5BResourceDescriptor;

STATIC_ASSERT(offsetof(Dll5BResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll5BResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll5BResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll5BResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll5BResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(Dll5BResourceDescriptor, missingOwnerMessage) == 0x20);
STATIC_ASSERT(sizeof(Dll5BResourceDescriptor) == 0x48);

extern Dll5BResourceDescriptor gDll5BResourceDescriptor;

s16 dll_5B_spawnModelEffects(GameObject* sourceObj, int effectId, PartFxSpawnParams* unusedSpawnParams, u32 spawnFlags,
                             int unusedModelId, const Dll5BSpawnCountRange* countRange);

#endif /* MAIN_DLL_DLL_005B_MODGFX_H_ */
