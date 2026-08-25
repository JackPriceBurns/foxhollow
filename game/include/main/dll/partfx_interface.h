#ifndef MAIN_DLL_PARTFX_INTERFACE_H_
#define MAIN_DLL_PARTFX_INTERFACE_H_

#include "main/resource.h"
#include "main/vec_types.h"

struct GameObject;
typedef struct PartFxSpawnParams PartFxSpawnParams;

typedef enum PartfxFlags {
    PARTFXFLAG_NONE = 0x0,
    PARTFXFLAG_1 = 0x1,
    PARTFXFLAG_2 = 0x2,
    PARTFXFLAG_4 = 0x4,
    PARTFXFLAG_10 = 0x10,
    PARTFXFLAG_800 = 0x800,
    PARTFXFLAG_10000 = 0x10000,
    PARTFXFLAG_200000 = 0x200000
} PartfxFlags;

typedef int (*EffectSpawnObjectFn)(struct GameObject* obj, int effectId, PartFxSpawnParams* params, u32 mode,
                                   u8 modelId, void* extraArg);
typedef void (*EffectOnMapSetupFn)(void);
typedef void (*EffectUpdateFrameStateFn)(int reset);
typedef void (*EffectFreeObjectFn)(void *obj);

/*
 * PartFxSpawnParams - the s16*-typed spawn-parameter packet passed to the
 * per-effect Effect*_func04 handlers (modgfx/dim_partfx/df_partfx and the
 * gameplay.c spawners). Offset/width layout observed consistent across all
 * handlers (s16 head + f32 block). The float block is consistently passed as
 * scale plus a local position/vector triple, though individual effect ids may
 * reinterpret one of those floats as an effect-specific magnitude.
 */
typedef struct PartFxSpawnParams {
    union {
        struct {
            s16 unk0;
            s16 unk2;
            s16 unk4;
            s16 effectParam;
        };
        struct {
            s16 rotX;
            s16 rotY;
            s16 rotZ;
            s16 pad06;
        };
        struct {
            s16 arg0;
            s16 arg1;
            s16 arg2;
            s16 arg3;
        };
    };
    f32 scale;
    union {
        struct {
            f32 posX;
            f32 posY;
            f32 posZ;
        };
        Vec3f pos;
    };
} PartFxSpawnParams;

STATIC_ASSERT(sizeof(PartFxSpawnParams) == 0x18);
STATIC_ASSERT(offsetof(PartFxSpawnParams, scale) == 0x08);
STATIC_ASSERT(offsetof(PartFxSpawnParams, posX) == 0x0C);
STATIC_ASSERT(offsetof(PartFxSpawnParams, posY) == 0x10);
STATIC_ASSERT(offsetof(PartFxSpawnParams, posZ) == 0x14);

typedef struct EffectInterface {
    void* reserved00;
    EffectOnMapSetupFn onMapSetup;
    EffectSpawnObjectFn spawnObject;
    EffectUpdateFrameStateFn updateFrameState;
} EffectInterface;

typedef struct EffectResourceDescriptor {
    ResourceDescriptorHeader header;
    EffectInterface interface;
} EffectResourceDescriptor;

#define EFFECT_RESOURCE_ADAPTERS(prefix, initialiseCallback, spawnCallback, updateCallback)                     \
    RESOURCE_ACQUIRE_ADAPTER(prefix##Acquire, initialiseCallback)                                               \
    static int prefix##Spawn(struct GameObject* obj, int effectId, PartFxSpawnParams* params, u32 mode,          \
                             u8 modelId, void* extraArg) {                                                       \
        return spawnCallback(obj, effectId, params, mode, modelId, extraArg);                                   \
    }                                                                                                           \
    static void prefix##Update(int reset) { updateCallback(); }

#define EFFECT_RESOURCE_ADAPTERS_NO_EXTRA(prefix, initialiseCallback, spawnCallback, updateCallback)            \
    RESOURCE_ACQUIRE_ADAPTER(prefix##Acquire, initialiseCallback)                                               \
    static int prefix##Spawn(struct GameObject* obj, int effectId, PartFxSpawnParams* params, u32 mode,          \
                             u8 modelId, void* extraArg) {                                                       \
        return spawnCallback(obj, effectId, params, mode, modelId);                                             \
    }                                                                                                           \
    static void prefix##Update(int reset) { updateCallback(); }

STATIC_ASSERT(offsetof(EffectInterface, onMapSetup) == 0x04);
STATIC_ASSERT(offsetof(EffectInterface, spawnObject) == 0x08);
STATIC_ASSERT(offsetof(EffectInterface, updateFrameState) == 0x0C);
extern EffectInterface **gPartfxInterface;

#endif /* MAIN_DLL_PARTFX_INTERFACE_H_ */
