#ifndef MAIN_DLL_MODGFX_INTERFACE_H_
#define MAIN_DLL_MODGFX_INTERFACE_H_

#include "global.h"

typedef struct ModgfxSpawnContext ModgfxSpawnContext;

typedef void (*ModgfxDetachSourceFn)(void* sourceObject);
typedef void (*ModgfxOnMapSetupFn)(void);
typedef void (*ModgfxUpdateActiveEffectsFn)(void);
typedef void (*ModgfxReleaseAllFn)(void);
typedef void (*ModgfxFreeSourceEffectsFn)(void* sourceObject);
typedef int (*ModgfxRenderEffectsFn)(void* drawContext, int arg1, int arg2, u8 sourceOnly, void* sourceObject);
typedef void (*ModgfxMarkSourceFrameUpdatedFn)(void);
typedef s16 (*ModgfxSpawnEffectFn)(ModgfxSpawnContext* spawnContext, int flags, int vertexCount, const void* vertices,
                                   int colorCount, const void* colors, int textureAssetId, void* textureResource);
typedef void (*ModgfxReleaseHandleFn)(s16* handle);
typedef void (*ModgfxBeginSequenceFn)(void* sourceObject, u8 sourceMode, u8 effectType, int word40, int word3C);
typedef void (*ModgfxResetSequenceSpawnsFn)(void);
typedef void (*ModgfxAddSequenceSpawnFn)(int modelOrResource, f32 posX, f32 posY, f32 posZ, s16 param14, void* param10);
typedef void (*ModgfxNextSequenceParamFn)(void);
typedef void (*ModgfxSetSequenceParamIndexFn)(s16 index);
typedef void (*ModgfxSetSequenceParamValueFn)(s16 value);
typedef void (*ModgfxSetSequenceParamsFn)(void* params);
typedef void (*ModgfxSpawnSequenceFn)(void* sourceObject, const void* vertices, int vertexCount, const void* colors,
                                      int colorCount, int textureAssetId, void* textureResource);
typedef void (*ModgfxAddSequenceFlagsFn)(u32 flags);
typedef s16 (*ModgfxGetLastSpawnHandleFn)(void);

typedef struct ModgfxInterface {
    void* reserved00;
    ModgfxOnMapSetupFn onMapSetup;
    ModgfxSpawnEffectFn spawnEffect;
    ModgfxUpdateActiveEffectsFn updateActiveEffects;
    ModgfxReleaseAllFn releaseAll;
    ModgfxFreeSourceEffectsFn freeSourceEffects;
    ModgfxDetachSourceFn detachSource;
    ModgfxRenderEffectsFn renderEffects;
    ModgfxReleaseHandleFn releaseHandle;
    void* reserved24;
    void (*setSourceValue)(void* source, char value);
    void (*clearSource)(void* source);
    ModgfxMarkSourceFrameUpdatedFn markSourceFrameUpdated;
    ModgfxBeginSequenceFn beginSequence;
    ModgfxResetSequenceSpawnsFn resetSequenceSpawns;
    ModgfxAddSequenceSpawnFn addSequenceSpawn;
    ModgfxNextSequenceParamFn nextSequenceParam;
    ModgfxSetSequenceParamIndexFn setSequenceParamIndex;
    ModgfxSetSequenceParamValueFn setSequenceParamValue;
    ModgfxSetSequenceParamsFn setSequenceParams;
    ModgfxSpawnSequenceFn spawnSequence;
    ModgfxAddSequenceFlagsFn addSequenceFlags;
    ModgfxGetLastSpawnHandleFn getLastSpawnHandle;
} ModgfxInterface;

STATIC_ASSERT(offsetof(ModgfxInterface, spawnEffect) == 0x08);
STATIC_ASSERT(offsetof(ModgfxInterface, updateActiveEffects) == 0x0C);
STATIC_ASSERT(offsetof(ModgfxInterface, releaseAll) == 0x10);
STATIC_ASSERT(offsetof(ModgfxInterface, freeSourceEffects) == 0x14);
STATIC_ASSERT(offsetof(ModgfxInterface, detachSource) == 0x18);
STATIC_ASSERT(offsetof(ModgfxInterface, renderEffects) == 0x1C);
STATIC_ASSERT(offsetof(ModgfxInterface, releaseHandle) == 0x20);
STATIC_ASSERT(offsetof(ModgfxInterface, markSourceFrameUpdated) == 0x30);
STATIC_ASSERT(offsetof(ModgfxInterface, beginSequence) == 0x34);
STATIC_ASSERT(offsetof(ModgfxInterface, resetSequenceSpawns) == 0x38);
STATIC_ASSERT(offsetof(ModgfxInterface, addSequenceSpawn) == 0x3C);
STATIC_ASSERT(offsetof(ModgfxInterface, nextSequenceParam) == 0x40);
STATIC_ASSERT(offsetof(ModgfxInterface, setSequenceParamIndex) == 0x44);
STATIC_ASSERT(offsetof(ModgfxInterface, setSequenceParamValue) == 0x48);
STATIC_ASSERT(offsetof(ModgfxInterface, setSequenceParams) == 0x4C);
STATIC_ASSERT(offsetof(ModgfxInterface, spawnSequence) == 0x50);
STATIC_ASSERT(offsetof(ModgfxInterface, addSequenceFlags) == 0x54);
STATIC_ASSERT(offsetof(ModgfxInterface, getLastSpawnHandle) == 0x58);

extern ModgfxInterface** gModgfxInterface;

#endif /* MAIN_DLL_MODGFX_INTERFACE_H_ */
