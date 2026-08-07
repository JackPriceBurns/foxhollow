/*
 * DLL 101 / 0x65 - a modgfx effect spawner.
 */
#include "main/dll/dll_0065_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"
#include "main/dll/partfx_interface.h"

typedef struct Dll65EffectResourceView {
    ModgfxEffectVertex vertices[14];
    s16 colors[12][3];
    s16 allVertexIndices[14];
    s16 firstGroupIndices[8];
    s16 secondGroupIndices[8];
    s16 sequenceParams[7];
    u8 pad11E[2];
} Dll65EffectResourceView;

STATIC_ASSERT(offsetof(Dll65EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll65EffectResourceView, colors) == 0x08C);
STATIC_ASSERT(offsetof(Dll65EffectResourceView, allVertexIndices) == 0x0D4);
STATIC_ASSERT(offsetof(Dll65EffectResourceView, firstGroupIndices) == 0x0F0);
STATIC_ASSERT(offsetof(Dll65EffectResourceView, secondGroupIndices) == 0x100);
STATIC_ASSERT(offsetof(Dll65EffectResourceView, sequenceParams) == 0x110);
STATIC_ASSERT(sizeof(Dll65EffectResourceView) == 0x120);

u32 gDll65EffectResourceData[sizeof(Dll65EffectResourceView) / sizeof(u32)] = {
    0x00000000, 0x03e80000, 0x00000362, 0x00c801f4, 0x0000000a, 0x03620028, 0xfe0c0000, 0x00150000, 0x0096fc18,
    0x0000001f, 0xfc9e005a, 0xfe0c0000, 0x002afc9e, 0x000a01f4, 0x00000034, 0x00000096, 0x03e80000, 0x003f0000,
    0x190003e8, 0x003f0000, 0x0362189c, 0x01f4003f, 0x000a0362, 0x1838fe0c, 0x003f0015, 0x00001932, 0xfc18003f,
    0x001ffc9e, 0x1900fe0c, 0x003f002a, 0xfc9e18ec, 0x01f4003f, 0x00340000, 0x192803e8, 0x003f003f, 0x00000001,
    0x00080000, 0x00080007, 0x00010002, 0x00090001, 0x00090008, 0x00020003, 0x000a0002, 0x000a0009, 0x00030004,
    0x000b0003, 0x000b000a, 0x00040005, 0x000c0004, 0x000c000b, 0x00050006, 0x000d0005, 0x000d000c, 0x00000001,
    0x00020003, 0x00040005, 0x00060007, 0x00080009, 0x000a000b, 0x000c000d, 0x00000001, 0x00020003, 0x00040005,
    0x00060000, 0x00070008, 0x0009000a, 0x000b000c, 0x000d0000, 0x00000104, 0x003c0001, 0x01040000, 0x00000000,
};

void dll_65_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    GfxCmd* commands = packet.entries;
    u8* resourceData = (u8*)(int)gDll65EffectResourceData;
    u8 effectScaleTenths;

    if (variant == 1) {
        *(s16*)&resourceData[offsetof(Dll65EffectResourceView, sequenceParams[1])] = 0;
    }
    effectScaleTenths = ((u8*)sourceObj->anim.placementData)[0x1a];
    commands[0].layer = 0;
    commands[0].flags = 7;
    commands[0].tex = &resourceData[offsetof(Dll65EffectResourceView, firstGroupIndices)];
    commands[0].mode = 8;
    commands[0].x = 50.0f;
    commands[0].y = 50.0f;
    commands[0].z = 50.0f;
    commands[1].layer = 0;
    commands[1].flags = 7;
    commands[1].tex = &resourceData[offsetof(Dll65EffectResourceView, secondGroupIndices)];
    commands[1].mode = 8;
    commands[1].x = 200.0f;
    commands[1].y = 200.0f;
    commands[1].z = 200.0f;
    commands[2].layer = 0;
    commands[2].flags = 0xe;
    commands[2].tex = &resourceData[offsetof(Dll65EffectResourceView, allVertexIndices)];
    commands[2].mode = 4;
    commands[2].x = 0.0f;
    commands[2].y = 0.0f;
    commands[2].z = 0.0f;
    commands[3].layer = 0;
    commands[3].flags = 7;
    commands[3].tex = &resourceData[offsetof(Dll65EffectResourceView, secondGroupIndices)];
    commands[3].mode = 2;
    commands[3].x = 0.225f;
    commands[3].y = 0.62f;
    commands[3].z = 0.225f;
    commands[4].layer = 0;
    commands[4].flags = 7;
    commands[4].tex = &resourceData[offsetof(Dll65EffectResourceView, firstGroupIndices)];
    commands[4].mode = 2;
    commands[4].x = 0.55f;
    commands[4].y = 1.0f;
    commands[4].z = 0.55f;
    commands[5].layer = 1;
    commands[5].layer = 1;
    commands[5].flags = 0x12;
    commands[5].tex = &resourceData[offsetof(Dll65EffectResourceView, allVertexIndices)];
    commands[5].mode = 0x100;
    commands[5].x = 0.0f;
    commands[5].y = 0.0f;
    commands[5].z = 20.0f;
    commands[6].layer = 1;
    commands[6].flags = 7;
    commands[6].tex = &resourceData[offsetof(Dll65EffectResourceView, firstGroupIndices)];
    commands[6].mode = 4;
    commands[6].x = 70.0f;
    commands[6].y = 0.0f;
    commands[6].z = 0.0f;
    commands[7].layer = 1;
    commands[7].flags = 7;
    commands[7].tex = &resourceData[offsetof(Dll65EffectResourceView, secondGroupIndices)];
    commands[7].mode = 4;
    commands[7].x = 12.0f;
    commands[7].y = 0.0f;
    commands[7].z = 0.0f;
    commands[8].layer = 2;
    commands[8].flags = 0x12;
    commands[8].tex = &resourceData[offsetof(Dll65EffectResourceView, allVertexIndices)];
    commands[8].mode = 0x4000;
    commands[8].x = -0.7f;
    commands[8].y = 0.0f;
    commands[8].z = 0.0f;
    commands[9].layer = 3;
    commands[9].flags = 1;
    commands[9].tex = NULL;
    commands[9].mode = 0x2000;
    commands[9].x = 0.0f;
    commands[9].y = 0.0f;
    commands[9].z = 0.0f;
    commands[10].layer = 4;
    commands[10].flags = 7;
    commands[10].tex = &resourceData[offsetof(Dll65EffectResourceView, firstGroupIndices)];
    commands[10].mode = 4;
    commands[10].x = 0.0f;
    commands[10].y = 0.0f;
    commands[10].z = 0.0f;
    commands[11].layer = 4;
    commands[11].flags = 7;
    commands[11].tex = &resourceData[offsetof(Dll65EffectResourceView, secondGroupIndices)];
    commands[11].mode = 4;
    commands[11].x = 0.0f;
    commands[11].y = 0.0f;
    commands[11].z = 0.0f;
    commands[12].layer = 4;
    commands[12].flags = 0x12;
    commands[12].tex = &resourceData[offsetof(Dll65EffectResourceView, allVertexIndices)];
    commands[12].mode = 0x4000;
    commands[12].x = -0.7f;
    commands[12].y = 0.0f;
    commands[12].z = 0.0f;
    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = 0.0f;
    packet.position[1] = 0.0f;
    packet.position[2] = 0.0f;
    packet.velocity[0] = 0.0f;
    packet.velocity[1] = 0.0f;
    packet.velocity[2] = 0.0f;
    if (effectScaleTenths != 0) {
        packet.scale = 0.1f * (f32)(u32)effectScaleTenths;
    } else {
        packet.scale = 1.0f;
    }
    packet.drawGroupCount = 1;
    packet.drawGroupStride = 0;
    packet.initialStateByte = 0xe;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0x1e;
    packet.commandCount = 13;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll65EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll65EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll65EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll65EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll65EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll65EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll65EffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + 0x60);
    packet.flags = 0x40000c0;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (packet.sourceObj != NULL) {
            packet.position[0] += packet.sourceObj->anim.worldPosX;
            packet.position[1] += packet.sourceObj->anim.worldPosY;
            packet.position[2] += packet.sourceObj->anim.worldPosZ;
        } else {
            PartFxSpawnParams* params = (PartFxSpawnParams*)spawnParams;

            packet.position[0] += params->posX;
            packet.position[1] += params->posY;
            packet.position[2] += params->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 0xe, (u8*)(int)gDll65EffectResourceData, 0xc,
                      &resourceData[offsetof(Dll65EffectResourceView, colors)], 0x40, 0);
}

void dll_65_release(void) {
}

void dll_65_initialise(void) {
}

Dll65ResourceDescriptor gDll65ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_65_initialise, dll_65_release, NULL, dll_65_spawnEffect,
};
