/*
 * DLL 110 / 0x6E - a modgfx effect spawner.
 */
#include "main/dll/dll_006E_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll6EEffectResourceView {
    ModgfxEffectVertex vertices[5];
    u8 pad32[2];
    s16 colors[5][3];
    u8 pad52[2];
    s16 allVertexIndices[6];
    s16 sequenceParams[7];
    u8 pad6E[2];
} Dll6EEffectResourceView;

STATIC_ASSERT(offsetof(Dll6EEffectResourceView, vertices) == 0x00);
STATIC_ASSERT(offsetof(Dll6EEffectResourceView, colors) == 0x34);
STATIC_ASSERT(offsetof(Dll6EEffectResourceView, allVertexIndices) == 0x54);
STATIC_ASSERT(offsetof(Dll6EEffectResourceView, sequenceParams) == 0x60);
STATIC_ASSERT(sizeof(Dll6EEffectResourceView) == 0x70);

u16 gDll6EEffectResourceData[sizeof(Dll6EEffectResourceView) / sizeof(u16)] = {
    0xfc18, 0x0000, 0xfc18, 0x0000, 0x0000, 0x03e8, 0x0000, 0xfc18, 0x003f,
    0x0000, 0x03e8, 0x0000, 0x03e8, 0x003f, 0x003f, 0xfc18, 0x0000, 0x03e8,
    0x0000, 0x003f, 0x0000, 0x0000, 0x0000, 0x0020, 0x0020, 0x0000, 0x0000,
    0x0001, 0x0004, 0x0001, 0x0002, 0x0004, 0x0004, 0x0002, 0x0003, 0x0000,
    0x0004, 0x0003, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0002,
    0x0003, 0x0004, 0x0000, 0x0000, 0x0050, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000,
};

void dll_6E_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)gDll6EEffectResourceData;
    GameObject* context;
    f32 originOffset = 0.0f;

    packet.entries[0].layer = 0;
    packet.entries[0].flags = 5;
    packet.entries[0].tex = &resourceData[offsetof(Dll6EEffectResourceView, allVertexIndices)];
    packet.entries[0].mode = 4;
    packet.entries[0].x = 255.0f;
    packet.entries[0].y = originOffset;
    packet.entries[0].z = originOffset;
    packet.entries[1].layer = 0;
    packet.entries[1].flags = 5;
    packet.entries[1].tex = &resourceData[offsetof(Dll6EEffectResourceView, allVertexIndices)];
    packet.entries[1].mode = 2;
    packet.entries[1].x = 0.01f;
    packet.entries[1].y = 0.01f;
    packet.entries[1].z = 0.01f;
    packet.entries[2].layer = 0;
    packet.entries[2].flags = 5;
    packet.entries[2].tex = &resourceData[offsetof(Dll6EEffectResourceView, allVertexIndices)];
    packet.entries[2].mode = 8;
    packet.entries[2].x = originOffset;
    packet.entries[2].y = 200.0f;
    packet.entries[2].z = originOffset;
    packet.entries[3].layer = 0;
    packet.entries[3].flags = 0x7a;
    packet.entries[3].tex = NULL;
    packet.entries[3].mode = 0x10000;
    packet.entries[3].x = originOffset;
    packet.entries[3].y = originOffset;
    packet.entries[3].z = originOffset;
    packet.entries[4].layer = 1;
    packet.entries[4].flags = 5;
    packet.entries[4].tex = &resourceData[offsetof(Dll6EEffectResourceView, allVertexIndices)];
    packet.entries[4].mode = 4;
    packet.entries[4].x = originOffset;
    packet.entries[4].y = originOffset;
    packet.entries[4].z = originOffset;
    packet.entries[5].layer = 1;
    packet.entries[5].flags = 5;
    packet.entries[5].tex = &resourceData[offsetof(Dll6EEffectResourceView, allVertexIndices)];
    packet.entries[5].mode = 2;
    packet.entries[5].x = 4000.0f;
    packet.entries[5].y = 1.0f;
    packet.entries[5].z = 4000.0f;
    packet.context.modeByte = 0;
    context = sourceObj;
    packet.context.attachedSource = context;
    packet.context.sourceMode = variant;
    packet.context.position[0] = originOffset;
    packet.context.position[1] = 10.0f;
    packet.context.position[2] = originOffset;
    packet.context.velocity[0] = originOffset;
    packet.context.velocity[1] = originOffset;
    packet.context.velocity[2] = originOffset;
    packet.context.scale = 1.0f;
    packet.context.drawGroupCount = 1;
    packet.context.drawGroupStride = 0;
    packet.context.initialStateByte = 5;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x10;
    packet.context.commandCount = 6;
    packet.context.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll6EEffectResourceView, sequenceParams[0])];
    packet.context.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll6EEffectResourceView, sequenceParams[1])];
    packet.context.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll6EEffectResourceView, sequenceParams[2])];
    packet.context.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll6EEffectResourceView, sequenceParams[3])];
    packet.context.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll6EEffectResourceView, sequenceParams[4])];
    packet.context.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll6EEffectResourceView, sequenceParams[5])];
    packet.context.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll6EEffectResourceView, sequenceParams[6])];
    packet.context.commands = packet.entries;
    packet.context.flags = 0x4000010;
    packet.context.flags |= spawnFlags;
    if ((packet.context.flags & 1) != 0) {
        if (context != NULL) {
            packet.context.position[0] = originOffset + context->anim.worldPosX;
            packet.context.position[1] = 10.0f + context->anim.worldPosY;
            packet.context.position[2] = originOffset + context->anim.worldPosZ;
        } else {
            packet.context.position[0] = originOffset + spawnParams->posX;
            packet.context.position[1] = 10.0f + spawnParams->posY;
            packet.context.position[2] = originOffset + spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet.context, 0, 5, (u8*)gDll6EEffectResourceData, 4,
                      &resourceData[offsetof(Dll6EEffectResourceView, colors)], 0x5e, 0);
}

void dll_6E_release(void) {
}

void dll_6E_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gDll6EResourceDescriptorAcquire, dll_6E_initialise)

Dll6EResourceDescriptor gDll6EResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll6EResourceDescriptorAcquire, dll_6E_release },
    NULL,
    dll_6E_spawnEffect,
};
