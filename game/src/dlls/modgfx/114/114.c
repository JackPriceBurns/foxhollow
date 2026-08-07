/*
 * DLL 114 / 0x72 - a modgfx effect spawner.
 */
#include "main/dll/dll_0072_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll72EffectResourceView {
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
} Dll72EffectResourceView;

STATIC_ASSERT(offsetof(Dll72EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll72EffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll72EffectResourceView, firstSevenVertexIndices) == 0x164);
STATIC_ASSERT(offsetof(Dll72EffectResourceView, secondSevenVertexIndices) == 0x174);
STATIC_ASSERT(offsetof(Dll72EffectResourceView, thirdSevenVertexIndices) == 0x184);
STATIC_ASSERT(offsetof(Dll72EffectResourceView, firstAndThirdVertexIndices) == 0x194);
STATIC_ASSERT(offsetof(Dll72EffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll72EffectResourceView, lastFourteenVertexIndices) == 0x1DC);
STATIC_ASSERT(offsetof(Dll72EffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(sizeof(Dll72EffectResourceView) == 0x208);

u32 gDll72EffectResourceData[sizeof(Dll72EffectResourceView) / sizeof(u32)] = {
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
    0x00000096, 0x012c0032, 0x00000000, 0x00000000,
};

void dll_72_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll72EffectResourceData;
    GfxCmd* commands = packet.entries;
    GameObject* context;

    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(Dll72EffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    commands[1].layer = 0;
    commands[1].flags = 0x15;
    commands[1].tex = &resourceData[offsetof(Dll72EffectResourceView, allVertexIndices)];
    commands[1].mode = 2;
    commands[1].x = 0.01f;
    commands[1].y = 2.0f;
    commands[1].z = 0.01f;
    commands[2].layer = 0;
    commands[2].flags = 0;
    commands[2].tex = NULL;
    commands[2].mode = 0x400000;
    commands[2].x = 0.0f;
    commands[2].y = 0.0f;
    commands[2].z = 0.0f;
    commands[3].layer = 1;
    commands[3].flags = 0x15;
    commands[3].tex = &resourceData[offsetof(Dll72EffectResourceView, allVertexIndices)];
    commands[3].mode = 2;
    commands[3].x = 300.0f;
    commands[3].y = 1.2f;
    commands[3].z = 300.0f;
    commands[4].layer = 1;
    commands[4].flags = 0xe;
    commands[4].tex = &resourceData[offsetof(Dll72EffectResourceView, lastFourteenVertexIndices)];
    commands[4].mode = 4;
    commands[4].x = 255.0f;
    commands[4].y = 0.0f;
    commands[4].z = 0.0f;
    commands[5].layer = 1;
    commands[5].flags = 0x15;
    commands[5].tex = &resourceData[offsetof(Dll72EffectResourceView, allVertexIndices)];
    commands[5].mode = 0x4000;
    commands[5].x = 2.0f;
    commands[5].y = 2.0f;
    commands[5].z = 0.0f;
    commands[6].layer = 1;
    commands[6].flags = 0;
    commands[6].tex = NULL;
    commands[6].mode = 0x100;
    commands[6].x = 0.0f;
    commands[6].y = 0.0f;
    commands[6].z = -150.0f;
    commands[7].layer = 2;
    commands[7].flags = 0x15;
    commands[7].tex = &resourceData[offsetof(Dll72EffectResourceView, allVertexIndices)];
    commands[7].mode = 0x4000;
    commands[7].x = 2.0f;
    commands[7].y = 2.0f;
    commands[7].z = 0.0f;
    commands[8].layer = 3;
    commands[8].flags = 0x15;
    commands[8].tex = &resourceData[offsetof(Dll72EffectResourceView, allVertexIndices)];
    commands[8].mode = 0x4000;
    commands[8].x = 2.0f;
    commands[8].y = 2.0f;
    commands[8].z = 0.0f;
    commands[9].layer = 3;
    commands[9].flags = 0xe;
    commands[9].tex = &resourceData[offsetof(Dll72EffectResourceView, lastFourteenVertexIndices)];
    commands[9].mode = 4;
    commands[9].x = 0.0f;
    commands[9].y = 0.0f;
    commands[9].z = 0.0f;
    packet.modeByte = 0;
    context = sourceObj;
    packet.sourceObj = context;
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
    packet.commandCount = (commands + 10) - packet.entries;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll72EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll72EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll72EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll72EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll72EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll72EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll72EffectResourceView, sequenceParams[6])];
    packet.commands = packet.entries;
    packet.flags = 0xc0100c0;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (context != NULL) {
            packet.position[0] += context->anim.worldPosX;
            packet.position[1] += context->anim.worldPosY;
            packet.position[2] += context->anim.worldPosZ;
        } else {
            packet.position[0] += spawnParams->posX;
            packet.position[1] += spawnParams->posY;
            packet.position[2] += spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll72EffectResourceData, 0x18,
                      &resourceData[offsetof(Dll72EffectResourceView, triangles)], 0x154, 0);
}

void dll_72_release(void) {
}

void dll_72_initialise(void) {
}

Dll72ResourceDescriptor gDll72ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_72_initialise, dll_72_release, NULL, dll_72_spawnEffect,
};
