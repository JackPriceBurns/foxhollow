#ifndef MAIN_DLL_EFFECTSPAWNCONFIG_STRUCT_H_
#define MAIN_DLL_EFFECTSPAWNCONFIG_STRUCT_H_

#include "global.h"

typedef struct EffectSpawnConfig {
    void* attachedSource;
    s32 quadVertex3Pad06;
    s32 lifetimeFrames;
    s16 sourceVecX;
    s16 sourceVecY;
    s16 sourceVecZ;
    s16 sourceVecPad;
    f32 sourceScale;
    f32 sourcePosX;
    f32 sourcePosY;
    f32 sourcePosZ;
    f32 velocityX;
    f32 velocityY;
    f32 velocityZ;
    f32 startPosX;
    f32 startPosY;
    f32 startPosZ;
    f32 scale;
    s16 textureSetupFlags;
    s16 textureId;
    u32 behaviorFlags;
    u32 renderFlags;
    u32 overrideColor0;
    u32 overrideColor1;
    u32 overrideColor2;
    u16 colorWord0;
    u16 colorWord1;
    u16 colorWord2;
    u8 effectIdByte;
    u8 pad5F;
    u8 initialAlpha;
    u8 linkGroup;
    u8 modelIdByte;
} EffectSpawnConfig;

STATIC_ASSERT(sizeof(EffectSpawnConfig) == 0x64);
STATIC_ASSERT(offsetof(EffectSpawnConfig, attachedSource) == 0x00);
STATIC_ASSERT(offsetof(EffectSpawnConfig, lifetimeFrames) == 0x08);
STATIC_ASSERT(offsetof(EffectSpawnConfig, sourceVecX) == 0x0C);
STATIC_ASSERT(offsetof(EffectSpawnConfig, sourceScale) == 0x14);
STATIC_ASSERT(offsetof(EffectSpawnConfig, sourcePosX) == 0x18);
STATIC_ASSERT(offsetof(EffectSpawnConfig, velocityX) == 0x24);
STATIC_ASSERT(offsetof(EffectSpawnConfig, startPosX) == 0x30);
STATIC_ASSERT(offsetof(EffectSpawnConfig, textureId) == 0x42);
STATIC_ASSERT(offsetof(EffectSpawnConfig, behaviorFlags) == 0x44);
STATIC_ASSERT(offsetof(EffectSpawnConfig, colorWord0) == 0x58);
STATIC_ASSERT(offsetof(EffectSpawnConfig, effectIdByte) == 0x5E);
STATIC_ASSERT(offsetof(EffectSpawnConfig, initialAlpha) == 0x60);
STATIC_ASSERT(offsetof(EffectSpawnConfig, linkGroup) == 0x61);
STATIC_ASSERT(offsetof(EffectSpawnConfig, modelIdByte) == 0x62);

#endif /* MAIN_DLL_EFFECTSPAWNCONFIG_STRUCT_H_ */
