/*
 * DLL 124 / 0x7C - a six-variant foodbag modgfx effect spawner.
 */
#include "main/dll/dll_007C_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll7CEffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 padD2[2];
    s16 triangles[24][3];
    s16 firstSevenVertexIndices[8];
    s16 secondSevenVertexIndices[8];
    s16 thirdSevenVertexIndices[8];
    s16 firstAndThirdVertexIndices[14];
    s16 allVertexIndices[22];
    s16 lastFourteenVertexIndices[14];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll7CEffectResourceView;

STATIC_ASSERT(offsetof(Dll7CEffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll7CEffectResourceView, padD2) == 0x0D2);
STATIC_ASSERT(offsetof(Dll7CEffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll7CEffectResourceView, firstSevenVertexIndices) == 0x164);
STATIC_ASSERT(offsetof(Dll7CEffectResourceView, secondSevenVertexIndices) == 0x174);
STATIC_ASSERT(offsetof(Dll7CEffectResourceView, thirdSevenVertexIndices) == 0x184);
STATIC_ASSERT(offsetof(Dll7CEffectResourceView, firstAndThirdVertexIndices) == 0x194);
STATIC_ASSERT(offsetof(Dll7CEffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll7CEffectResourceView, lastFourteenVertexIndices) == 0x1DC);
STATIC_ASSERT(offsetof(Dll7CEffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(offsetof(Dll7CEffectResourceView, opaqueTail) == 0x206);
STATIC_ASSERT(sizeof(Dll7CEffectResourceView) == 0x208);

u8 gFoodbagEffectResourceTable[sizeof(Dll7CEffectResourceView)] = {
    0x00, 0x00, 0x00, 0x00, 0x03, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x03, 0x62, 0x00, 0x00, 0x01, 0xF4, 0x00, 0x0B, 0x00,
    0x00, 0x03, 0x62, 0x00, 0x00, 0xFE, 0x0C, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x18, 0x00, 0x20,
    0x00, 0x00, 0xFC, 0x9E, 0x00, 0x00, 0xFE, 0x0C, 0x00, 0x16, 0x00, 0x00, 0xFC, 0x9E, 0x00, 0x00, 0x01, 0xF4, 0x00,
    0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF4, 0x03, 0xE8,
    0x00, 0x00, 0x00, 0x0F, 0x03, 0x62, 0x01, 0xF4, 0x01, 0xF4, 0x00, 0x0B, 0x00, 0x0F, 0x03, 0x62, 0x01, 0xF4, 0xFE,
    0x0C, 0x00, 0x16, 0x00, 0x0F, 0x00, 0x00, 0x01, 0xF4, 0xFC, 0x18, 0x00, 0x20, 0x00, 0x0F, 0xFC, 0x9E, 0x01, 0xF4,
    0xFE, 0x0C, 0x00, 0x16, 0x00, 0x0F, 0xFC, 0x9E, 0x01, 0xF4, 0x01, 0xF4, 0x00, 0x0B, 0x00, 0x0F, 0x00, 0x00, 0x01,
    0xF4, 0x03, 0xE8, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x17, 0x70, 0x03, 0xE8, 0x00, 0x00, 0x00, 0x7F, 0x03, 0x62,
    0x17, 0x70, 0x01, 0xF4, 0x00, 0x0B, 0x00, 0x7F, 0x03, 0x62, 0x17, 0x70, 0xFE, 0x0C, 0x00, 0x16, 0x00, 0x7F, 0x00,
    0x00, 0x17, 0x70, 0xFC, 0x18, 0x00, 0x20, 0x00, 0x7F, 0xFC, 0x9E, 0x17, 0x70, 0xFE, 0x0C, 0x00, 0x16, 0x00, 0x7F,
    0xFC, 0x9E, 0x17, 0x70, 0x01, 0xF4, 0x00, 0x0B, 0x00, 0x7F, 0x00, 0x00, 0x17, 0x70, 0x03, 0xE8, 0x00, 0x00, 0x00,
    0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x07, 0x00, 0x01, 0x00, 0x02,
    0x00, 0x09, 0x00, 0x01, 0x00, 0x09, 0x00, 0x08, 0x00, 0x02, 0x00, 0x03, 0x00, 0x0A, 0x00, 0x02, 0x00, 0x0A, 0x00,
    0x09, 0x00, 0x03, 0x00, 0x04, 0x00, 0x0B, 0x00, 0x03, 0x00, 0x0B, 0x00, 0x0A, 0x00, 0x04, 0x00, 0x05, 0x00, 0x0C,
    0x00, 0x04, 0x00, 0x0C, 0x00, 0x0B, 0x00, 0x05, 0x00, 0x06, 0x00, 0x0D, 0x00, 0x05, 0x00, 0x0D, 0x00, 0x0C, 0x00,
    0x07, 0x00, 0x08, 0x00, 0x0F, 0x00, 0x07, 0x00, 0x0F, 0x00, 0x0E, 0x00, 0x08, 0x00, 0x09, 0x00, 0x10, 0x00, 0x08,
    0x00, 0x10, 0x00, 0x0F, 0x00, 0x09, 0x00, 0x0A, 0x00, 0x11, 0x00, 0x09, 0x00, 0x11, 0x00, 0x10, 0x00, 0x0A, 0x00,
    0x0B, 0x00, 0x12, 0x00, 0x0A, 0x00, 0x12, 0x00, 0x11, 0x00, 0x0B, 0x00, 0x0C, 0x00, 0x13, 0x00, 0x0B, 0x00, 0x13,
    0x00, 0x12, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x14, 0x00, 0x0C, 0x00, 0x14, 0x00, 0x13, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0A,
    0x00, 0x0B, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x0F, 0x00, 0x10, 0x00, 0x11, 0x00, 0x12, 0x00,
    0x13, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06,
    0x00, 0x0E, 0x00, 0x0F, 0x00, 0x10, 0x00, 0x11, 0x00, 0x12, 0x00, 0x13, 0x00, 0x14, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0A, 0x00, 0x0B,
    0x00, 0x0C, 0x00, 0x0D, 0x00, 0x0E, 0x00, 0x0F, 0x00, 0x10, 0x00, 0x11, 0x00, 0x12, 0x00, 0x13, 0x00, 0x14, 0x00,
    0x00, 0x00, 0x07, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0A, 0x00, 0x0B, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x0E, 0x00, 0x0F,
    0x00, 0x10, 0x00, 0x11, 0x00, 0x12, 0x00, 0x13, 0x00, 0x14, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x3C, 0x00, 0x1E, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void dll_7C_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = gFoodbagEffectResourceTable;
    GfxCmd* commands = packet.entries;
    GfxCmd* commandCursor = &commands[1];

    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(Dll7CEffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    if (variant == 0 || variant == 3) {
        commandCursor->layer = 0;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll7CEffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 0.5f;
        commandCursor->y = 0.05f;
        commandCursor->z = 0.5f;
        commandCursor++;
    } else if (variant == 1 || variant == 2) {
        commandCursor->layer = 0;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll7CEffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 0.35f;
        commandCursor->y = 0.05f;
        commandCursor->z = 0.35f;
        commandCursor++;
    } else {
        commandCursor->layer = 0;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll7CEffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 0.35f;
        commandCursor->y = 0.05f;
        commandCursor->z = 0.35f;
        commandCursor++;
    }
    commandCursor[0].layer = 0;
    commandCursor[0].flags = 0;
    commandCursor[0].tex = NULL;
    commandCursor[0].mode = 0x400000;
    commandCursor[0].x = 0.0f;
    commandCursor[0].y = -10.0f;
    commandCursor[0].z = 0.0f;
    commandCursor[1].layer = 1;
    commandCursor[1].flags = 0x15;
    commandCursor[1].tex = &resourceData[offsetof(Dll7CEffectResourceView, allVertexIndices)];
    commandCursor[1].mode = 2;
    commandCursor[1].x = 1.0f;
    commandCursor[1].y = 10.0f;
    commandCursor[1].z = 1.0f;
    commandCursor[2].layer = 1;
    commandCursor[2].flags = 7;
    commandCursor[2].tex = &resourceData[offsetof(Dll7CEffectResourceView, firstSevenVertexIndices)];
    commandCursor[2].mode = 4;
    commandCursor[2].x = 155.0f;
    commandCursor[2].y = 0.0f;
    commandCursor[2].z = 0.0f;
    commandCursor[3].layer = 1;
    commandCursor[3].flags = 7;
    commandCursor[3].tex = &resourceData[offsetof(Dll7CEffectResourceView, secondSevenVertexIndices)];
    commandCursor[3].mode = 4;
    commandCursor[3].x = 55.0f;
    commandCursor[3].y = 0.0f;
    commandCursor[3].z = 0.0f;
    commandCursor[4].layer = 1;
    commandCursor[4].flags = 0x15;
    commandCursor[4].tex = &resourceData[offsetof(Dll7CEffectResourceView, allVertexIndices)];
    commandCursor[4].mode = 0x4000;
    commandCursor[4].x = 4.0f;
    commandCursor[4].y = 8.0f;
    commandCursor[4].z = 0.0f;
    commandCursor[5].layer = 1;
    commandCursor[5].flags = 0;
    commandCursor[5].tex = NULL;
    commandCursor[5].mode = 0x400000;
    commandCursor[5].x = 0.0f;
    commandCursor[5].y = 15.0f;
    commandCursor[5].z = 0.0f;
    commandCursor[6].layer = 2;
    commandCursor[6].flags = 0x1e;
    commandCursor[6].tex = NULL;
    commandCursor[6].mode = 0x20000;
    commandCursor[6].x = 1.0f;
    commandCursor[6].y = 0.0f;
    commandCursor[6].z = 0.0f;
    commandCursor[7].layer = 2;
    commandCursor[7].flags = 0x15;
    commandCursor[7].tex = &resourceData[offsetof(Dll7CEffectResourceView, allVertexIndices)];
    commandCursor[7].mode = 0x4000;
    commandCursor[7].x = 4.0f;
    commandCursor[7].y = 8.0f;
    commandCursor[7].z = 0.0f;
    commandCursor[8].layer = 2;
    commandCursor[8].flags = 0;
    commandCursor[8].tex = NULL;
    commandCursor[8].mode = 0x400000;
    commandCursor[8].x = 0.0f;
    commandCursor[8].y = 30.0f;
    commandCursor[8].z = 0.0f;
    commandCursor[9].layer = 3;
    commandCursor[9].flags = 0x15;
    commandCursor[9].tex = &resourceData[offsetof(Dll7CEffectResourceView, allVertexIndices)];
    commandCursor[9].mode = 0x4000;
    commandCursor[9].x = 4.0f;
    commandCursor[9].y = 8.0f;
    commandCursor[9].z = 0.0f;
    commandCursor[10].layer = 3;
    commandCursor[10].flags = 7;
    commandCursor[10].tex = &resourceData[offsetof(Dll7CEffectResourceView, firstSevenVertexIndices)];
    commandCursor[10].mode = 4;
    commandCursor[10].x = 0.0f;
    commandCursor[10].y = 0.0f;
    commandCursor[10].z = 0.0f;
    commandCursor[11].layer = 3;
    commandCursor[11].flags = 7;
    commandCursor[11].tex = &resourceData[offsetof(Dll7CEffectResourceView, secondSevenVertexIndices)];
    commandCursor[11].mode = 4;
    commandCursor[11].x = 0.0f;
    commandCursor[11].y = 0.0f;
    commandCursor[11].z = 0.0f;
    commandCursor[12].layer = 3;
    commandCursor[12].flags = 0x1e;
    commandCursor[12].tex = NULL;
    commandCursor[12].mode = 0x20000;
    commandCursor[12].x = 1.0f;
    commandCursor[12].y = 0.0f;
    commandCursor[12].z = 0.0f;
    commandCursor[13].layer = 3;
    commandCursor[13].flags = 0;
    commandCursor[13].tex = NULL;
    commandCursor[13].mode = 0x400000;
    commandCursor[13].x = 0.0f;
    commandCursor[13].y = 15.0f;
    commandCursor[13].z = 0.0f;
    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = 0.0f;
    packet.position[1] = 0.0f;
    packet.position[2] = 0.0f;
    switch (variant) {
    case 0:
        packet.position[0] = 0.0f;
        packet.position[2] = 23.0f;
        break;
    case 1:
        packet.position[0] = -17.0f;
        packet.position[2] = 18.0f;
        break;
    case 2:
        packet.position[0] = 17.0f;
        packet.position[2] = 18.0f;
        break;
    case 3:
        packet.position[0] = 0.0f;
        packet.position[2] = -26.0f;
        break;
    case 4:
        packet.position[0] = -17.0f;
        packet.position[2] = -12.0f;
        break;
    case 5:
        packet.position[0] = 17.0f;
        packet.position[2] = -12.0f;
        break;
    }
    packet.velocity[0] = 0.0f;
    packet.velocity[1] = 0.0f;
    packet.velocity[2] = 0.0f;
    packet.scale = 1.0f;
    packet.drawGroupCount = 2;
    packet.drawGroupStride = 7;
    packet.initialStateByte = 0xe;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0xa;
    packet.commandCount = (GfxCmd*)((u8*)commandCursor + sizeof(GfxCmd) * 14) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll7CEffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll7CEffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll7CEffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll7CEffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll7CEffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll7CEffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll7CEffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + 0x60);
    packet.flags = 0xc010080;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if ((u32)packet.sourceObj != 0) {
            packet.position[0] += packet.sourceObj->anim.worldPosX;
            packet.position[1] += packet.sourceObj->anim.worldPosY;
            packet.position[2] += packet.sourceObj->anim.worldPosZ;
        } else {
            packet.position[0] += spawnParams->posX;
            packet.position[1] += spawnParams->posY;
            packet.position[2] += spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 0x15, resourceData, 0x18, &resourceData[offsetof(Dll7CEffectResourceView, triangles)],
                      0x2e, 0);
}

void dll_7C_release(void) {
}

void dll_7C_initialise(void) {
}

Dll7CResourceDescriptor gDll7CResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_7C_initialise, dll_7C_release, NULL, dll_7C_spawnEffect,
};
