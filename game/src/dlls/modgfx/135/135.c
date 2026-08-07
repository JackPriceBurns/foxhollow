/*
 * DLL 135 / 0x87 - a ten-command layered modgfx effect spawner.
 */
#include "main/dll/dll_0087_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll87EffectResourceView {
    ModgfxEffectVertex vertices[10];
    u8 opaque064[0x104];
    s16 triangles[8][3];
    s16 wrappedVertexIndices[10];
    s16 allVertexIndices[10];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll87EffectResourceView;

STATIC_ASSERT(offsetof(Dll87EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll87EffectResourceView, opaque064) == 0x064);
STATIC_ASSERT(offsetof(Dll87EffectResourceView, triangles) == 0x168);
STATIC_ASSERT(offsetof(Dll87EffectResourceView, wrappedVertexIndices) == 0x198);
STATIC_ASSERT(offsetof(Dll87EffectResourceView, allVertexIndices) == 0x1AC);
STATIC_ASSERT(offsetof(Dll87EffectResourceView, sequenceParams) == 0x1C0);
STATIC_ASSERT(offsetof(Dll87EffectResourceView, opaqueTail) == 0x1CE);
STATIC_ASSERT(sizeof(Dll87EffectResourceView) == 0x1D0);

u8 gDll87ZeroIndexData[8] = {0};

u8 gDll87EffectResourceData[sizeof(Dll87EffectResourceView)] = {
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

void dll_87_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll87EffectResourceData;
    GfxCmd* commands = packet.entries;
    f32 originOffset = 0.0f;

    commands[0].layer = 0;
    commands[0].flags = 10;
    commands[0].tex = &resourceData[offsetof(Dll87EffectResourceView, allVertexIndices)];
    commands[0].mode = 2;
    commands[0].x = 1.1f;
    commands[0].y = 1.2f;
    commands[0].z = 1.1f;
    commands[1].layer = 0;
    commands[1].flags = 10;
    commands[1].tex = &resourceData[offsetof(Dll87EffectResourceView, allVertexIndices)];
    commands[1].mode = 4;
    commands[1].x = originOffset;
    commands[1].y = originOffset;
    commands[1].z = originOffset;
    commands[2].layer = 0;
    commands[2].flags = 0;
    commands[2].tex = NULL;
    commands[2].mode = 0x400000;
    commands[2].x = 8.0f;
    commands[2].y = 72.0f;
    commands[2].z = 5.0f;
    commands[3].layer = 1;
    commands[3].flags = 10;
    commands[3].tex = &resourceData[offsetof(Dll87EffectResourceView, allVertexIndices)];
    commands[3].mode = 0x4000;
    commands[3].x = 1.0f;
    commands[3].y = 1.0f;
    commands[3].z = originOffset;
    commands[4].layer = 0;
    commands[4].flags = 9;
    commands[4].tex = &resourceData[offsetof(Dll87EffectResourceView, wrappedVertexIndices)];
    commands[4].mode = 2;
    commands[4].x = 32.1f;
    commands[4].y = 1.2f;
    commands[4].z = 32.1f;
    commands[5].layer = 2;
    commands[5].flags = 1;
    commands[5].tex = gDll87ZeroIndexData;
    commands[5].mode = 4;
    commands[5].x = 255.0f;
    commands[5].y = originOffset;
    commands[5].z = originOffset;
    commands[6].layer = 2;
    commands[6].flags = 10;
    commands[6].tex = &resourceData[offsetof(Dll87EffectResourceView, allVertexIndices)];
    commands[6].mode = 0x4000;
    commands[6].x = 1.0f;
    commands[6].y = 1.0f;
    commands[6].z = originOffset;
    commands[7].layer = 3;
    commands[7].flags = 10;
    commands[7].tex = &resourceData[offsetof(Dll87EffectResourceView, allVertexIndices)];
    commands[7].mode = 0x4000;
    commands[7].x = 1.0f;
    commands[7].y = 1.0f;
    commands[7].z = originOffset;
    commands[8].layer = 4;
    commands[8].flags = 10;
    commands[8].tex = &resourceData[offsetof(Dll87EffectResourceView, allVertexIndices)];
    commands[8].mode = 0x4000;
    commands[8].x = 1.0f;
    commands[8].y = 1.0f;
    commands[8].z = originOffset;
    commands[9].layer = 4;
    commands[9].flags = 10;
    commands[9].tex = &resourceData[offsetof(Dll87EffectResourceView, allVertexIndices)];
    commands[9].mode = 4;
    commands[9].x = originOffset;
    commands[9].y = originOffset;
    commands[9].z = originOffset;
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
    packet.drawGroupCount = 1;
    packet.drawGroupStride = 10;
    packet.initialStateByte = 10;
    packet.byte5A = 0;
    packet.textureFrameTimer = 16;
    packet.flags = 0x4000494;
    packet.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd) * 10) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll87EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll87EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll87EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll87EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll87EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll87EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll87EffectResourceView, sequenceParams[6])];
    packet.commands = commands;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (sourceObj != NULL) {
            GameObject* anchorObj = sourceObj;
            packet.position[0] = originOffset + anchorObj->anim.worldPosX;
            packet.position[1] = originOffset + anchorObj->anim.worldPosY;
            packet.position[2] = originOffset + anchorObj->anim.worldPosZ;
        } else {
            PartFxSpawnParams* anchorParams = spawnParams;
            packet.position[0] = originOffset + anchorParams->posX;
            packet.position[1] = originOffset + anchorParams->posY;
            packet.position[2] = originOffset + anchorParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 10, (u8*)(int)gDll87EffectResourceData, 8,
                      &resourceData[offsetof(Dll87EffectResourceView, triangles)], 0x1fd, 0);
}

void dll_87_release(void) {
}

void dll_87_initialise(void) {
}

Dll87ResourceDescriptor gDll87ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_87_initialise, dll_87_release, NULL, dll_87_spawnEffect,
};
