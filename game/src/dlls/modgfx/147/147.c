/*
 * DLL 147 / 0x93 - a randomized six-command layered modgfx effect spawner.
 */
#include "main/dll/dll_0093_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"
#include "main/vecmath.h"

typedef struct Dll93EffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 padD2[2];
    s16 triangles[24][3];
    u8 opaque164[0x4C];
    s16 allVertexIndices[21];
    s16 opaque1DA;
    u8 opaque1DC[0x1C];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll93EffectResourceView;

STATIC_ASSERT(offsetof(Dll93EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll93EffectResourceView, padD2) == 0x0D2);
STATIC_ASSERT(offsetof(Dll93EffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll93EffectResourceView, opaque164) == 0x164);
STATIC_ASSERT(offsetof(Dll93EffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll93EffectResourceView, opaque1DA) == 0x1DA);
STATIC_ASSERT(offsetof(Dll93EffectResourceView, opaque1DC) == 0x1DC);
STATIC_ASSERT(offsetof(Dll93EffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(offsetof(Dll93EffectResourceView, opaqueTail) == 0x206);
STATIC_ASSERT(sizeof(Dll93EffectResourceView) == 0x208);

extern u16 gDll93EffectResourceData[sizeof(Dll93EffectResourceView) / sizeof(u16)];

void dll_93_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)gDll93EffectResourceData;
    GfxCmd* commands = packet.entries;
    f32 randomScale;

    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(Dll93EffectResourceView, allVertexIndices)];
    commands[0].mode = 0x4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    commands[1].layer = 0;
    commands[1].flags = 0x15;
    commands[1].tex = &resourceData[offsetof(Dll93EffectResourceView, allVertexIndices)];
    commands[1].mode = 0x2;
    randomScale = 0.1f * (f32)randomGetRange(0, 10) + 1.0f;
    commands[1].x = randomScale;
    commands[1].y = 10.5f;
    commands[1].z = randomScale;
    commands[2].layer = 1;
    commands[2].flags = 0x15;
    commands[2].tex = &resourceData[offsetof(Dll93EffectResourceView, allVertexIndices)];
    commands[2].mode = 0x4;
    commands[2].x = 255.0f;
    commands[2].y = 0.0f;
    commands[2].z = 0.0f;
    commands[3].layer = 1;
    commands[3].flags = 0x15;
    commands[3].tex = &resourceData[offsetof(Dll93EffectResourceView, allVertexIndices)];
    commands[3].mode = 0x4000;
    commands[3].x = 1.1f;
    commands[3].y = 0.0f;
    commands[3].z = 0.0f;
    commands[4].layer = 2;
    commands[4].flags = 0x15;
    commands[4].tex = &resourceData[offsetof(Dll93EffectResourceView, allVertexIndices)];
    commands[4].mode = 0x4;
    commands[4].x = 0.0f;
    commands[4].y = 0.0f;
    commands[4].z = 0.0f;
    commands[5].layer = 2;
    commands[5].flags = 0x15;
    commands[5].tex = &resourceData[offsetof(Dll93EffectResourceView, allVertexIndices)];
    commands[5].mode = 0x4000;
    commands[5].x = 1.1f;
    commands[5].y = 0.0f;
    commands[5].z = 0.0f;
    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = 0.0f;
    packet.position[1] = 0.0f;
    packet.position[2] = 0.0f;
    packet.velocity[0] = 0.0f;
    packet.velocity[1] = 0.0f;
    packet.velocity[2] = 0.0f;
    packet.scale = 1.2f;
    packet.drawGroupCount = 2;
    packet.drawGroupStride = 7;
    packet.initialStateByte = 0xE;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0x1E;
    packet.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd) * 6) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll93EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll93EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll93EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll93EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll93EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll93EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll93EffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + offsetof(ModgfxSpawnPacket, entries));
    packet.flags = 0xc0104c0;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if ((u32)sourceObj != 0) {
            GameObject* anchorObj = sourceObj;
            packet.position[0] += anchorObj->anim.localPosX;
            packet.position[1] += anchorObj->anim.localPosY;
            packet.position[2] += anchorObj->anim.localPosZ;
        } else {
            PartFxSpawnParams* anchorParams = spawnParams;
            packet.position[0] += anchorParams->posX;
            packet.position[1] += anchorParams->posY;
            packet.position[2] += anchorParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 0x15, (u8*)gDll93EffectResourceData, 0x18,
                      &resourceData[offsetof(Dll93EffectResourceView, triangles)], 0x89, 0);
}

void dll_93_release(void) {
}

void dll_93_initialise(void) {
}

u16 gDll93EffectResourceData[sizeof(Dll93EffectResourceView) / sizeof(u16)] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x00ff, 0x0362, 0x0000, 0x01f4, 0x000b,
    0x00ff, 0x0362, 0x0000, 0xfe0c, 0x0016, 0x00ff, 0x0000, 0x0000, 0xfc18,
    0x0020, 0x00ff, 0xfc9e, 0x0000, 0xfe0c, 0x0016, 0x00ff, 0xfc9e, 0x0000,
    0x01f4, 0x000b, 0x00ff, 0x0000, 0x0000, 0x03e8, 0x0000, 0x00ff, 0x0000,
    0x0bb8, 0x03e8, 0x0000, 0x007f, 0x0362, 0x0bb8, 0x01f4, 0x000b, 0x007f,
    0x0362, 0x0bb8, 0xfe0c, 0x0016, 0x007f, 0x0000, 0x0bb8, 0xfc18, 0x0020,
    0x007f, 0xfc9e, 0x0bb8, 0xfe0c, 0x0016, 0x007f, 0xfc9e, 0x0bb8, 0x01f4,
    0x000b, 0x007f, 0x0000, 0x0bb8, 0x03e8, 0x0000, 0x007f, 0x0000, 0x1770,
    0x03e8, 0x0000, 0x0000, 0x0362, 0x1770, 0x01f4, 0x000b, 0x0000, 0x0362,
    0x1770, 0xfe0c, 0x0016, 0x0000, 0x0000, 0x1770, 0xfc18, 0x0020, 0x0000,
    0xfc9e, 0x1770, 0xfe0c, 0x0016, 0x0000, 0xfc9e, 0x1770, 0x01f4, 0x000b,
    0x0000, 0x0000, 0x1770, 0x03e8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001,
    0x0008, 0x0000, 0x0008, 0x0007, 0x0001, 0x0002, 0x0009, 0x0001, 0x0009,
    0x0008, 0x0002, 0x0003, 0x000a, 0x0002, 0x000a, 0x0009, 0x0003, 0x0004,
    0x000b, 0x0003, 0x000b, 0x000a, 0x0004, 0x0005, 0x000c, 0x0004, 0x000c,
    0x000b, 0x0005, 0x0006, 0x000d, 0x0005, 0x000d, 0x000c, 0x0007, 0x0008,
    0x000f, 0x0007, 0x000f, 0x000e, 0x0008, 0x0009, 0x0010, 0x0008, 0x0010,
    0x000f, 0x0009, 0x000a, 0x0011, 0x0009, 0x0011, 0x0010, 0x000a, 0x000b,
    0x0012, 0x000a, 0x0012, 0x0011, 0x000b, 0x000c, 0x0013, 0x000b, 0x0013,
    0x0012, 0x000c, 0x000d, 0x0014, 0x000c, 0x0014, 0x0013, 0x0000, 0x0001,
    0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0000, 0x0007, 0x0008, 0x0009,
    0x000a, 0x000b, 0x000c, 0x000d, 0x0000, 0x000e, 0x000f, 0x0010, 0x0011,
    0x0012, 0x0013, 0x0014, 0x0000, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004,
    0x0005, 0x0006, 0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014,
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
    0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010, 0x0011,
    0x0012, 0x0013, 0x0014, 0x0000, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b,
    0x000c, 0x000d, 0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014,
    0x0000, 0x0032, 0x0032, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};
Dll93ResourceDescriptor gDll93ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_93_initialise, dll_93_release, NULL, dll_93_spawnEffect,
};
