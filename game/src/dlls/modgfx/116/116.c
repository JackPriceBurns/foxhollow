/*
 * DLL 116 / 0x74 - a two-variant modgfx effect spawner.
 */
#include "main/dll/dll_0074_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll74EffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 padD2[2];
    s16 triangles[24][3];
    s16 firstSevenVertexIndices[8];
    s16 secondSevenVertexIndices[8];
    s16 thirdSevenVertexIndices[8];
    s16 firstAndThirdVertexIndices[14];
    s16 allVertexIndices[22];
    s16 lastFourteenVertexIndices[14];
    s16 firstFourteenVertexIndices[14];
    s16 sequenceParams[7];
    u8 pad222[2];
} Dll74EffectResourceView;

STATIC_ASSERT(offsetof(Dll74EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll74EffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll74EffectResourceView, firstSevenVertexIndices) == 0x164);
STATIC_ASSERT(offsetof(Dll74EffectResourceView, secondSevenVertexIndices) == 0x174);
STATIC_ASSERT(offsetof(Dll74EffectResourceView, thirdSevenVertexIndices) == 0x184);
STATIC_ASSERT(offsetof(Dll74EffectResourceView, firstAndThirdVertexIndices) == 0x194);
STATIC_ASSERT(offsetof(Dll74EffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll74EffectResourceView, lastFourteenVertexIndices) == 0x1DC);
STATIC_ASSERT(offsetof(Dll74EffectResourceView, firstFourteenVertexIndices) == 0x1F8);
STATIC_ASSERT(offsetof(Dll74EffectResourceView, sequenceParams) == 0x214);
STATIC_ASSERT(sizeof(Dll74EffectResourceView) == 0x224);

u32 gDll74EffectResourceData[sizeof(Dll74EffectResourceView) / sizeof(u32)] = {
    0x00000000, 0x03e80000, 0x00000362, 0x000001f4, 0x000b0000, 0x03620000, 0xfe0c0016, 0x00000000, 0x0000fc18,
    0x00200000, 0xfc9e0000, 0xfe0c0016, 0x0000fc9e, 0x000001f4, 0x000b0000, 0x00000000, 0x03e80000, 0x00000000,
    0x157c03e8, 0x0000003b, 0x0362157c, 0x01f4000b, 0x003b0362, 0x157cfe0c, 0x0016003b, 0x0000157c, 0xfc180020,
    0x003bfc9e, 0x157cfe0c, 0x0016003b, 0xfc9e157c, 0x01f4000b, 0x003b0000, 0x157c03e8, 0x0000003b, 0x00001770,
    0x03e80000, 0x003f0362, 0x177001f4, 0x000b003f, 0x03621770, 0xfe0c0016, 0x003f0000, 0x1770fc18, 0x0020003f,
    0xfc9e1770, 0xfe0c0016, 0x003ffc9e, 0x177001f4, 0x000b003f, 0x00001770, 0x03e80000, 0x003f0000, 0x00000001,
    0x00080000, 0x00080007, 0x00010002, 0x00090001, 0x00090008, 0x00020003, 0x000a0002, 0x000a0009, 0x00030004,
    0x000b0003, 0x000b000a, 0x00040005, 0x000c0004, 0x000c000b, 0x00050006, 0x000d0005, 0x000d000c, 0x00070008,
    0x000f0007, 0x000f000e, 0x00080009, 0x00100008, 0x0010000f, 0x0009000a, 0x00110009, 0x00110010, 0x000a000b,
    0x0012000a, 0x00120011, 0x000b000c, 0x0013000b, 0x00130012, 0x000c000d, 0x0014000c, 0x00140013, 0x00000001,
    0x00020003, 0x00040005, 0x00060000, 0x00070008, 0x0009000a, 0x000b000c, 0x000d0000, 0x000e000f, 0x00100011,
    0x00120013, 0x00140000, 0x00000001, 0x00020003, 0x00040005, 0x0006000e, 0x000f0010, 0x00110012, 0x00130014,
    0x00000001, 0x00020003, 0x00040005, 0x00060007, 0x00080009, 0x000a000b, 0x000c000d, 0x000e000f, 0x00100011,
    0x00120013, 0x00140000, 0x00070008, 0x0009000a, 0x000b000c, 0x000d000e, 0x000f0010, 0x00110012, 0x00130014,
    0x00000001, 0x00020003, 0x00040005, 0x00060007, 0x00080009, 0x000a000b, 0x000c000d, 0x0000000a, 0x00b40028,
    0x00000000, 0x00000000,
};

void dll_74_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    GfxCmd* commandCursor;
    u8* resourceData = (u8*)(int)gDll74EffectResourceData;
    GfxCmd* commands = packet.entries;
    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(Dll74EffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    if (variant == 0) {
        commands[1].layer = 0;
        commands[1].flags = 0x15;
        commands[1].tex = &resourceData[offsetof(Dll74EffectResourceView, allVertexIndices)];
        commands[1].mode = 2;
        commands[1].x = 0.01f;
        commands[1].y = 1.0f;
        commands[1].z = 0.01f;
        commandCursor = &commands[2];
    } else {
        commands[1].layer = 0;
        commands[1].flags = 0x15;
        commands[1].tex = &resourceData[offsetof(Dll74EffectResourceView, allVertexIndices)];
        commands[1].mode = 2;
        commands[1].x = 0.01f;
        commands[1].y = 3.0f;
        commands[1].z = 0.01f;
        commandCursor = &commands[2];
    }
    if (variant == 0) {
        commandCursor->layer = 0;
        commandCursor->flags = 0;
        commandCursor->tex = NULL;
        commandCursor->mode = 0x400000;
        commandCursor->x = 0.0f;
        commandCursor->y = -90.0f;
        commandCursor->z = 0.0f;
        commandCursor++;
    } else {
        commandCursor->layer = 0;
        commandCursor->flags = 0;
        commandCursor->tex = NULL;
        commandCursor->mode = 0x400000;
        commandCursor->x = 0.0f;
        commandCursor->y = -290.0f;
        commandCursor->z = 0.0f;
        commandCursor++;
    }
    commandCursor[0].layer = 1;
    commandCursor[0].flags = 0x15;
    commandCursor[0].tex = &resourceData[offsetof(Dll74EffectResourceView, allVertexIndices)];
    commandCursor[0].mode = 2;
    commandCursor[0].x = 70.0f;
    commandCursor[0].y = 1.5f;
    commandCursor[0].z = 70.0f;
    commandCursor[1].layer = 1;
    commandCursor[1].flags = 0xe;
    commandCursor[1].tex = &resourceData[offsetof(Dll74EffectResourceView, firstFourteenVertexIndices)];
    commandCursor[1].mode = 4;
    commandCursor[1].x = 255.0f;
    commandCursor[1].y = 0.0f;
    commandCursor[1].z = 0.0f;
    if (variant == 0) {
        commandCursor[2].layer = 1;
        commandCursor[2].flags = 0x15;
        commandCursor[2].tex = &resourceData[offsetof(Dll74EffectResourceView, allVertexIndices)];
        commandCursor[2].mode = 0x4000;
        commandCursor[2].x = 0.0f;
        commandCursor[2].y = 4.0f;
        commandCursor[2].z = 0.0f;
        commandCursor += 3;
    } else {
        commandCursor[2].layer = 1;
        commandCursor[2].flags = 0x15;
        commandCursor[2].tex = &resourceData[offsetof(Dll74EffectResourceView, allVertexIndices)];
        commandCursor[2].mode = 0x4000;
        commandCursor[2].x = 0.0f;
        commandCursor[2].y = -4.0f;
        commandCursor[2].z = 0.0f;
        commandCursor += 3;
    }
    commandCursor[0].layer = 2;
    commandCursor[0].flags = 7;
    commandCursor[0].tex = &resourceData[offsetof(Dll74EffectResourceView, firstSevenVertexIndices)];
    commandCursor[0].mode = 2;
    commandCursor[0].x = 17.0f;
    commandCursor[0].y = 1.0f;
    commandCursor[0].z = 17.0f;
    commandCursor[1].layer = 2;
    commandCursor[1].flags = 7;
    commandCursor[1].tex = &resourceData[offsetof(Dll74EffectResourceView, secondSevenVertexIndices)];
    commandCursor[1].mode = 2;
    commandCursor[1].x = 1.5f;
    commandCursor[1].y = 1.0f;
    commandCursor[1].z = 1.5f;
    if (variant == 0) {
        commandCursor[2].layer = 2;
        commandCursor[2].flags = 0x15;
        commandCursor[2].tex = &resourceData[offsetof(Dll74EffectResourceView, allVertexIndices)];
        commandCursor[2].mode = 0x4000;
        commandCursor[2].x = 0.0f;
        commandCursor[2].y = 4.0f;
        commandCursor[2].z = 0.0f;
        commandCursor += 3;
    } else {
        commandCursor[2].layer = 2;
        commandCursor[2].flags = 0x15;
        commandCursor[2].tex = &resourceData[offsetof(Dll74EffectResourceView, allVertexIndices)];
        commandCursor[2].mode = 0x4000;
        commandCursor[2].x = 0.0f;
        commandCursor[2].y = -4.0f;
        commandCursor[2].z = 0.0f;
        commandCursor += 3;
    }
    commandCursor[0].layer = 2;
    commandCursor[0].flags = 0xe;
    commandCursor[0].tex = &resourceData[offsetof(Dll74EffectResourceView, firstFourteenVertexIndices)];
    commandCursor[0].mode = 4;
    commandCursor[0].x = 0.0f;
    commandCursor[0].y = 0.0f;
    commandCursor[0].z = 0.0f;
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
    packet.commandCount = (commandCursor + 1) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll74EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll74EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll74EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll74EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll74EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll74EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll74EffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + 0x60);
    packet.flags = 0xc0104c0;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (sourceObj != NULL) {
            packet.position[0] += sourceObj->anim.localPosX;
            packet.position[1] += sourceObj->anim.localPosY;
            packet.position[2] += sourceObj->anim.localPosZ;
        } else {
            packet.position[0] += spawnParams->posX;
            packet.position[1] += spawnParams->posY;
            packet.position[2] += spawnParams->posZ;
        }
    }
    if (variant == 0) {
        (*gModgfxInterface)
            ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll74EffectResourceData, 0x18,
                          &resourceData[offsetof(Dll74EffectResourceView, triangles)], 0x2e, 0);
    } else {
        (*gModgfxInterface)
            ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll74EffectResourceData, 0x18,
                          &resourceData[offsetof(Dll74EffectResourceView, triangles)], 0xd9, 0);
    }
}

void dll_74_release(void) {
}

void dll_74_initialise(void) {
}

Dll74ResourceDescriptor gDll74ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_74_initialise, dll_74_release, NULL, dll_74_spawnEffect, 0,
};
