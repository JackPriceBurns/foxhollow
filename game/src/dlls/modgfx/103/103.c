/*
 * DLL 103 / 0x67 - a gameplay-preview effect spawner.
 *
 * dll_67_spawnEffect builds a seven-command effect description and submits
 * it through the modgfx interface.
 */
#include "main/dll/dll_0067_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"
#include "main/dll/partfx_interface.h"

typedef struct Dll67EffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 padD2[2];
    s16 colors[24][3];
    s16 firstGroupIndices[8];
    s16 secondGroupIndices[8];
    s16 thirdGroupIndices[8];
    s16 firstAndThirdGroupIndices[14];
    s16 allVertexIndices[22];
    s16 sequenceParams[7];
    u8 pad1EA[2];
} Dll67EffectResourceView;

STATIC_ASSERT(offsetof(Dll67EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll67EffectResourceView, colors) == 0x0D4);
STATIC_ASSERT(offsetof(Dll67EffectResourceView, firstGroupIndices) == 0x164);
STATIC_ASSERT(offsetof(Dll67EffectResourceView, secondGroupIndices) == 0x174);
STATIC_ASSERT(offsetof(Dll67EffectResourceView, thirdGroupIndices) == 0x184);
STATIC_ASSERT(offsetof(Dll67EffectResourceView, firstAndThirdGroupIndices) == 0x194);
STATIC_ASSERT(offsetof(Dll67EffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll67EffectResourceView, sequenceParams) == 0x1DC);
STATIC_ASSERT(sizeof(Dll67EffectResourceView) == 0x1EC);

u32 gDll67EffectResourceData[sizeof(Dll67EffectResourceView) / sizeof(u32)] = {
    0x00000000, 0x03e80000, 0x00000362, 0x000001f4, 0x000b0000, 0x03620000, 0xfe0c0016, 0x00000000, 0x0000fc18,
    0x00200000, 0xfc9e0000, 0xfe0c002a, 0x0000fc9e, 0x000001f4, 0x00340000, 0x00000000, 0x03e8003f, 0x00000000,
    0x0bb803e8, 0x0000003f, 0x03620bb8, 0x01f4000b, 0x003f0362, 0x0bb8fe0c, 0x0016003f, 0x00000bb8, 0xfc180020,
    0x003ffc9e, 0x0bb8fe0c, 0x002a003f, 0xfc9e0bb8, 0x01f40034, 0x003f0000, 0x0bb803e8, 0x003f003f, 0x00001770,
    0x03e80000, 0x007f0362, 0x177001f4, 0x000b007f, 0x03621770, 0xfe0c0016, 0x007f0000, 0x1770fc18, 0x0020007f,
    0xfc9e1770, 0xfe0c002a, 0x007ffc9e, 0x177001f4, 0x0034007f, 0x00001770, 0x03e8003f, 0x007f0000, 0x00000001,
    0x00080000, 0x00080007, 0x00010002, 0x00090001, 0x00090008, 0x00020003, 0x000a0002, 0x000a0009, 0x00030004,
    0x000b0003, 0x000b000a, 0x00040005, 0x000c0004, 0x000c000b, 0x00050006, 0x000d0005, 0x000d000c, 0x00070008,
    0x000f0007, 0x000f000e, 0x00080009, 0x00100008, 0x0010000f, 0x0009000a, 0x00110009, 0x00110010, 0x000a000b,
    0x0012000a, 0x00120011, 0x000b000c, 0x0013000b, 0x00130012, 0x000c000d, 0x0014000c, 0x00140013, 0x00000001,
    0x00020003, 0x00040005, 0x00060000, 0x00070008, 0x0009000a, 0x000b000c, 0x000d0000, 0x000e000f, 0x00100011,
    0x00120013, 0x00140000, 0x00000001, 0x00020003, 0x00040005, 0x0006000e, 0x000f0010, 0x00110012, 0x00130014,
    0x00000001, 0x00020003, 0x00040005, 0x00060007, 0x00080009, 0x000a000b, 0x000c000d, 0x000e000f, 0x00100011,
    0x00120013, 0x00140000, 0x00000032, 0x00640032, 0x00000000, 0x00000000,
};

void dll_67_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll67EffectResourceData;

    packet.entries[0].layer = 0;
    packet.entries[0].flags = 0x15;
    packet.entries[0].tex = &resourceData[offsetof(Dll67EffectResourceView, allVertexIndices)];
    packet.entries[0].mode = 4;
    packet.entries[0].x = 0.0f;
    packet.entries[0].y = 0.0f;
    packet.entries[0].z = 0.0f;
    packet.entries[1].layer = 0;
    packet.entries[1].flags = 0x15;
    packet.entries[1].tex = &resourceData[offsetof(Dll67EffectResourceView, allVertexIndices)];
    packet.entries[1].mode = 2;
    packet.entries[1].x = 1.8f;
    packet.entries[1].y = 2.0f;
    packet.entries[1].z = 1.8f;
    packet.entries[2].layer = 1;
    packet.entries[2].flags = 7;
    packet.entries[2].tex = &resourceData[offsetof(Dll67EffectResourceView, secondGroupIndices)];
    packet.entries[2].mode = 4;
    packet.entries[2].x = 255.0f;
    packet.entries[2].y = 0.0f;
    packet.entries[2].z = 0.0f;
    packet.entries[3].layer = 1;
    packet.entries[3].flags = 0x15;
    packet.entries[3].tex = &resourceData[offsetof(Dll67EffectResourceView, allVertexIndices)];
    packet.entries[3].mode = 0x4000;
    packet.entries[3].x = 0.0f;
    packet.entries[3].y = -8.0f;
    packet.entries[3].z = 0.0f;
    packet.entries[4].layer = 2;
    packet.entries[4].flags = 0x15;
    packet.entries[4].tex = &resourceData[offsetof(Dll67EffectResourceView, allVertexIndices)];
    packet.entries[4].mode = 0x4000;
    packet.entries[4].x = 0.0f;
    packet.entries[4].y = -8.0f;
    packet.entries[4].z = 0.0f;
    packet.entries[5].layer = 3;
    packet.entries[5].flags = 7;
    packet.entries[5].tex = &resourceData[offsetof(Dll67EffectResourceView, secondGroupIndices)];
    packet.entries[5].mode = 4;
    packet.entries[5].x = 0.0f;
    packet.entries[5].y = 0.0f;
    packet.entries[5].z = 0.0f;
    packet.entries[6].layer = 3;
    packet.entries[6].flags = 0x15;
    packet.entries[6].tex = &resourceData[offsetof(Dll67EffectResourceView, allVertexIndices)];
    packet.entries[6].mode = 0x4000;
    packet.entries[6].x = 0.0f;
    packet.entries[6].y = -8.0f;
    packet.entries[6].z = 0.0f;
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
    packet.commandCount = 7;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll67EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll67EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll67EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll67EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll67EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll67EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll67EffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + 0x60);
    packet.flags = 0xc010040;
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
        ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll67EffectResourceData, 0x18,
                      &resourceData[offsetof(Dll67EffectResourceView, colors)], 0xe3, 0);
}

void dll_67_release(void) {
}

void dll_67_initialise(void) {
}

Dll67ResourceDescriptor gDll67ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_67_initialise, dll_67_release, NULL, dll_67_spawnEffect, 0,
};
