/*
 * DLL 138 / 0x8A - a single-command modgfx effect spawner.
 */
#include "main/dll/dll_008A_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll8AEffectResourceView {
    ModgfxEffectVertex vertices[8];
    s16 triangles[12][3];
    s16 allVertexIndices[8];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll8AEffectResourceView;

STATIC_ASSERT(offsetof(Dll8AEffectResourceView, vertices) == 0x00);
STATIC_ASSERT(offsetof(Dll8AEffectResourceView, triangles) == 0x50);
STATIC_ASSERT(offsetof(Dll8AEffectResourceView, allVertexIndices) == 0x98);
STATIC_ASSERT(offsetof(Dll8AEffectResourceView, sequenceParams) == 0xA8);
STATIC_ASSERT(offsetof(Dll8AEffectResourceView, opaqueTail) == 0xB6);
STATIC_ASSERT(sizeof(Dll8AEffectResourceView) == 0xB8);

u8 gDll8AEffectResourceData[sizeof(Dll8AEffectResourceView)] = {
    254, 12,  254, 12, 254, 12,  0,   0,   0,   0,   1,  244, 254, 12, 254, 12,  0,  32,  0,  32,  1,   244, 254,
    12,  1,   244, 0,  0,   0,   0,   254, 12,  254, 12, 1,   244, 0,  32,  0,   32, 254, 12, 1,   244, 254, 12,
    0,   0,   0,   0,  1,   244, 1,   244, 254, 12,  0,  32,  0,   32, 1,   244, 1,  244, 1,  244, 0,   0,   0,
    0,   254, 12,  1,  244, 1,   244, 0,   32,  0,   32, 0,   0,   0,  4,   0,   5,  0,   0,  0,   5,   0,   1,
    0,   1,   0,   5,  0,   6,   0,   1,   0,   6,   0,  2,   0,   2,  0,   6,   0,  7,   0,  2,   0,   7,   0,
    3,   0,   3,   0,  7,   0,   4,   0,   3,   0,   4,  0,   0,   0,  0,   0,   1,  0,   2,  0,   0,   0,   2,
    0,   3,   0,   4,  0,   7,   0,   6,   0,   4,   0,  6,   0,   5,  0,   0,   0,  1,   0,  2,   0,   3,   0,
    4,   0,   5,   0,  6,   0,   7,   0,   0,   0,   10, 0,   0,   0,  0,   0,   0,  0,   0,  0,   0,   0,   0};

void dll_8A_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll8AEffectResourceData;
    GfxCmd* commands = packet.entries;

    commands[0].layer = 0;
    commands[0].flags = 8;
    commands[0].tex = &resourceData[offsetof(Dll8AEffectResourceView, allVertexIndices)];
    commands[0].mode = 2;
    commands[0].x = 0.5f;
    commands[0].y = 0.5f;
    commands[0].z = 0.5f;
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
    packet.initialStateByte = 8;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0x10;
    packet.flags = 0x2000492;
    packet.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd)) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll8AEffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll8AEffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll8AEffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll8AEffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll8AEffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll8AEffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll8AEffectResourceView, sequenceParams[6])];
    packet.commands = commands;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (sourceObj != NULL) {
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
        ->spawnEffect(&packet, 0, 8, (u8*)(int)gDll8AEffectResourceData, 0xC,
                      &resourceData[offsetof(Dll8AEffectResourceView, triangles)], 0x1FD, 0);
}

void dll_8A_release(void) {
}

void dll_8A_initialise(void) {
}

Dll8AResourceDescriptor gDll8AResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_8A_initialise, dll_8A_release, NULL, dll_8A_spawnEffect,
};
