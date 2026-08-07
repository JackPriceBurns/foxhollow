/*
 * DLL 112 / 0x70 - a modgfx effect spawner.
 */
#include "main/dll/dll_0070_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll70EffectResourceView {
    ModgfxEffectVertex vertices[18];
    s16 triangles[16][3];
    s16 firstNineVertexIndices[10];
    s16 secondNineVertexIndices[10];
    s16 thirdNineVertexIndices[10];
    s16 allVertexIndices[18];
    s16 evenVertexIndices[10];
    s16 oddVertexIndices[6];
    s16 sequenceParams[7];
    u8 pad1A2[2];
} Dll70EffectResourceView;

STATIC_ASSERT(offsetof(Dll70EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll70EffectResourceView, triangles) == 0x0B4);
STATIC_ASSERT(offsetof(Dll70EffectResourceView, firstNineVertexIndices) == 0x114);
STATIC_ASSERT(offsetof(Dll70EffectResourceView, secondNineVertexIndices) == 0x128);
STATIC_ASSERT(offsetof(Dll70EffectResourceView, thirdNineVertexIndices) == 0x13C);
STATIC_ASSERT(offsetof(Dll70EffectResourceView, allVertexIndices) == 0x150);
STATIC_ASSERT(offsetof(Dll70EffectResourceView, evenVertexIndices) == 0x174);
STATIC_ASSERT(offsetof(Dll70EffectResourceView, oddVertexIndices) == 0x188);
STATIC_ASSERT(offsetof(Dll70EffectResourceView, sequenceParams) == 0x194);
STATIC_ASSERT(sizeof(Dll70EffectResourceView) == 0x1A4);

s16 gDll70FourVertexIndices[4] = {10, 12, 14, 16};

u32 gDll70EffectResourceData[sizeof(Dll70EffectResourceView) / sizeof(u32)] = {
    0x03e80000, 0x00000000, 0x000002c3, 0x0000fd3d, 0x000f0000, 0x00000000, 0xfc18001f, 0x0000fd3d, 0x0000fd3d,
    0x002f0000, 0xfc180000, 0x0000003f, 0x0000fd3d, 0x000002c3, 0x004f0000, 0x00000000, 0x03e8005f, 0x000002c3,
    0x000002c3, 0x006f0000, 0x03e80000, 0x0000007f, 0x000003e8, 0x07d00000, 0x0000001f, 0x02c307d0, 0xfd3d000f,
    0x001f0000, 0x07d0fc18, 0x001f001f, 0xfd3d07d0, 0xfd3d002f, 0x001ffc18, 0x07d00000, 0x003f001f, 0xfd3d07d0,
    0x02c3004f, 0x001f0000, 0x07d003e8, 0x005f001f, 0x02c307d0, 0x02c3006f, 0x001f03e8, 0x07d00000, 0x007f001f,
    0x00000001, 0x000a0000, 0x000a0009, 0x00010002, 0x000b0001, 0x000b000a, 0x00020003, 0x000c0002, 0x000c000b,
    0x00030004, 0x000d0003, 0x000d000c, 0x00040005, 0x000e0004, 0x000e000d, 0x00050006, 0x000f0005, 0x000f000e,
    0x00060007, 0x00100006, 0x0010000f, 0x00070008, 0x00110007, 0x00110010, 0x00000001, 0x00020003, 0x00040005,
    0x00060007, 0x00080000, 0x0009000a, 0x000b000c, 0x000d000e, 0x000f0010, 0x00110000, 0x00120013, 0x00140015,
    0x00160017, 0x00180019, 0x001a0000, 0x00000001, 0x00020003, 0x00040005, 0x00060007, 0x00080009, 0x000a000b,
    0x000c000d, 0x000e000f, 0x00100011, 0x00000002, 0x00040006, 0x0008000a, 0x000c000e, 0x00100000, 0x0009000b,
    0x000d000f, 0x00110000, 0x0000002d, 0x00000012, 0x00120000, 0x001e0000,
};

void dll_70_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll70EffectResourceData;
    GfxCmd* commands = packet.entries;

    commands[0].layer = 0;
    commands[0].flags = 0x12;
    commands[0].tex = &resourceData[offsetof(Dll70EffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    commands[1].layer = 0;
    commands[1].flags = 9;
    commands[1].tex = &resourceData[offsetof(Dll70EffectResourceView, firstNineVertexIndices)];
    commands[1].mode = 8;
    commands[1].x = 255.0f;
    commands[1].y = 255.0f;
    commands[1].z = 0.0f;
    commands[2].layer = 0;
    commands[2].flags = 9;
    commands[2].tex = &resourceData[offsetof(Dll70EffectResourceView, secondNineVertexIndices)];
    commands[2].mode = 2;
    commands[2].x = 1.0f;
    commands[2].y = 0.01f;
    commands[2].z = 1.0f;
    commands[3].layer = 0;
    commands[3].flags = 0x12;
    commands[3].tex = &resourceData[offsetof(Dll70EffectResourceView, allVertexIndices)];
    commands[3].mode = 2;
    commands[3].x = 3.5f;
    commands[3].y = 1.0f;
    commands[3].z = 3.5f;
    commands[4].layer = 0;
    commands[4].flags = 9;
    commands[4].tex = &resourceData[offsetof(Dll70EffectResourceView, secondNineVertexIndices)];
    commands[4].mode = 8;
    commands[4].x = 205.0f;
    commands[4].y = 0.0f;
    commands[4].z = 0.0f;
    commands[5].layer = 0;
    commands[5].flags = 1;
    commands[5].tex = NULL;
    commands[5].mode = 0x8000;
    commands[5].x = 255.0f;
    commands[5].y = 125.0f;
    commands[5].z = 0.0f;
    commands[6].layer = 0;
    commands[6].flags = 0;
    commands[6].tex = NULL;
    commands[6].mode = 0x80000;
    commands[6].x = 0.0f;
    commands[6].y = 10.0f;
    commands[6].z = 0.0f;
    commands[7].layer = 1;
    commands[7].flags = 0x12;
    commands[7].tex = &resourceData[offsetof(Dll70EffectResourceView, allVertexIndices)];
    commands[7].mode = 4;
    commands[7].x = 255.0f;
    commands[7].y = 0.0f;
    commands[7].z = 0.0f;
    commands[8].layer = 1;
    commands[8].flags = 9;
    commands[8].tex = &resourceData[offsetof(Dll70EffectResourceView, secondNineVertexIndices)];
    commands[8].mode = 2;
    commands[8].x = 1.0f;
    commands[8].y = 150.0f;
    commands[8].z = 1.0f;
    commands[9].layer = 1;
    commands[9].flags = 0x7a;
    commands[9].tex = NULL;
    commands[9].mode = 0x10000;
    commands[9].x = 0.0f;
    commands[9].y = 0.0f;
    commands[9].z = 0.0f;
    commands[10].layer = 1;
    commands[10].flags = 0;
    commands[10].tex = NULL;
    commands[10].mode = 0x80000;
    commands[10].x = 0.0f;
    commands[10].y = 10.0f;
    commands[10].z = 0.0f;
    commands[11].layer = 2;
    commands[11].flags = 0x9d;
    commands[11].tex = NULL;
    commands[11].mode = 0x20000;
    commands[11].x = 0.0f;
    commands[11].y = 0.0f;
    commands[11].z = 0.0f;
    commands[12].layer = 3;
    commands[12].flags = 9;
    commands[12].tex = &resourceData[offsetof(Dll70EffectResourceView, firstNineVertexIndices)];
    commands[12].mode = 8;
    commands[12].x = 255.0f;
    commands[12].y = 155.0f;
    commands[12].z = 0.0f;
    commands[13].layer = 3;
    commands[13].flags = 0x12;
    commands[13].tex = &resourceData[offsetof(Dll70EffectResourceView, allVertexIndices)];
    commands[13].mode = 0x100;
    commands[13].x = 0.0f;
    commands[13].y = 0.0f;
    commands[13].z = -10.0f;
    commands[14].layer = 3;
    commands[14].flags = 5;
    commands[14].tex = &resourceData[offsetof(Dll70EffectResourceView, oddVertexIndices)];
    commands[14].mode = 2;
    commands[14].x = 0.98f;
    commands[14].y = 1.0f;
    commands[14].z = 0.98f;
    commands[15].layer = 3;
    commands[15].flags = 4;
    commands[15].tex = gDll70FourVertexIndices;
    commands[15].mode = 2;
    commands[15].x = 1.02f;
    commands[15].y = 1.0f;
    commands[15].z = 1.02f;
    commands[16].layer = 3;
    commands[16].flags = 0;
    commands[16].tex = NULL;
    commands[16].mode = 0x80000;
    commands[16].x = 0.0f;
    commands[16].y = -30.0f;
    commands[16].z = 0.0f;
    commands[17].layer = 4;
    commands[17].flags = 9;
    commands[17].tex = &resourceData[offsetof(Dll70EffectResourceView, firstNineVertexIndices)];
    commands[17].mode = 8;
    commands[17].x = 255.0f;
    commands[17].y = 255.0f;
    commands[17].z = 0.0f;
    commands[18].layer = 4;
    commands[18].flags = 0x12;
    commands[18].tex = &resourceData[offsetof(Dll70EffectResourceView, allVertexIndices)];
    commands[18].mode = 0x100;
    commands[18].x = 0.0f;
    commands[18].y = 0.0f;
    commands[18].z = -10.0f;
    commands[19].layer = 4;
    commands[19].flags = 5;
    commands[19].tex = &resourceData[offsetof(Dll70EffectResourceView, oddVertexIndices)];
    commands[19].mode = 2;
    commands[19].x = 1.02f;
    commands[19].y = 1.0f;
    commands[19].z = 1.02f;
    commands[20].layer = 4;
    commands[20].flags = 4;
    commands[20].tex = gDll70FourVertexIndices;
    commands[20].mode = 2;
    commands[20].x = 0.98f;
    commands[20].y = 1.0f;
    commands[20].z = 0.98f;
    commands[21].layer = 5;
    commands[21].flags = 2;
    commands[21].tex = NULL;
    commands[21].mode = 0x1000;
    commands[21].x = 1.0f;
    commands[21].y = 0.0f;
    commands[21].z = 0.0f;
    commands[22].layer = 6;
    commands[22].flags = 0x9d;
    commands[22].tex = NULL;
    commands[22].mode = 0x20000;
    commands[22].x = 0.0f;
    commands[22].y = 0.0f;
    commands[22].z = 0.0f;
    commands[23].layer = 6;
    commands[23].flags = 0x9b;
    commands[23].tex = NULL;
    commands[23].mode = 0x10000;
    commands[23].x = 0.0f;
    commands[23].y = 0.0f;
    commands[23].z = 0.0f;
    commands[24].layer = 6;
    commands[24].flags = 0x12;
    commands[24].tex = &resourceData[offsetof(Dll70EffectResourceView, allVertexIndices)];
    commands[24].mode = 4;
    commands[24].x = 0.0f;
    commands[24].y = 0.0f;
    commands[24].z = 0.0f;
    commands[25].layer = 6;
    commands[25].flags = 0x12;
    commands[25].tex = &resourceData[offsetof(Dll70EffectResourceView, allVertexIndices)];
    commands[25].mode = 2;
    commands[25].x = 2.0f;
    commands[25].y = 1.0f;
    commands[25].z = 2.0f;
    commands[26].layer = 6;
    commands[26].flags = 0;
    commands[26].tex = NULL;
    commands[26].mode = 0x80000;
    commands[26].x = 0.0f;
    commands[26].y = -30.0f;
    commands[26].z = 0.0f;
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
    packet.textureFrameTimer = 0xc;
    packet.flags = 0x1000082;
    packet.commandCount = 27;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll70EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll70EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll70EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll70EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll70EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll70EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll70EffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet.velocity[0] + 0x40);
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
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
        ->spawnEffect(&packet, 0, 0x12, (u8*)(int)gDll70EffectResourceData, 0x10,
                      &resourceData[offsetof(Dll70EffectResourceView, triangles)], 0x45, 0);
}

void dll_70_release(void) {
}

void dll_70_initialise(void) {
}

Dll70ResourceDescriptor gDll70ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_70_initialise, dll_70_release, NULL, dll_70_spawnEffect, 0,
};
