#ifndef MAIN_DLL_DLL_0059_DLL59FUNC0_H_
#define MAIN_DLL_DLL_0059_DLL59FUNC0_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/resource.h"

typedef struct Dll59ResourceDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*spawnEffect)(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
} Dll59ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll59ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll59ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll59ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll59ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll59ResourceDescriptor, spawnEffect) == 0x1C);
STATIC_ASSERT(sizeof(Dll59ResourceDescriptor) == 0x20);

extern u8 gDll59EffectResourceData[0x130];
extern Dll59ResourceDescriptor gDll59ResourceDescriptor;

void dll_59_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags);
void dll_59_release(void);
void dll_59_initialise(void);

#endif /* MAIN_DLL_DLL_0059_DLL59FUNC0_H_ */
