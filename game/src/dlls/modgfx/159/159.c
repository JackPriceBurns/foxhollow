/*
 * DLL 159 / 0x9F - a rotation-aware layered pickup effect spawner.
 */
#include "main/dll/dll_009F_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll9FSevenIndexList {
    s16 indices[7];
    s16 opaqueTail;
} Dll9FSevenIndexList;

STATIC_ASSERT(offsetof(Dll9FSevenIndexList, indices) == 0x00);
STATIC_ASSERT(offsetof(Dll9FSevenIndexList, opaqueTail) == 0x0E);
STATIC_ASSERT(sizeof(Dll9FSevenIndexList) == 0x10);

typedef struct Dll9FEffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 opaqueD2[2];
    s16 triangles[24][3];
    Dll9FSevenIndexList sevenVertexIndexLists[3];
    s16 firstAndThirdVertexIndices[14];
    s16 allVertexIndices[21];
    s16 opaque1DA;
    s16 lastFourteenVertexIndices[14];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll9FEffectResourceView;

STATIC_ASSERT(offsetof(Dll9FEffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll9FEffectResourceView, opaqueD2) == 0x0D2);
STATIC_ASSERT(offsetof(Dll9FEffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll9FEffectResourceView, sevenVertexIndexLists) == 0x164);
STATIC_ASSERT(offsetof(Dll9FEffectResourceView, firstAndThirdVertexIndices) == 0x194);
STATIC_ASSERT(offsetof(Dll9FEffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll9FEffectResourceView, opaque1DA) == 0x1DA);
STATIC_ASSERT(offsetof(Dll9FEffectResourceView, lastFourteenVertexIndices) == 0x1DC);
STATIC_ASSERT(offsetof(Dll9FEffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(offsetof(Dll9FEffectResourceView, opaqueTail) == 0x206);
STATIC_ASSERT(sizeof(Dll9FEffectResourceView) == 0x208);

extern u8 gDll9FEffectResourceData[sizeof(Dll9FEffectResourceView)];

void dll_9F_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = gDll9FEffectResourceData;
    GfxCmd* commands = packet.entries;
    GfxCmd* commandCursor = commands;
    int sourceRotationX = sourceObj->anim.rotX;
    u32 fl;

    if (sourceRotationX != 0) {
        commandCursor->layer = 0;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll9FEffectResourceView, allVertexIndices)];
        commandCursor->mode = 0x80;
        commandCursor->x = 0.0f;
        commandCursor->y = 0.0f;
        commandCursor->z = sourceRotationX;
        commandCursor = commands + 1;
    }
    commandCursor[0].layer = 0;
    commandCursor[0].flags = 0x15;
    commandCursor[0].tex = &resourceData[offsetof(Dll9FEffectResourceView, allVertexIndices)];
    commandCursor[0].mode = 4;
    commandCursor[0].x = 0.0f;
    commandCursor[0].y = 0.0f;
    commandCursor[0].z = 0.0f;
    commandCursor[1].layer = 0;
    commandCursor[1].flags = 7;
    commandCursor[1].tex = &resourceData[offsetof(Dll9FEffectResourceView, sevenVertexIndexLists[0].indices)];
    commandCursor[1].mode = 2;
    commandCursor[1].x = 0.8f;
    commandCursor[1].y = 0.8f;
    commandCursor[1].z = 0.5f;
    commandCursor[2].layer = 0;
    commandCursor[2].flags = 7;
    commandCursor[2].tex = &resourceData[offsetof(Dll9FEffectResourceView, sevenVertexIndexLists[1].indices)];
    commandCursor[2].mode = 2;
    commandCursor[2].x = 1.2f;
    commandCursor[2].y = 1.2f;
    commandCursor[2].z = 0.5f;
    commandCursor[3].layer = 0;
    commandCursor[3].flags = 7;
    commandCursor[3].tex = &resourceData[offsetof(Dll9FEffectResourceView, sevenVertexIndexLists[2].indices)];
    commandCursor[3].mode = 2;
    commandCursor[3].x = 0.8f;
    commandCursor[3].y = 0.8f;
    commandCursor[3].z = 0.5f;
    commandCursor[4].layer = 1;
    commandCursor[4].flags = 7;
    commandCursor[4].tex = &resourceData[offsetof(Dll9FEffectResourceView, sevenVertexIndexLists[1].indices)];
    commandCursor[4].mode = 4;
    commandCursor[4].x = 195.0f;
    commandCursor[4].y = 0.0f;
    commandCursor[4].z = 0.0f;
    commandCursor[5].layer = 1;
    commandCursor[5].flags = 0x15;
    commandCursor[5].tex = &resourceData[offsetof(Dll9FEffectResourceView, allVertexIndices)];
    commandCursor[5].mode = 0x4000;
    commandCursor[5].x = 2.0f;
    commandCursor[5].y = -2.0f;
    commandCursor[5].z = 0.0f;
    commandCursor[6].layer = 1;
    commandCursor[6].flags = 0;
    commandCursor[6].tex = NULL;
    commandCursor[6].mode = 0x400000;
    commandCursor[6].x = 0.0f;
    commandCursor[6].y = 0.0f;
    commandCursor[6].z = 160.0f;
    commandCursor[7].layer = 2;
    commandCursor[7].flags = 0x15;
    commandCursor[7].tex = &resourceData[offsetof(Dll9FEffectResourceView, allVertexIndices)];
    commandCursor[7].mode = 0x4000;
    commandCursor[7].x = 2.0f;
    commandCursor[7].y = -2.0f;
    commandCursor[7].z = 0.0f;
    commandCursor[8].layer = 2;
    commandCursor[8].flags = 0;
    commandCursor[8].tex = NULL;
    commandCursor[8].mode = 0x400000;
    commandCursor[8].x = 0.0f;
    commandCursor[8].y = 0.0f;
    commandCursor[8].z = 740.0f;
    commandCursor[9].layer = 2;
    commandCursor[9].flags = 0x15;
    commandCursor[9].tex = &resourceData[offsetof(Dll9FEffectResourceView, allVertexIndices)];
    commandCursor[9].mode = 8;
    commandCursor[9].x = 255.0f;
    commandCursor[9].y = 255.0f;
    commandCursor[9].z = 85.0f;
    commandCursor[10].layer = 3;
    commandCursor[10].flags = 0x15;
    commandCursor[10].tex = &resourceData[offsetof(Dll9FEffectResourceView, allVertexIndices)];
    commandCursor[10].mode = 0x4000;
    commandCursor[10].x = 2.0f;
    commandCursor[10].y = 2.0f;
    commandCursor[10].z = 0.0f;
    commandCursor[11].layer = 3;
    commandCursor[11].flags = 0;
    commandCursor[11].tex = NULL;
    commandCursor[11].mode = 0x400000;
    commandCursor[11].x = 0.0f;
    commandCursor[11].y = 0.0f;
    commandCursor[11].z = -740.0f;
    commandCursor[12].layer = 3;
    commandCursor[12].flags = 0x15;
    commandCursor[12].tex = &resourceData[offsetof(Dll9FEffectResourceView, allVertexIndices)];
    commandCursor[12].mode = 8;
    commandCursor[12].x = 255.0f;
    commandCursor[12].y = 255.0f;
    commandCursor[12].z = 255.0f;
    commandCursor[13].layer = 4;
    commandCursor[13].flags = 0x15;
    commandCursor[13].tex = &resourceData[offsetof(Dll9FEffectResourceView, allVertexIndices)];
    commandCursor[13].mode = 0x4000;
    commandCursor[13].x = 2.0f;
    commandCursor[13].y = 2.0f;
    commandCursor[13].z = 0.0f;
    commandCursor[14].layer = 4;
    commandCursor[14].flags = 7;
    commandCursor[14].tex = &resourceData[offsetof(Dll9FEffectResourceView, sevenVertexIndexLists[1].indices)];
    commandCursor[14].mode = 4;
    commandCursor[14].x = 0.0f;
    commandCursor[14].y = 0.0f;
    commandCursor[14].z = 0.0f;
    commandCursor[15].layer = 4;
    commandCursor[15].flags = 0;
    commandCursor[15].tex = NULL;
    commandCursor[15].mode = 0x400000;
    commandCursor[15].x = 0.0f;
    commandCursor[15].y = 0.0f;
    commandCursor[15].z = -160.0f;

    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = 0.0f;
    packet.position[1] = 0.0f;
    packet.position[2] = 0.0f;
    packet.velocity[0] = 0.0f;
    packet.velocity[1] = 0.0f;
    packet.velocity[2] = 0.0f;
    packet.scale = 2.2f;
    packet.drawGroupCount = 2;
    packet.drawGroupStride = 7;
    packet.initialStateByte = 0xe;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0x1e;
    packet.commandCount = &commandCursor[16] - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll9FEffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll9FEffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll9FEffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll9FEffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll9FEffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll9FEffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll9FEffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + offsetof(ModgfxSpawnPacket, entries));
    fl = 0xc0104c0;
    packet.flags = fl;
    fl |= flags;
    packet.flags = fl;
    if (fl & 1) {
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
        ->spawnEffect(&packet, 0, 0x15, resourceData, 0x18, &resourceData[offsetof(Dll9FEffectResourceView, triangles)],
                      0x46c, 0);
}

void dll_9F_release(void) {
}

void dll_9F_initialise(void) {
}

u8 gDll9FEffectResourceData[sizeof(Dll9FEffectResourceView)] = {
    0,   0,   3,  232, 0,   0,   0,   0,   0,   0,   3,   98,  1,  244, 0,   0,   0,   22,  0,   0,   3,   98,  254,
    12,  0,   0,  0,   44,  0,   0,   0,   0,   252, 24,  0,   0,  0,   63,  0,   0,   252, 158, 254, 12,  0,   0,
    0,   44,  0,  0,   252, 158, 1,   244, 0,   0,   0,   22,  0,  0,   0,   0,   3,   232, 0,   0,   0,   0,   0,
    0,   0,   0,  3,   232, 11,  184, 0,   0,   0,   15,  3,   98, 1,   244, 11,  184, 0,   22,  0,   15,  3,   98,
    254, 12,  11, 184, 0,   44,  0,   15,  0,   0,   252, 24,  11, 184, 0,   63,  0,   15,  252, 158, 254, 12,  11,
    184, 0,   44, 0,   15,  252, 158, 1,   244, 11,  184, 0,   22, 0,   15,  0,   0,   3,   232, 11,  184, 0,   0,
    0,   15,  0,  0,   3,   232, 23,  112, 0,   0,   0,   31,  3,  98,  1,   244, 23,  112, 0,   22,  0,   31,  3,
    98,  254, 12, 23,  112, 0,   44,  0,   31,  0,   0,   252, 24, 23,  112, 0,   63,  0,   31,  252, 158, 254, 12,
    23,  112, 0,  44,  0,   31,  252, 158, 1,   244, 23,  112, 0,  22,  0,   31,  0,   0,   3,   232, 23,  112, 0,
    0,   0,   31, 0,   0,   0,   0,   0,   1,   0,   8,   0,   0,  0,   8,   0,   7,   0,   1,   0,   2,   0,   9,
    0,   1,   0,  9,   0,   8,   0,   2,   0,   3,   0,   10,  0,  2,   0,   10,  0,   9,   0,   3,   0,   4,   0,
    11,  0,   3,  0,   11,  0,   10,  0,   4,   0,   5,   0,   12, 0,   4,   0,   12,  0,   11,  0,   5,   0,   6,
    0,   13,  0,  5,   0,   13,  0,   12,  0,   7,   0,   8,   0,  15,  0,   7,   0,   15,  0,   14,  0,   8,   0,
    9,   0,   16, 0,   8,   0,   16,  0,   15,  0,   9,   0,   10, 0,   17,  0,   9,   0,   17,  0,   16,  0,   10,
    0,   11,  0,  18,  0,   10,  0,   18,  0,   17,  0,   11,  0,  12,  0,   19,  0,   11,  0,   19,  0,   18,  0,
    12,  0,   13, 0,   20,  0,   12,  0,   20,  0,   19,  0,   0,  0,   1,   0,   2,   0,   3,   0,   4,   0,   5,
    0,   6,   0,  0,   0,   7,   0,   8,   0,   9,   0,   10,  0,  11,  0,   12,  0,   13,  0,   0,   0,   14,  0,
    15,  0,   16, 0,   17,  0,   18,  0,   19,  0,   20,  0,   0,  0,   0,   0,   1,   0,   2,   0,   3,   0,   4,
    0,   5,   0,  6,   0,   14,  0,   15,  0,   16,  0,   17,  0,  18,  0,   19,  0,   20,  0,   0,   0,   1,   0,
    2,   0,   3,  0,   4,   0,   5,   0,   6,   0,   7,   0,   8,  0,   9,   0,   10,  0,   11,  0,   12,  0,   13,
    0,   14,  0,  15,  0,   16,  0,   17,  0,   18,  0,   19,  0,  20,  0,   0,   0,   7,   0,   8,   0,   9,   0,
    10,  0,   11, 0,   12,  0,   13,  0,   14,  0,   15,  0,   16, 0,   17,  0,   18,  0,   19,  0,   20,  0,   0,
    0,   50,  0,  250, 0,   250, 0,   50,  0,   0,   0,   0,   0,  0,
};

Dll9FResourceDescriptor gDll9FResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_9F_initialise, dll_9F_release, NULL, dll_9F_spawnEffect,
};
