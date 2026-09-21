/*
 * DLL 156 / 0x9C - a variant-driven layered modgfx effect spawner.
 */
#include "main/dll/dll_009C_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll9CEffectResourceView {
    ModgfxEffectVertex vertices[21];
    u8 opaqueD2[2];
    s16 triangles[24][3];
    u8 opaque164[0x4C];
    s16 allVertexIndices[21];
    s16 opaque1DA;
    s16 lastFourteenVertexIndices[14];
    s16 sequenceParams[2][7];
} Dll9CEffectResourceView;

STATIC_ASSERT(offsetof(Dll9CEffectResourceView, vertices) == 0x000);
STATIC_ASSERT(offsetof(Dll9CEffectResourceView, opaqueD2) == 0x0D2);
STATIC_ASSERT(offsetof(Dll9CEffectResourceView, triangles) == 0x0D4);
STATIC_ASSERT(offsetof(Dll9CEffectResourceView, opaque164) == 0x164);
STATIC_ASSERT(offsetof(Dll9CEffectResourceView, allVertexIndices) == 0x1B0);
STATIC_ASSERT(offsetof(Dll9CEffectResourceView, opaque1DA) == 0x1DA);
STATIC_ASSERT(offsetof(Dll9CEffectResourceView, lastFourteenVertexIndices) == 0x1DC);
STATIC_ASSERT(offsetof(Dll9CEffectResourceView, sequenceParams) == 0x1F8);
STATIC_ASSERT(sizeof(Dll9CEffectResourceView) == 0x214);

extern u16 gDll9CEffectResourceData[sizeof(Dll9CEffectResourceView) / sizeof(u16)];

void dll_9C_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    GfxCmd* commandCursor;
    u8* resourceData = (u8*)gDll9CEffectResourceData;
    GfxCmd* commands = packet.entries;
    int idx;
    u8* q;

    commandCursor = commands;
    commandCursor[0].layer = 0;
    commandCursor[0].flags = 0x15;
    commandCursor[0].tex = &resourceData[offsetof(Dll9CEffectResourceView, allVertexIndices)];
    commandCursor[0].mode = 4;
    commandCursor[0].x = 0.0f;
    commandCursor[0].y = 0.0f;
    commandCursor[0].z = 0.0f;
    commandCursor[1].layer = 0;
    commandCursor[1].flags = 0x15;
    commandCursor[1].tex = &resourceData[offsetof(Dll9CEffectResourceView, allVertexIndices)];
    commandCursor[1].mode = 2;
    commandCursor[1].x = 0.01f;
    commandCursor[1].y = 2.0f;
    commandCursor[1].z = 0.01f;
    commandCursor += 2;
    if (variant != 1) {
        commandCursor->layer = 0;
        commandCursor->flags = 0;
        commandCursor->tex = NULL;
        commandCursor->mode = 0x400000;
        commandCursor->x = 0.0f;
        commandCursor->y = 0.0f;
        commandCursor->z = 0.0f;
        commandCursor++;
    }
    if (variant == 1) {
        commandCursor->layer = 0;
        commandCursor->flags = 0;
        commandCursor->tex = NULL;
        commandCursor->mode = 0x80;
        commandCursor->x = (f32)spawnParams->arg2;
        commandCursor->y = (f32)spawnParams->arg1;
        commandCursor->z = (f32)spawnParams->arg0;
        commandCursor++;
    }
    if (variant == 1) {
        commandCursor->layer = 1;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll9CEffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 30.0f;
        commandCursor->y = spawnParams->posY / 30.0f;
        commandCursor->z = 30.0f;
    } else {
        commandCursor->layer = 1;
        commandCursor->flags = 0x15;
        commandCursor->tex = &resourceData[offsetof(Dll9CEffectResourceView, allVertexIndices)];
        commandCursor->mode = 2;
        commandCursor->x = 30.0f;
        commandCursor->y = 1.2f;
        commandCursor->z = 30.0f;
    }
    commandCursor[1].layer = 1;
    commandCursor[1].flags = 0xe;
    commandCursor[1].tex = &resourceData[offsetof(Dll9CEffectResourceView, lastFourteenVertexIndices)];
    commandCursor[1].mode = 4;
    commandCursor[1].x = 155.0f;
    commandCursor[1].y = 0.0f;
    commandCursor[1].z = 0.0f;
    commandCursor[2].layer = 1;
    commandCursor[2].flags = 0x15;
    commandCursor[2].tex = &resourceData[offsetof(Dll9CEffectResourceView, allVertexIndices)];
    commandCursor[2].mode = 0x4000;
    commandCursor[2].x = 2.0f;
    commandCursor[2].y = -4.0f;
    commandCursor[2].z = 0.0f;
    commandCursor += 3;
    if (variant != 1) {
        commandCursor->layer = 1;
        commandCursor->flags = 0;
        commandCursor->tex = NULL;
        commandCursor->mode = 0x100;
        commandCursor->x = 0.0f;
        commandCursor->y = 0.0f;
        commandCursor->z = -150.0f;
        commandCursor++;
    }
    commandCursor[0].layer = 2;
    commandCursor[0].flags = 0x15;
    commandCursor[0].tex = &resourceData[offsetof(Dll9CEffectResourceView, allVertexIndices)];
    commandCursor[0].mode = 0x4000;
    commandCursor[0].x = 2.0f;
    commandCursor[0].y = -4.0f;
    commandCursor[0].z = 0.0f;
    commandCursor[1].layer = 3;
    commandCursor[1].flags = 0x15;
    commandCursor[1].tex = &resourceData[offsetof(Dll9CEffectResourceView, allVertexIndices)];
    commandCursor[1].mode = 0x4000;
    commandCursor[1].x = 2.0f;
    commandCursor[1].y = -4.0f;
    commandCursor[1].z = 0.0f;
    commandCursor[2].layer = 3;
    commandCursor[2].flags = 0xe;
    commandCursor[2].tex = &resourceData[offsetof(Dll9CEffectResourceView, lastFourteenVertexIndices)];
    commandCursor[2].mode = 4;
    commandCursor[2].x = 0.0f;
    commandCursor[2].y = 0.0f;
    commandCursor[2].z = 0.0f;
    commandCursor[3].layer = 1;

    packet.context.modeByte = 0;
    packet.context.attachedSource = sourceObj;
    packet.context.sourceMode = variant;
    packet.context.position[0] = 0.0f;
    packet.context.position[1] = 0.0f;
    packet.context.position[2] = 0.0f;
    packet.context.velocity[0] = 0.0f;
    packet.context.velocity[1] = 0.0f;
    packet.context.velocity[2] = 0.0f;
    packet.context.scale = 4.0f;
    packet.context.drawGroupCount = 2;
    packet.context.drawGroupStride = 7;
    packet.context.initialStateByte = 0xe;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x1e;
    packet.context.commandCount = (s8)(((u8*)(commandCursor + 3) - (u8*)commands) / (int)sizeof(GfxCmd));
    idx = variant * 7;
    q = resourceData + idx * 2;
    packet.context.sequenceParams[0] = *(s16*)&q[offsetof(Dll9CEffectResourceView, sequenceParams)];
    q = resourceData + (idx + 1) * 2;
    packet.context.sequenceParams[1] = *(s16*)&q[offsetof(Dll9CEffectResourceView, sequenceParams)];
    q = resourceData + (idx + 2) * 2;
    packet.context.sequenceParams[2] = *(s16*)&q[offsetof(Dll9CEffectResourceView, sequenceParams)];
    q = resourceData + (idx + 3) * 2;
    packet.context.sequenceParams[3] = *(s16*)&q[offsetof(Dll9CEffectResourceView, sequenceParams)];
    q = resourceData + (idx + 4) * 2;
    packet.context.sequenceParams[4] = *(s16*)&q[offsetof(Dll9CEffectResourceView, sequenceParams)];
    q = resourceData + (idx + 5) * 2;
    packet.context.sequenceParams[5] = *(s16*)&q[offsetof(Dll9CEffectResourceView, sequenceParams)];
    q = resourceData + (idx + 6) * 2;
    packet.context.sequenceParams[6] = *(s16*)&q[offsetof(Dll9CEffectResourceView, sequenceParams)];
    packet.context.commands = packet.entries;
    packet.context.flags = 0xc010480;
    packet.context.flags |= spawnFlags;
    if ((packet.context.flags & 1) != 0) {
        if ((void*)sourceObj != NULL) {
            packet.context.position[0] = packet.context.position[0] + sourceObj->anim.worldPosX;
            packet.context.position[1] = packet.context.position[1] + sourceObj->anim.worldPosY;
            packet.context.position[2] = packet.context.position[2] + sourceObj->anim.worldPosZ;
        } else {
            packet.context.position[0] = packet.context.position[0] + spawnParams->posX;
            packet.context.position[1] = packet.context.position[1] + spawnParams->posY;
            packet.context.position[2] = packet.context.position[2] + spawnParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet.context, 0, 0x15, (u8*)gDll9CEffectResourceData, 0x18,
                      &resourceData[offsetof(Dll9CEffectResourceView, triangles)], 0x154, 0);
}

void dll_9C_release(void) {
}

void dll_9C_initialise(void) {
}

u16 gDll9CEffectResourceData[sizeof(Dll9CEffectResourceView) / sizeof(u16)] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0362, 0x0000, 0x01f4, 0x000b,
    0x0000, 0x0362, 0x0000, 0xfe0c, 0x0016, 0x0000, 0x0000, 0x0000, 0xfc18,
    0x0020, 0x0000, 0xfc9e, 0x0000, 0xfe0c, 0x0016, 0x0000, 0xfc9e, 0x0000,
    0x01f4, 0x000b, 0x0000, 0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0000,
    0x01f4, 0x03e8, 0x0000, 0x000f, 0x0362, 0x01f4, 0x01f4, 0x000b, 0x000f,
    0x0362, 0x01f4, 0xfe0c, 0x0016, 0x000f, 0x0000, 0x01f4, 0xfc18, 0x0020,
    0x000f, 0xfc9e, 0x01f4, 0xfe0c, 0x0016, 0x000f, 0xfc9e, 0x01f4, 0x01f4,
    0x000b, 0x000f, 0x0000, 0x01f4, 0x03e8, 0x0000, 0x000f, 0x0000, 0x1770,
    0x03e8, 0x0000, 0x007f, 0x0362, 0x1770, 0x01f4, 0x000b, 0x007f, 0x0362,
    0x1770, 0xfe0c, 0x0016, 0x007f, 0x0000, 0x1770, 0xfc18, 0x0020, 0x007f,
    0xfc9e, 0x1770, 0xfe0c, 0x0016, 0x007f, 0xfc9e, 0x1770, 0x01f4, 0x000b,
    0x007f, 0x0000, 0x1770, 0x03e8, 0x0000, 0x007f, 0x0000, 0x0000, 0x0001,
    0x0008, 0x0000, 0x0008, 0x0007, 0x0001, 0x0002, 0x0009, 0x0001, 0x0009,
    0x0008, 0x0002, 0x0003, 0x000a, 0x0002, 0x000a, 0x0009, 0x0003, 0x0004,
    0x000b, 0x0003, 0x000b, 0x000a, 0x0004, 0x0005, 0x000c, 0x0004, 0x000c,
    0x000b, 0x0005, 0x0006, 0x000d, 0x0005, 0x000d, 0x000c, 0x0007, 0x0008,
    0x000f, 0x0007, 0x000f, 0x000e, 0x0008, 0x0009, 0x0010, 0x0008, 0x0010,
    0x000f, 0x0009, 0x000a, 0x0011, 0x0009, 0x0011, 0x0010, 0x000a, 0x000b,
    0x0012, 0x000a, 0x0012, 0x0011, 0x000b, 0x000c, 0x0013, 0x000b, 0x0013,
    0x0012, 0x000c, 0x000d, 0x0014, 0x000c, 0x0014, 0x0013, 0x0000, 0x0001,
    0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0000, 0x0007, 0x0008, 0x0009,
    0x000a, 0x000b, 0x000c, 0x000d, 0x0000, 0x000e, 0x000f, 0x0010, 0x0011,
    0x0012, 0x0013, 0x0014, 0x0000, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004,
    0x0005, 0x0006, 0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014,
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
    0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010, 0x0011,
    0x0012, 0x0013, 0x0014, 0x0000, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b,
    0x000c, 0x000d, 0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014,
    0x0000, 0x0096, 0x044c, 0x0032, 0x0000, 0x0000, 0x0000, 0x0000, 0x0028,
    0x000a, 0x0014, 0x0001, 0x0000, 0x0000,
};
RESOURCE_ACQUIRE_ADAPTER(gDll9CResourceDescriptorAcquire, dll_9C_initialise)

Dll9CResourceDescriptor gDll9CResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll9CResourceDescriptorAcquire, dll_9C_release },
    NULL,
    dll_9C_spawnEffect,
    0x00000000,
};
