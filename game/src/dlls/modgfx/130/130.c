/*
 * DLL 130 / 0x82 - a layered object-spawn modgfx effect spawner.
 */
#include "main/dll/dll_0082_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll82EffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 padD2[2];
    s16 triangles[24][3];
    s16 firstSevenVertexIndices[7];
    s16 opaque172;
    s16 middleSevenVertexIndices[7];
    s16 opaque182;
    u8 opaqueIndexData184[0x2C];
    s16 allVertexIndices[21];
    s16 opaque1DA;
    u8 opaqueIndexData1DC[0x1C];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll82EffectResourceView;

STATIC_ASSERT(offsetof(Dll82EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, padD2) == 0x0D2);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, firstSevenVertexIndices) == 0x164);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, opaque172) == 0x172);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, middleSevenVertexIndices) == 0x174);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, opaque182) == 0x182);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, opaqueIndexData184) == 0x184);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, opaque1DA) == 0x1DA);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, opaqueIndexData1DC) == 0x1DC);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(offsetof(Dll82EffectResourceView, opaqueTail) == 0x206);
STATIC_ASSERT(sizeof(Dll82EffectResourceView) == 0x208);

u8 gDll82EffectResourceData[sizeof(Dll82EffectResourceView)] = {
    0,   0,   0,   0,   3,   232, 0,   0,   0,   0,   3,   98,  0,   0,   1,   244, 0,   11,  0,   0,   3,   98,  0,
    0,   254, 12,  0,   22,  0,   0,   0,   0,   0,   0,   252, 24,  0,   32,  0,   0,   252, 158, 0,   0,   254, 12,
    0,   22,  0,   0,   252, 158, 0,   0,   1,   244, 0,   11,  0,   0,   0,   0,   0,   0,   3,   232, 0,   0,   0,
    0,   0,   0,   1,   244, 3,   232, 0,   0,   0,   15,  3,   98,  1,   244, 1,   244, 0,   11,  0,   15,  3,   98,
    1,   244, 254, 12,  0,   22,  0,   15,  0,   0,   1,   244, 252, 24,  0,   32,  0,   15,  252, 158, 1,   244, 254,
    12,  0,   22,  0,   15,  252, 158, 1,   244, 1,   244, 0,   11,  0,   15,  0,   0,   1,   244, 3,   232, 0,   0,
    0,   15,  0,   0,   23,  112, 3,   232, 0,   0,   0,   127, 3,   98,  23,  112, 1,   244, 0,   11,  0,   127, 3,
    98,  23,  112, 254, 12,  0,   22,  0,   127, 0,   0,   23,  112, 252, 24,  0,   32,  0,   127, 252, 158, 23,  112,
    254, 12,  0,   22,  0,   127, 252, 158, 23,  112, 1,   244, 0,   11,  0,   127, 0,   0,   23,  112, 3,   232, 0,
    0,   0,   127, 0,   0,   0,   0,   0,   1,   0,   8,   0,   0,   0,   8,   0,   7,   0,   1,   0,   2,   0,   9,
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
    0,   14,  0,   15,  0,   16,  0,   17,  0,   18,  0,   19,  0,   20,  0,   0,   0,   7,   0,   8,   0,   9,   0,
    10,  0,   11,  0,   12,  0,   13,  0,   14,  0,   15,  0,   16,  0,   17,  0,   18,  0,   19,  0,   20,  0,   0,
    0,   20,  0,   40,  0,   20,  0,   0,   0,   0,   0,   0,   0,   0};

void dll_82_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int modelId,
                        void* extraArg) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll82EffectResourceData;
    GfxCmd* commands;
    f32 originOffset = 0.0f;
    if (variant == 1 || variant == 4) {
        *(s16*)&resourceData[offsetof(Dll82EffectResourceView, sequenceParams[2])] = 0x50;
    }
    if (variant == 2) {
        *(s16*)&resourceData[offsetof(Dll82EffectResourceView, sequenceParams[2])] = 0x6e;
    }
    commands = packet.entries;
    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(Dll82EffectResourceView, allVertexIndices)];
    commands[0].mode = 0x4;
    commands[0].x = originOffset;
    commands[0].y = originOffset;
    commands[0].z = originOffset;
    commands[1].layer = 0;
    commands[1].flags = 0x15;
    commands[1].tex = &resourceData[offsetof(Dll82EffectResourceView, allVertexIndices)];
    commands[1].mode = 0x2;
    commands[1].x = 0.85f;
    commands[1].y = 0.08f;
    commands[1].z = 0.85f;
    commands[2].layer = 1;
    commands[2].flags = 0x15;
    commands[2].tex = &resourceData[offsetof(Dll82EffectResourceView, allVertexIndices)];
    commands[2].mode = 0x2;
    commands[2].x = 1.0f;
    commands[2].y = 10.0f;
    commands[2].z = 1.0f;
    commands[3].layer = 1;
    commands[3].flags = 0x7;
    commands[3].tex = &resourceData[offsetof(Dll82EffectResourceView, firstSevenVertexIndices)];
    commands[3].mode = 0x4;
    commands[3].x = 255.0f;
    commands[3].y = originOffset;
    commands[3].z = originOffset;
    commands[4].layer = 1;
    commands[4].flags = 0x7;
    commands[4].tex = &resourceData[offsetof(Dll82EffectResourceView, middleSevenVertexIndices)];
    commands[4].mode = 0x4;
    commands[4].x = 55.0f;
    commands[4].y = originOffset;
    commands[4].z = originOffset;
    commands[5].layer = 1;
    commands[5].flags = 0x15;
    commands[5].tex = &resourceData[offsetof(Dll82EffectResourceView, allVertexIndices)];
    commands[5].mode = 0x4000;
    commands[5].x = 4.0f;
    commands[5].y = 2.0f;
    commands[5].z = originOffset;
    commands[6].layer = 2;
    commands[6].flags = 0x1e;
    commands[6].tex = NULL;
    commands[6].mode = 0x20000;
    commands[6].x = 1.0f;
    commands[6].y = originOffset;
    commands[6].z = originOffset;
    commands[7].layer = 2;
    commands[7].flags = 0x15;
    commands[7].tex = &resourceData[offsetof(Dll82EffectResourceView, allVertexIndices)];
    commands[7].mode = 0x2;
    commands[7].x = 2.0f;
    commands[7].y = 1.0f;
    commands[7].z = 2.0f;
    commands[8].layer = 2;
    commands[8].flags = 0x15;
    commands[8].tex = &resourceData[offsetof(Dll82EffectResourceView, allVertexIndices)];
    commands[8].mode = 0x4000;
    commands[8].x = 4.0f;
    commands[8].y = 2.0f;
    commands[8].z = originOffset;
    commands[9].layer = 3;
    commands[9].flags = 0x15;
    commands[9].tex = &resourceData[offsetof(Dll82EffectResourceView, allVertexIndices)];
    commands[9].mode = 0x2;
    commands[9].x = 2.0f;
    commands[9].y = 1.0f;
    commands[9].z = 2.0f;
    commands[10].layer = 3;
    commands[10].flags = 0x15;
    commands[10].tex = &resourceData[offsetof(Dll82EffectResourceView, allVertexIndices)];
    commands[10].mode = 0x4000;
    commands[10].x = 4.0f;
    commands[10].y = 2.0f;
    commands[10].z = originOffset;
    commands[11].layer = 3;
    commands[11].flags = 0x7;
    commands[11].tex = &resourceData[offsetof(Dll82EffectResourceView, firstSevenVertexIndices)];
    commands[11].mode = 0x4;
    commands[11].x = originOffset;
    commands[11].y = originOffset;
    commands[11].z = originOffset;
    commands[12].layer = 3;
    commands[12].flags = 0x7;
    commands[12].tex = &resourceData[offsetof(Dll82EffectResourceView, middleSevenVertexIndices)];
    commands[12].mode = 0x4;
    commands[12].x = originOffset;
    commands[12].y = originOffset;
    commands[12].z = originOffset;
    commands[13].layer = 3;
    commands[13].flags = 0x1e;
    commands[13].tex = NULL;
    commands[13].mode = 0x20000;
    commands[13].x = 1.0f;
    commands[13].y = originOffset;
    commands[13].z = originOffset;
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
    packet.drawGroupCount = 2;
    packet.drawGroupStride = 7;
    packet.initialStateByte = 0xe;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0xa;
    packet.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd) * 14) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll82EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll82EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll82EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll82EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll82EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll82EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll82EffectResourceView, sequenceParams[6])];
    packet.commands = commands;
    packet.flags = 0xc010480;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (sourceObj != NULL) {
            packet.position[0] = originOffset + sourceObj->anim.worldPosX;
            packet.position[1] = originOffset + sourceObj->anim.worldPosY;
            packet.position[2] = originOffset + sourceObj->anim.worldPosZ;
        } else {
            packet.position[0] = originOffset + spawnParams->posX;
            packet.position[1] = originOffset + spawnParams->posY;
            packet.position[2] = originOffset + spawnParams->posZ;
        }
    }
    if (variant == 3 || variant == 4) {
        (*gModgfxInterface)
            ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll82EffectResourceData, 0x18,
                          &resourceData[offsetof(Dll82EffectResourceView, triangles)], 0xd9, 0);
    } else {
        (*gModgfxInterface)
            ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll82EffectResourceData, 0x18,
                          &resourceData[offsetof(Dll82EffectResourceView, triangles)], 0x2e, 0);
    }
}

void dll_82_release(void) {
}

void dll_82_initialise(void) {
}

Dll82ResourceDescriptor gDll82ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_82_initialise, dll_82_release, NULL, dll_82_spawnEffect,
};
