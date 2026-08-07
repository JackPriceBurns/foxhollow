/*
 * DLL 117 / 0x75 - a variant-driven modgfx effect spawner.
 */
#include "main/dll/dll_0075_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll75SequenceParamBlock {
    s16 params[7];
    s16 opaqueTail;
} Dll75SequenceParamBlock;

STATIC_ASSERT(offsetof(Dll75SequenceParamBlock, params) == 0x00);
STATIC_ASSERT(offsetof(Dll75SequenceParamBlock, opaqueTail) == 0x0E);
STATIC_ASSERT(sizeof(Dll75SequenceParamBlock) == 0x10);

Dll75SequenceParamBlock gDll75SequenceParams = {
    {0, 155, 200, 155, 0, 0, 0},
    0,
};

s16 dll_75_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                       int unusedArg4, void* unusedArg5) {
    ModgfxSpawnPacket packet;
    int commandFlags;
    GfxCmd* commands;
    GfxCmd* commandCursor;
    f32 originOffset = 0.0f;
    f32 startFrame = 81.0f;
    f32 endFrame = 82.0f;
    commandFlags = 100;
    if (variant == 0) {
        commandFlags = 0x8c;
    } else if (variant == 1) {
        startFrame = 100.0f;
        endFrame = 101.0f;
        commandFlags = 0x8c;
    } else if (variant == 2) {
        startFrame = 102.0f;
        endFrame = 103.0f;
        commandFlags = 0x8c;
    } else if (variant == 3) {
        startFrame = 104.0f;
        endFrame = 105.0f;
        commandFlags = 0x8c;
    } else if (variant == 4) {
        startFrame = 106.0f;
        endFrame = 107.0f;
        commandFlags = 0x154;
    } else if (variant == 5) {
        startFrame = 111.0f;
        endFrame = 112.0f;
        commandFlags = 0x280;
        gDll75SequenceParams.params[2] = 800;
    } else if (variant == 6) {
        startFrame = 125.0f;
        endFrame = 126.0f;
        commandFlags = 100;
        gDll75SequenceParams.params[2] = 0x14;
    } else if (variant == 7) {
        startFrame = 133.0f;
        endFrame = 134.0f;
        commandFlags = 200;
        gDll75SequenceParams.params[1] = 0x14;
        gDll75SequenceParams.params[2] = 0x14;
        gDll75SequenceParams.params[3] = 0x14;
    } else if (variant == 8) {
        startFrame = 157.0f;
        endFrame = 158.0f;
        commandFlags = 0x41;
        gDll75SequenceParams.params[1] = 0x14;
        gDll75SequenceParams.params[2] = 0x14;
        gDll75SequenceParams.params[3] = 0x14;
    }
    commands = packet.entries;
    commands[0].layer = 0;
    commands[0].flags = commandFlags;
    commands[0].tex = NULL;
    commands[0].mode = 0x20000000;
    commands[0].x = 999.0f;
    commands[0].y = startFrame;
    commands[0].z = endFrame;
    commandCursor = &commands[1];
    if (variant == 0) {
        commandCursor[0].layer = 0;
        commandCursor[0].flags = 0;
        commandCursor[0].tex = NULL;
        commandCursor[0].mode = 0x80000;
        commandCursor[0].x = originOffset;
        commandCursor[0].y = 200.0f;
        commandCursor[0].z = originOffset;
        commandCursor[1].layer = 1;
        commandCursor[1].flags = 0;
        commandCursor[1].tex = NULL;
        commandCursor[1].mode = 0x80000;
        commandCursor[1].x = originOffset;
        commandCursor[1].y = originOffset;
        commandCursor[1].z = originOffset;
        commandCursor[2].layer = 3;
        commandCursor[2].flags = 0;
        commandCursor[2].tex = NULL;
        commandCursor[2].mode = 0x80000;
        commandCursor[2].x = originOffset;
        commandCursor[2].y = 200.0f;
        commandCursor[2].z = originOffset;
        commandCursor += 3;
    } else if (variant == 6) {
        commandCursor[0].layer = 3;
        commandCursor[0].flags = 1;
        commandCursor[0].tex = NULL;
        commandCursor[0].mode = 0x2000;
        commandCursor[0].x = originOffset;
        commandCursor[0].y = originOffset;
        commandCursor[0].z = originOffset;
        commandCursor += 1;
    } else if (variant == 8) {
        commandCursor[0].layer = 3;
        commandCursor[0].flags = 1;
        commandCursor[0].tex = NULL;
        commandCursor[0].mode = 0x2000;
        commandCursor[0].x = originOffset;
        commandCursor[0].y = originOffset;
        commandCursor[0].z = originOffset;
        commandCursor += 1;
    }
    commandCursor[0].layer = 4;
    commandCursor[0].flags = 0;
    commandCursor[0].tex = NULL;
    commandCursor[0].mode = 0x20000000;
    commandCursor[0].x = 999.0f;
    commandCursor[0].y = startFrame;
    commandCursor[0].z = endFrame;
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
    packet.drawGroupCount = 0;
    packet.drawGroupStride = 0;
    packet.initialStateByte = 0;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0;
    packet.commandCount = (commandCursor + 1) - commands;
    packet.sequenceParams[0] = gDll75SequenceParams.params[0];
    packet.sequenceParams[1] = gDll75SequenceParams.params[1];
    packet.sequenceParams[2] = gDll75SequenceParams.params[2];
    packet.sequenceParams[3] = gDll75SequenceParams.params[3];
    packet.sequenceParams[4] = gDll75SequenceParams.params[4];
    packet.sequenceParams[5] = gDll75SequenceParams.params[5];
    packet.sequenceParams[6] = gDll75SequenceParams.params[6];
    packet.commands = (GfxCmd*)((u8*)&packet + 0x60);
    packet.flags = 0x10800;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (sourceObj != NULL) {
            packet.position[0] = originOffset + sourceObj->anim.worldPosX;
            packet.position[1] = originOffset + sourceObj->anim.worldPosY;
            packet.position[2] = originOffset + sourceObj->anim.worldPosZ;
        } else {
            packet.position[0] = originOffset + spawnParams->posX;
            packet.position[1] = originOffset + spawnParams->posY;
            packet.position[2] = originOffset + spawnParams->posZ;
        }
    }
    return (*gModgfxInterface)->spawnEffect(&packet, 0, 0, 0, 0, 0, 0, 0);
}

void dll_75_release(void) {
}

void dll_75_initialise(void) {
}

Dll75ResourceDescriptor gDll75ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_75_initialise, dll_75_release, NULL, dll_75_spawnEffect,
};
