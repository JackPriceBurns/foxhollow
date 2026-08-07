/*
 * DLL 118 / 0x76 - a fixed-command modgfx effect spawner.
 */
#include "main/dll/dll_0076_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll76SequenceParamBlock {
    s16 params[7];
    s16 opaqueTail;
} Dll76SequenceParamBlock;

STATIC_ASSERT(offsetof(Dll76SequenceParamBlock, params) == 0x00);
STATIC_ASSERT(offsetof(Dll76SequenceParamBlock, opaqueTail) == 0x0E);
STATIC_ASSERT(sizeof(Dll76SequenceParamBlock) == 0x10);

Dll76SequenceParamBlock gDll76SequenceParams = {
    {0, 155, 200, 1, 155, 0, 0},
    0,
};

const f32 gDll76Cmd0X = 999.0f;
const f32 gDll76Cmd0Y = 83.0f;
const f32 gDll76Cmd0Z = 84.0f;
const f32 gDll76Zero = 0.0f;
const f32 gDll76CmdY = 200.0f;
const f32 gDll76Scale = 1.0f;

void dll_76_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    GfxCmd* commands = packet.entries;
    commands[0].layer = 0;
    commands[0].flags = 0x8c;
    commands[0].tex = NULL;
    commands[0].mode = 0x20000000;
    commands[0].x = *(f32*)&gDll76Cmd0X;
    commands[0].y = *(f32*)&gDll76Cmd0Y;
    commands[0].z = *(f32*)&gDll76Cmd0Z;
    commands[1].layer = 0;
    commands[1].flags = 0;
    commands[1].tex = NULL;
    commands[1].mode = 0x80000;
    commands[1].x = *(f32*)&gDll76Zero;
    commands[1].y = *(f32*)&gDll76CmdY;
    commands[1].z = *(f32*)&gDll76Zero;
    commands[2].layer = 1;
    commands[2].flags = 0;
    commands[2].tex = NULL;
    commands[2].mode = 0x80000;
    commands[2].x = *(f32*)&gDll76Zero;
    commands[2].y = *(f32*)&gDll76Zero;
    commands[2].z = *(f32*)&gDll76Zero;
    commands[3].layer = 3;
    commands[3].flags = 1;
    commands[3].tex = NULL;
    commands[3].mode = 0x2000;
    commands[3].x = *(f32*)&gDll76Zero;
    commands[3].y = *(f32*)&gDll76Zero;
    commands[3].z = *(f32*)&gDll76Zero;
    commands[4].layer = 4;
    commands[4].flags = 0;
    commands[4].tex = NULL;
    commands[4].mode = 0x80000;
    commands[4].x = *(f32*)&gDll76Zero;
    commands[4].y = *(f32*)&gDll76CmdY;
    commands[4].z = *(f32*)&gDll76Zero;
    commands[5].layer = 5;
    commands[5].flags = 0;
    commands[5].tex = NULL;
    commands[5].mode = 0x20000000;
    commands[5].x = *(f32*)&gDll76Cmd0X;
    commands[5].y = *(f32*)&gDll76Cmd0Y;
    commands[5].z = *(f32*)&gDll76Cmd0Z;
    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = *(f32*)&gDll76Zero;
    packet.position[1] = *(f32*)&gDll76Zero;
    packet.position[2] = *(f32*)&gDll76Zero;
    packet.velocity[0] = *(f32*)&gDll76Zero;
    packet.velocity[1] = *(f32*)&gDll76Zero;
    packet.velocity[2] = *(f32*)&gDll76Zero;
    packet.scale = *(f32*)&gDll76Scale;
    packet.drawGroupCount = 0;
    packet.drawGroupStride = 0;
    packet.initialStateByte = 0;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0;
    packet.commandCount = (commands + 6) - packet.entries;
    packet.sequenceParams[0] = gDll76SequenceParams.params[0];
    packet.sequenceParams[1] = gDll76SequenceParams.params[1];
    packet.sequenceParams[2] = gDll76SequenceParams.params[2];
    packet.sequenceParams[3] = gDll76SequenceParams.params[3];
    packet.sequenceParams[4] = gDll76SequenceParams.params[4];
    packet.sequenceParams[5] = gDll76SequenceParams.params[5];
    packet.sequenceParams[6] = gDll76SequenceParams.params[6];
    packet.commands = packet.entries;
    packet.flags = 0x10c00;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (sourceObj != NULL) {
            packet.position[0] = *(f32*)&gDll76Zero + sourceObj->anim.worldPosX;
            packet.position[1] = *(f32*)&gDll76Zero + sourceObj->anim.worldPosY;
            packet.position[2] = *(f32*)&gDll76Zero + sourceObj->anim.worldPosZ;
        } else {
            packet.position[0] = *(f32*)&gDll76Zero + spawnParams->posX;
            packet.position[1] = *(f32*)&gDll76Zero + spawnParams->posY;
            packet.position[2] = *(f32*)&gDll76Zero + spawnParams->posZ;
        }
    }
    (*gModgfxInterface)->spawnEffect(&packet, 0, 0, 0, 0, 0, 0, 0);
}

void dll_76_release(void) {
}

void dll_76_initialise(void) {
}

Dll76ResourceDescriptor gDll76ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, dll_76_initialise, dll_76_release, NULL, dll_76_spawnEffect,
};
