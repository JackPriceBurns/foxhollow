/*
 * DLL 129 / 0x81 - a multi-variant laser-beam modgfx effect spawner.
 */
#include "main/dll/dll_0081_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll81EffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 padD2[2];
    s16 triangles[24][3];
    u8 opaqueIndexData164[0x4C];
    s16 allVertexIndices[21];
    s16 opaque1DA;
    u8 opaqueIndexData1DC[0x1C];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll81EffectResourceView;

STATIC_ASSERT(offsetof(Dll81EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll81EffectResourceView, padD2) == 0x0D2);
STATIC_ASSERT(offsetof(Dll81EffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll81EffectResourceView, opaqueIndexData164) == 0x164);
STATIC_ASSERT(offsetof(Dll81EffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll81EffectResourceView, opaque1DA) == 0x1DA);
STATIC_ASSERT(offsetof(Dll81EffectResourceView, opaqueIndexData1DC) == 0x1DC);
STATIC_ASSERT(offsetof(Dll81EffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(offsetof(Dll81EffectResourceView, opaqueTail) == 0x206);
STATIC_ASSERT(sizeof(Dll81EffectResourceView) == 0x208);

u8 gDll81EffectResourceData[sizeof(Dll81EffectResourceView)] = {
    0,   0,   3,   232, 0,   0,   0,   0,   0,   0,   3,   98,  1,  244, 0,   0,   0,   8,   0,   0,   3,   98,  254,
    12,  0,   0,   0,   15,  0,   0,   0,   0,   252, 24,  0,   0,  0,   31,  0,   0,   252, 158, 254, 12,  0,   0,
    0,   15,  0,   0,   252, 158, 1,   244, 0,   0,   0,   8,   0,  0,   0,   0,   3,   232, 0,   0,   0,   0,   0,
    0,   0,   0,   3,   232, 11,  184, 0,   0,   0,   127, 3,   98, 1,   244, 11,  184, 0,   8,   0,   127, 3,   98,
    254, 12,  11,  184, 0,   15,  0,   127, 0,   0,   252, 24,  11, 184, 0,   31,  0,   127, 252, 158, 254, 12,  11,
    184, 0,   15,  0,   127, 252, 158, 1,   244, 11,  184, 0,   8,  0,   127, 0,   0,   3,   232, 11,  184, 0,   0,
    0,   127, 0,   0,   3,   232, 23,  112, 0,   0,   0,   255, 3,  98,  1,   244, 23,  112, 0,   8,   0,   255, 3,
    98,  254, 12,  23,  112, 0,   15,  0,   255, 0,   0,   252, 24, 23,  112, 0,   31,  0,   255, 252, 158, 254, 12,
    23,  112, 0,   15,  0,   255, 252, 158, 1,   244, 23,  112, 0,  8,   0,   255, 0,   0,   3,   232, 23,  112, 0,
    0,   0,   255, 0,   0,   0,   0,   0,   8,   0,   1,   0,   0,  0,   7,   0,   8,   0,   1,   0,   9,   0,   2,
    0,   1,   0,   8,   0,   9,   0,   2,   0,   10,  0,   3,   0,  2,   0,   9,   0,   10,  0,   3,   0,   11,  0,
    4,   0,   3,   0,   10,  0,   11,  0,   4,   0,   12,  0,   5,  0,   4,   0,   11,  0,   12,  0,   5,   0,   13,
    0,   6,   0,   5,   0,   12,  0,   13,  0,   7,   0,   15,  0,  8,   0,   7,   0,   14,  0,   15,  0,   8,   0,
    16,  0,   9,   0,   8,   0,   15,  0,   16,  0,   9,   0,   17, 0,   10,  0,   9,   0,   16,  0,   17,  0,   10,
    0,   18,  0,   11,  0,   10,  0,   17,  0,   18,  0,   11,  0,  19,  0,   12,  0,   11,  0,   18,  0,   19,  0,
    12,  0,   20,  0,   13,  0,   12,  0,   19,  0,   20,  0,   0,  0,   1,   0,   2,   0,   3,   0,   4,   0,   5,
    0,   6,   0,   0,   0,   7,   0,   8,   0,   9,   0,   10,  0,  11,  0,   12,  0,   13,  0,   0,   0,   14,  0,
    15,  0,   16,  0,   17,  0,   18,  0,   19,  0,   20,  0,   0,  0,   0,   0,   1,   0,   2,   0,   3,   0,   4,
    0,   5,   0,   6,   0,   14,  0,   15,  0,   16,  0,   17,  0,  18,  0,   19,  0,   20,  0,   0,   0,   1,   0,
    2,   0,   3,   0,   4,   0,   5,   0,   6,   0,   7,   0,   8,  0,   9,   0,   10,  0,   11,  0,   12,  0,   13,
    0,   14,  0,   15,  0,   16,  0,   17,  0,   18,  0,   19,  0,  20,  0,   0,   0,   7,   0,   8,   0,   9,   0,
    10,  0,   11,  0,   12,  0,   13,  0,   14,  0,   15,  0,   16, 0,   17,  0,   18,  0,   19,  0,   20,  0,   0,
    0,   4,   0,   10,  0,   1,   0,   36,  0,   0,   0,   0,   0,  0};

s16 dll_81_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int owner,
                       int unused) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll81EffectResourceData;
    f32 verticalStep = -3.0f;
    GfxCmd* commandCursor;
    GfxCmd* commands;
    if (variant == 0 || variant == 2 || variant == 0x1e) {
        *(s16*)&resourceData[offsetof(Dll81EffectResourceView, sequenceParams[1])] = 0xc;
    } else if (variant == 1 || variant == 3) {
        verticalStep *= -1.0f;
        *(s16*)&resourceData[offsetof(Dll81EffectResourceView, sequenceParams[1])] = 4;
        *(s16*)&resourceData[offsetof(Dll81EffectResourceView, sequenceParams[4])] = 0x32;
    }
    commands = packet.entries;
    commandCursor = &commands[1];
    commands[0].layer = 0;
    commands[0].flags = 0x15;
    commands[0].tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
    commands[0].mode = 4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    if (variant == 0 || variant == 2) {
        commandCursor->layer = 0;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 0.35f;
        commandCursor->y = 0.35f;
        commandCursor->z = 0.01f;
        commandCursor++;
    } else if (variant == 0xe) {
        commandCursor->layer = 0;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 0.2f;
        commandCursor->y = 0.2f;
        commandCursor->z = 5.0f;
        commandCursor++;
    } else if (variant == 0x1e) {
        commandCursor->layer = 0;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 0.45f;
        commandCursor->y = 0.45f;
        commandCursor->z = 0.01f;
        commandCursor++;
    } else {
        commandCursor->layer = 0;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 0.35f;
        commandCursor->y = 0.35f;
        commandCursor->z = 3.0f;
        commandCursor++;
    }
    commandCursor[0].layer = 0;
    commandCursor[0].flags = 0x77;
    commandCursor[0].tex = NULL;
    commandCursor[0].mode = 0x10000;
    commandCursor[0].x = 0.0f;
    commandCursor[0].y = 0.0f;
    commandCursor[0].z = 0.0f;
    commandCursor[1].layer = 0;
    commandCursor[1].flags = 0x79;
    commandCursor[1].tex = NULL;
    commandCursor[1].mode = 0x10000;
    commandCursor[1].x = 0.0f;
    commandCursor[1].y = 0.0f;
    commandCursor[1].z = 0.0f;
    commandCursor[2].layer = 1;
    commandCursor[2].flags = 0x15;
    commandCursor[2].tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
    commandCursor[2].mode = 4;
    commandCursor[2].x = 255.0f;
    commandCursor[2].y = 0.0f;
    commandCursor[2].z = 0.0f;
    commandCursor += 3;
    if (variant == 0 || variant == 2) {
        commandCursor->layer = 1;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 1.0f;
        commandCursor->y = 1.0f;
        commandCursor->z = 300.0f;
        commandCursor++;
    } else if (variant == 0x1e) {
        commandCursor->layer = 1;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 1.0f;
        commandCursor->y = 1.0f;
        commandCursor->z = 460.0f;
        commandCursor++;
    }
    commandCursor[0].layer = 1;
    commandCursor[0].flags = 0x15;
    commandCursor[0].tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
    commandCursor[0].mode = 0x4000;
    commandCursor[0].x = 1.0f;
    commandCursor[0].y = verticalStep;
    commandCursor[0].z = 0.0f;
    commandCursor[1].layer = 2;
    commandCursor[1].flags = 0x15;
    commandCursor[1].tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
    commandCursor[1].mode = 4;
    commandCursor[1].x = 255.0f;
    commandCursor[1].y = 0.0f;
    commandCursor[1].z = 0.0f;
    commandCursor[2].layer = 2;
    commandCursor[2].flags = 0x15;
    commandCursor[2].tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
    commandCursor[2].mode = 0x4000;
    commandCursor[2].x = 1.0f;
    commandCursor[2].y = verticalStep;
    commandCursor[2].z = 0.0f;
    commandCursor[3].layer = 3;
    commandCursor[3].flags = 0x15;
    commandCursor[3].tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
    commandCursor[3].mode = 0x4000;
    commandCursor[3].x = 1.0f;
    commandCursor[3].y = verticalStep;
    commandCursor[3].z = 0.0f;
    commandCursor[4].layer = 4;
    commandCursor[4].flags = 0x15;
    commandCursor[4].tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
    commandCursor[4].mode = 0x4000;
    commandCursor[4].x = 1.0f;
    commandCursor[4].y = verticalStep;
    commandCursor[4].z = 0.0f;
    commandCursor += 5;
    if (variant == 0 || variant == 0x1e) {
        commandCursor->layer = 4;
        commandCursor->flags = 2;
        commandCursor->tex = NULL;
        commandCursor->mode = 0x2000;
        commandCursor->x = 0.0f;
        commandCursor->y = 0.0f;
        commandCursor->z = 0.0f;
        commandCursor++;
    }
    commandCursor[0].layer = 5;
    commandCursor[0].flags = 0x15;
    commandCursor[0].tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
    commandCursor[0].mode = 0x4000;
    commandCursor[0].x = 1.0f;
    commandCursor[0].y = verticalStep;
    commandCursor[0].z = 0.0f;
    commandCursor[1].layer = 5;
    commandCursor[1].flags = 0x15;
    commandCursor[1].tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
    commandCursor[1].mode = 4;
    commandCursor[1].x = 0.0f;
    commandCursor[1].y = 0.0f;
    commandCursor[1].z = 0.0f;
    commandCursor += 2;
    if (variant == 1 || variant == 3) {
        commandCursor->layer = 5;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll81EffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 1.0f;
        commandCursor->y = 1.0f;
        commandCursor->z = 0.01f;
        commandCursor++;
    }
    commandCursor[0].layer = 5;
    commandCursor[0].flags = 0x78;
    commandCursor[0].tex = NULL;
    commandCursor[0].mode = 0x10000;
    commandCursor[0].x = 0.0f;
    commandCursor[0].y = 0.0f;
    commandCursor[0].z = 0.0f;
    commandCursor[1].layer = 5;
    commandCursor[1].flags = -1;
    commandCursor[1].tex = NULL;
    commandCursor[1].mode = 0x10000;
    commandCursor[1].x = 0.0f;
    commandCursor[1].y = 0.0f;
    commandCursor[1].z = 0.0f;
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
    packet.textureFrameTimer = 0xa;
    packet.commandCount = (GfxCmd*)((u8*)commandCursor + sizeof(GfxCmd) * 2) - commands;
    packet.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll81EffectResourceView, sequenceParams[0])];
    packet.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll81EffectResourceView, sequenceParams[1])];
    packet.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll81EffectResourceView, sequenceParams[2])];
    packet.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll81EffectResourceView, sequenceParams[3])];
    packet.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll81EffectResourceView, sequenceParams[4])];
    packet.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll81EffectResourceView, sequenceParams[5])];
    packet.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll81EffectResourceView, sequenceParams[6])];
    packet.commands = (GfxCmd*)((u8*)&packet + 0x60);
    packet.flags = 0xc0104c0;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if ((u32)sourceObj != 0) {
            packet.position[0] += sourceObj->anim.worldPosX;
            packet.position[1] += sourceObj->anim.worldPosY;
            packet.position[2] += sourceObj->anim.worldPosZ;
        } else {
            packet.position[0] += spawnParams->posX;
            packet.position[1] += spawnParams->posY;
            packet.position[2] += spawnParams->posZ;
        }
    }
    if (variant == 0x1e) {
        return (*gModgfxInterface)
            ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll81EffectResourceData, 0x18,
                          &resourceData[offsetof(Dll81EffectResourceView, triangles)], 0x3e9, 0);
    } else if (variant == 2 || variant == 3) {
        return (*gModgfxInterface)
            ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll81EffectResourceData, 0x18,
                          &resourceData[offsetof(Dll81EffectResourceView, triangles)], 0x23d, 0);
    } else if ((u32)(variant - 10) <= 3 || variant == 0xe) {
        return (*gModgfxInterface)
            ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll81EffectResourceData, 0x18,
                          &resourceData[offsetof(Dll81EffectResourceView, triangles)], 0x2e, 0);
    } else {
        return (*gModgfxInterface)
            ->spawnEffect(&packet, 0, 0x15, (u8*)(int)gDll81EffectResourceData, 0x18,
                          &resourceData[offsetof(Dll81EffectResourceView, triangles)], 0xd9, 0);
    }
}

void dll_81_release(void) {
}

void dll_81_initialise(void) {
}

Dll81ResourceDescriptor gDll81ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_81_initialise, dll_81_release, NULL, dll_81_spawnEffect,
};
