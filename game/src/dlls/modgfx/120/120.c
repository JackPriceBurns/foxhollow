/*
 * DLL 120 / 0x78 - a fixed-resource modgfx effect spawner.
 */
#include "main/dll/dll_0078_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll78EffectResourceView {
    ModgfxEffectVertex vertices[14];
    s16 triangles[12][3];
    s16 allVertexIndices[14];
    s16 firstSevenVertexIndices[8];
    s16 secondSevenVertexIndices[8];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll78EffectResourceView;

STATIC_ASSERT(offsetof(Dll78EffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll78EffectResourceView, triangles) == 0x08C);
STATIC_ASSERT(offsetof(Dll78EffectResourceView, allVertexIndices) == 0x0D4);
STATIC_ASSERT(offsetof(Dll78EffectResourceView, firstSevenVertexIndices) == 0x0F0);
STATIC_ASSERT(offsetof(Dll78EffectResourceView, secondSevenVertexIndices) == 0x100);
STATIC_ASSERT(offsetof(Dll78EffectResourceView, sequenceParams) == 0x110);
STATIC_ASSERT(offsetof(Dll78EffectResourceView, opaqueTail) == 0x11E);
STATIC_ASSERT(sizeof(Dll78EffectResourceView) == 0x120);

u16 gDll78EffectResourceData[sizeof(Dll78EffectResourceView) / sizeof(u16)] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0362, 0x0000, 0x01f4, 0x000b, 0x0000, 0x0362, 0x0000, 0xfe0c, 0x0016,
    0x0000, 0x0000, 0x0000, 0xfc18, 0x0020, 0x0000, 0xfc9e, 0x0000, 0xfe0c, 0x002a, 0x0000, 0xfc9e, 0x0000, 0x01f4,
    0x0035, 0x0000, 0x0000, 0x0000, 0x03e8, 0x0040, 0x0000, 0x0000, 0x1770, 0x03e8, 0x0000, 0x001f, 0x0362, 0x1770,
    0x01f4, 0x000b, 0x001f, 0x0362, 0x1770, 0xfe0c, 0x0016, 0x001f, 0x0000, 0x1770, 0xfc18, 0x0020, 0x001f, 0xfc9e,
    0x1770, 0xfe0c, 0x002a, 0x001f, 0xfc9e, 0x1770, 0x01f4, 0x0035, 0x001f, 0x0000, 0x1770, 0x03e8, 0x0040, 0x001f,
    0x0000, 0x0001, 0x0008, 0x0000, 0x0008, 0x0007, 0x0001, 0x0002, 0x0009, 0x0001, 0x0009, 0x0008, 0x0002, 0x0003,
    0x000a, 0x0002, 0x000a, 0x0009, 0x0003, 0x0004, 0x000b, 0x0003, 0x000b, 0x000a, 0x0004, 0x0005, 0x000c, 0x0004,
    0x000c, 0x000b, 0x0005, 0x0006, 0x000d, 0x0005, 0x000d, 0x000c, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
    0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005,
    0x0006, 0x0000, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x0000, 0x0000, 0x0014, 0x0028, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000,
};

void dll_78_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    u8* resourceData = (u8*)gDll78EffectResourceData;
    GfxCmd* commands = packet.entries;
    f32 originOffset = 0.0f;

    commands[0].layer = 0;
    commands[0].flags = 0xc8;
    commands[0].tex = NULL;
    commands[0].mode = 0x800000;
    commands[0].x = 1.0f;
    commands[0].y = originOffset;
    commands[0].z = originOffset;
    commands[1].layer = 0;
    commands[1].flags = 0xe;
    commands[1].tex = &resourceData[offsetof(Dll78EffectResourceView, allVertexIndices)];
    commands[1].mode = 0x80;
    commands[1].x = originOffset;
    commands[1].y = originOffset;
    if (spawnParams != NULL) {
        commands[1].z = (f32)spawnParams->unk0;
    } else {
        commands[1].z = originOffset;
    }
    commands[2].layer = 0;
    commands[2].flags = 7;
    commands[2].tex = &resourceData[offsetof(Dll78EffectResourceView, secondSevenVertexIndices)];
    commands[2].mode = 4;
    commands[2].x = originOffset;
    commands[2].y = originOffset;
    commands[2].z = originOffset;
    commands[3].layer = 0;
    commands[3].flags = 7;
    commands[3].tex = &resourceData[offsetof(Dll78EffectResourceView, firstSevenVertexIndices)];
    commands[3].mode = 2;
    commands[3].x = 0.3f;
    commands[3].y = 0.7f;
    commands[3].z = 0.3f;
    commands[4].layer = 0;
    commands[4].flags = 7;
    commands[4].tex = &resourceData[offsetof(Dll78EffectResourceView, secondSevenVertexIndices)];
    commands[4].mode = 2;
    if (spawnParams != NULL) {
        commands[4].x = 1.0f;
        commands[4].y = 0.5f;
        commands[4].z = 1.0f;
    } else {
        commands[4].x = 1.0f;
        commands[4].y = 0.5f;
        commands[4].z = 1.0f;
    }
    commands[5].layer = 1;
    commands[5].flags = 7;
    commands[5].tex = &resourceData[offsetof(Dll78EffectResourceView, secondSevenVertexIndices)];
    commands[5].mode = 2;
    if (spawnParams != NULL) {
        commands[5].x = 0.01f * (3.5f * (f32)spawnParams->unk4);
        commands[5].y = 0.01f * (2.0f * (f32)spawnParams->unk4);
        commands[5].z = 0.01f * (3.5f * (f32)spawnParams->unk4);
    } else {
        commands[5].x = 3.5f;
        commands[5].y = 2.0f;
        commands[5].z = 3.5f;
    }
    commands[6].layer = 1;
    commands[6].flags = 0x7a;
    commands[6].tex = NULL;
    commands[6].mode = 0x10000;
    commands[6].x = originOffset;
    commands[6].y = originOffset;
    commands[6].z = originOffset;
    commands[7].layer = 1;
    commands[7].flags = 0xe;
    commands[7].tex = &resourceData[offsetof(Dll78EffectResourceView, allVertexIndices)];
    commands[7].mode = 0x4000;
    commands[7].x = originOffset;
    commands[7].y = -1.5f;
    commands[7].z = originOffset;
    commands[8].layer = 1;
    commands[8].flags = 7;
    commands[8].tex = &resourceData[offsetof(Dll78EffectResourceView, firstSevenVertexIndices)];
    commands[8].mode = 4;
    commands[8].x = 255.0f;
    commands[8].y = originOffset;
    commands[8].z = originOffset;
    commands[9].layer = 2;
    commands[9].flags = 0xe;
    commands[9].tex = &resourceData[offsetof(Dll78EffectResourceView, allVertexIndices)];
    commands[9].mode = 2;
    commands[9].x = 3.0f;
    commands[9].y = 0.1f;
    commands[9].z = 3.0f;
    commands[10].layer = 2;
    commands[10].flags = 0xe;
    commands[10].tex = &resourceData[offsetof(Dll78EffectResourceView, allVertexIndices)];
    commands[10].mode = 0x4000;
    commands[10].x = originOffset;
    commands[10].y = -3.0f;
    commands[10].z = originOffset;
    commands[11].layer = 2;
    commands[11].flags = 7;
    commands[11].tex = &resourceData[offsetof(Dll78EffectResourceView, firstSevenVertexIndices)];
    commands[11].mode = 4;
    commands[11].x = originOffset;
    commands[11].y = originOffset;
    commands[11].z = originOffset;
    packet.context.modeByte = 0;
    packet.context.attachedSource = sourceObj;
    packet.context.sourceMode = variant;
    if (spawnParams != NULL) {
        packet.context.position[0] = spawnParams->posX;
        packet.context.position[1] = spawnParams->posY;
        packet.context.position[2] = spawnParams->posZ;
    } else {
        packet.context.position[0] = originOffset;
        packet.context.position[1] = originOffset;
        packet.context.position[2] = originOffset;
    }
    packet.context.velocity[0] = originOffset;
    packet.context.velocity[1] = originOffset;
    packet.context.velocity[2] = originOffset;
    packet.context.scale = 1.0f;
    packet.context.drawGroupCount = 1;
    packet.context.drawGroupStride = 0;
    packet.context.initialStateByte = 0xe;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x10;
    packet.context.commandCount = (commands + 11) - packet.entries;
    packet.context.sequenceParams[0] = *(s16*)&resourceData[offsetof(Dll78EffectResourceView, sequenceParams[0])];
    packet.context.sequenceParams[1] = *(s16*)&resourceData[offsetof(Dll78EffectResourceView, sequenceParams[1])];
    packet.context.sequenceParams[2] = *(s16*)&resourceData[offsetof(Dll78EffectResourceView, sequenceParams[2])];
    packet.context.sequenceParams[3] = *(s16*)&resourceData[offsetof(Dll78EffectResourceView, sequenceParams[3])];
    packet.context.sequenceParams[4] = *(s16*)&resourceData[offsetof(Dll78EffectResourceView, sequenceParams[4])];
    packet.context.sequenceParams[5] = *(s16*)&resourceData[offsetof(Dll78EffectResourceView, sequenceParams[5])];
    packet.context.sequenceParams[6] = *(s16*)&resourceData[offsetof(Dll78EffectResourceView, sequenceParams[6])];
    packet.context.commands = packet.entries;
    packet.context.flags = 0x4000400;
    packet.context.flags |= spawnFlags;
    if ((packet.context.flags & 1) != 0) {
        if (packet.context.attachedSource != NULL) {
            packet.context.position[0] += packet.context.attachedSource->anim.worldPosX;
            packet.context.position[1] += packet.context.attachedSource->anim.worldPosY;
            packet.context.position[2] += packet.context.attachedSource->anim.worldPosZ;
        } else {
            packet.context.position[0] += spawnParams->posX;
            packet.context.position[1] += spawnParams->posY;
            packet.context.position[2] += spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet.context, 0, 0xe, resourceData, 0xc,
                      &resourceData[offsetof(Dll78EffectResourceView, triangles)], 0x34, 0);
}

void dll_78_release(void) {
}

void dll_78_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gDll78ResourceDescriptorAcquire, dll_78_initialise)

Dll78ResourceDescriptor gDll78ResourceDescriptor = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll78ResourceDescriptorAcquire, dll_78_release},
    NULL,
    dll_78_spawnEffect,
};
