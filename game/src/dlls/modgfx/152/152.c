/*
 * DLL 152 / 0x98 - an invertible nine-command layered modgfx effect spawner.
 */
#include "main/dll/dll_0098_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"
#include "main/vecmath.h"

typedef struct Dll98EffectResourceView {
    ModgfxEffectVertex primaryVertices[18];
    ModgfxEffectVertex invertedVertices[18];
    s16 triangles[16][3];
    u8 opaque1C8[0x14];
    s16 allVertexIndices[18];
    u8 opaque200[0x14];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll98EffectResourceView;

STATIC_ASSERT(offsetof(Dll98EffectResourceView, primaryVertices) == 0x000);
STATIC_ASSERT(offsetof(Dll98EffectResourceView, invertedVertices) == 0x0B4);
STATIC_ASSERT(offsetof(Dll98EffectResourceView, triangles) == 0x168);
STATIC_ASSERT(offsetof(Dll98EffectResourceView, opaque1C8) == 0x1C8);
STATIC_ASSERT(offsetof(Dll98EffectResourceView, allVertexIndices) == 0x1DC);
STATIC_ASSERT(offsetof(Dll98EffectResourceView, opaque200) == 0x200);
STATIC_ASSERT(offsetof(Dll98EffectResourceView, sequenceParams) == 0x214);
STATIC_ASSERT(offsetof(Dll98EffectResourceView, opaqueTail) == 0x222);
STATIC_ASSERT(sizeof(Dll98EffectResourceView) == 0x224);

extern u32 gDll98EffectResourceData[sizeof(Dll98EffectResourceView) / sizeof(u32)];

void dll_98_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int unused,
                        int invertY) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)(int)gDll98EffectResourceData;
    Dll98EffectResourceView* resource = (Dll98EffectResourceView*)resourceData;
    GfxCmd* commands;
    int effectId;
    resource->sequenceParams[1] = randomGetRange(0, 0x1E) + 0x1E;
    resource->sequenceParams[2] = (s32)resource->sequenceParams[1];
    commands = packet.entries;
    commands[0].layer = 0;
    commands[0].flags = 0x12;
    commands[0].tex = &resourceData[offsetof(Dll98EffectResourceView, allVertexIndices)];
    commands[0].mode = 0x4;
    commands[0].x = 0.0f;
    commands[0].y = 0.0f;
    commands[0].z = 0.0f;
    commands[1].layer = 0;
    commands[1].flags = 0x12;
    commands[1].tex = &resourceData[offsetof(Dll98EffectResourceView, allVertexIndices)];
    commands[1].mode = 0x2;
    commands[1].z = commands[1].x = 0.22f;
    commands[1].y = 0.3f;
    commands[2].layer = 1;
    commands[2].flags = 0x12;
    commands[2].tex = &resourceData[offsetof(Dll98EffectResourceView, allVertexIndices)];
    commands[2].mode = 0x4;
    commands[2].x = 255.0f;
    commands[2].y = 0.0f;
    commands[2].z = 0.0f;
    commands[3].layer = 1;
    commands[3].flags = 0x12;
    commands[3].tex = &resourceData[offsetof(Dll98EffectResourceView, allVertexIndices)];
    commands[3].mode = 0x400000;
    commands[3].x = 0.0f;
    if ((u32)invertY != 0) {
        commands[3].y = -7.0f;
    } else {
        commands[3].y = 7.0f;
    }
    commands[3].z = 0.0f;
    commands[4].layer = 1;
    commands[4].flags = 0x12;
    commands[4].tex = &resourceData[offsetof(Dll98EffectResourceView, allVertexIndices)];
    commands[4].mode = 0x4000;
    commands[4].x = 0.0f;
    if ((u32)invertY != 0) {
        commands[4].y = 1.0f;
    } else {
        commands[4].y = -1.0f;
    }
    commands[4].z = 0.0f;
    commands[5].layer = 2;
    commands[5].flags = 0x12;
    commands[5].tex = &resourceData[offsetof(Dll98EffectResourceView, allVertexIndices)];
    commands[5].mode = 0x4;
    commands[5].x = 0.0f;
    commands[5].y = 0.0f;
    commands[5].z = 0.0f;
    commands[6].layer = 2;
    commands[6].flags = 0x12;
    commands[6].tex = &resourceData[offsetof(Dll98EffectResourceView, allVertexIndices)];
    commands[6].mode = 0x400000;
    commands[6].x = 0.0f;
    if ((u32)invertY != 0) {
        commands[6].y = -7.0f;
    } else {
        commands[6].y = 7.0f;
    }
    commands[6].z = 0.0f;
    commands[7].layer = 2;
    commands[7].flags = 0x12;
    commands[7].tex = &resourceData[offsetof(Dll98EffectResourceView, allVertexIndices)];
    commands[7].mode = 0x4000;
    commands[7].x = 0.0f;
    if ((u32)invertY != 0) {
        commands[7].y = 1.0f;
    } else {
        commands[7].y = -1.0f;
    }
    commands[7].z = 0.0f;
    commands[8].layer = 2;
    commands[8].flags = 0x12;
    commands[8].tex = &resourceData[offsetof(Dll98EffectResourceView, allVertexIndices)];
    commands[8].mode = 0x2;
    commands[8].x = 1.0f;
    commands[8].y = 1.0f;
    commands[8].z = 1.0f;
    packet.modeByte = 0;
    packet.sourceObj = sourceObj;
    packet.sourceMode = variant;
    packet.position[0] = 0.0f;
    if ((u32)invertY != 0) {
        packet.position[1] = -2.0f;
    } else {
        packet.position[1] = 2.0f;
    }
    packet.position[2] = 0.0f;
    packet.velocity[0] = 0.0f;
    packet.velocity[1] = 0.0f;
    packet.velocity[2] = 0.0f;
    packet.scale = 1.0f;
    packet.drawGroupCount = 1;
    packet.drawGroupStride = 0;
    packet.initialStateByte = 0x12;
    packet.byte5A = 0;
    packet.textureFrameTimer = 0x10;
    packet.flags = 0x4080400;
    packet.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd) * 9) - commands;
    packet.sequenceParams[0] = resource->sequenceParams[0];
    packet.sequenceParams[1] = resource->sequenceParams[1];
    packet.sequenceParams[2] = resource->sequenceParams[2];
    packet.sequenceParams[3] = resource->sequenceParams[3];
    packet.sequenceParams[4] = resource->sequenceParams[4];
    packet.sequenceParams[5] = resource->sequenceParams[5];
    packet.sequenceParams[6] = resource->sequenceParams[6];
    packet.commands = (GfxCmd*)((u8*)&packet + offsetof(ModgfxSpawnPacket, entries));
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
    if (variant == 0) {
        effectId = 0x3E9;
    } else if (variant == 1) {
        effectId = 0x3F0;
    } else {
        effectId = 0x3F3;
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet, 0, 0x12,
                      (u32)invertY != 0 ? &resourceData[offsetof(Dll98EffectResourceView, invertedVertices)]
                                        : (u8*)(int)gDll98EffectResourceData,
                      0x10, &resourceData[offsetof(Dll98EffectResourceView, triangles)], effectId, 0);
}

void dll_98_release(void) {
}

void dll_98_initialise(void) {
}

u32 gDll98EffectResourceData[sizeof(Dll98EffectResourceView) / sizeof(u32)] = {
    0x00000000, 0x03e80000, 0x0000fd3d, 0x000002c3, 0x000f0000, 0xfc180000, 0x0000001f, 0x0000fd3d, 0x0000fd3d,
    0x002f0000, 0x00000000, 0xfc18003f, 0x000002c3, 0x0000fd3d, 0x004f0000, 0x03e80000, 0x0000005f, 0x000002c3,
    0x000002c3, 0x006f0000, 0x00000000, 0x03e8007f, 0x00000000, 0x07d003e8, 0x0000001f, 0xfd3d07d0, 0x02c3000f,
    0x001ffc18, 0x07d00000, 0x001f001f, 0xfd3d07d0, 0xfd3d002f, 0x001f0000, 0x07d0fc18, 0x003f001f, 0x02c307d0,
    0xfd3d004f, 0x001f03e8, 0x07d00000, 0x005f001f, 0x02c307d0, 0x02c3006f, 0x001f0000, 0x07d003e8, 0x007f001f,
    0x00000000, 0x03e80000, 0x0000fd3d, 0x000002c3, 0x000f0000, 0xfc180000, 0x0000001f, 0x0000fd3d, 0x0000fd3d,
    0x002f0000, 0x00000000, 0xfc18003f, 0x000002c3, 0x0000fd3d, 0x004f0000, 0x03e80000, 0x0000005f, 0x000002c3,
    0x000002c3, 0x006f0000, 0x00000000, 0x03e8007f, 0x00000000, 0xf83003e8, 0x0000001f, 0xfd3df830, 0x02c3000f,
    0x001ffc18, 0xf8300000, 0x001f001f, 0xfd3df830, 0xfd3d002f, 0x001f0000, 0xf830fc18, 0x003f001f, 0x02c3f830,
    0xfd3d004f, 0x001f03e8, 0xf8300000, 0x005f001f, 0x02c3f830, 0x02c3006f, 0x001f0000, 0xf83003e8, 0x007f001f,
    0x00000001, 0x000a0000, 0x000a0009, 0x00010002, 0x000b0001, 0x000b000a, 0x00020003, 0x000c0002, 0x000c000b,
    0x00030004, 0x000d0003, 0x000d000c, 0x00040005, 0x000e0004, 0x000e000d, 0x00050006, 0x000f0005, 0x000f000e,
    0x00060007, 0x00100006, 0x0010000f, 0x00070008, 0x00110007, 0x00110010, 0x00000001, 0x00020003, 0x00040005,
    0x00060007, 0x00080000, 0x00000001, 0x00020003, 0x00040005, 0x00060007, 0x00080009, 0x000a000b, 0x000c000d,
    0x000e000f, 0x00100011, 0x0009000a, 0x000b000c, 0x000d000e, 0x000f0010, 0x00110000, 0x00000064, 0x00640000,
    0x00000000, 0x00000000};
Dll98ResourceDescriptor gDll98ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    dll_98_initialise,
    dll_98_release,
    NULL,
    dll_98_spawnEffect,
    0x00000000,
};
