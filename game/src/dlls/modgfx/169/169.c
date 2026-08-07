/*
 * DLL 169 / 0xA9 - an alternate-style layered effect spawner.
 */
#include "main/dll/dll_00A9_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct DllA9SevenIndexList {
    s16 indices[7];
    s16 opaqueTail;
} DllA9SevenIndexList;

STATIC_ASSERT(offsetof(DllA9SevenIndexList, indices) == 0x00);
STATIC_ASSERT(offsetof(DllA9SevenIndexList, opaqueTail) == 0x0E);
STATIC_ASSERT(sizeof(DllA9SevenIndexList) == 0x10);

typedef struct DllA9EffectResourceView {
    ModgfxEffectVertex vertices[14];
    s16 triangles[12][3];
    DllA9SevenIndexList firstSevenVertexIndices;
    DllA9SevenIndexList lastSevenVertexIndices;
    s16 allVertexIndices[14];
    s16 sequenceParams[7];
    s16 opaqueTail;
} DllA9EffectResourceView;

STATIC_ASSERT(offsetof(DllA9EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(DllA9EffectResourceView, triangles) == 0x08C);
STATIC_ASSERT(offsetof(DllA9EffectResourceView, firstSevenVertexIndices) == 0x0D4);
STATIC_ASSERT(offsetof(DllA9EffectResourceView, lastSevenVertexIndices) == 0x0E4);
STATIC_ASSERT(offsetof(DllA9EffectResourceView, allVertexIndices) == 0x0F4);
STATIC_ASSERT(offsetof(DllA9EffectResourceView, sequenceParams) == 0x110);
STATIC_ASSERT(offsetof(DllA9EffectResourceView, opaqueTail) == 0x11E);
STATIC_ASSERT(sizeof(DllA9EffectResourceView) == 0x120);

extern u8 gDllA9EffectResourceData[sizeof(DllA9EffectResourceView)];

void dll_A9_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags, int unused,
                        void* alternateStyle) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDllA9EffectResourceData;
    f32 scaleX;
    GfxCmd* commands;
    GfxCmd* commandCursor;
    u32 effectFlags;
    f32 originOffset = 0.0f;

    if (alternateStyle != NULL) {
        scaleX = -2.0f;
    } else {
        scaleX = 2.0f;
    }
    commands = packet.entries;
    commands[0].layer = 0;
    commands[0].flags = 0xe;
    commands[0].tex = &resourceData[offsetof(DllA9EffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = originOffset;
    commands[0].y = originOffset;
    commands[0].z = originOffset;
    if (alternateStyle != NULL) {
        commands[1].layer = 0;
        commands[1].flags = 7;
        commands[1].tex = &resourceData[offsetof(DllA9EffectResourceView, firstSevenVertexIndices.indices)];
        commands[1].mode = 2;
        commands[1].x = 0.8f;
        commands[1].y = 0.006f;
        commands[1].z = 0.8f;
        commands[2].layer = 0;
        commands[2].flags = 7;
        commands[2].tex = &resourceData[offsetof(DllA9EffectResourceView, lastSevenVertexIndices.indices)];
        commands[2].mode = 2;
        commands[2].x = 1.5f;
        commands[2].y = 0.006f;
        commands[2].z = 1.5f;
        commandCursor = commands + 3;
    } else {
        commands[1].layer = 0;
        commands[1].flags = 7;
        commands[1].tex = &resourceData[offsetof(DllA9EffectResourceView, firstSevenVertexIndices.indices)];
        commands[1].mode = 2;
        commands[1].x = 0.8f;
        commands[1].y = 0.028f;
        commands[1].z = 0.8f;
        commands[2].layer = 0;
        commands[2].flags = 7;
        commands[2].tex = &resourceData[offsetof(DllA9EffectResourceView, lastSevenVertexIndices.indices)];
        commands[2].mode = 2;
        commands[2].x = 1.2f;
        commands[2].y = 0.028f;
        commands[2].z = 1.2f;
        commandCursor = commands + 3;
    }
    commandCursor[0].layer = 1;
    commandCursor[0].flags = 0xe;
    commandCursor[0].tex = &resourceData[offsetof(DllA9EffectResourceView, allVertexIndices)];
    commandCursor[0].mode = 2;
    commandCursor[0].x = 1.0f;
    commandCursor[0].y = 130.0f;
    commandCursor[0].z = 1.0f;
    commandCursor[1].layer = 1;
    commandCursor[1].flags = 0xe;
    commandCursor[1].tex = &resourceData[offsetof(DllA9EffectResourceView, allVertexIndices)];
    commandCursor[1].mode = 4;
    commandCursor[1].x = 255.0f;
    commandCursor[1].y = originOffset;
    commandCursor[1].z = originOffset;
    commandCursor[2].layer = 1;
    commandCursor[2].flags = 0xe;
    commandCursor[2].tex = &resourceData[offsetof(DllA9EffectResourceView, allVertexIndices)];
    commandCursor[2].mode = 0x4000;
    commandCursor[2].x = scaleX;
    commandCursor[2].y = originOffset;
    commandCursor[2].z = originOffset;
    commandCursor[3].layer = 2;
    commandCursor[3].flags = 0xe;
    commandCursor[3].tex = &resourceData[offsetof(DllA9EffectResourceView, allVertexIndices)];
    commandCursor[3].mode = 0x4000;
    commandCursor[3].x = scaleX;
    commandCursor[3].y = originOffset;
    commandCursor[3].z = originOffset;
    commandCursor[4].layer = 3;
    commandCursor[4].flags = 1;
    commandCursor[4].tex = NULL;
    commandCursor[4].mode = 0x2000;
    commandCursor[4].x = originOffset;
    commandCursor[4].y = originOffset;
    commandCursor[4].z = originOffset;
    commandCursor[5].layer = 4;
    commandCursor[5].flags = 0xe;
    commandCursor[5].tex = &resourceData[offsetof(DllA9EffectResourceView, allVertexIndices)];
    commandCursor[5].mode = 4;
    commandCursor[5].x = originOffset;
    commandCursor[5].y = originOffset;
    commandCursor[5].z = originOffset;
    commandCursor[6].layer = 4;
    commandCursor[6].flags = 0xe;
    commandCursor[6].tex = &resourceData[offsetof(DllA9EffectResourceView, allVertexIndices)];
    commandCursor[6].mode = 0x4000;
    commandCursor[6].x = scaleX;
    commandCursor[6].y = originOffset;
    commandCursor[6].z = originOffset;
    commandCursor[7].layer = 4;
    commandCursor[7].flags = 0xe;
    commandCursor[7].tex = &resourceData[offsetof(DllA9EffectResourceView, allVertexIndices)];
    commandCursor[7].mode = 2;
    commandCursor[7].x = 1.0f;
    commandCursor[7].y = 0.01f;
    commandCursor[7].z = 1.0f;

    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = originOffset;
    packet.position[1] = originOffset;
    packet.position[2] = originOffset;
    packet.velocity[0] = originOffset;
    packet.velocity[1] = originOffset;
    packet.velocity[2] = originOffset;
    packet.scale = 1.0f;
    packet.drawGroupCount = 1;
    packet.drawGroupStride = 0;
    packet.initialStateByte = 0xe;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0x1e;
    packet.commandCount = &commandCursor[8] - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(DllA9EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(DllA9EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(DllA9EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(DllA9EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(DllA9EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(DllA9EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(DllA9EffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + offsetof(ModgfxSpawnPacket, entries));
    effectFlags = 0xc010040;
    packet.flags = effectFlags;
    effectFlags |= flags;
    packet.flags = effectFlags;
    if (effectFlags & 1) {
        if (sourceObj != NULL) {
            packet.position[0] += sourceObj->anim.worldPosX;
            packet.position[1] += sourceObj->anim.worldPosY;
            packet.position[2] += sourceObj->anim.worldPosZ;
        } else {
            packet.position[0] += spawnParams->posX;
            packet.position[1] += spawnParams->posY;
            packet.position[2] += spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 0xe, (u8*)(int)gDllA9EffectResourceData, 0xc,
                      &resourceData[offsetof(DllA9EffectResourceView, triangles)], 0x586, 0);
}

void dll_A9_release(void) {
}

void dll_A9_initialise(void) {
}

u8 gDllA9EffectResourceData[sizeof(DllA9EffectResourceView)] = {
    0,   0,   0,   0,   3, 232, 0, 0,   0,   0,   3,   98,  0, 0,   1,   244, 0,   31,  0,   0,   3,   98,  0,   0,
    254, 12,  0,   63,  0, 0,   0, 0,   0,   0,   252, 24,  0, 95,  0,   0,   252, 158, 0,   0,   254, 12,  0,   127,
    0,   0,   252, 158, 0, 0,   1, 244, 0,   158, 0,   0,   0, 0,   0,   0,   3,   232, 0,   188, 0,   0,   0,   0,
    3,   232, 3,   232, 0, 0,   0, 31,  3,   98,  3,   232, 1, 244, 0,   31,  0,   31,  3,   98,  3,   232, 254, 12,
    0,   63,  0,   31,  0, 0,   3, 232, 252, 24,  0,   95,  0, 31,  252, 158, 3,   232, 254, 12,  0,   127, 0,   31,
    252, 158, 3,   232, 1, 244, 0, 158, 0,   31,  0,   0,   3, 232, 3,   232, 0,   188, 0,   31,  0,   0,   0,   1,
    0,   8,   0,   0,   0, 8,   0, 7,   0,   1,   0,   2,   0, 9,   0,   1,   0,   9,   0,   8,   0,   2,   0,   3,
    0,   10,  0,   2,   0, 10,  0, 9,   0,   3,   0,   4,   0, 11,  0,   3,   0,   11,  0,   10,  0,   4,   0,   5,
    0,   12,  0,   4,   0, 12,  0, 11,  0,   5,   0,   6,   0, 13,  0,   5,   0,   13,  0,   12,  0,   0,   0,   1,
    0,   2,   0,   3,   0, 4,   0, 5,   0,   6,   0,   0,   0, 7,   0,   8,   0,   9,   0,   10,  0,   11,  0,   12,
    0,   13,  0,   0,   0, 0,   0, 1,   0,   2,   0,   3,   0, 4,   0,   5,   0,   6,   0,   7,   0,   8,   0,   9,
    0,   10,  0,   11,  0, 12,  0, 13,  0,   0,   0,   150, 0, 250, 0,   1,   0,   50,  0,   0,   0,   0,   0,   0};
DllA9ResourceDescriptor gDllA9ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_A9_initialise, dll_A9_release, NULL, dll_A9_spawnEffect,
};
