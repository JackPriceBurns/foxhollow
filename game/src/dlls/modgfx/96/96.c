/*
 * DLL 96 / 0x60 - a modgfx effect spawner.
 */
#include "main/dll/dll_0060_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"
#include "main/vecmath.h"

typedef struct Dll60EffectResourceView {
    ModgfxEffectVertex vertices[14];
    s16 colors[12][3];
    s16 firstGroupIndices[8];
    s16 secondGroupIndices[8];
    s16 allVertexIndices[14];
    s16 tenVertexIndices[10];
    s16 sequenceParams[7];
    u8 pad132[2];
} Dll60EffectResourceView;

STATIC_ASSERT(offsetof(Dll60EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll60EffectResourceView, colors) == 0x08C);
STATIC_ASSERT(offsetof(Dll60EffectResourceView, firstGroupIndices) == 0x0D4);
STATIC_ASSERT(offsetof(Dll60EffectResourceView, secondGroupIndices) == 0x0E4);
STATIC_ASSERT(offsetof(Dll60EffectResourceView, allVertexIndices) == 0x0F4);
STATIC_ASSERT(offsetof(Dll60EffectResourceView, tenVertexIndices) == 0x110);
STATIC_ASSERT(offsetof(Dll60EffectResourceView, sequenceParams) == 0x124);
STATIC_ASSERT(sizeof(Dll60EffectResourceView) == 0x134);

u32 gDll60EffectResourceData[sizeof(Dll60EffectResourceView) / sizeof(u32)] = {
    0xf4480000, 0x00000000, 0x0000f768, 0x0000044c, 0x000b0000, 0xfc180000, 0x08980016, 0x00000000, 0x000009c4,
    0x00200000, 0x03e80000, 0x0898002a, 0x00000898, 0x0000044c, 0x00340000, 0x0bb80000, 0x0000003f, 0x0000f448,
    0x05dc0000, 0x0000001f, 0xf76805dc, 0x044c000b, 0x001ffc18, 0x05dc0898, 0x0016001f, 0x000005dc, 0x09c40020,
    0x001f03e8, 0x05dc0898, 0x002a001f, 0x089805dc, 0x044c0034, 0x001f0bb8, 0x05dc0000, 0x003f001f, 0x00000008,
    0x00070000, 0x00010008, 0x00010009, 0x00080001, 0x00020009, 0x0002000a, 0x00090002, 0x0003000a, 0x0003000b,
    0x000a0003, 0x0004000b, 0x0004000c, 0x000b0004, 0x0005000c, 0x0005000d, 0x000c0005, 0x0006000d, 0x00000001,
    0x00020003, 0x00040005, 0x00060000, 0x00070008, 0x0009000a, 0x000b000c, 0x000d0000, 0x00000001, 0x00020003,
    0x00040005, 0x00060007, 0x00080009, 0x000a000b, 0x000c000d, 0x00010002, 0x00030004, 0x00050008, 0x0009000a,
    0x000b000c, 0x00000032, 0x01900032, 0x00000000, 0x00000000,
};

void dll_60_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll60EffectResourceData;
    GfxCmd* commandCursor;
    GfxCmd* commands;
    f32 randomAngle;
    commands = packet.entries;
    commandCursor = commands;
    commandCursor = (GfxCmd*)((int)commandCursor | (int)commands);
    commandCursor[0].layer = 0;
    commandCursor[0].flags = 0xe;
    commandCursor[0].tex = &resourceData[offsetof(Dll60EffectResourceView, allVertexIndices)];
    commandCursor[0].mode = 4;
    commandCursor[0].x = 0.0f;
    commandCursor[0].y = 0.0f;
    commandCursor[0].z = 0.0f;
    commandCursor[1].layer = 0;
    commandCursor[1].flags = 0xe;
    commandCursor[1].tex = &resourceData[offsetof(Dll60EffectResourceView, allVertexIndices)];
    commandCursor[1].mode = 2;
    commandCursor[1].x = 0.1f;
    commandCursor[1].y = 0.1f;
    commandCursor[1].z = 0.1f;
    commandCursor[2].layer = 0;
    commandCursor[2].flags = 0xe;
    commandCursor[2].tex = &resourceData[offsetof(Dll60EffectResourceView, allVertexIndices)];
    commandCursor[2].mode = 8;
    commandCursor[2].x = 150.0f + (f32)randomGetRange(0, 0x69);
    commandCursor[2].y = 150.0f + (f32)randomGetRange(0, 0x69);
    commandCursor[2].z = 150.0f + (f32)randomGetRange(0, 0x69);
    commandCursor[3].layer = 0;
    commandCursor[3].flags = 0x7a;
    commandCursor[3].tex = NULL;
    commandCursor[3].mode = 0x10000;
    commandCursor[3].x = 0.0f;
    commandCursor[3].y = 0.0f;
    commandCursor[3].z = 0.0f;
    randomAngle = (f32)randomGetRange(0, 0xfffe);
    commandCursor[4].layer = 0;
    commandCursor[4].flags = 0;
    commandCursor[4].tex = NULL;
    commandCursor[4].mode = 0x80;
    commandCursor[4].x = 0.0f;
    commandCursor[4].y = 0.0f;
    commandCursor[4].z = randomAngle;
    commandCursor[5].layer = 1;
    commandCursor[5].flags = 0xa;
    commandCursor[5].tex = &resourceData[offsetof(Dll60EffectResourceView, tenVertexIndices)];
    commandCursor[5].mode = 4;
    commandCursor[5].x = 255.0f;
    commandCursor[5].y = 0.0f;
    commandCursor[5].z = 0.0f;
    commandCursor[6].layer = 1;
    commandCursor[6].flags = 0xe;
    commandCursor[6].tex = &resourceData[offsetof(Dll60EffectResourceView, allVertexIndices)];
    commandCursor[6].mode = 2;
    commandCursor[6].x = 5.0f;
    commandCursor[6].y = 5.0f;
    commandCursor[6].z = 5.0f;
    commandCursor[7].layer = 2;
    commandCursor[7].flags = 0xe;
    commandCursor[7].tex = &resourceData[offsetof(Dll60EffectResourceView, allVertexIndices)];
    commandCursor[7].mode = 0x4000;
    commandCursor[7].x = 0.5f;
    commandCursor[7].y = 0.0f;
    commandCursor[7].z = 0.0f;
    commandCursor[8].layer = 2;
    commandCursor[8].flags = 0xe;
    commandCursor[8].tex = &resourceData[offsetof(Dll60EffectResourceView, allVertexIndices)];
    commandCursor[8].mode = 0x4000;
    commandCursor[8].x = 0.5f;
    commandCursor[8].y = 0.0f;
    commandCursor[8].z = 0.0f;
    commandCursor[9].layer = 2;
    commandCursor[9].flags = 0x53;
    commandCursor[9].tex = NULL;
    commandCursor[9].mode = 0x800000;
    commandCursor[9].x = 1.0f;
    commandCursor[9].y = 0.0f;
    commandCursor[9].z = 0.0f;
    commandCursor[10].layer = 2;
    commandCursor[10].flags = 0x54;
    commandCursor[10].tex = NULL;
    commandCursor[10].mode = 0x1800000;
    commandCursor[10].x = 1.0f;
    commandCursor[10].y = 0.0f;
    commandCursor[10].z = 8.0f;
    commandCursor[11].layer = 2;
    commandCursor[11].flags = 0xa;
    commandCursor[11].tex = &resourceData[offsetof(Dll60EffectResourceView, tenVertexIndices)];
    commandCursor[11].mode = 4;
    commandCursor[11].x = 0.0f;
    commandCursor[11].y = 0.0f;
    commandCursor[11].z = 0.0f;
    commandCursor[12].layer = 2;
    commandCursor[12].flags = 0xe;
    commandCursor[12].tex = &resourceData[offsetof(Dll60EffectResourceView, allVertexIndices)];
    commandCursor[12].mode = 2;
    commandCursor[12].x = 5.0f;
    commandCursor[12].y = 5.0f;
    commandCursor[12].z = 5.0f;
    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = 0.0f;
    packet.position[1] = 5.0f;
    packet.position[2] = 0.0f;
    packet.velocity[0] = 0.0f;
    packet.velocity[1] = 0.0f;
    packet.velocity[2] = 0.0f;
    packet.scale = 1.0f;
    packet.drawGroupCount = 1;
    packet.drawGroupStride = 0;
    packet.initialStateByte = 0xe;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0x10;
    packet.commandCount = (commandCursor + 13) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll60EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll60EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll60EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll60EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll60EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll60EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll60EffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + offsetof(ModgfxSpawnPacket, entries));
    packet.flags = 0x1000000;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (packet.sourceObj != NULL) {
            packet.position[0] += packet.sourceObj->anim.worldPosX;
            packet.position[1] += packet.sourceObj->anim.worldPosY;
            packet.position[2] += packet.sourceObj->anim.worldPosZ;
        } else {
            packet.position[0] += spawnParams->posX;
            packet.position[1] += spawnParams->posY;
            packet.position[2] += spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 0xe, (u8*)(int)gDll60EffectResourceData, 0xc,
                      &resourceData[offsetof(Dll60EffectResourceView, colors)], 0x46, 0);
}

void dll_60_release(void) {
}

void dll_60_initialise(void) {
}

Dll60ResourceDescriptor gDll60ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_60_initialise, dll_60_release, NULL, dll_60_spawnEffect, 0,
};
