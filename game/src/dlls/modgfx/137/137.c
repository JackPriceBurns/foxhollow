/*
 * DLL 137 / 0x89 - a ten-command layered modgfx effect spawner.
 */
#include "main/dll/dll_0089_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll89EffectResourceView {
    ModgfxEffectVertex vertices[10];
    u8 opaque064[0x104];
    s16 triangles[8][3];
    s16 wrappedVertexIndices[10];
    s16 allVertexIndices[10];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll89EffectResourceView;

STATIC_ASSERT(offsetof(Dll89EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll89EffectResourceView, opaque064) == 0x064);
STATIC_ASSERT(offsetof(Dll89EffectResourceView, triangles) == 0x168);
STATIC_ASSERT(offsetof(Dll89EffectResourceView, wrappedVertexIndices) == 0x198);
STATIC_ASSERT(offsetof(Dll89EffectResourceView, allVertexIndices) == 0x1AC);
STATIC_ASSERT(offsetof(Dll89EffectResourceView, sequenceParams) == 0x1C0);
STATIC_ASSERT(offsetof(Dll89EffectResourceView, opaqueTail) == 0x1CE);
STATIC_ASSERT(sizeof(Dll89EffectResourceView) == 0x1D0);

u8 gDll89ZeroIndexData[8] = {0};

u8 gDll89EffectResourceData[sizeof(Dll89EffectResourceView)] = {
    0, 0,   248, 48,  0, 0,   0, 0,   0,   0,  3, 232, 3, 232, 0,   0,   0, 32,  0,   32, 2, 195, 3, 232, 253, 61,
    0, 0,   0,   32,  0, 0,   3, 232, 252, 24, 0, 32,  0, 32,  253, 61,  3, 232, 253, 61, 0, 0,   0, 32,  252, 24,
    3, 232, 0,   0,   0, 32,  0, 32,  253, 61, 3, 232, 2, 195, 0,   0,   0, 32,  0,   0,  3, 232, 3, 232, 0,   32,
    0, 32,  2,   195, 3, 232, 2, 195, 0,   0,  0, 32,  3, 232, 3,   232, 0, 0,   0,   32, 0, 32,  0, 0,   0,   0,
    0, 0,   0,   0,   0, 0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,
    0, 0,   0,   0,   0, 0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,
    0, 0,   0,   0,   0, 0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,
    0, 0,   0,   0,   0, 0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,
    0, 0,   0,   0,   0, 0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,
    0, 0,   0,   0,   0, 0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,
    0, 0,   0,   0,   0, 0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,
    0, 0,   0,   0,   0, 0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,
    0, 0,   0,   0,   0, 0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,
    0, 0,   0,   0,   0, 0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   0,   0, 0,   0,   0,  0, 0,   0, 0,   0,   1,
    0, 2,   0,   0,   0, 2,   0, 3,   0,   0,  0, 3,   0, 4,   0,   0,   0, 4,   0,   5,  0, 0,   0, 5,   0,   6,
    0, 0,   0,   6,   0, 7,   0, 0,   0,   7,  0, 8,   0, 0,   0,   8,   0, 9,   0,   1,  0, 2,   0, 3,   0,   4,
    0, 5,   0,   6,   0, 7,   0, 8,   0,   9,  0, 0,   0, 0,   0,   1,   0, 2,   0,   3,  0, 4,   0, 5,   0,   6,
    0, 7,   0,   8,   0, 9,   0, 0,   0,   90, 0, 200, 0, 90,  0,   0,   0, 0,   0,   0,  0, 0};

void dll_89_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll89EffectResourceData;
    GfxCmd* commands = packet.entries;

    commands[0].layer = 0;
    commands[0].flags = 10;
    commands[0].tex = &resourceData[offsetof(Dll89EffectResourceView, allVertexIndices)];
    commands[0].mode = 2;
    commands[0].x = 1.1f;
    commands[0].y = 1.2f;
    commands[0].z = 1.1f;
    commands[1].layer = 0;
    commands[1].flags = 10;
    commands[1].tex = &resourceData[offsetof(Dll89EffectResourceView, allVertexIndices)];
    commands[1].mode = 4;
    commands[1].x = 0.0f;
    commands[1].y = 0.0f;
    commands[1].z = 0.0f;
    commands[2].layer = 0;
    commands[2].flags = 0;
    commands[2].tex = NULL;
    commands[2].mode = 0x400000;
    commands[2].x = 8.0f;
    commands[2].y = 72.0f;
    commands[2].z = 5.0f;
    commands[3].layer = 1;
    commands[3].flags = 10;
    commands[3].tex = &resourceData[offsetof(Dll89EffectResourceView, allVertexIndices)];
    commands[3].mode = 0x4000;
    commands[3].x = 1.0f;
    commands[3].y = 1.0f;
    commands[3].z = 0.0f;
    commands[4].layer = 0;
    commands[4].flags = 9;
    commands[4].tex = &resourceData[offsetof(Dll89EffectResourceView, wrappedVertexIndices)];
    commands[4].mode = 2;
    commands[4].x = 32.1f;
    commands[4].y = 1.2f;
    commands[4].z = 32.1f;
    commands[5].layer = 2;
    commands[5].flags = 1;
    commands[5].tex = gDll89ZeroIndexData;
    commands[5].mode = 4;
    commands[5].x = 255.0f;
    commands[5].y = 0.0f;
    commands[5].z = 0.0f;
    commands[6].layer = 2;
    commands[6].flags = 10;
    commands[6].tex = &resourceData[offsetof(Dll89EffectResourceView, allVertexIndices)];
    commands[6].mode = 0x4000;
    commands[6].x = 1.0f;
    commands[6].y = 1.0f;
    commands[6].z = 0.0f;
    commands[7].layer = 3;
    commands[7].flags = 10;
    commands[7].tex = &resourceData[offsetof(Dll89EffectResourceView, allVertexIndices)];
    commands[7].mode = 0x4000;
    commands[7].x = 1.0f;
    commands[7].y = 1.0f;
    commands[7].z = 0.0f;
    commands[8].layer = 4;
    commands[8].flags = 10;
    commands[8].tex = &resourceData[offsetof(Dll89EffectResourceView, allVertexIndices)];
    commands[8].mode = 0x4000;
    commands[8].x = 1.0f;
    commands[8].y = 1.0f;
    commands[8].z = 0.0f;
    commands[9].layer = 4;
    commands[9].flags = 10;
    commands[9].tex = &resourceData[offsetof(Dll89EffectResourceView, allVertexIndices)];
    commands[9].mode = 4;
    commands[9].x = 0.0f;
    commands[9].y = 0.0f;
    commands[9].z = 0.0f;
    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = 0.0f;
    packet.position[1] = 0.0f;
    packet.position[2] = 0.0f;
    packet.velocity[0] = 0.0f;
    packet.velocity[1] = 0.0f;
    packet.velocity[2] = 0.0f;
    packet.scale = 0.0f;
    packet.drawGroupCount = 1;
    packet.drawGroupStride = 10;
    packet.initialStateByte = 10;
    packet.byte5A = 0;
    packet.textureFrameTimer = 16;
    packet.flags = 0x4000494;
    packet.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd) * 10) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll89EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll89EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll89EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll89EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll89EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll89EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll89EffectResourceView, sequenceParams[6])];
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
        ->spawnEffect(&packet, 0, 10, (u8*)(int)gDll89EffectResourceData, 8,
                      &resourceData[offsetof(Dll89EffectResourceView, triangles)], 0x1fd, 0);
}

void dll_89_release(void) {
}

void dll_89_initialise(void) {
}

Dll89ResourceDescriptor gDll89ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_89_initialise, dll_89_release, NULL, dll_89_spawnEffect,
};
