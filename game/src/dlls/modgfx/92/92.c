/*
 * DLL 92 / 0x5C - a modgfx particle-sequence spawn DLL.
 */
#include "main/dll/dll_005C_modgfx.h"
#include "game/objects/object.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll5CEffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 padD2[2];
    s16 colors[24][3];
    u8 opaque164[0x10];
    s16 partialIndices[30];
    s16 fullIndices[22];
    s16 sequenceParams[7];
    u8 pad1EA[2];
} Dll5CEffectResourceView;

STATIC_ASSERT(offsetof(Dll5CEffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll5CEffectResourceView, colors) == 0x0D4);
STATIC_ASSERT(offsetof(Dll5CEffectResourceView, partialIndices) == 0x174);
STATIC_ASSERT(offsetof(Dll5CEffectResourceView, fullIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll5CEffectResourceView, sequenceParams) == 0x1DC);
STATIC_ASSERT(sizeof(Dll5CEffectResourceView) == 0x1EC);

u8 gDll5CEffectResourceData[sizeof(Dll5CEffectResourceView)] = {
    0,   0,   0,   0,   3,   232, 0,   0,   0,   0,   3,   98,  0,   0,   1,   244, 0,   11,  0,   0,   3,   98,  0,
    0,   254, 12,  0,   22,  0,   0,   0,   0,   0,   0,   252, 24,  0,   32,  0,   0,   252, 158, 0,   0,   254, 12,
    0,   42,  0,   0,   252, 158, 0,   0,   1,   244, 0,   52,  0,   0,   0,   0,   0,   0,   3,   232, 0,   63,  0,
    0,   0,   0,   11,  184, 3,   232, 0,   0,   0,   31,  3,   98,  11,  184, 1,   244, 0,   11,  0,   31,  3,   98,
    11,  184, 254, 12,  0,   22,  0,   31,  0,   0,   11,  184, 252, 24,  0,   32,  0,   31,  252, 158, 11,  184, 254,
    12,  0,   42,  0,   31,  252, 158, 11,  184, 1,   244, 0,   52,  0,   31,  0,   0,   11,  184, 3,   232, 0,   63,
    0,   31,  0,   0,   23,  112, 3,   232, 0,   0,   0,   63,  3,   98,  23,  112, 1,   244, 0,   11,  0,   63,  3,
    98,  23,  112, 254, 12,  0,   22,  0,   63,  0,   0,   23,  112, 252, 24,  0,   32,  0,   63,  252, 158, 23,  112,
    254, 12,  0,   42,  0,   63,  252, 158, 23,  112, 1,   244, 0,   52,  0,   63,  0,   0,   23,  112, 3,   232, 0,
    63,  0,   63,  0,   0,   0,   0,   0,   1,   0,   8,   0,   0,   0,   8,   0,   7,   0,   1,   0,   2,   0,   9,
    0,   1,   0,   9,   0,   8,   0,   2,   0,   3,   0,   10,  0,   2,   0,   10,  0,   9,   0,   3,   0,   4,   0,
    11,  0,   3,   0,   11,  0,   10,  0,   4,   0,   5,   0,   12,  0,   4,   0,   12,  0,   11,  0,   5,   0,   6,
    0,   13,  0,   5,   0,   13,  0,   12,  0,   7,   0,   8,   0,   15,  0,   7,   0,   15,  0,   14,  0,   8,   0,
    9,   0,   16,  0,   8,   0,   16,  0,   15,  0,   9,   0,   10,  0,   17,  0,   9,   0,   17,  0,   16,  0,   10,
    0,   11,  0,   18,  0,   10,  0,   18,  0,   17,  0,   11,  0,   12,  0,   19,  0,   11,  0,   19,  0,   18,  0,
    12,  0,   13,  0,   20,  0,   12,  0,   20,  0,   19,  0,   0,   0,   1,   0,   2,   0,   3,   0,   4,   0,   5,
    0,   6,   0,   0,   0,   7,   0,   8,   0,   9,   0,   10,  0,   11,  0,   12,  0,   13,  0,   0,   0,   14,  0,
    15,  0,   16,  0,   17,  0,   18,  0,   19,  0,   20,  0,   0,   0,   0,   0,   1,   0,   2,   0,   3,   0,   4,
    0,   5,   0,   6,   0,   14,  0,   15,  0,   16,  0,   17,  0,   18,  0,   19,  0,   20,  0,   0,   0,   1,   0,
    2,   0,   3,   0,   4,   0,   5,   0,   6,   0,   7,   0,   8,   0,   9,   0,   10,  0,   11,  0,   12,  0,   13,
    0,   14,  0,   15,  0,   16,  0,   17,  0,   18,  0,   19,  0,   20,  0,   0,   0,   0,   0,   30,  0,   80,  0,
    30,  0,   0,   0,   0,   0,   0,   0,   0};

void dll_5C_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)gDll5CEffectResourceData;
    GfxCmd* commands = packet.entries;
    GameObject* sourceContext;
    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(Dll5CEffectResourceView, fullIndices)];
    commands[0].mode = 4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    commands[1].layer = 0;
    commands[1].flags = 0x15;
    commands[1].tex = &resourceData[offsetof(Dll5CEffectResourceView, fullIndices)];
    commands[1].mode = 2;
    commands[1].x = 0.065f;
    commands[1].y = 1.3f;
    commands[1].z = 0.065f;
    commands[2].layer = 0;
    commands[2].flags = 0x15;
    commands[2].tex = &resourceData[offsetof(Dll5CEffectResourceView, fullIndices)];
    commands[2].mode = 0x400000;
    commands[2].x = 0.0f;
    commands[2].y = 100.0f;
    commands[2].z = 0.0f;
    commands[3].layer = 1;
    commands[3].flags = 7;
    commands[3].tex = &resourceData[offsetof(Dll5CEffectResourceView, partialIndices)];
    commands[3].mode = 4;
    commands[3].x = 155.0f;
    commands[3].y = 0.0f;
    commands[3].z = 0.0f;
    commands[4].layer = 1;
    commands[4].flags = 0x15;
    commands[4].tex = &resourceData[offsetof(Dll5CEffectResourceView, fullIndices)];
    commands[4].mode = 0x4000;
    commands[4].x = -2.0f;
    commands[4].y = 2.0f;
    commands[4].z = 0.0f;
    commands[5].layer = 1;
    commands[5].flags = 0x15;
    commands[5].tex = &resourceData[offsetof(Dll5CEffectResourceView, fullIndices)];
    commands[5].mode = 0x400000;
    commands[5].x = 0.0f;
    commands[5].y = -100.0f;
    commands[5].z = 0.0f;
    commands[6].layer = 2;
    commands[6].flags = 0x15;
    commands[6].tex = &resourceData[offsetof(Dll5CEffectResourceView, fullIndices)];
    commands[6].mode = 0x4000;
    commands[6].x = 2.0f;
    commands[6].y = -2.0f;
    commands[6].z = 0.0f;
    commands[7].layer = 2;
    commands[7].flags = 0x15;
    commands[7].tex = &resourceData[offsetof(Dll5CEffectResourceView, fullIndices)];
    commands[7].mode = 0x400000;
    commands[7].x = 0.0f;
    commands[7].y = 10.0f;
    commands[7].z = 0.0f;
    commands[8].layer = 2;
    commands[8].flags = 0x15;
    commands[8].tex = &resourceData[offsetof(Dll5CEffectResourceView, fullIndices)];
    commands[8].mode = 2;
    commands[8].x = 12.0f;
    commands[8].y = 1.3f;
    commands[8].z = 12.0f;
    commands[9].layer = 3;
    commands[9].flags = 7;
    commands[9].tex = &resourceData[offsetof(Dll5CEffectResourceView, partialIndices)];
    commands[9].mode = 4;
    commands[9].x = 0.0f;
    commands[9].y = 0.0f;
    commands[9].z = 0.0f;
    commands[10].layer = 3;
    commands[10].flags = 0x15;
    commands[10].tex = &resourceData[offsetof(Dll5CEffectResourceView, fullIndices)];
    commands[10].mode = 0x4000;
    commands[10].x = 2.0f;
    commands[10].y = -2.0f;
    commands[10].z = 0.0f;
    packet.context.modeByte = 0;
    sourceContext = sourceObj;
    packet.context.attachedSource = sourceContext;
    packet.context.sourceMode = variant;
    packet.context.position[0] = 0.0f;
    packet.context.position[1] = -10.0f;
    packet.context.position[2] = 0.0f;
    packet.context.velocity[0] = 0.0f;
    packet.context.velocity[1] = 0.0f;
    packet.context.velocity[2] = 0.0f;
    packet.context.scale = 1.0f;
    packet.context.drawGroupCount = 2;
    packet.context.drawGroupStride = 7;
    packet.context.initialStateByte = 0xe;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x1e;
    packet.context.commandCount = (commands + 11) - packet.entries;
    packet.context.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll5CEffectResourceView, sequenceParams[0])];
    packet.context.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll5CEffectResourceView, sequenceParams[1])];
    packet.context.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll5CEffectResourceView, sequenceParams[2])];
    packet.context.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll5CEffectResourceView, sequenceParams[3])];
    packet.context.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll5CEffectResourceView, sequenceParams[4])];
    packet.context.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll5CEffectResourceView, sequenceParams[5])];
    packet.context.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll5CEffectResourceView, sequenceParams[6])];
    packet.context.commands = packet.entries;
    packet.context.flags = 0xc000040;
    packet.context.flags |= spawnFlags;
    if ((packet.context.flags & 1) != 0) {
        if ((void*)sourceContext != NULL) {
            packet.context.position[0] += sourceContext->anim.worldPosX;
            packet.context.position[1] = -10.0f + sourceContext->anim.worldPosY;
            packet.context.position[2] += sourceContext->anim.worldPosZ;
        } else {
            packet.context.position[0] += spawnParams->posX;
            packet.context.position[1] = -10.0f + spawnParams->posY;
            packet.context.position[2] += spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet.context, 0, 0x15, (u8*)gDll5CEffectResourceData, 0x18,
                      &resourceData[offsetof(Dll5CEffectResourceView, colors)], 0x20B, 0);
}

void dll_5C_release(void) {
}

void dll_5C_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gDll5CResourceDescriptorAcquire, dll_5C_initialise)

Dll5CResourceDescriptor gDll5CResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll5CResourceDescriptorAcquire, dll_5C_release },
    NULL,
    dll_5C_spawnEffect,
    0,
};
