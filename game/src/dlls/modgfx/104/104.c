/*
 * DLL 104 / 0x68 - a modgfx effect spawner.
 */
#include "main/dll/dll_0068_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"
#include "main/dll/partfx_interface.h"

typedef struct Dll68EffectResourceView {
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
} Dll68EffectResourceView;

STATIC_ASSERT(offsetof(Dll68EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll68EffectResourceView, colors) == 0x0D4);
STATIC_ASSERT(offsetof(Dll68EffectResourceView, firstGroupIndices) == 0x164);
STATIC_ASSERT(offsetof(Dll68EffectResourceView, secondGroupIndices) == 0x174);
STATIC_ASSERT(offsetof(Dll68EffectResourceView, thirdGroupIndices) == 0x184);
STATIC_ASSERT(offsetof(Dll68EffectResourceView, firstAndThirdGroupIndices) == 0x194);
STATIC_ASSERT(offsetof(Dll68EffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll68EffectResourceView, sequenceParams) == 0x1DC);
STATIC_ASSERT(sizeof(Dll68EffectResourceView) == 0x1EC);

u32 gDll68EffectResourceData[sizeof(Dll68EffectResourceView) / sizeof(u32)] = {
    0x00000000, 0x03e80000, 0x00000362, 0x000001f4, 0x000b0000, 0x03620000, 0xfe0c0016, 0x00000000, 0x0000fc18,
    0x00200000, 0xfc9e0000, 0xfe0c002a, 0x0000fc9e, 0x000001f4, 0x00340000, 0x00000000, 0x03e8003f, 0x00000000,
    0x064003e8, 0x0000001f, 0x03620640, 0x01f4000b, 0x001f0362, 0x0640fe0c, 0x0016001f, 0x00000640, 0xfc180020,
    0x001ffc9e, 0x0640fe0c, 0x002a001f, 0xfc9e0640, 0x01f40034, 0x001f0000, 0x064003e8, 0x003f001f, 0x00001770,
    0x03e80000, 0x003f0362, 0x177001f4, 0x000b003f, 0x03621770, 0xfe0c0016, 0x003f0000, 0x1770fc18, 0x0020003f,
    0xfc9e1770, 0xfe0c002a, 0x003ffc9e, 0x177001f4, 0x0034003f, 0x00001770, 0x03e8003f, 0x003f0000, 0x00000001,
    0x00080000, 0x00080007, 0x00010002, 0x00090001, 0x00090008, 0x00020003, 0x000a0002, 0x000a0009, 0x00030004,
    0x000b0003, 0x000b000a, 0x00040005, 0x000c0004, 0x000c000b, 0x00050006, 0x000d0005, 0x000d000c, 0x00070008,
    0x000f0007, 0x000f000e, 0x00080009, 0x00100008, 0x0010000f, 0x0009000a, 0x00110009, 0x00110010, 0x000a000b,
    0x0012000a, 0x00120011, 0x000b000c, 0x0013000b, 0x00130012, 0x000c000d, 0x0014000c, 0x00140013, 0x00000001,
    0x00020003, 0x00040005, 0x00060000, 0x00070008, 0x0009000a, 0x000b000c, 0x000d0000, 0x000e000f, 0x00100011,
    0x00120013, 0x00140000, 0x00000001, 0x00020003, 0x00040005, 0x0006000e, 0x000f0010, 0x00110012, 0x00130014,
    0x00000001, 0x00020003, 0x00040005, 0x00060007, 0x00080009, 0x000a000b, 0x000c000d, 0x000e000f, 0x00100011,
    0x00120013, 0x00140000, 0x00000032, 0x00640032, 0x00000000, 0x00000000,
};

void dll_68_spawnEffect(GameObject* sourceObj, int variant, void* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll68EffectResourceData;

    packet.entries[0].layer = 0;
    packet.entries[0].flags = 0x15;
    packet.entries[0].tex = &resourceData[offsetof(Dll68EffectResourceView, allVertexIndices)];
    packet.entries[0].mode = 4;
    packet.entries[0].x = 0.0f;
    packet.entries[0].y = 0.0f;
    packet.entries[0].z = 0.0f;
    packet.entries[1].layer = 0;
    packet.entries[1].flags = 0x15;
    packet.entries[1].tex = &resourceData[offsetof(Dll68EffectResourceView, allVertexIndices)];
    packet.entries[1].mode = 2;
    packet.entries[1].x = 2.1f;
    packet.entries[1].y = 2.0f;
    packet.entries[1].z = 2.1f;
    packet.entries[2].layer = 1;
    packet.entries[2].flags = 7;
    packet.entries[2].tex = &resourceData[offsetof(Dll68EffectResourceView, firstGroupIndices)];
    packet.entries[2].mode = 2;
    packet.entries[2].x = 2.0f;
    packet.entries[2].y = 1.0f;
    packet.entries[2].z = 2.0f;
    packet.entries[3].layer = 1;
    packet.entries[3].flags = 7;
    packet.entries[3].tex = &resourceData[offsetof(Dll68EffectResourceView, secondGroupIndices)];
    packet.entries[3].mode = 4;
    packet.entries[3].x = 255.0f;
    packet.entries[3].y = 0.0f;
    packet.entries[3].z = 0.0f;
    packet.entries[4].layer = 1;
    packet.entries[4].flags = 0x15;
    packet.entries[4].tex = &resourceData[offsetof(Dll68EffectResourceView, allVertexIndices)];
    packet.entries[4].mode = 0x4000;
    packet.entries[4].x = 0.0f;
    packet.entries[4].y = -2.0f;
    packet.entries[4].z = 0.0f;
    packet.entries[5].layer = 1;
    packet.entries[5].flags = 0;
    packet.entries[5].tex = NULL;
    packet.entries[5].mode = 0x100;
    packet.entries[5].x = 0.0f;
    packet.entries[5].y = 0.0f;
    packet.entries[5].z = 150.0f;
    packet.entries[6].layer = 2;
    packet.entries[6].flags = 0x15;
    packet.entries[6].tex = &resourceData[offsetof(Dll68EffectResourceView, allVertexIndices)];
    packet.entries[6].mode = 0x4000;
    packet.entries[6].x = 0.0f;
    packet.entries[6].y = -2.0f;
    packet.entries[6].z = 0.0f;
    packet.entries[7].layer = 2;
    packet.entries[7].flags = 0;
    packet.entries[7].tex = NULL;
    packet.entries[7].mode = 0x100;
    packet.entries[7].x = 0.0f;
    packet.entries[7].y = 0.0f;
    packet.entries[7].z = 150.0f;
    packet.entries[8].layer = 3;
    packet.entries[8].flags = 7;
    packet.entries[8].tex = &resourceData[offsetof(Dll68EffectResourceView, secondGroupIndices)];
    packet.entries[8].mode = 4;
    packet.entries[8].x = 0.0f;
    packet.entries[8].y = 0.0f;
    packet.entries[8].z = 0.0f;
    packet.entries[9].layer = 3;
    packet.entries[9].flags = 0x15;
    packet.entries[9].tex = &resourceData[offsetof(Dll68EffectResourceView, allVertexIndices)];
    packet.entries[9].mode = 0x4000;
    packet.entries[9].x = 0.0f;
    packet.entries[9].y = -2.0f;
    packet.entries[9].z = 0.0f;
    packet.entries[10].layer = 3;
    packet.entries[10].flags = 0;
    packet.entries[10].tex = NULL;
    packet.entries[10].mode = 0x100;
    packet.entries[10].x = 0.0f;
    packet.entries[10].y = 0.0f;
    packet.entries[10].z = 150.0f;
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
    packet.commandCount = 11;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll68EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll68EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll68EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll68EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll68EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll68EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll68EffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + 0x60);
    packet.flags = 0xc0100c0;
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
        ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll68EffectResourceData, 0x18,
                      &resourceData[offsetof(Dll68EffectResourceView, colors)], 0x41, 0);
}

void dll_68_release(void) {
}

void dll_68_initialise(void) {
}

Dll68ResourceDescriptor gDll68ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_68_initialise, dll_68_release, NULL, dll_68_spawnEffect, 0,
};
