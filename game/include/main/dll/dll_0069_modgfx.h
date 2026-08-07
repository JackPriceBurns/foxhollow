#ifndef MAIN_DLL_DLL_0069_MODGFX_H_
#define MAIN_DLL_DLL_0069_MODGFX_H_

#include "global.h"
#include "game/objects/object_fwd.h"
#include "main/resource.h"

typedef struct Dll69EffectParams {
    int param0;
    int param1;
    int param2;
    int param3;
} Dll69EffectParams;

STATIC_ASSERT(offsetof(Dll69EffectParams, param0) == 0x00);
STATIC_ASSERT(offsetof(Dll69EffectParams, param1) == 0x04);
STATIC_ASSERT(offsetof(Dll69EffectParams, param2) == 0x08);
STATIC_ASSERT(offsetof(Dll69EffectParams, param3) == 0x0C);
STATIC_ASSERT(sizeof(Dll69EffectParams) == 0x10);

typedef s16 (*Dll69SpawnFn)(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags, int unusedArg4,
                            Dll69EffectParams* overrideParams);

typedef struct Dll69Interface {
    ResourceDescriptorCallback reserved;
    Dll69SpawnFn spawn;
} Dll69Interface;

STATIC_ASSERT(offsetof(Dll69Interface, reserved) == 0x00);
STATIC_ASSERT(offsetof(Dll69Interface, spawn) == 0x04);
STATIC_ASSERT(sizeof(Dll69Interface) == 0x08);

typedef struct Dll69ResourceDescriptor {
    u32 metadata[4];
    ResourceDescriptorCallback initialise;
    ResourceDescriptorCallback release;
    ResourceDescriptorCallback reserved18;
    Dll69SpawnFn spawn;
} Dll69ResourceDescriptor;

STATIC_ASSERT(offsetof(Dll69ResourceDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(Dll69ResourceDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(Dll69ResourceDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(Dll69ResourceDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(Dll69ResourceDescriptor, spawn) == 0x1C);
STATIC_ASSERT(sizeof(Dll69ResourceDescriptor) == 0x20);

extern Dll69ResourceDescriptor gDll69ResourceDescriptor;

s16 dll_69_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags, int unusedArg4,
                       Dll69EffectParams* overrideParams);
void dll_69_release(void);
void dll_69_initialise(void);

#endif /* MAIN_DLL_DLL_0069_MODGFX_H_ */
