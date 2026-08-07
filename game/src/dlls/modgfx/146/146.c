/*
 * DLL 146 / 0x92 - a scaled nine-command layered modgfx effect spawner.
 */
#include "main/dll/dll_0092_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll92EffectResource {
    u8 opaque00[0x3C];
    u8 spawnData[0x18];
    u8 sharedTexture[0x0C];
    u8 primaryTexture[0x0C];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll92EffectResource;

STATIC_ASSERT(offsetof(Dll92EffectResource, opaque00) == 0x00);
STATIC_ASSERT(offsetof(Dll92EffectResource, spawnData) == 0x3C);
STATIC_ASSERT(offsetof(Dll92EffectResource, sharedTexture) == 0x54);
STATIC_ASSERT(offsetof(Dll92EffectResource, primaryTexture) == 0x60);
STATIC_ASSERT(offsetof(Dll92EffectResource, sequenceParams) == 0x6C);
STATIC_ASSERT(offsetof(Dll92EffectResource, opaqueTail) == 0x7A);
STATIC_ASSERT(sizeof(Dll92EffectResource) == 0x7C);

s16 gDll92VertexIndices[4] = {1, 0, 0, 0};

extern u32 gDll92EffectResourceData[sizeof(Dll92EffectResource) / sizeof(u32)];

void dll_92_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, u32 unused,
                        f32* scaleOverride) {
    Dll92EffectResource* resource[1];
    ModgfxSpawnPacket packet;
    GfxCmd* commands;
    f32 scale;
    resource[0] = (Dll92EffectResource*)gDll92EffectResourceData;
    scale = 1.0f;
    if (scaleOverride != NULL) {
        scale = *scaleOverride;
    }
    commands = packet.entries;
    commands[0].layer = 0;
    commands[0].flags = 5;
    commands[0].tex = resource[0]->primaryTexture;
    commands[0].mode = 4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    commands[1].layer = 0;
    commands[1].flags = 1;
    commands[1].tex = gDll92VertexIndices;
    commands[1].mode = 4;
    if (variant == 1) {
        commands[1].x = 155.0f;
    } else {
        commands[1].x = 55.0f;
    }
    commands[1].y = 0.0f;
    commands[1].z = 0.0f;
    commands[2].layer = 0;
    commands[2].flags = 6;
    commands[2].tex = resource[0]->sharedTexture;
    commands[2].mode = 2;
    if (variant == 1) {
        commands[2].z = commands[2].y = commands[2].x = 0.15f * scale;
    } else {
        commands[2].z = commands[2].y = commands[2].x = 0.1f * scale;
    }
    commands[3].layer = 1;
    commands[3].flags = 6;
    commands[3].tex = resource[0]->sharedTexture;
    commands[3].mode = 0x4000;
    commands[3].x = -0.5f;
    commands[3].y = 1.0f;
    commands[3].z = 0.0f;
    commands[4].layer = 1;
    commands[4].flags = 6;
    commands[4].tex = resource[0]->sharedTexture;
    commands[4].mode = 2;
    commands[4].x = 4.0f;
    commands[4].y = 4.0f;
    commands[4].z = 25.0f;
    commands[5].layer = 2;
    commands[5].flags = 6;
    commands[5].tex = resource[0]->sharedTexture;
    commands[5].mode = 0x4000;
    commands[5].x = -0.5f;
    commands[5].y = 1.0f;
    commands[5].z = 0.0f;
    commands[6].layer = 2;
    commands[6].flags = 6;
    commands[6].tex = resource[0]->sharedTexture;
    commands[6].mode = 2;
    commands[6].x = 8.0f;
    commands[6].y = 8.0f;
    commands[6].z = 1.0f;
    commands[7].layer = 3;
    commands[7].flags = 6;
    commands[7].tex = resource[0]->sharedTexture;
    commands[7].mode = 0x4000;
    commands[7].x = -0.5f;
    commands[7].y = 1.0f;
    commands[7].z = 0.0f;
    commands[8].layer = 3;
    commands[8].flags = 1;
    commands[8].tex = gDll92VertexIndices;
    commands[8].mode = 4;
    commands[8].x = 0.0f;
    commands[8].y = 0.0f;
    commands[8].z = 0.0f;
    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = 0.0f;
    packet.position[1] = 0.0f;
    packet.position[2] = 0.0f;
    packet.velocity[0] = 0.0f;
    packet.velocity[1] = 0.0f;
    packet.velocity[2] = 0.0f;
    packet.scale = 2.0f;
    packet.drawGroupCount = 1;
    packet.drawGroupStride = 0;
    packet.initialStateByte = 6;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0;
    packet.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd) * 9) - commands;
    packet.sequenceParams[0] = resource[0]->sequenceParams[0];
    packet.sequenceParams[1] = resource[0]->sequenceParams[1];
    packet.sequenceParams[2] = resource[0]->sequenceParams[2];
    packet.sequenceParams[3] = resource[0]->sequenceParams[3];
    packet.sequenceParams[4] = resource[0]->sequenceParams[4];
    packet.sequenceParams[5] = resource[0]->sequenceParams[5];
    packet.sequenceParams[6] = resource[0]->sequenceParams[6];
    packet.commands = (GfxCmd*)((u8*)&packet + offsetof(ModgfxSpawnPacket, entries));
    packet.flags = 0x4000400;
    packet.flags |= spawnFlags;
    if ((packet.flags & 1) != 0) {
        if (sourceObj != NULL && spawnParams != NULL) {
            packet.position[0] += sourceObj->anim.worldPosX + spawnParams->posX;
            packet.position[1] += sourceObj->anim.worldPosY + spawnParams->posY;
            packet.position[2] += sourceObj->anim.worldPosZ + spawnParams->posZ;
        } else if (sourceObj != NULL) {
            packet.position[0] += sourceObj->anim.worldPosX;
            packet.position[1] += packet.sourceObj->anim.worldPosY;
            packet.position[2] += packet.sourceObj->anim.worldPosZ;
        } else if (spawnParams != NULL) {
            packet.position[0] += spawnParams->posX;
            packet.position[1] += spawnParams->posY;
            packet.position[2] += spawnParams->posZ;
        }
    }
    (*gModgfxInterface)->spawnEffect(&packet, 0, 6, resource[0], 4, resource[0]->spawnData, 0x3C, 0);
}

void dll_92_release(void) {
}

void dll_92_initialise(void) {
}

u32 gDll92EffectResourceData[sizeof(Dll92EffectResource) / sizeof(u32)] = {
    0xff1a0000, 0x00000000, 0x000f0000, 0x00000000, 0x007f000f, 0x00e60000, 0x000000ff, 0x000fff1a,
    0x000003e8, 0x00000000, 0x00000000, 0x03e8007f, 0x000000e6, 0x000003e8, 0x00ff0000, 0x00000004,
    0x00030000, 0x00010004, 0x00010002, 0x00040002, 0x00050004, 0x00000001, 0x00020003, 0x00040005,
    0x00000002, 0x00030004, 0x00050000, 0x00000006, 0x0014001a, 0x00000000, 0x00000000,
};

Dll92ResourceDescriptor gDll92ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    dll_92_initialise,
    dll_92_release,
    NULL,
    dll_92_spawnEffect,
    0x00000000,
};
