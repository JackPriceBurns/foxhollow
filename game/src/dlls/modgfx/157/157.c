/*
 * DLL 157 / 0x9D - a multi-layer pickup glow effect spawner.
 */
#include "main/dll/dll_009D_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll9DSevenIndexList {
    s16 indices[7];
    s16 opaqueTail;
} Dll9DSevenIndexList;

STATIC_ASSERT(offsetof(Dll9DSevenIndexList, indices) == 0x00);
STATIC_ASSERT(offsetof(Dll9DSevenIndexList, opaqueTail) == 0x0E);
STATIC_ASSERT(sizeof(Dll9DSevenIndexList) == 0x10);

typedef struct Dll9DEffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 opaqueD2[2];
    s16 triangles[24][3];
    Dll9DSevenIndexList sevenVertexIndexLists[3];
    s16 firstAndThirdVertexIndices[14];
    s16 allVertexIndices[21];
    s16 opaque1DA;
    s16 lastFourteenVertexIndices[14];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll9DEffectResourceView;

STATIC_ASSERT(offsetof(Dll9DEffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll9DEffectResourceView, opaqueD2) == 0x0D2);
STATIC_ASSERT(offsetof(Dll9DEffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll9DEffectResourceView, sevenVertexIndexLists) == 0x164);
STATIC_ASSERT(offsetof(Dll9DEffectResourceView, firstAndThirdVertexIndices) == 0x194);
STATIC_ASSERT(offsetof(Dll9DEffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll9DEffectResourceView, opaque1DA) == 0x1DA);
STATIC_ASSERT(offsetof(Dll9DEffectResourceView, lastFourteenVertexIndices) == 0x1DC);
STATIC_ASSERT(offsetof(Dll9DEffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(offsetof(Dll9DEffectResourceView, opaqueTail) == 0x206);
STATIC_ASSERT(sizeof(Dll9DEffectResourceView) == 0x208);

extern u16 gDll9DEffectResourceData[sizeof(Dll9DEffectResourceView) / sizeof(u16)];

void dll_9D_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)gDll9DEffectResourceData;
    GfxCmd* commands = packet.entries;
    u32 effectFlags;
    f32 originOffset = 0.0f;

    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(Dll9DEffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = originOffset;
    commands[0].y = originOffset;
    commands[0].z = originOffset;
    commands[1].layer = 0;
    commands[1].flags = 7;
    commands[1].tex = &resourceData[offsetof(Dll9DEffectResourceView, sevenVertexIndexLists[0].indices)];
    commands[1].mode = 2;
    commands[1].x = 16.0f;
    commands[1].y = 20.0f;
    commands[1].z = 16.0f;
    commands[2].layer = 0;
    commands[2].flags = 7;
    commands[2].tex = &resourceData[offsetof(Dll9DEffectResourceView, sevenVertexIndexLists[1].indices)];
    commands[2].mode = 2;
    commands[2].x = 20.0f;
    commands[2].y = 20.0f;
    commands[2].z = 20.0f;
    commands[3].layer = 0;
    commands[3].flags = 7;
    commands[3].tex = &resourceData[offsetof(Dll9DEffectResourceView, sevenVertexIndexLists[2].indices)];
    commands[3].mode = 2;
    commands[3].x = 16.0f;
    commands[3].y = 20.0f;
    commands[3].z = 16.0f;
    commands[4].layer = 0;
    commands[4].flags = 0;
    commands[4].tex = NULL;
    commands[4].mode = 0x400000;
    commands[4].x = originOffset;
    commands[4].y = -600.0f;
    commands[4].z = originOffset;
    commands[5].layer = 1;
    commands[5].flags = 7;
    commands[5].tex = &resourceData[offsetof(Dll9DEffectResourceView, sevenVertexIndexLists[1].indices)];
    commands[5].mode = 4;
    commands[5].x = 105.0f;
    commands[5].y = originOffset;
    commands[5].z = originOffset;
    commands[6].layer = 1;
    commands[6].flags = 0x15;
    commands[6].tex = &resourceData[offsetof(Dll9DEffectResourceView, allVertexIndices)];
    commands[6].mode = 0x4000;
    commands[6].x = originOffset;
    commands[6].y = originOffset;
    commands[6].z = originOffset;
    commands[7].layer = 1;
    commands[7].flags = 0;
    commands[7].tex = NULL;
    commands[7].mode = 0x400000;
    commands[7].x = originOffset;
    commands[7].y = 1200.0f;
    commands[7].z = originOffset;
    commands[8].layer = 2;
    commands[8].flags = 0x15;
    commands[8].tex = &resourceData[offsetof(Dll9DEffectResourceView, allVertexIndices)];
    commands[8].mode = 0x4000;
    commands[8].x = originOffset;
    commands[8].y = originOffset;
    commands[8].z = originOffset;
    commands[9].layer = 2;
    commands[9].flags = 0;
    commands[9].tex = NULL;
    commands[9].mode = 0x400000;
    commands[9].x = originOffset;
    commands[9].y = -1200.0f;
    commands[9].z = originOffset;
    commands[10].layer = 3;
    commands[10].flags = 0x15;
    commands[10].tex = &resourceData[offsetof(Dll9DEffectResourceView, allVertexIndices)];
    commands[10].mode = 0x4000;
    commands[10].x = originOffset;
    commands[10].y = originOffset;
    commands[10].z = originOffset;
    commands[11].layer = 3;
    commands[11].flags = 0;
    commands[11].tex = NULL;
    commands[11].mode = 0x400000;
    commands[11].x = originOffset;
    commands[11].y = 1200.0f;
    commands[11].z = originOffset;
    commands[12].layer = 3;
    commands[12].flags = 7;
    commands[12].tex = &resourceData[offsetof(Dll9DEffectResourceView, sevenVertexIndexLists[1].indices)];
    commands[12].mode = 4;
    commands[12].x = originOffset;
    commands[12].y = originOffset;
    commands[12].z = originOffset;

    packet.context.modeByte = 0;
    packet.context.attachedSource = sourceObj;
    packet.context.sourceMode = variant;
    packet.context.position[0] = originOffset;
    packet.context.position[1] = originOffset;
    packet.context.position[2] = originOffset;
    packet.context.velocity[0] = originOffset;
    packet.context.velocity[1] = originOffset;
    packet.context.velocity[2] = originOffset;
    packet.context.scale = 1.0f;
    packet.context.drawGroupCount = 2;
    packet.context.drawGroupStride = 7;
    packet.context.initialStateByte = 0xe;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x1e;
    packet.context.commandCount = (GfxCmd*)((u8*)commands + (int)sizeof(GfxCmd) * 13) - commands;
    packet.context.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll9DEffectResourceView, sequenceParams[0])];
    packet.context.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll9DEffectResourceView, sequenceParams[1])];
    packet.context.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll9DEffectResourceView, sequenceParams[2])];
    packet.context.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll9DEffectResourceView, sequenceParams[3])];
    packet.context.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll9DEffectResourceView, sequenceParams[4])];
    packet.context.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll9DEffectResourceView, sequenceParams[5])];
    packet.context.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll9DEffectResourceView, sequenceParams[6])];
    packet.context.commands = packet.entries;
    packet.context.flags = 0xc0100c0;
    packet.context.flags |= spawnFlags;
    effectFlags = packet.context.flags;
    if (effectFlags & 1) {
        if (sourceObj != NULL) {
            packet.context.position[0] += sourceObj->anim.worldPosX;
            packet.context.position[1] += sourceObj->anim.worldPosY;
            packet.context.position[2] += sourceObj->anim.worldPosZ;
        } else {
            packet.context.position[0] += spawnParams->posX;
            packet.context.position[1] += spawnParams->posY;
            packet.context.position[2] += spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet.context, 0, 0x15, (u8*)gDll9DEffectResourceData, 0x18,
                      &resourceData[offsetof(Dll9DEffectResourceView, triangles)], 0x46c, 0);
}

void dll_9D_release(void) {
}

void dll_9D_initialise(void) {
}

u16 gDll9DEffectResourceData[sizeof(Dll9DEffectResourceView) / sizeof(u16)] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0362, 0x0000, 0x01f4, 0x0016,
    0x0000, 0x0362, 0x0000, 0xfe0c, 0x002c, 0x0000, 0x0000, 0x0000, 0xfc18,
    0x003f, 0x0000, 0xfc9e, 0x0000, 0xfe0c, 0x002c, 0x0000, 0xfc9e, 0x0000,
    0x01f4, 0x0016, 0x0000, 0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0000,
    0x0bb8, 0x03e8, 0x0000, 0x000f, 0x0362, 0x0bb8, 0x01f4, 0x0016, 0x000f,
    0x0362, 0x0bb8, 0xfe0c, 0x002c, 0x000f, 0x0000, 0x0bb8, 0xfc18, 0x003f,
    0x000f, 0xfc9e, 0x0bb8, 0xfe0c, 0x002c, 0x000f, 0xfc9e, 0x0bb8, 0x01f4,
    0x0016, 0x000f, 0x0000, 0x0bb8, 0x03e8, 0x0000, 0x000f, 0x0000, 0x1770,
    0x03e8, 0x0000, 0x001f, 0x0362, 0x1770, 0x01f4, 0x0016, 0x001f, 0x0362,
    0x1770, 0xfe0c, 0x002c, 0x001f, 0x0000, 0x1770, 0xfc18, 0x003f, 0x001f,
    0xfc9e, 0x1770, 0xfe0c, 0x002c, 0x001f, 0xfc9e, 0x1770, 0x01f4, 0x0016,
    0x001f, 0x0000, 0x1770, 0x03e8, 0x0000, 0x001f, 0x0000, 0x0000, 0x0001,
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
    0x0000, 0x00fa, 0x00fa, 0x00fa, 0x0001, 0x0000, 0x0000, 0x0000,
};
RESOURCE_ACQUIRE_ADAPTER(gDll9DResourceDescriptorAcquire, dll_9D_initialise)

Dll9DResourceDescriptor gDll9DResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll9DResourceDescriptorAcquire, dll_9D_release },
    NULL,
    dll_9D_spawnEffect,
};
