#ifndef MAIN_DLL_DLL_00AA_MODGFX_H_
#define MAIN_DLL_DLL_00AA_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef void (*DllAASpawnFn)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 sequenceFlags);

typedef struct DllAAResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    DllAASpawnFn spawn;
    u32 opaqueTail;
} DllAAResourceDescriptor;

STATIC_ASSERT(offsetof(DllAAResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(DllAAResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(DllAAResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(DllAAResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(DllAAResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(offsetof(DllAAResourceDescriptor, opaqueTail) == 0x20);
STATIC_ASSERT(sizeof(DllAAResourceDescriptor) == 0x24);

extern DllAAResourceDescriptor gDllAAResourceDescriptor;

void dll_AA_spawnSequence(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 sequenceFlags);
void dll_AA_release(void);
void dll_AA_initialise(void);

#endif /* MAIN_DLL_DLL_00AA_MODGFX_H_ */
