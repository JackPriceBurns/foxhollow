/*
 * DLL 109 / 0x6D - a modgfx effect spawner.
 */
#include "main/dll/dll_006D_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll6DEffectResourceView {
    ModgfxEffectVertex vertices[14];
    s16 colors[12][3];
    s16 allVertexIndices[14];
    s16 firstGroupIndices[8];
    s16 secondGroupIndices[8];
    s16 sequenceParams[7];
    u8 pad11E[2];
} Dll6DEffectResourceView;

STATIC_ASSERT(offsetof(Dll6DEffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll6DEffectResourceView, colors) == 0x08C);
STATIC_ASSERT(offsetof(Dll6DEffectResourceView, allVertexIndices) == 0x0D4);
STATIC_ASSERT(offsetof(Dll6DEffectResourceView, firstGroupIndices) == 0x0F0);
STATIC_ASSERT(offsetof(Dll6DEffectResourceView, secondGroupIndices) == 0x100);
STATIC_ASSERT(offsetof(Dll6DEffectResourceView, sequenceParams) == 0x110);
STATIC_ASSERT(sizeof(Dll6DEffectResourceView) == 0x120);

u16 gDll6DEffectResourceData[sizeof(Dll6DEffectResourceView) / sizeof(u16)] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0362, 0x0000, 0x01f4, 0x000b,
    0x0000, 0x0362, 0x0000, 0xfe0c, 0x0016, 0x0000, 0x0000, 0x0000, 0xfc18,
    0x0020, 0x0000, 0xfc9e, 0x0000, 0xfe0c, 0x002a, 0x0000, 0xfc9e, 0x0000,
    0x01f4, 0x0035, 0x0000, 0x0000, 0x0000, 0x03e8, 0x0040, 0x0000, 0x0000,
    0x1770, 0x03e8, 0x0000, 0x001f, 0x0362, 0x1770, 0x01f4, 0x000b, 0x001f,
    0x0362, 0x1770, 0xfe0c, 0x0016, 0x001f, 0x0000, 0x1770, 0xfc18, 0x0020,
    0x001f, 0xfc9e, 0x1770, 0xfe0c, 0x002a, 0x001f, 0xfc9e, 0x1770, 0x01f4,
    0x0035, 0x001f, 0x0000, 0x1770, 0x03e8, 0x0040, 0x001f, 0x0000, 0x0001,
    0x0008, 0x0000, 0x0008, 0x0007, 0x0001, 0x0002, 0x0009, 0x0001, 0x0009,
    0x0008, 0x0002, 0x0003, 0x000a, 0x0002, 0x000a, 0x0009, 0x0003, 0x0004,
    0x000b, 0x0003, 0x000b, 0x000a, 0x0004, 0x0005, 0x000c, 0x0004, 0x000c,
    0x000b, 0x0005, 0x0006, 0x000d, 0x0005, 0x000d, 0x000c, 0x0000, 0x0001,
    0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a,
    0x000b, 0x000c, 0x000d, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
    0x0006, 0x0000, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d,
    0x0000, 0x0000, 0x0028, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

void dll_6D_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)gDll6DEffectResourceData;
    GameObject* context;

    packet.entries[0].layer = 0;
    packet.entries[0].flags = 0xe;
    packet.entries[0].tex = &resourceData[offsetof(Dll6DEffectResourceView, allVertexIndices)];
    packet.entries[0].mode = 0x80;
    packet.entries[0].x = 0.0f;
    packet.entries[0].y = -16000.0f;
    packet.entries[0].z = 0.0f;
    packet.entries[1].layer = 0;
    packet.entries[1].flags = 7;
    packet.entries[1].tex = &resourceData[offsetof(Dll6DEffectResourceView, secondGroupIndices)];
    packet.entries[1].mode = 4;
    packet.entries[1].x = 0.0f;
    packet.entries[1].y = 0.0f;
    packet.entries[1].z = 0.0f;
    packet.entries[2].layer = 0;
    packet.entries[2].flags = 7;
    packet.entries[2].tex = &resourceData[offsetof(Dll6DEffectResourceView, firstGroupIndices)];
    packet.entries[2].mode = 2;
    packet.entries[2].x = 0.3f;
    packet.entries[2].y = 0.7f;
    packet.entries[2].z = 0.3f;
    packet.entries[3].layer = 0;
    packet.entries[3].flags = 7;
    packet.entries[3].tex = &resourceData[offsetof(Dll6DEffectResourceView, secondGroupIndices)];
    packet.entries[3].mode = 2;
    packet.entries[3].x = 6.5f;
    packet.entries[3].y = 0.7f;
    packet.entries[3].z = 6.5f;
    packet.entries[4].layer = 1;
    packet.entries[4].flags = 0xe;
    packet.entries[4].tex = &resourceData[offsetof(Dll6DEffectResourceView, allVertexIndices)];
    packet.entries[4].mode = 0x4000;
    packet.entries[4].x = 0.0f;
    packet.entries[4].y = -3.0f;
    packet.entries[4].z = 0.0f;
    packet.entries[5].layer = 1;
    packet.entries[5].flags = 7;
    packet.entries[5].tex = &resourceData[offsetof(Dll6DEffectResourceView, firstGroupIndices)];
    packet.entries[5].mode = 4;
    packet.entries[5].x = 0.0f;
    packet.entries[5].y = 0.0f;
    packet.entries[5].z = 0.0f;
    packet.context.modeByte = 0;
    context = sourceObj;
    packet.context.attachedSource = context;
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
    packet.context.initialStateByte = 0xe;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x10;
    packet.context.commandCount = 6;
    packet.context.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll6DEffectResourceView, sequenceParams[0])];
    packet.context.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll6DEffectResourceView, sequenceParams[1])];
    packet.context.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll6DEffectResourceView, sequenceParams[2])];
    packet.context.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll6DEffectResourceView, sequenceParams[3])];
    packet.context.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll6DEffectResourceView, sequenceParams[4])];
    packet.context.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll6DEffectResourceView, sequenceParams[5])];
    packet.context.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll6DEffectResourceView, sequenceParams[6])];
    packet.context.commands = packet.entries;
    packet.context.flags = 0x4000004;
    packet.context.flags |= spawnFlags;
    if ((packet.context.flags & 1) != 0) {
        if (context != NULL) {
            packet.context.position[0] += context->anim.worldPosX;
            packet.context.position[1] += context->anim.worldPosY;
            packet.context.position[2] += context->anim.worldPosZ;
        } else {
            packet.context.position[0] += spawnParams->posX;
            packet.context.position[1] += spawnParams->posY;
            packet.context.position[2] += spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet.context, 0, 0xe, (u8*)gDll6DEffectResourceData, 0xc,
                      &resourceData[offsetof(Dll6DEffectResourceView, colors)], 0x34, 0);
}

void dll_6D_release(void) {
}

void dll_6D_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gDll6DResourceDescriptorAcquire, dll_6D_initialise)

Dll6DResourceDescriptor gDll6DResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll6DResourceDescriptorAcquire, dll_6D_release },
    NULL,
    dll_6D_spawnEffect,
};
