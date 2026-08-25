/*
 * DLL 128 / 0x80 - a two-scale foodbag modgfx effect spawner.
 */
#include "main/dll/dll_0080_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll80EffectResourceView {
    ModgfxEffectVertex vertices[9];
    u8 pad5A[2];
    s16 triangles[8][3];
    s16 allVertexIndices[10];
    s16 firstEightVertexIndices[8];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll80EffectResourceView;

STATIC_ASSERT(offsetof(Dll80EffectResourceView, vertices) == 0x00);
STATIC_ASSERT(offsetof(Dll80EffectResourceView, pad5A) == 0x5A);
STATIC_ASSERT(offsetof(Dll80EffectResourceView, triangles) == 0x5C);
STATIC_ASSERT(offsetof(Dll80EffectResourceView, allVertexIndices) == 0x8C);
STATIC_ASSERT(offsetof(Dll80EffectResourceView, firstEightVertexIndices) == 0xA0);
STATIC_ASSERT(offsetof(Dll80EffectResourceView, sequenceParams) == 0xB0);
STATIC_ASSERT(offsetof(Dll80EffectResourceView, opaqueTail) == 0xBE);
STATIC_ASSERT(sizeof(Dll80EffectResourceView) == 0xC0);

u8 gDll80EffectResourceData[sizeof(Dll80EffectResourceView)] = {
    3, 232, 0,   0,   1, 144, 0,   31,  0,   31,  4, 83,  251, 173, 1, 144, 0,   0,   0, 31, 0, 0,   252, 24,
    1, 144, 0,   31,  0, 31,  251, 173, 251, 173, 1, 144, 0,   0,   0, 31,  252, 24,  0, 0,  1, 144, 0,   31,
    0, 31,  251, 173, 4, 83,  1,   144, 0,   0,   0, 31,  0,   0,   3, 232, 1,   144, 0, 31, 0, 31,  4,   83,
    4, 83,  1,   144, 0, 0,   0,   31,  0,   0,   0, 0,   0,   0,   0, 15,  0,   0,   0, 0,  0, 0,   0,   1,
    0, 8,   0,   1,   0, 2,   0,   8,   0,   2,   0, 3,   0,   8,   0, 3,   0,   4,   0, 8,  0, 4,   0,   5,
    0, 8,   0,   5,   0, 6,   0,   8,   0,   6,   0, 7,   0,   8,   0, 7,   0,   0,   0, 8,  0, 0,   0,   1,
    0, 2,   0,   3,   0, 4,   0,   5,   0,   6,   0, 7,   0,   8,   0, 0,   0,   0,   0, 1,  0, 2,   0,   3,
    0, 4,   0,   5,   0, 6,   0,   7,   0,   0,   0, 15,  0,   0,   0, 0,   0,   0,   0, 0,  0, 0,   0,   0};

void dll_80_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = gDll80EffectResourceData;
    GfxCmd* commands = packet.entries;
    GfxCmd* commandCursor;

    commands[0].layer = 0;
    commands[0].flags = 9;
    commands[0].tex = &resourceData[offsetof(Dll80EffectResourceView, allVertexIndices)];
    commands[0].mode = 0x80;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 16383.0f;
    if (variant == 1) {
        commands[1].layer = 0;
        commands[1].flags = 8;
        commands[1].tex = &resourceData[offsetof(Dll80EffectResourceView, firstEightVertexIndices)];
        commands[1].mode = 2;
        commands[1].x = 4.2f;
        commands[1].y = 4.2f;
        commands[1].z = 20.0f;
        commandCursor = commands + 2;
    } else {
        commands[1].layer = 0;
        commands[1].flags = 8;
        commands[1].tex = &resourceData[offsetof(Dll80EffectResourceView, firstEightVertexIndices)];
        commands[1].mode = 2;
        commands[1].x = 0.42f;
        commands[1].y = 0.42f;
        commands[1].z = 2.0f;
        commandCursor = commands + 2;
    }
    commandCursor[0].layer = 1;
    commandCursor[0].flags = 8;
    commandCursor[0].tex = &resourceData[offsetof(Dll80EffectResourceView, allVertexIndices)];
    commandCursor[0].mode = 2;
    commandCursor[0].x = 2.0f;
    commandCursor[0].y = 2.0f;
    commandCursor[0].z = 1.0f;
    commandCursor[1].layer = 1;
    commandCursor[1].flags = 9;
    commandCursor[1].tex = &resourceData[offsetof(Dll80EffectResourceView, allVertexIndices)];
    commandCursor[1].mode = 0x100;
    commandCursor[1].x = -900.0f;
    commandCursor[1].y = 0.0f;
    commandCursor[1].z = 0.0f;
    commandCursor[2].layer = 1;
    commandCursor[2].flags = 9;
    commandCursor[2].tex = &resourceData[offsetof(Dll80EffectResourceView, allVertexIndices)];
    commandCursor[2].mode = 4;
    commandCursor[2].x = 0.0f;
    commandCursor[2].y = 0.0f;
    commandCursor[2].z = 0.0f;
    packet.context.attachedSource = sourceObj;
    packet.context.sourceMode = variant;
    packet.context.position[0] = 0.0f;
    packet.context.position[1] = 0.0f;
    packet.context.position[2] = 0.0f;
    packet.context.velocity[0] = 0.0f;
    packet.context.velocity[1] = 0.0f;
    packet.context.velocity[2] = 0.0f;
    packet.context.scale = 1.0f;
    packet.context.drawGroupCount = 1;
    packet.context.drawGroupStride = 0;
    packet.context.initialStateByte = 9;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x20;
    packet.context.commandCount = &commandCursor[3] - commands;
    packet.context.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll80EffectResourceView, sequenceParams[0])];
    packet.context.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll80EffectResourceView, sequenceParams[1])];
    packet.context.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll80EffectResourceView, sequenceParams[2])];
    packet.context.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll80EffectResourceView, sequenceParams[3])];
    packet.context.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll80EffectResourceView, sequenceParams[4])];
    packet.context.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll80EffectResourceView, sequenceParams[5])];
    packet.context.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll80EffectResourceView, sequenceParams[6])];
    packet.context.commands = packet.entries;
    packet.context.flags = 0x4000010;
    packet.context.flags |= spawnFlags;
    if ((packet.context.flags & 1) != 0) {
        if ((u32)sourceObj != 0) {
            packet.context.position[0] += sourceObj->anim.worldPosX;
            packet.context.position[1] += sourceObj->anim.worldPosY;
            packet.context.position[2] += sourceObj->anim.worldPosZ;
        } else {
            packet.context.position[0] += spawnParams->posX;
            packet.context.position[1] += spawnParams->posY;
            packet.context.position[2] += spawnParams->posZ;
        }
    }
    packet.context.modeByte = 0;
    (*gModgfxInterface)
        ->spawnEffect(&packet.context, 0, 9, resourceData, 8, &resourceData[offsetof(Dll80EffectResourceView, triangles)],
                      0x156, 0);
}

void dll_80_release(void) {
}

void dll_80_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gDll80ResourceDescriptorAcquire, dll_80_initialise)

Dll80ResourceDescriptor gDll80ResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll80ResourceDescriptorAcquire, dll_80_release },
    NULL,
    dll_80_spawnEffect,
};
