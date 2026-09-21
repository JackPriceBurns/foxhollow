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

extern u16 gDll98EffectResourceData[sizeof(Dll98EffectResourceView) / sizeof(u16)];

void dll_98_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags, int unused,
                        int invertY) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)gDll98EffectResourceData;
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
    packet.context.modeByte = 0;
    packet.context.attachedSource = sourceObj;
    packet.context.sourceMode = variant;
    packet.context.position[0] = 0.0f;
    if ((u32)invertY != 0) {
        packet.context.position[1] = -2.0f;
    } else {
        packet.context.position[1] = 2.0f;
    }
    packet.context.position[2] = 0.0f;
    packet.context.velocity[0] = 0.0f;
    packet.context.velocity[1] = 0.0f;
    packet.context.velocity[2] = 0.0f;
    packet.context.scale = 1.0f;
    packet.context.drawGroupCount = 1;
    packet.context.drawGroupStride = 0;
    packet.context.initialStateByte = 0x12;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x10;
    packet.context.flags = 0x4080400;
    packet.context.commandCount = (GfxCmd*)((u8*)commands + sizeof(GfxCmd) * 9) - commands;
    packet.context.sequenceParams[0] = resource->sequenceParams[0];
    packet.context.sequenceParams[1] = resource->sequenceParams[1];
    packet.context.sequenceParams[2] = resource->sequenceParams[2];
    packet.context.sequenceParams[3] = resource->sequenceParams[3];
    packet.context.sequenceParams[4] = resource->sequenceParams[4];
    packet.context.sequenceParams[5] = resource->sequenceParams[5];
    packet.context.sequenceParams[6] = resource->sequenceParams[6];
    packet.context.commands = packet.entries;
    packet.context.flags |= spawnFlags;
    if ((packet.context.flags & 1) != 0) {
        if ((u32)packet.context.attachedSource != 0) {
            packet.context.position[0] += packet.context.attachedSource->anim.worldPosX;
            packet.context.position[1] += packet.context.attachedSource->anim.worldPosY;
            packet.context.position[2] += packet.context.attachedSource->anim.worldPosZ;
        } else {
            packet.context.position[0] += spawnParams->posX;
            packet.context.position[1] += spawnParams->posY;
            packet.context.position[2] += spawnParams->posZ;
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
        ->spawnEffect(&packet.context, 0, 0x12,
                      (u32)invertY != 0 ? &resourceData[offsetof(Dll98EffectResourceView, invertedVertices)]
                                        : (u8*)gDll98EffectResourceData,
                      0x10, &resourceData[offsetof(Dll98EffectResourceView, triangles)], effectId, 0);
}

void dll_98_release(void) {
}

void dll_98_initialise(void) {
}

u16 gDll98EffectResourceData[sizeof(Dll98EffectResourceView) / sizeof(u16)] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0xfd3d, 0x0000, 0x02c3, 0x000f,
    0x0000, 0xfc18, 0x0000, 0x0000, 0x001f, 0x0000, 0xfd3d, 0x0000, 0xfd3d,
    0x002f, 0x0000, 0x0000, 0x0000, 0xfc18, 0x003f, 0x0000, 0x02c3, 0x0000,
    0xfd3d, 0x004f, 0x0000, 0x03e8, 0x0000, 0x0000, 0x005f, 0x0000, 0x02c3,
    0x0000, 0x02c3, 0x006f, 0x0000, 0x0000, 0x0000, 0x03e8, 0x007f, 0x0000,
    0x0000, 0x07d0, 0x03e8, 0x0000, 0x001f, 0xfd3d, 0x07d0, 0x02c3, 0x000f,
    0x001f, 0xfc18, 0x07d0, 0x0000, 0x001f, 0x001f, 0xfd3d, 0x07d0, 0xfd3d,
    0x002f, 0x001f, 0x0000, 0x07d0, 0xfc18, 0x003f, 0x001f, 0x02c3, 0x07d0,
    0xfd3d, 0x004f, 0x001f, 0x03e8, 0x07d0, 0x0000, 0x005f, 0x001f, 0x02c3,
    0x07d0, 0x02c3, 0x006f, 0x001f, 0x0000, 0x07d0, 0x03e8, 0x007f, 0x001f,
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0xfd3d, 0x0000, 0x02c3, 0x000f,
    0x0000, 0xfc18, 0x0000, 0x0000, 0x001f, 0x0000, 0xfd3d, 0x0000, 0xfd3d,
    0x002f, 0x0000, 0x0000, 0x0000, 0xfc18, 0x003f, 0x0000, 0x02c3, 0x0000,
    0xfd3d, 0x004f, 0x0000, 0x03e8, 0x0000, 0x0000, 0x005f, 0x0000, 0x02c3,
    0x0000, 0x02c3, 0x006f, 0x0000, 0x0000, 0x0000, 0x03e8, 0x007f, 0x0000,
    0x0000, 0xf830, 0x03e8, 0x0000, 0x001f, 0xfd3d, 0xf830, 0x02c3, 0x000f,
    0x001f, 0xfc18, 0xf830, 0x0000, 0x001f, 0x001f, 0xfd3d, 0xf830, 0xfd3d,
    0x002f, 0x001f, 0x0000, 0xf830, 0xfc18, 0x003f, 0x001f, 0x02c3, 0xf830,
    0xfd3d, 0x004f, 0x001f, 0x03e8, 0xf830, 0x0000, 0x005f, 0x001f, 0x02c3,
    0xf830, 0x02c3, 0x006f, 0x001f, 0x0000, 0xf830, 0x03e8, 0x007f, 0x001f,
    0x0000, 0x0001, 0x000a, 0x0000, 0x000a, 0x0009, 0x0001, 0x0002, 0x000b,
    0x0001, 0x000b, 0x000a, 0x0002, 0x0003, 0x000c, 0x0002, 0x000c, 0x000b,
    0x0003, 0x0004, 0x000d, 0x0003, 0x000d, 0x000c, 0x0004, 0x0005, 0x000e,
    0x0004, 0x000e, 0x000d, 0x0005, 0x0006, 0x000f, 0x0005, 0x000f, 0x000e,
    0x0006, 0x0007, 0x0010, 0x0006, 0x0010, 0x000f, 0x0007, 0x0008, 0x0011,
    0x0007, 0x0011, 0x0010, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
    0x0006, 0x0007, 0x0008, 0x0000, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004,
    0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d,
    0x000e, 0x000f, 0x0010, 0x0011, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d,
    0x000e, 0x000f, 0x0010, 0x0011, 0x0000, 0x0000, 0x0064, 0x0064, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
};
RESOURCE_ACQUIRE_ADAPTER(gDll98ResourceDescriptorAcquire, dll_98_initialise)

Dll98ResourceDescriptor gDll98ResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll98ResourceDescriptorAcquire, dll_98_release },
    NULL,
    dll_98_spawnEffect,
    0x00000000,
};
