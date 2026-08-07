/*
 * DLL 126 / 0x7E - a scaled foodbag billboard-effect spawner.
 */
#include "main/dll/dll_007E_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll7EEffectResourceView {
    ModgfxEffectVertex vertices[9];
    u8 pad5A[2];
    s16 triangles[5][3];
    u8 pad7A[2];
    s16 allVertexIndices[10];
    s16 lastSixVertexIndices[6];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll7EEffectResourceView;

STATIC_ASSERT(offsetof(Dll7EEffectResourceView, vertices) == 0x00);
STATIC_ASSERT(offsetof(Dll7EEffectResourceView, pad5A) == 0x5A);
STATIC_ASSERT(offsetof(Dll7EEffectResourceView, triangles) == 0x5C);
STATIC_ASSERT(offsetof(Dll7EEffectResourceView, pad7A) == 0x7A);
STATIC_ASSERT(offsetof(Dll7EEffectResourceView, allVertexIndices) == 0x7C);
STATIC_ASSERT(offsetof(Dll7EEffectResourceView, lastSixVertexIndices) == 0x90);
STATIC_ASSERT(offsetof(Dll7EEffectResourceView, sequenceParams) == 0x9C);
STATIC_ASSERT(offsetof(Dll7EEffectResourceView, opaqueTail) == 0xAA);
STATIC_ASSERT(sizeof(Dll7EEffectResourceView) == 0xAC);

s16 gDll7EThreeVertexIndices[4] = {0, 1, 2, 0};

u8 gDll7EEffectResourceData[sizeof(Dll7EEffectResourceView)] = {
    0, 0,   0, 0,   254, 12,  0,   31,  0, 0,   0, 0,   0, 0, 254, 12,  0, 31,  0, 0,   0, 0,   0,   0,   254, 12,
    0, 31,  0, 0,   0,   107, 0,   100, 1, 151, 0, 78,  0, 8, 2,   195, 0, 100, 2, 195, 0, 63,  0,   31,  2,   226,
    0, 100, 1, 127, 0,   46,  0,   31,  2, 168, 0, 100, 0, 0, 0,   31,  0, 31,  1, 119, 0, 100, 254, 130, 0,   15,
    0, 31,  0, 141, 0,   100, 254, 105, 0, 0,   0, 31,  0, 0, 0,   0,   0, 4,   0, 3,   0, 1,   0,   5,   0,   4,
    0, 1,   0, 6,   0,   5,   0,   1,   0, 7,   0, 6,   0, 2, 0,   8,   0, 7,   0, 0,   0, 0,   0,   1,   0,   2,
    0, 3,   0, 4,   0,   5,   0,   6,   0, 7,   0, 8,   0, 0, 0,   3,   0, 4,   0, 5,   0, 6,   0,   7,   0,   8,
    0, 0,   0, 10,  0,   40,  0,   0,   0, 0,   0, 0,   0, 0, 0,   0};

void dll_7E_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, u32 unused,
                        f32* scaleOverride) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll7EEffectResourceData;
    f32 scale = 1.0f;
    f32 originOffset = 0.0f;
    GfxCmd* commands;
    GfxCmd* commandCursor;
    if (scaleOverride != NULL) {
        scale = *scaleOverride;
    }
    if ((u32)spawnParams != 0) {
        scale = spawnParams->scale;
    }
    commands = packet.entries;
    commandCursor = &commands[2];
    commands[0].layer = 0;
    commands[0].flags = 5;
    commands[0].tex = &resourceData[offsetof(Dll7EEffectResourceView, lastSixVertexIndices)];
    commands[0].mode = 0x4000;
    commands[0].x = originOffset;
    commands[0].y = -1.0f;
    commands[0].z = originOffset;
    commands[1].layer = 0;
    commands[1].flags = 9;
    commands[1].tex = &resourceData[offsetof(Dll7EEffectResourceView, allVertexIndices)];
    commands[1].mode = 4;
    commands[1].x = originOffset;
    commands[1].y = originOffset;
    commands[1].z = originOffset;
    if (variant == 1) {
        commandCursor->layer = 0;
        commandCursor->flags = 9;
        commandCursor->tex = &resourceData[offsetof(Dll7EEffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = -6.0f * scale;
        commandCursor->y = 1.0f;
        commandCursor->z = 4.0f;
        commandCursor++;
    } else {
        commandCursor->layer = 0;
        commandCursor->flags = 9;
        commandCursor->tex = &resourceData[offsetof(Dll7EEffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 6.0f * scale;
        commandCursor->y = 1.0f;
        commandCursor->z = 4.0f;
        commandCursor++;
    }
    commandCursor[0].layer = 1;
    commandCursor[0].flags = 3;
    commandCursor[0].tex = gDll7EThreeVertexIndices;
    commandCursor[0].mode = 4;
    commandCursor[0].x = 255.0f;
    commandCursor[0].y = originOffset;
    commandCursor[0].z = originOffset;
    commandCursor[1].layer = 1;
    commandCursor[1].flags = 5;
    commandCursor[1].tex = &resourceData[offsetof(Dll7EEffectResourceView, lastSixVertexIndices)];
    commandCursor[1].mode = 0x4000;
    commandCursor[1].x = -2.5f;
    commandCursor[1].y = -1.0f;
    commandCursor[1].z = originOffset;
    commandCursor[2].layer = 2;
    commandCursor[2].flags = 5;
    commandCursor[2].tex = &resourceData[offsetof(Dll7EEffectResourceView, lastSixVertexIndices)];
    commandCursor[2].mode = 0x4000;
    commandCursor[2].x = -2.5f;
    commandCursor[2].y = -1.0f;
    commandCursor[2].z = originOffset;
    commandCursor[3].layer = 2;
    commandCursor[3].flags = 3;
    commandCursor[3].tex = gDll7EThreeVertexIndices;
    commandCursor[3].mode = 4;
    commandCursor[3].x = originOffset;
    commandCursor[3].y = originOffset;
    commandCursor[3].z = originOffset;
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
    packet.drawGroupStride = 9;
    packet.initialStateByte = 9;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0xa;
    packet.commandCount = (GfxCmd*)((u8*)commandCursor + sizeof(GfxCmd) * 4) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll7EEffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll7EEffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll7EEffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll7EEffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll7EEffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll7EEffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll7EEffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + 0x60);
    packet.flags = 0x4010080;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if ((u32)sourceObj != 0) {
            packet.position[0] = originOffset + sourceObj->anim.worldPosX;
            packet.position[1] = originOffset + sourceObj->anim.worldPosY;
            packet.position[2] = originOffset + sourceObj->anim.worldPosZ;
        } else {
            packet.position[0] = originOffset + spawnParams->posX;
            packet.position[1] = originOffset + spawnParams->posY;
            packet.position[2] = originOffset + spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 9, (u8*)(int)gDll7EEffectResourceData, 5,
                      &resourceData[offsetof(Dll7EEffectResourceView, triangles)], 0x3c, 0);
}

void dll_7E_release(void) {
}

void dll_7E_initialise(void) {
}

Dll7EResourceDescriptor gDll7EResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    dll_7E_initialise,
    dll_7E_release,
    NULL,
    dll_7E_spawnEffect,
    0x00000000,
};
