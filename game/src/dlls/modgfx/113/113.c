/*
 * DLL 113 / 0x71 - a modgfx effect spawner.
 */
#include "main/dll/dll_0071_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll71EffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 padD2[2];
    s16 triangles[24][3];
    s16 firstSevenVertexIndices[8];
    s16 secondSevenVertexIndices[8];
    s16 thirdSevenVertexIndices[8];
    s16 firstAndThirdVertexIndices[14];
    s16 allVertexIndices[22];
    s16 lastFourteenVertexIndices[14];
    s16 sequenceParams[7];
    u8 pad206[2];
} Dll71EffectResourceView;

STATIC_ASSERT(offsetof(Dll71EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll71EffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll71EffectResourceView, firstSevenVertexIndices) == 0x164);
STATIC_ASSERT(offsetof(Dll71EffectResourceView, secondSevenVertexIndices) == 0x174);
STATIC_ASSERT(offsetof(Dll71EffectResourceView, thirdSevenVertexIndices) == 0x184);
STATIC_ASSERT(offsetof(Dll71EffectResourceView, firstAndThirdVertexIndices) == 0x194);
STATIC_ASSERT(offsetof(Dll71EffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll71EffectResourceView, lastFourteenVertexIndices) == 0x1DC);
STATIC_ASSERT(offsetof(Dll71EffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(sizeof(Dll71EffectResourceView) == 0x208);

u32 gDll71EffectResourceData[sizeof(Dll71EffectResourceView) / sizeof(u32)] = {
    0x00000000, 0x03e80000, 0x00000362, 0x000001f4, 0x000b0000, 0x03620000, 0xfe0c0016, 0x00000000, 0x0000fc18,
    0x00200000, 0xfc9e0000, 0xfe0c0016, 0x0000fc9e, 0x000001f4, 0x000b0000, 0x00000000, 0x03e80000, 0x00000000,
    0x01f403e8, 0x0000000f, 0x036201f4, 0x01f4000b, 0x000f0362, 0x01f4fe0c, 0x0016000f, 0x000001f4, 0xfc180020,
    0x000ffc9e, 0x01f4fe0c, 0x0016000f, 0xfc9e01f4, 0x01f4000b, 0x000f0000, 0x01f403e8, 0x0000000f, 0x00001770,
    0x03e80000, 0x007f0362, 0x177001f4, 0x000b007f, 0x03621770, 0xfe0c0016, 0x007f0000, 0x1770fc18, 0x0020007f,
    0xfc9e1770, 0xfe0c0016, 0x007ffc9e, 0x177001f4, 0x000b007f, 0x00001770, 0x03e80000, 0x007f0000, 0x00000001,
    0x00080000, 0x00080007, 0x00010002, 0x00090001, 0x00090008, 0x00020003, 0x000a0002, 0x000a0009, 0x00030004,
    0x000b0003, 0x000b000a, 0x00040005, 0x000c0004, 0x000c000b, 0x00050006, 0x000d0005, 0x000d000c, 0x00070008,
    0x000f0007, 0x000f000e, 0x00080009, 0x00100008, 0x0010000f, 0x0009000a, 0x00110009, 0x00110010, 0x000a000b,
    0x0012000a, 0x00120011, 0x000b000c, 0x0013000b, 0x00130012, 0x000c000d, 0x0014000c, 0x00140013, 0x00000001,
    0x00020003, 0x00040005, 0x00060000, 0x00070008, 0x0009000a, 0x000b000c, 0x000d0000, 0x000e000f, 0x00100011,
    0x00120013, 0x00140000, 0x00000001, 0x00020003, 0x00040005, 0x0006000e, 0x000f0010, 0x00110012, 0x00130014,
    0x00000001, 0x00020003, 0x00040005, 0x00060007, 0x00080009, 0x000a000b, 0x000c000d, 0x000e000f, 0x00100011,
    0x00120013, 0x00140000, 0x00070008, 0x0009000a, 0x000b000c, 0x000d000e, 0x000f0010, 0x00110012, 0x00130014,
    0x00000096, 0x00fa0032, 0x00010000, 0x00000000,
};

void dll_71_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll71EffectResourceData;
    GfxCmd* commands = packet.entries;
    GameObject* context;
    f32 originOffset = 0.0f;

    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(Dll71EffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = originOffset;
    commands[0].y = originOffset;
    commands[0].z = originOffset;
    commands[1].layer = 0;
    commands[1].flags = 0x15;
    commands[1].tex = &resourceData[offsetof(Dll71EffectResourceView, allVertexIndices)];
    commands[1].mode = 2;
    commands[1].x = 0.01f;
    commands[1].y = 2.0f;
    commands[1].z = 0.01f;
    commands[2].layer = 0;
    commands[2].flags = 0;
    commands[2].tex = NULL;
    commands[2].mode = 0x400000;
    commands[2].x = originOffset;
    commands[2].y = 100.0f;
    commands[2].z = originOffset;
    commands[3].layer = 0;
    commands[3].flags = 0x124;
    commands[3].tex = NULL;
    commands[3].mode = 0x20000;
    commands[3].x = originOffset;
    commands[3].y = originOffset;
    commands[3].z = originOffset;
    commands[4].layer = 1;
    commands[4].flags = 0x15;
    commands[4].tex = &resourceData[offsetof(Dll71EffectResourceView, allVertexIndices)];
    commands[4].mode = 2;
    commands[4].x = 200.0f;
    commands[4].y = 1.3f;
    commands[4].z = 200.0f;
    commands[5].layer = 1;
    commands[5].flags = 0xe;
    commands[5].tex = &resourceData[offsetof(Dll71EffectResourceView, lastFourteenVertexIndices)];
    commands[5].mode = 4;
    commands[5].x = 255.0f;
    commands[5].y = originOffset;
    commands[5].z = originOffset;
    commands[6].layer = 1;
    commands[6].flags = 0x15;
    commands[6].tex = &resourceData[offsetof(Dll71EffectResourceView, allVertexIndices)];
    commands[6].mode = 0x4000;
    commands[6].x = 2.0f;
    commands[6].y = 2.0f;
    commands[6].z = originOffset;
    commands[7].layer = 1;
    commands[7].flags = 0;
    commands[7].tex = NULL;
    commands[7].mode = 0x400000;
    commands[7].x = originOffset;
    commands[7].y = -100.0f;
    commands[7].z = originOffset;
    commands[8].layer = 2;
    commands[8].flags = 0x15;
    commands[8].tex = &resourceData[offsetof(Dll71EffectResourceView, allVertexIndices)];
    commands[8].mode = 0x4000;
    commands[8].x = 2.0f;
    commands[8].y = 2.0f;
    commands[8].z = originOffset;
    commands[9].layer = 3;
    commands[9].flags = 0x124;
    commands[9].tex = NULL;
    commands[9].mode = 0x20000;
    commands[9].x = originOffset;
    commands[9].y = originOffset;
    commands[9].z = originOffset;
    commands[10].layer = 3;
    commands[10].flags = 0xe;
    commands[10].tex = &resourceData[offsetof(Dll71EffectResourceView, lastFourteenVertexIndices)];
    commands[10].mode = 4;
    commands[10].x = originOffset;
    commands[10].y = originOffset;
    commands[10].z = originOffset;
    commands[11].layer = 3;
    commands[11].flags = 0x15;
    commands[11].tex = &resourceData[offsetof(Dll71EffectResourceView, allVertexIndices)];
    commands[11].mode = 0x4000;
    commands[11].x = 2.0f;
    commands[11].y = 2.0f;
    commands[11].z = originOffset;
    commands[12].layer = 3;
    commands[12].flags = 0x15;
    commands[12].tex = &resourceData[offsetof(Dll71EffectResourceView, allVertexIndices)];
    commands[12].mode = 2;
    commands[12].x = 0.01f;
    commands[12].y = 1.0f;
    commands[12].z = 0.01f;
    commands[13].layer = 3;
    commands[13].flags = 0;
    commands[13].tex = NULL;
    commands[13].mode = 0x400000;
    commands[13].x = originOffset;
    commands[13].y = 100.0f;
    commands[13].z = originOffset;
    packet.modeByte = 0;
    context = sourceObj;
    packet.sourceObj = context;
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
    packet.textureFrameTimer = 0x1e;
    packet.commandCount = (commands + 14) - packet.entries;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll71EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll71EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll71EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll71EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll71EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll71EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll71EffectResourceView, sequenceParams[6])];
    packet.commands = packet.entries;
    packet.flags = 0xc0100c0;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (context != NULL) {
            packet.position[0] = originOffset + context->anim.worldPosX;
            packet.position[1] = originOffset + context->anim.worldPosY;
            packet.position[2] = originOffset + context->anim.worldPosZ;
        } else {
            packet.position[0] = originOffset + spawnParams->posX;
            packet.position[1] = originOffset + spawnParams->posY;
            packet.position[2] = originOffset + spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll71EffectResourceData, 0x18,
                      &resourceData[offsetof(Dll71EffectResourceView, triangles)], 0x154, 0);
}

void dll_71_release(void) {
}

void dll_71_initialise(void) {
}

Dll71ResourceDescriptor gDll71ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_71_initialise, dll_71_release, NULL, dll_71_spawnEffect,
};
