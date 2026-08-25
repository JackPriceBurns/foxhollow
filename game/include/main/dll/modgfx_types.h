#ifndef MAIN_DLL_MODGFX_TYPES_H_
#define MAIN_DLL_MODGFX_TYPES_H_

#include "game/objects/object.h"
#include "main/vec_types.h"

typedef struct
{
    u32 mode;
    f32 x, y, z;
    void* tex;
    s16 flags;
    u8 layer;
} GfxCmd;

STATIC_ASSERT(sizeof(GfxCmd) == 0x18);
STATIC_ASSERT(offsetof(GfxCmd, tex) == 0x10);
STATIC_ASSERT(offsetof(GfxCmd, flags) == 0x14);
STATIC_ASSERT(offsetof(GfxCmd, layer) == 0x16);

typedef struct ModgfxSpawnContext
{
    GfxCmd* commands;
    GameObject* attachedSource;
    u8 pad08[0x18];
    f32 velocity[3];
    f32 position[3];
    f32 scale;
    u32 drawGroupStride;
    u32 drawGroupCount;
    s16 sourceMode;
    s16 sequenceParams[7];
    u32 flags;
    u8 modeByte;
    u8 initialStateByte;
    u8 byte5A;
    u8 textureFrameTimer;
    u8 sourceYawIndex;
    s8 commandCount;
    u8 pad5E[2];
} ModgfxSpawnContext;

typedef struct ModgfxSpawnPacket
{
    ModgfxSpawnContext context;
    GfxCmd entries[32];
} ModgfxSpawnPacket;

STATIC_ASSERT(offsetof(ModgfxSpawnContext, commands) == 0x00);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, attachedSource) == 0x04);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, velocity) == 0x20);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, position) == 0x2C);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, scale) == 0x38);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, drawGroupStride) == 0x3C);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, drawGroupCount) == 0x40);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, sourceMode) == 0x44);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, sequenceParams) == 0x46);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, flags) == 0x54);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, modeByte) == 0x58);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, initialStateByte) == 0x59);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, byte5A) == 0x5A);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, textureFrameTimer) == 0x5B);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, sourceYawIndex) == 0x5C);
STATIC_ASSERT(offsetof(ModgfxSpawnContext, commandCount) == 0x5D);
STATIC_ASSERT(sizeof(ModgfxSpawnContext) == 0x60);
STATIC_ASSERT(offsetof(ModgfxSpawnPacket, context) == 0x00);
STATIC_ASSERT(offsetof(ModgfxSpawnPacket, entries) == 0x60);
STATIC_ASSERT(sizeof(ModgfxSpawnPacket) == 0x360);

typedef struct ModgfxEffectVertex
{
    s16 positionX;
    s16 positionY;
    s16 positionZ;
    s16 texCoordS;
    s16 texCoordT;
} ModgfxEffectVertex;

STATIC_ASSERT(offsetof(ModgfxEffectVertex, positionX) == 0x00);
STATIC_ASSERT(offsetof(ModgfxEffectVertex, positionY) == 0x02);
STATIC_ASSERT(offsetof(ModgfxEffectVertex, positionZ) == 0x04);
STATIC_ASSERT(offsetof(ModgfxEffectVertex, texCoordS) == 0x06);
STATIC_ASSERT(offsetof(ModgfxEffectVertex, texCoordT) == 0x08);
STATIC_ASSERT(sizeof(ModgfxEffectVertex) == 0x0A);

typedef struct ModgfxVertexData
{
    s16 posX;
    s16 posY;
    s16 posZ;
    s16 unused06;
    s16 texCoordS;
    s16 texCoordT;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 alpha;
} ModgfxVertexData;

typedef struct ModgfxVertexGroupCmd
{
    u8 unk00[4];
    f32 valueX; /* rgb r / scale x / alpha */
    f32 valueY;
    f32 valueZ;
    s16* indices; /* vertex indices, stride 2 */
    s16 indexCount;
} ModgfxVertexGroupCmd;
STATIC_ASSERT(offsetof(ModgfxVertexGroupCmd, valueX) == 0x04);

typedef struct ModgfxActiveEffect
{
    int instanceHandle;
    int ownerToken;
    u8 pad08[0x98 - 0x08];
    int sharedResourceHandle;
    int releaseTransformSource;
    u8 padA4[0x10C - 0xA4];
    s16 effectType;
    u8 pad10E[0x12C - 0x10E];
    int state;
    u8 pad130[0x13F - 0x130];
    u8 keepSharedResource;
} ModgfxActiveEffect;

#define PARTFX_STAGE_COUNT 7

typedef struct PartfxEffectState
{
    GameObject* instanceObject;
    void* sourceObject;
    void* auxSequenceBuffer;
    s16 sourceRotX;
    s16 sourceRotY;
    s16 sourceRotZ;
    f32 sourceScale;
    f32 sourcePosX;
    f32 sourcePosY;
    f32 sourcePosZ;
    f32 posStepX;
    f32 posStepY;
    f32 posStepZ;
    Vec3f scaleVectors[4];
    f32 drawPosX;
    f32 drawPosY;
    f32 drawPosZ;
    f32 velocityX;
    f32 velocityY;
    f32 velocityZ;
    void* vertexBuffers[3];
    void* colorBuffers[3];
    void* baseVertexBuffer;
    void* baseColorBuffer;
    void* textureResource;
    void* emitterCommands;
    void* auxAllocation;
    u32 flags;
    s32 initialDelayFrames;
    f32 alphaValues[4];
    union
    {
        f32 blendColorR;
        f32 sourceAlphaStep;
    };
    union
    {
        f32 blendColorG;
        f32 sourceAlphaCurrent;
    };
    f32 blendColorB;
    f32 blendColorStepR;
    f32 blendColorStepG;
    f32 blendColorStepB;
    f32 renderScale;
    u8 padD8[0xE6 - 0xD8];
    s16 soundHandle;
    u8 padE8[0xEA - 0xE8];
    s16 vertexCount;
    s16 colorVertexCount;
    s16 stageDurations[PARTFX_STAGE_COUNT];
    s16 currentStage;
    s16 stageFrameCountdown;
    s16 rotStepZ; /* 0x100: per-frame rotation delta added into rotOffset* */
    s16 rotStepY;
    s16 rotStepX;
    s16 rotOffsetZ;
    s16 rotOffsetY;
    s16 rotOffsetX;
    s16 sequenceId;
    s16 nextStage;
    s16 stageTimer;
    u8 pad112[0x114 - 0x112];
    int word114;
    int word118;
    int word11C;
    s16 vec120;
    s16 vec122;
    s16 vec124;
    s8 byte126;
    u8 pad127[0x12C - 0x127];
    void* inlineData;
    u8 activeVertexBufferIndex;
    u8 textureFrame;
    u8 textureFrameTimer;
    u8 textureFrameStep;
    u8 textureFrameFadeStep;
    s8 sourceYawIndex;
    u8 drawGroupCount;
    u8 drawGroupStride;
    u8 initialStateByte;
    s8 emitterCount;
    u8 releaseRequested;
    char byte13B;
    u8 requestedStage;
    u8 byte13D;
    u8 frameUpdated;
    u8 textureIsBorrowed;
} PartfxEffectState;

#endif
