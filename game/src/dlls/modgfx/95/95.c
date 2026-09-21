/*
 * DLL 95 / 0x5F - a modgfx effect spawner.
 */
#include "main/dll/dll_005F_modgfx.h"
#include "game/objects/object.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll5FEffectResourceView {
    ModgfxEffectVertex vertices[14];
    s16 colors[12][3];
    s16 allVertexIndices[14];
    s16 firstGroupIndices[8];
    s16 secondGroupIndices[8];
    s16 sequenceParams[7];
    u8 pad11E[2];
} Dll5FEffectResourceView;

STATIC_ASSERT(offsetof(Dll5FEffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll5FEffectResourceView, colors) == 0x08C);
STATIC_ASSERT(offsetof(Dll5FEffectResourceView, allVertexIndices) == 0x0D4);
STATIC_ASSERT(offsetof(Dll5FEffectResourceView, firstGroupIndices) == 0x0F0);
STATIC_ASSERT(offsetof(Dll5FEffectResourceView, secondGroupIndices) == 0x100);
STATIC_ASSERT(offsetof(Dll5FEffectResourceView, sequenceParams) == 0x110);
STATIC_ASSERT(sizeof(Dll5FEffectResourceView) == 0x120);

u16 gDll5FEffectResourceData[sizeof(Dll5FEffectResourceView) / sizeof(u16)] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0362, 0x0000, 0x01f4, 0x001f,
    0x0000, 0x0362, 0x0000, 0xfe0c, 0x003f, 0x0000, 0x0000, 0x0000, 0xfc18,
    0x005f, 0x0000, 0xfc9e, 0x0000, 0xfe0c, 0x007f, 0x0000, 0xfc9e, 0x0000,
    0x01f4, 0x009f, 0x0000, 0x0000, 0x0000, 0x03e8, 0x00bf, 0x0000, 0x0000,
    0x1770, 0x03e8, 0x0000, 0x003f, 0x0362, 0x1770, 0x01f4, 0x001f, 0x003f,
    0x0362, 0x1770, 0xfe0c, 0x003f, 0x003f, 0x0000, 0x1770, 0xfc18, 0x005f,
    0x003f, 0xfc9e, 0x1770, 0xfe0c, 0x007f, 0x003f, 0xfc9e, 0x1770, 0x01f4,
    0x009f, 0x003f, 0x0000, 0x1770, 0x03e8, 0x00bf, 0x003f, 0x0000, 0x0001,
    0x0008, 0x0000, 0x0008, 0x0007, 0x0001, 0x0002, 0x0009, 0x0001, 0x0009,
    0x0008, 0x0002, 0x0003, 0x000a, 0x0002, 0x000a, 0x0009, 0x0003, 0x0004,
    0x000b, 0x0003, 0x000b, 0x000a, 0x0004, 0x0005, 0x000c, 0x0004, 0x000c,
    0x000b, 0x0005, 0x0006, 0x000d, 0x0005, 0x000d, 0x000c, 0x0000, 0x0001,
    0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a,
    0x000b, 0x000c, 0x000d, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
    0x0006, 0x0000, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d,
    0x0000, 0x0000, 0x0014, 0x00aa, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

void dll_5F_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)gDll5FEffectResourceData;
    GameObject* sourceContext;
    f32 originOffset = 0.0f;
    packet.entries[0].layer = 0;
    packet.entries[0].flags = 0x32;
    packet.entries[0].tex = NULL;
    packet.entries[0].mode = 0x800000;
    packet.entries[0].x = 1.0f;
    packet.entries[0].y = originOffset;
    packet.entries[0].z = originOffset;
    packet.entries[1].layer = 0;
    packet.entries[1].flags = 0x7a;
    packet.entries[1].tex = NULL;
    packet.entries[1].mode = 0x10000;
    packet.entries[1].x = originOffset;
    packet.entries[1].y = originOffset;
    packet.entries[1].z = originOffset;
    packet.entries[2].layer = 0;
    packet.entries[2].flags = 7;
    packet.entries[2].tex = &resourceData[offsetof(Dll5FEffectResourceView, secondGroupIndices)];
    packet.entries[2].mode = 4;
    packet.entries[2].x = originOffset;
    packet.entries[2].y = originOffset;
    packet.entries[2].z = originOffset;
    packet.entries[3].layer = 0;
    packet.entries[3].flags = 7;
    packet.entries[3].tex = &resourceData[offsetof(Dll5FEffectResourceView, firstGroupIndices)];
    packet.entries[3].mode = 2;
    packet.entries[3].x = 0.7f;
    packet.entries[3].y = 1.0f;
    packet.entries[3].z = 0.7f;
    packet.entries[4].layer = 0;
    packet.entries[4].flags = 7;
    packet.entries[4].tex = &resourceData[offsetof(Dll5FEffectResourceView, secondGroupIndices)];
    packet.entries[4].mode = 2;
    packet.entries[4].x = 1.2f;
    packet.entries[4].y = -1.0f;
    packet.entries[4].z = 1.2f;
    packet.entries[5].layer = 0;
    packet.entries[5].flags = 7;
    packet.entries[5].tex = &resourceData[offsetof(Dll5FEffectResourceView, firstGroupIndices)];
    packet.entries[5].mode = 8;
    packet.entries[5].x = originOffset;
    packet.entries[5].y = 160.0f;
    packet.entries[5].z = 115.0f;
    packet.entries[6].layer = 0;
    packet.entries[6].flags = 7;
    packet.entries[6].tex = &resourceData[offsetof(Dll5FEffectResourceView, secondGroupIndices)];
    packet.entries[6].mode = 8;
    packet.entries[6].x = 255.0f;
    packet.entries[6].y = 255.0f;
    packet.entries[6].z = 115.0f;
    packet.entries[7].layer = 0;
    packet.entries[7].flags = 1;
    packet.entries[7].tex = NULL;
    packet.entries[7].mode = 0x8000;
    packet.entries[7].x = originOffset;
    packet.entries[7].y = 255.0f;
    packet.entries[7].z = originOffset;
    packet.entries[8].layer = 0;
    packet.entries[8].flags = 1;
    packet.entries[8].tex = NULL;
    packet.entries[8].mode = 0x80000;
    packet.entries[8].x = originOffset;
    packet.entries[8].y = -130.0f;
    packet.entries[8].z = originOffset;
    packet.entries[9].layer = 1;
    packet.entries[9].flags = 1;
    packet.entries[9].tex = NULL;
    packet.entries[9].mode = 0x80000;
    packet.entries[9].x = originOffset;
    packet.entries[9].y = originOffset;
    packet.entries[9].z = originOffset;
    packet.entries[10].layer = 2;
    packet.entries[10].flags = 0xe;
    packet.entries[10].tex = &resourceData[offsetof(Dll5FEffectResourceView, allVertexIndices)];
    packet.entries[10].mode = 0x4000;
    packet.entries[10].x = originOffset;
    packet.entries[10].y = -4.0f;
    packet.entries[10].z = originOffset;
    packet.entries[11].layer = 2;
    packet.entries[11].flags = 7;
    packet.entries[11].tex = &resourceData[offsetof(Dll5FEffectResourceView, firstGroupIndices)];
    packet.entries[11].mode = 4;
    packet.entries[11].x = originOffset;
    packet.entries[11].y = originOffset;
    packet.entries[11].z = originOffset;
    packet.entries[12].layer = 2;
    packet.entries[12].flags = 1;
    packet.entries[12].tex = NULL;
    packet.entries[12].mode = 0x80000;
    packet.entries[12].x = originOffset;
    packet.entries[12].y = 90.0f;
    packet.entries[12].z = originOffset;
    packet.context.modeByte = 0;
    sourceContext = sourceObj;
    packet.context.attachedSource = sourceContext;
    packet.context.sourceMode = variant;
    packet.context.position[0] = originOffset;
    packet.context.position[1] = originOffset;
    packet.context.position[2] = originOffset;
    packet.context.velocity[0] = originOffset;
    packet.context.velocity[1] = originOffset;
    packet.context.velocity[2] = originOffset;
    packet.context.scale = 1.0f;
    packet.context.drawGroupCount = 1;
    packet.context.drawGroupStride = 0;
    packet.context.initialStateByte = 0xe;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x10;
    packet.context.commandCount = 0;
    packet.context.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll5FEffectResourceView, sequenceParams[0])];
    packet.context.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll5FEffectResourceView, sequenceParams[1])];
    packet.context.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll5FEffectResourceView, sequenceParams[2])];
    packet.context.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll5FEffectResourceView, sequenceParams[3])];
    packet.context.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll5FEffectResourceView, sequenceParams[4])];
    packet.context.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll5FEffectResourceView, sequenceParams[5])];
    packet.context.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll5FEffectResourceView, sequenceParams[6])];
    packet.context.commands = packet.entries;
    packet.context.flags = 0x4000002;
    packet.context.flags |= spawnFlags;
    if ((packet.context.flags & 1) != 0) {
        if ((void*)sourceContext != NULL) {
            packet.context.position[0] = originOffset + sourceContext->anim.worldPosX;
            packet.context.position[1] = originOffset + sourceContext->anim.worldPosY;
            packet.context.position[2] = originOffset + sourceContext->anim.worldPosZ;
        } else {
            packet.context.position[0] = originOffset + spawnParams->posX;
            packet.context.position[1] = originOffset + spawnParams->posY;
            packet.context.position[2] = originOffset + spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet.context, 0, 0xe, (u8*)gDll5FEffectResourceData, 0xc,
                      &resourceData[offsetof(Dll5FEffectResourceView, colors)], 0x48, 0);
}

void dll_5F_release(void) {
}

void dll_5F_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gDll5FResourceDescriptorAcquire, dll_5F_initialise)

Dll5FResourceDescriptor gDll5FResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll5FResourceDescriptorAcquire, dll_5F_release },
    NULL,
    dll_5F_spawnEffect,
};
