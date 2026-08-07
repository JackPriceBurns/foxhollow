/*
 * DLL 165 / 0xA5 - a rotation-aware layered effect spawner.
 */
#include "main/dll/dll_00A5_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct DllA5EffectResourceView {
    ModgfxEffectVertex vertices[8];
    s16 triangles[4][3];
    s16 allVertexIndices[8];
    s16 sequenceParams[7];
    s16 opaqueTail;
} DllA5EffectResourceView;

STATIC_ASSERT(offsetof(DllA5EffectResourceView, vertices) == 0x00);
STATIC_ASSERT(offsetof(DllA5EffectResourceView, triangles) == 0x50);
STATIC_ASSERT(offsetof(DllA5EffectResourceView, allVertexIndices) == 0x68);
STATIC_ASSERT(offsetof(DllA5EffectResourceView, sequenceParams) == 0x78);
STATIC_ASSERT(offsetof(DllA5EffectResourceView, opaqueTail) == 0x86);
STATIC_ASSERT(sizeof(DllA5EffectResourceView) == 0x88);

extern u8 gDllA5EffectResourceData[sizeof(DllA5EffectResourceView)];

s16 gDllA5FirstFourVertexIndices[4] = {0, 1, 2, 3};
s16 gDllA5LastFourVertexIndices[4] = {4, 5, 6, 7};

void dll_A5_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDllA5EffectResourceData;
    GfxCmd* commands = packet.entries;
    u32 fl;

    commands[0].layer = 0;
    commands[0].flags = 8;
    commands[0].tex = &resourceData[offsetof(DllA5EffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    commands[1].layer = 0;
    commands[1].flags = 4;
    commands[1].tex = gDllA5FirstFourVertexIndices;
    commands[1].mode = 2;
    commands[1].x = 1.0f;
    commands[1].y = 1.0f;
    commands[1].z = 1.26f;
    commands[2].layer = 0;
    commands[2].flags = 4;
    commands[2].tex = gDllA5LastFourVertexIndices;
    commands[2].mode = 2;
    commands[2].x = 1.9f;
    commands[2].y = 1.9f;
    commands[2].z = 1.26f;
    commands[3].layer = 0;
    commands[3].flags = 0;
    commands[3].tex = NULL;
    commands[3].mode = 0x80;
    commands[3].x = 0.0f;
    commands[3].y = 0.0f;
    commands[3].z = (f32)sourceObj->anim.rotX;
    commands[4].layer = 0;
    commands[4].flags = 0x7a;
    commands[4].tex = NULL;
    commands[4].mode = 0x10000;
    commands[4].x = 0.0f;
    commands[4].y = 0.0f;
    commands[4].z = 0.0f;
    commands[5].layer = 1;
    commands[5].flags = 8;
    commands[5].tex = &resourceData[offsetof(DllA5EffectResourceView, allVertexIndices)];
    commands[5].mode = 4;
    commands[5].x = 255.0f;
    commands[5].y = 0.0f;
    commands[5].z = 0.0f;
    commands[6].layer = 1;
    commands[6].flags = 0;
    commands[6].tex = NULL;
    commands[6].mode = 0x400000;
    commands[6].x = 0.0f;
    commands[6].y = 0.0f;
    commands[6].z = 1.0f;
    commands[7].layer = 1;
    commands[7].flags = 8;
    commands[7].tex = &resourceData[offsetof(DllA5EffectResourceView, allVertexIndices)];
    commands[7].mode = 2;
    commands[7].x = 1.0f;
    commands[7].y = 1.0f;
    commands[7].z = 3.0f;
    commands[8].layer = 1;
    commands[8].flags = 0x3a1;
    commands[8].tex = NULL;
    commands[8].mode = 0x1800000;
    commands[8].x = 1.0f;
    commands[8].y = 0.0f;
    commands[8].z = 2.0f;
    commands[9].layer = 2;
    commands[9].flags = 0x7a;
    commands[9].tex = NULL;
    commands[9].mode = 0x10000;
    commands[9].x = 0.0f;
    commands[9].y = 0.0f;
    commands[9].z = 0.0f;
    commands[10].layer = 2;
    commands[10].flags = 8;
    commands[10].tex = &resourceData[offsetof(DllA5EffectResourceView, allVertexIndices)];
    commands[10].mode = 4;
    commands[10].x = 0.0f;
    commands[10].y = 0.0f;
    commands[10].z = 0.0f;
    commands[11].layer = 2;
    commands[11].flags = 0;
    commands[11].tex = NULL;
    commands[11].mode = 0x400000;
    commands[11].x = 0.0f;
    commands[11].y = 0.0f;
    commands[11].z = 25.0f;
    commands[12].layer = 2;
    commands[12].flags = 0x3a0;
    commands[12].tex = NULL;
    commands[12].mode = 0x800000;
    commands[12].x = 1.0f;
    commands[12].y = 0.0f;
    commands[12].z = 0.0f;

    packet.modeByte = variant;
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
    packet.textureFrameTimer = 0x3c;
    packet.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd) * 13) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(DllA5EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(DllA5EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(DllA5EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(DllA5EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(DllA5EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(DllA5EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(DllA5EffectResourceView, sequenceParams[6])];
    packet.commands = commands;
    packet.flags = 0x4040000;
    packet.flags |= (flags | 0x80);
    fl = packet.flags;
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
        ->spawnEffect(&packet, 0, 8, (u8*)(int)gDllA5EffectResourceData, 4,
                      &resourceData[offsetof(DllA5EffectResourceView, triangles)], 0x5e0, 0);
}

void dll_A5_release(void) {
}

void dll_A5_initialise(void) {
}

u8 gDllA5EffectResourceData[sizeof(DllA5EffectResourceView)] = {
    252, 24, 5, 120, 0,  0,  0,   0,   0,   0,   0,  0, 1, 144, 0,  0,   0, 0,   0,  0,   3,   232, 5,
    120, 0,  0, 0,   15, 0,  0,   0,   0,   9,   96, 0, 0, 0,   15, 0,   0, 252, 24, 5,   120, 15,  160,
    0,   0,  0, 31,  0,  0,  1,   144, 15,  160, 0,  0, 0, 31,  3,  232, 5, 120, 15, 160, 0,   15,  0,
    31,  0,  0, 9,   96, 15, 160, 0,   15,  0,   31, 0, 0, 0,   2,  0,   6, 0,   0,  0,   6,   0,   4,
    0,   1,  0, 3,   0,  7,  0,   1,   0,   7,   0,  5, 0, 0,   0,  1,   0, 2,   0,  3,   0,   4,   0,
    5,   0,  6, 0,   7,  0,  0,   0,   130, 0,   26, 0, 0, 0,   0,  0,   0, 0,   0,  0,   0};
DllA5ResourceDescriptor gDllA5ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_A5_initialise, dll_A5_release, NULL, dll_A5_spawnEffect,
};
