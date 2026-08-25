/*
 * DLL 158 / 0x9E - a multi-layer pickup effect spawner.
 */
#include "main/dll/dll_009E_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll9ESevenIndexList {
    s16 indices[7];
    s16 opaqueTail;
} Dll9ESevenIndexList;

STATIC_ASSERT(offsetof(Dll9ESevenIndexList, indices) == 0x00);
STATIC_ASSERT(offsetof(Dll9ESevenIndexList, opaqueTail) == 0x0E);
STATIC_ASSERT(sizeof(Dll9ESevenIndexList) == 0x10);

typedef struct Dll9EEffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 opaqueD2[2];
    s16 triangles[24][3];
    Dll9ESevenIndexList sevenVertexIndexLists[3];
    s16 firstAndThirdVertexIndices[14];
    s16 allVertexIndices[21];
    s16 opaque1DA;
    s16 lastFourteenVertexIndices[14];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll9EEffectResourceView;

STATIC_ASSERT(offsetof(Dll9EEffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll9EEffectResourceView, opaqueD2) == 0x0D2);
STATIC_ASSERT(offsetof(Dll9EEffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll9EEffectResourceView, sevenVertexIndexLists) == 0x164);
STATIC_ASSERT(offsetof(Dll9EEffectResourceView, firstAndThirdVertexIndices) == 0x194);
STATIC_ASSERT(offsetof(Dll9EEffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll9EEffectResourceView, opaque1DA) == 0x1DA);
STATIC_ASSERT(offsetof(Dll9EEffectResourceView, lastFourteenVertexIndices) == 0x1DC);
STATIC_ASSERT(offsetof(Dll9EEffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(offsetof(Dll9EEffectResourceView, opaqueTail) == 0x206);
STATIC_ASSERT(sizeof(Dll9EEffectResourceView) == 0x208);

extern u16 gDll9EEffectResourceData[sizeof(Dll9EEffectResourceView) / sizeof(u16)];

void dll_9E_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 flags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)gDll9EEffectResourceData;
    GfxCmd* commands = packet.entries;
    u32 spawnFlags;

    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(Dll9EEffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    commands[1].layer = 0;
    commands[1].flags = 0x15;
    commands[1].tex = &resourceData[offsetof(Dll9EEffectResourceView, allVertexIndices)];
    commands[1].mode = 2;
    commands[1].x = 1.0f;
    commands[1].y = 20.0f;
    commands[1].z = 1.0f;
    commands[2].layer = 0;
    commands[2].flags = 0;
    commands[2].tex = NULL;
    commands[2].mode = 0x400000;
    commands[2].x = 0.0f;
    commands[2].y = -300.0f;
    commands[2].z = 0.0f;
    commands[3].layer = 1;
    commands[3].flags = 0x15;
    commands[3].tex = &resourceData[offsetof(Dll9EEffectResourceView, allVertexIndices)];
    commands[3].mode = 2;
    commands[3].x = 1.5f;
    commands[3].y = 1.5f;
    commands[3].z = 1.5f;
    commands[4].layer = 1;
    commands[4].flags = 7;
    commands[4].tex = &resourceData[offsetof(Dll9EEffectResourceView, sevenVertexIndexLists[1].indices)];
    commands[4].mode = 4;
    commands[4].x = 155.0f;
    commands[4].y = 0.0f;
    commands[4].z = 0.0f;
    commands[5].layer = 1;
    commands[5].flags = 0x15;
    commands[5].tex = &resourceData[offsetof(Dll9EEffectResourceView, allVertexIndices)];
    commands[5].mode = 0x4000;
    commands[5].x = 2.0f;
    commands[5].y = 0.0f;
    commands[5].z = 0.0f;
    commands[6].layer = 1;
    commands[6].flags = 0;
    commands[6].tex = NULL;
    commands[6].mode = 0x400000;
    commands[6].x = 0.0f;
    commands[6].y = 0.0f;
    commands[6].z = 0.0f;
    commands[7].layer = 2;
    commands[7].flags = 0x7a;
    commands[7].tex = NULL;
    commands[7].mode = 0x10000;
    commands[7].x = 0.0f;
    commands[7].y = 0.0f;
    commands[7].z = 0.0f;
    commands[8].layer = 2;
    commands[8].flags = 0x15;
    commands[8].tex = &resourceData[offsetof(Dll9EEffectResourceView, allVertexIndices)];
    commands[8].mode = 8;
    commands[8].x = 255.0f;
    commands[8].y = 125.0f;
    commands[8].z = 0.0f;
    commands[9].layer = 2;
    commands[9].flags = 0x15;
    commands[9].tex = &resourceData[offsetof(Dll9EEffectResourceView, allVertexIndices)];
    commands[9].mode = 0x4000;
    commands[9].x = 2.0f;
    commands[9].y = 0.0f;
    commands[9].z = 0.0f;
    commands[10].layer = 2;
    commands[10].flags = 0;
    commands[10].tex = NULL;
    commands[10].mode = 0x400000;
    commands[10].x = 0.0f;
    commands[10].y = 600.0f;
    commands[10].z = 0.0f;
    commands[11].layer = 3;
    commands[11].flags = 0x15;
    commands[11].tex = &resourceData[offsetof(Dll9EEffectResourceView, allVertexIndices)];
    commands[11].mode = 0x4000;
    commands[11].x = 2.0f;
    commands[11].y = 0.0f;
    commands[11].z = 0.0f;
    commands[12].layer = 3;
    commands[12].flags = 0;
    commands[12].tex = NULL;
    commands[12].mode = 0x400000;
    commands[12].x = 0.0f;
    commands[12].y = 600.0f;
    commands[12].z = 0.0f;
    commands[13].layer = 3;
    commands[13].flags = 7;
    commands[13].tex = &resourceData[offsetof(Dll9EEffectResourceView, sevenVertexIndexLists[1].indices)];
    commands[13].mode = 4;
    commands[13].x = 0.0f;
    commands[13].y = 0.0f;
    commands[13].z = 0.0f;

    packet.context.modeByte = 0;
    packet.context.attachedSource = sourceObj;
    packet.context.sourceMode = variant;
    packet.context.position[0] = 0.0f;
    packet.context.position[1] = 0.0f;
    packet.context.position[2] = 0.0f;
    packet.context.velocity[0] = 0.0f;
    packet.context.velocity[1] = 0.0f;
    packet.context.velocity[2] = 0.0f;
    packet.context.scale = 4.0f;
    packet.context.drawGroupCount = 2;
    packet.context.drawGroupStride = 7;
    packet.context.initialStateByte = 0xe;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x1e;
    packet.context.commandCount = (GfxCmd*)((u8*)commands + (int)sizeof(GfxCmd) * 14) - commands;
    packet.context.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll9EEffectResourceView, sequenceParams[0])];
    packet.context.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll9EEffectResourceView, sequenceParams[1])];
    packet.context.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll9EEffectResourceView, sequenceParams[2])];
    packet.context.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll9EEffectResourceView, sequenceParams[3])];
    packet.context.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll9EEffectResourceView, sequenceParams[4])];
    packet.context.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll9EEffectResourceView, sequenceParams[5])];
    packet.context.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll9EEffectResourceView, sequenceParams[6])];
    packet.context.commands = packet.entries;
    spawnFlags = 0xc0100c0;
    packet.context.flags = spawnFlags;
    spawnFlags |= flags;
    packet.context.flags = spawnFlags;
    if (spawnFlags & 1) {
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
        ->spawnEffect(&packet.context, 0, 0x15, (u8*)gDll9EEffectResourceData, 0x18,
                      &resourceData[offsetof(Dll9EEffectResourceView, triangles)], 0x46c, 0);
}

void dll_9E_release(void) {
}

void dll_9E_initialise(void) {
}

u16 gDll9EEffectResourceData[sizeof(Dll9EEffectResourceView) / sizeof(u16)] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0362, 0x0000, 0x01f4, 0x0016,
    0x0000, 0x0362, 0x0000, 0xfe0c, 0x002c, 0x0000, 0x0000, 0x0000, 0xfc18,
    0x003f, 0x0000, 0xfc9e, 0x0000, 0xfe0c, 0x002c, 0x0000, 0xfc9e, 0x0000,
    0x01f4, 0x0016, 0x0000, 0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0000,
    0x0bb8, 0x03e8, 0x0000, 0x003f, 0x0362, 0x0bb8, 0x01f4, 0x0016, 0x003f,
    0x0362, 0x0bb8, 0xfe0c, 0x002c, 0x003f, 0x0000, 0x0bb8, 0xfc18, 0x003f,
    0x003f, 0xfc9e, 0x0bb8, 0xfe0c, 0x002c, 0x003f, 0xfc9e, 0x0bb8, 0x01f4,
    0x0016, 0x003f, 0x0000, 0x0bb8, 0x03e8, 0x0000, 0x003f, 0x0000, 0x1770,
    0x03e8, 0x0000, 0x0000, 0x0362, 0x1770, 0x01f4, 0x0016, 0x0000, 0x0362,
    0x1770, 0xfe0c, 0x002c, 0x0000, 0x0000, 0x1770, 0xfc18, 0x003f, 0x0000,
    0xfc9e, 0x1770, 0xfe0c, 0x002c, 0x0000, 0xfc9e, 0x1770, 0x01f4, 0x0016,
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
    0x0000, 0x0096, 0x0096, 0x0096, 0x0001, 0x0000, 0x0000, 0x0000,
};
RESOURCE_ACQUIRE_ADAPTER(gDll9EResourceDescriptorAcquire, dll_9E_initialise)

Dll9EResourceDescriptor gDll9EResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll9EResourceDescriptorAcquire, dll_9E_release },
    NULL,
    dll_9E_spawnEffect,
};
