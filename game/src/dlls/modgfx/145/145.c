#include "main/dll/dll_0091_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll91EffectResourceView {
    ModgfxEffectVertex vertices[18];
    s16 triangles[16][3];
    s16 firstNineVertexIndices[9];
    s16 opaque126;
    s16 secondNineVertexIndices[9];
    s16 opaque13A;
    s16 thirdNineVertexIndices[9];
    s16 opaque14E;
    s16 allVertexIndices[18];
    s16 evenVertexIndices[9];
    s16 opaque186;
    s16 oddVertexIndices[5];
    s16 opaque192;
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll91EffectResourceView;

STATIC_ASSERT(offsetof(Dll91EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, triangles) == 0x0B4);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, firstNineVertexIndices) == 0x114);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, opaque126) == 0x126);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, secondNineVertexIndices) == 0x128);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, opaque13A) == 0x13A);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, thirdNineVertexIndices) == 0x13C);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, opaque14E) == 0x14E);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, allVertexIndices) == 0x150);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, evenVertexIndices) == 0x174);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, opaque186) == 0x186);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, oddVertexIndices) == 0x188);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, opaque192) == 0x192);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, sequenceParams) == 0x194);
STATIC_ASSERT(offsetof(Dll91EffectResourceView, opaqueTail) == 0x1A2);
STATIC_ASSERT(sizeof(Dll91EffectResourceView) == 0x1A4);

s16 gDll91VertexIndices[4] = {10, 12, 14, 16};

extern u8 gDll91EffectResourceData[sizeof(Dll91EffectResourceView)];

void dll_91_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll91EffectResourceData;
    GfxCmd* commands = packet.entries;

    commands[0].layer = 0;
    commands[0].flags = 0x12;
    commands[0].tex = &resourceData[offsetof(Dll91EffectResourceView, allVertexIndices)];
    commands[0].mode = 0x4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    commands[1].layer = 0;
    commands[1].flags = 9;
    commands[1].tex = &resourceData[offsetof(Dll91EffectResourceView, firstNineVertexIndices)];
    commands[1].mode = 0x8;
    commands[1].x = 0.0f;
    commands[1].y = 0.0f;
    commands[1].z = 255.0f;
    commands[2].layer = 0;
    commands[2].flags = 9;
    commands[2].tex = &resourceData[offsetof(Dll91EffectResourceView, secondNineVertexIndices)];
    commands[2].mode = 0x2;
    commands[2].x = 3.0f;
    commands[2].y = 0.03f;
    commands[2].z = 3.0f;
    commands[3].layer = 0;
    commands[3].flags = 0x12;
    commands[3].tex = &resourceData[offsetof(Dll91EffectResourceView, allVertexIndices)];
    commands[3].mode = 0x2;
    commands[3].x = 1.75f;
    commands[3].y = 0.5f;
    commands[3].z = 1.75f;
    commands[4].layer = 0;
    commands[4].flags = 9;
    commands[4].tex = &resourceData[offsetof(Dll91EffectResourceView, secondNineVertexIndices)];
    commands[4].mode = 0x8;
    commands[4].x = 255.0f;
    commands[4].y = 0.0f;
    commands[4].z = 255.0f;
    commands[5].layer = 1;
    commands[5].flags = 0x12;
    commands[5].tex = &resourceData[offsetof(Dll91EffectResourceView, allVertexIndices)];
    commands[5].mode = 0x4;
    commands[5].x = 255.0f;
    commands[5].y = 0.0f;
    commands[5].z = 0.0f;
    commands[6].layer = 1;
    commands[6].flags = 9;
    commands[6].tex = &resourceData[offsetof(Dll91EffectResourceView, secondNineVertexIndices)];
    commands[6].mode = 0x2;
    commands[6].x = 1.0f;
    commands[6].y = 150.0f;
    commands[6].z = 1.0f;
    commands[7].layer = 2;
    commands[7].flags = 0;
    commands[7].tex = NULL;
    commands[7].mode = 0x20;
    commands[7].x = 0.0f;
    commands[7].y = 0.0f;
    commands[7].z = 0.0f;
    commands[8].layer = 3;
    commands[8].flags = 9;
    commands[8].tex = &resourceData[offsetof(Dll91EffectResourceView, firstNineVertexIndices)];
    commands[8].mode = 0x8;
    commands[8].x = 255.0f;
    commands[8].y = 155.0f;
    commands[8].z = 0.0f;
    commands[9].layer = 3;
    commands[9].flags = 0x12;
    commands[9].tex = &resourceData[offsetof(Dll91EffectResourceView, allVertexIndices)];
    commands[9].mode = 0x100;
    commands[9].x = 0.0f;
    commands[9].y = 0.0f;
    commands[9].z = -10.0f;
    commands[10].layer = 3;
    commands[10].flags = 5;
    commands[10].tex = &resourceData[offsetof(Dll91EffectResourceView, oddVertexIndices)];
    commands[10].mode = 0x2;
    commands[10].x = 0.98f;
    commands[10].y = 1.0f;
    commands[10].z = 0.98f;
    commands[11].layer = 3;
    commands[11].flags = 4;
    commands[11].tex = gDll91VertexIndices;
    commands[11].mode = 0x2;
    commands[11].x = 1.02f;
    commands[11].y = 1.0f;
    commands[11].z = 1.02f;
    commands[12].layer = 4;
    commands[12].flags = 9;
    commands[12].tex = &resourceData[offsetof(Dll91EffectResourceView, firstNineVertexIndices)];
    commands[12].mode = 0x8;
    commands[12].x = 255.0f;
    commands[12].y = 0.0f;
    commands[12].z = 255.0f;
    commands[13].layer = 4;
    commands[13].flags = 0x12;
    commands[13].tex = &resourceData[offsetof(Dll91EffectResourceView, allVertexIndices)];
    commands[13].mode = 0x100;
    commands[13].x = 0.0f;
    commands[13].y = 0.0f;
    commands[13].z = -10.0f;
    commands[14].layer = 4;
    commands[14].flags = 5;
    commands[14].tex = &resourceData[offsetof(Dll91EffectResourceView, oddVertexIndices)];
    commands[14].mode = 0x2;
    commands[14].x = 1.02f;
    commands[14].y = 1.0f;
    commands[14].z = 1.02f;
    commands[15].layer = 4;
    commands[15].flags = 4;
    commands[15].tex = gDll91VertexIndices;
    commands[15].mode = 0x2;
    commands[15].x = 0.98f;
    commands[15].y = 1.0f;
    commands[15].z = 0.98f;
    commands[16].layer = 5;
    commands[16].flags = 2;
    commands[16].tex = NULL;
    commands[16].mode = 0x1000;
    commands[16].x = 1.0f;
    commands[16].y = 0.0f;
    commands[16].z = 0.0f;
    commands[17].layer = 6;
    commands[17].flags = 0x12;
    commands[17].tex = &resourceData[offsetof(Dll91EffectResourceView, allVertexIndices)];
    commands[17].mode = 0x4;
    commands[17].x = 0.0f;
    commands[17].y = 0.0f;
    commands[17].z = 0.0f;
    commands[18].layer = 6;
    commands[18].flags = 0x12;
    commands[18].tex = &resourceData[offsetof(Dll91EffectResourceView, allVertexIndices)];
    commands[18].mode = 0x2;
    commands[18].x = 2.0f;
    commands[18].y = 1.0f;
    commands[18].z = 2.0f;
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
    packet.drawGroupCount = 1;
    packet.drawGroupStride = 0;
    packet.initialStateByte = 0x12;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0xC;
    packet.flags = 0x1000082;
    packet.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd) * 19) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll91EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll91EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll91EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll91EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll91EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll91EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll91EffectResourceView, sequenceParams[6])];
    packet.commands = commands;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if ((u32)sourceObj != 0) {
            GameObject* anchorObj = sourceObj;
            packet.position[0] += anchorObj->anim.worldPosX;
            packet.position[1] += anchorObj->anim.worldPosY;
            packet.position[2] += anchorObj->anim.worldPosZ;
        } else {
            PartFxSpawnParams* anchorParams = spawnParams;
            packet.position[0] += anchorParams->posX;
            packet.position[1] += anchorParams->posY;
            packet.position[2] += anchorParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 0x12, (u8*)(int)gDll91EffectResourceData, 0x10,
                      &resourceData[offsetof(Dll91EffectResourceView, triangles)], 0x45, 0);
}

void dll_91_release(void) {
}

void dll_91_initialise(void) {
}

u8 gDll91EffectResourceData[sizeof(Dll91EffectResourceView)] = {
    3,   232, 0,   0,   0,   0,   0,   0,   0,   0,   2,   195, 0, 0,   253, 61,  0,   15,  0,   0,   0,   0,   0, 0,
    252, 24,  0,   31,  0,   0,   253, 61,  0,   0,   253, 61,  0, 47,  0,   0,   252, 24,  0,   0,   0,   0,   0, 63,
    0,   0,   253, 61,  0,   0,   2,   195, 0,   79,  0,   0,   0, 0,   0,   0,   3,   232, 0,   95,  0,   0,   2, 195,
    0,   0,   2,   195, 0,   111, 0,   0,   3,   232, 0,   0,   0, 0,   0,   127, 0,   0,   3,   232, 7,   208, 0, 0,
    0,   0,   0,   31,  2,   195, 7,   208, 253, 61,  0,   15,  0, 31,  0,   0,   7,   208, 252, 24,  0,   31,  0, 31,
    253, 61,  7,   208, 253, 61,  0,   47,  0,   31,  252, 24,  7, 208, 0,   0,   0,   63,  0,   31,  253, 61,  7, 208,
    2,   195, 0,   79,  0,   31,  0,   0,   7,   208, 3,   232, 0, 95,  0,   31,  2,   195, 7,   208, 2,   195, 0, 111,
    0,   31,  3,   232, 7,   208, 0,   0,   0,   127, 0,   31,  0, 0,   0,   1,   0,   10,  0,   0,   0,   10,  0, 9,
    0,   1,   0,   2,   0,   11,  0,   1,   0,   11,  0,   10,  0, 2,   0,   3,   0,   12,  0,   2,   0,   12,  0, 11,
    0,   3,   0,   4,   0,   13,  0,   3,   0,   13,  0,   12,  0, 4,   0,   5,   0,   14,  0,   4,   0,   14,  0, 13,
    0,   5,   0,   6,   0,   15,  0,   5,   0,   15,  0,   14,  0, 6,   0,   7,   0,   16,  0,   6,   0,   16,  0, 15,
    0,   7,   0,   8,   0,   17,  0,   7,   0,   17,  0,   16,  0, 0,   0,   1,   0,   2,   0,   3,   0,   4,   0, 5,
    0,   6,   0,   7,   0,   8,   0,   0,   0,   9,   0,   10,  0, 11,  0,   12,  0,   13,  0,   14,  0,   15,  0, 16,
    0,   17,  0,   0,   0,   18,  0,   19,  0,   20,  0,   21,  0, 22,  0,   23,  0,   24,  0,   25,  0,   26,  0, 0,
    0,   0,   0,   1,   0,   2,   0,   3,   0,   4,   0,   5,   0, 6,   0,   7,   0,   8,   0,   9,   0,   10,  0, 11,
    0,   12,  0,   13,  0,   14,  0,   15,  0,   16,  0,   17,  0, 0,   0,   2,   0,   4,   0,   6,   0,   8,   0, 10,
    0,   12,  0,   14,  0,   16,  0,   0,   0,   9,   0,   11,  0, 13,  0,   15,  0,   17,  0,   0,   0,   0,   0, 45,
    0,   0,   0,   18,  0,   18,  0,   0,   0,   30,  0,   0,
};
Dll91ResourceDescriptor gDll91ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    dll_91_initialise,
    dll_91_release,
    NULL,
    dll_91_spawnEffect,
    0x00000000,
};
