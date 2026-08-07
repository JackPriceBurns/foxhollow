/*
 * DLL 162 / 0xA2 - a layered pickup effect spawner.
 */
#include "main/dll/dll_00A2_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct DllA2SevenIndexList {
    s16 indices[7];
    s16 opaqueTail;
} DllA2SevenIndexList;

STATIC_ASSERT(offsetof(DllA2SevenIndexList, indices) == 0x00);
STATIC_ASSERT(offsetof(DllA2SevenIndexList, opaqueTail) == 0x0E);
STATIC_ASSERT(sizeof(DllA2SevenIndexList) == 0x10);

typedef struct DllA2EffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 opaqueD2[2];
    s16 triangles[24][3];
    DllA2SevenIndexList sevenVertexIndexLists[3];
    s16 firstAndThirdVertexIndices[14];
    s16 allVertexIndices[21];
    s16 opaque1DA;
    s16 lastFourteenVertexIndices[14];
    s16 sequenceParams[7];
    s16 opaqueTail;
} DllA2EffectResourceView;

STATIC_ASSERT(offsetof(DllA2EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(DllA2EffectResourceView, opaqueD2) == 0x0D2);
STATIC_ASSERT(offsetof(DllA2EffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(DllA2EffectResourceView, sevenVertexIndexLists) == 0x164);
STATIC_ASSERT(offsetof(DllA2EffectResourceView, firstAndThirdVertexIndices) == 0x194);
STATIC_ASSERT(offsetof(DllA2EffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(DllA2EffectResourceView, opaque1DA) == 0x1DA);
STATIC_ASSERT(offsetof(DllA2EffectResourceView, lastFourteenVertexIndices) == 0x1DC);
STATIC_ASSERT(offsetof(DllA2EffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(offsetof(DllA2EffectResourceView, opaqueTail) == 0x206);
STATIC_ASSERT(sizeof(DllA2EffectResourceView) == 0x208);

extern u8 gDllA2EffectResourceData[sizeof(DllA2EffectResourceView)];

void dll_A2_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDllA2EffectResourceData;
    GfxCmd* commands = packet.entries;
    u32 fl;

    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(DllA2EffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    commands[1].layer = 0;
    commands[1].flags = 0x15;
    commands[1].tex = &resourceData[offsetof(DllA2EffectResourceView, allVertexIndices)];
    commands[1].mode = 2;
    commands[1].x = 0.05f;
    commands[1].y = 0.05f;
    commands[1].z = -0.35f;
    commands[2].layer = 0;
    commands[2].flags = 7;
    commands[2].tex = &resourceData[offsetof(DllA2EffectResourceView, sevenVertexIndexLists[0].indices)];
    commands[2].mode = 8;
    commands[2].x = 255.0f;
    commands[2].y = 0.0f;
    commands[2].z = 0.0f;
    commands[3].layer = 1;
    commands[3].flags = 7;
    commands[3].tex = &resourceData[offsetof(DllA2EffectResourceView, sevenVertexIndexLists[1].indices)];
    commands[3].mode = 2;
    commands[3].x = 4.0f;
    commands[3].y = 4.0f;
    commands[3].z = 8.0f;
    commands[4].layer = 1;
    commands[4].flags = 7;
    commands[4].tex = &resourceData[offsetof(DllA2EffectResourceView, sevenVertexIndexLists[2].indices)];
    commands[4].mode = 2;
    commands[4].x = 8.0f;
    commands[4].y = 8.0f;
    commands[4].z = 10.0f;
    commands[5].layer = 1;
    commands[5].flags = 7;
    commands[5].tex = &resourceData[offsetof(DllA2EffectResourceView, sevenVertexIndexLists[1].indices)];
    commands[5].mode = 4;
    commands[5].x = 255.0f;
    commands[5].y = 0.0f;
    commands[5].z = 0.0f;
    commands[6].layer = 1;
    commands[6].flags = 0x15;
    commands[6].tex = &resourceData[offsetof(DllA2EffectResourceView, allVertexIndices)];
    commands[6].mode = 0x4000;
    commands[6].x = 1.0f;
    commands[6].y = -4.0f;
    commands[6].z = 0.0f;
    commands[7].layer = 2;
    commands[7].flags = 7;
    commands[7].tex = &resourceData[offsetof(DllA2EffectResourceView, sevenVertexIndexLists[1].indices)];
    commands[7].mode = 2;
    commands[7].x = 1.0f;
    commands[7].y = 1.0f;
    commands[7].z = 1.0f;
    commands[8].layer = 2;
    commands[8].flags = 7;
    commands[8].tex = &resourceData[offsetof(DllA2EffectResourceView, sevenVertexIndexLists[2].indices)];
    commands[8].mode = 2;
    commands[8].x = 1.0f;
    commands[8].y = 1.0f;
    commands[8].z = 1.0f;
    commands[9].layer = 2;
    commands[9].flags = 0x15;
    commands[9].tex = &resourceData[offsetof(DllA2EffectResourceView, allVertexIndices)];
    commands[9].mode = 0x4000;
    commands[9].x = 1.0f;
    commands[9].y = -4.0f;
    commands[9].z = 0.0f;
    commands[10].layer = 3;
    commands[10].flags = 7;
    commands[10].tex = &resourceData[offsetof(DllA2EffectResourceView, sevenVertexIndexLists[1].indices)];
    commands[10].mode = 4;
    commands[10].x = 0.0f;
    commands[10].y = 0.0f;
    commands[10].z = 0.0f;
    commands[11].layer = 3;
    commands[11].flags = 0x15;
    commands[11].tex = &resourceData[offsetof(DllA2EffectResourceView, allVertexIndices)];
    commands[11].mode = 0x4000;
    commands[11].x = 1.0f;
    commands[11].y = -4.0f;
    commands[11].z = 0.0f;

    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = 0.0f;
    packet.position[1] = 0.0f;
    packet.position[2] = 0.0f;
    packet.velocity[0] = 0.0f;
    packet.velocity[1] = 0.0f;
    packet.velocity[2] = 0.0f;
    packet.scale = 1.0f;
    packet.drawGroupCount = 2;
    packet.drawGroupStride = 7;
    packet.initialStateByte = 0xe;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0x1e;
    packet.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd) * 12) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(DllA2EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(DllA2EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(DllA2EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(DllA2EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(DllA2EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(DllA2EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(DllA2EffectResourceView, sequenceParams[6])];
    packet.commands = commands;
    packet.flags = 0xc010480;
    packet.flags |= flags;
    fl = packet.flags;
    if ((fl & 1) != 0) {
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
        ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDllA2EffectResourceData, 0x18,
                      &resourceData[offsetof(DllA2EffectResourceView, triangles)], 0x24, 0);
}

void dll_A2_release(void) {
}

void dll_A2_initialise(void) {
}

u8 gDllA2EffectResourceData[sizeof(DllA2EffectResourceView)] = {
    0,   0,   3,   232, 0,   0,   0,   0,   0,   0,   3, 98,  1,   244, 0,   0,   0,   11,  0,   0,   3,   98, 254, 12,
    0,   0,   0,   22,  0,   0,   0,   0,   252, 24,  0, 0,   0,   31,  0,   0,   252, 158, 254, 12,  0,   0,  0,   22,
    0,   0,   252, 158, 1,   244, 0,   0,   0,   11,  0, 0,   0,   0,   3,   232, 0,   0,   0,   0,   0,   0,  0,   0,
    3,   232, 1,   244, 0,   0,   0,   15,  3,   98,  1, 244, 1,   44,  0,   11,  0,   15,  3,   98,  254, 12, 1,   244,
    0,   22,  0,   15,  0,   0,   252, 24,  1,   44,  0, 31,  0,   15,  252, 158, 254, 12,  1,   244, 0,   22, 0,   15,
    252, 158, 1,   244, 1,   44,  0,   11,  0,   15,  0, 0,   3,   232, 1,   244, 0,   0,   0,   15,  0,   0,  3,   232,
    3,   32,  0,   0,   0,   31,  3,   98,  1,   244, 3, 232, 0,   11,  0,   31,  3,   98,  254, 12,  3,   32, 0,   22,
    0,   31,  0,   0,   252, 24,  3,   232, 0,   31,  0, 31,  252, 158, 254, 12,  3,   32,  0,   22,  0,   31, 252, 158,
    1,   244, 3,   232, 0,   11,  0,   31,  0,   0,   3, 232, 3,   32,  0,   0,   0,   31,  0,   0,   0,   0,  0,   1,
    0,   8,   0,   0,   0,   8,   0,   7,   0,   1,   0, 2,   0,   9,   0,   1,   0,   9,   0,   8,   0,   2,  0,   3,
    0,   10,  0,   2,   0,   10,  0,   9,   0,   3,   0, 4,   0,   11,  0,   3,   0,   11,  0,   10,  0,   4,  0,   5,
    0,   12,  0,   4,   0,   12,  0,   11,  0,   5,   0, 6,   0,   13,  0,   5,   0,   13,  0,   12,  0,   7,  0,   8,
    0,   15,  0,   7,   0,   15,  0,   14,  0,   8,   0, 9,   0,   16,  0,   8,   0,   16,  0,   15,  0,   9,  0,   10,
    0,   17,  0,   9,   0,   17,  0,   16,  0,   10,  0, 11,  0,   18,  0,   10,  0,   18,  0,   17,  0,   11, 0,   12,
    0,   19,  0,   11,  0,   19,  0,   18,  0,   12,  0, 13,  0,   20,  0,   12,  0,   20,  0,   19,  0,   0,  0,   1,
    0,   2,   0,   3,   0,   4,   0,   5,   0,   6,   0, 0,   0,   7,   0,   8,   0,   9,   0,   10,  0,   11, 0,   12,
    0,   13,  0,   0,   0,   14,  0,   15,  0,   16,  0, 17,  0,   18,  0,   19,  0,   20,  0,   0,   0,   0,  0,   1,
    0,   2,   0,   3,   0,   4,   0,   5,   0,   6,   0, 14,  0,   15,  0,   16,  0,   17,  0,   18,  0,   19, 0,   20,
    0,   0,   0,   1,   0,   2,   0,   3,   0,   4,   0, 5,   0,   6,   0,   7,   0,   8,   0,   9,   0,   10, 0,   11,
    0,   12,  0,   13,  0,   14,  0,   15,  0,   16,  0, 17,  0,   18,  0,   19,  0,   20,  0,   0,   0,   7,  0,   8,
    0,   9,   0,   10,  0,   11,  0,   12,  0,   13,  0, 14,  0,   15,  0,   16,  0,   17,  0,   18,  0,   19, 0,   20,
    0,   0,   0,   35,  0,   6,   0,   35,  0,   0,   0, 0,   0,   0,   0,   0};
DllA2ResourceDescriptor gDllA2ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_A2_initialise, dll_A2_release, NULL, dll_A2_spawnEffect,
};
