#include "main/dll/dll_008A_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"

typedef struct Dll8AEffectResourceView {
    ModgfxEffectVertex vertices[8];
    s16 triangles[12][3];
    s16 allVertexIndices[8];
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll8AEffectResourceView;

STATIC_ASSERT(offsetof(Dll8AEffectResourceView, vertices) == 0x00);
STATIC_ASSERT(offsetof(Dll8AEffectResourceView, triangles) == 0x50);
STATIC_ASSERT(offsetof(Dll8AEffectResourceView, allVertexIndices) == 0x98);
STATIC_ASSERT(offsetof(Dll8AEffectResourceView, sequenceParams) == 0xA8);
STATIC_ASSERT(offsetof(Dll8AEffectResourceView, opaqueTail) == 0xB6);
STATIC_ASSERT(sizeof(Dll8AEffectResourceView) == 0xB8);

u8 gDll8AEffectResourceData[sizeof(Dll8AEffectResourceView)] = {
    254, 12,  254, 12, 254, 12,  0,   0,   0,   0,   1,  244, 254, 12, 254, 12,  0,  32,  0,  32,  1,   244, 254,
    12,  1,   244, 0,  0,   0,   0,   254, 12,  254, 12, 1,   244, 0,  32,  0,   32, 254, 12, 1,   244, 254, 12,
    0,   0,   0,   0,  1,   244, 1,   244, 254, 12,  0,  32,  0,   32, 1,   244, 1,  244, 1,  244, 0,   0,   0,
    0,   254, 12,  1,  244, 1,   244, 0,   32,  0,   32, 0,   0,   0,  4,   0,   5,  0,   0,  0,   5,   0,   1,
    0,   1,   0,   5,  0,   6,   0,   1,   0,   6,   0,  2,   0,   2,  0,   6,   0,  7,   0,  2,   0,   7,   0,
    3,   0,   3,   0,  7,   0,   4,   0,   3,   0,   4,  0,   0,   0,  0,   0,   1,  0,   2,  0,   0,   0,   2,
    0,   3,   0,   4,  0,   7,   0,   6,   0,   4,   0,  6,   0,   5,  0,   0,   0,  1,   0,  2,   0,   3,   0,
    4,   0,   5,   0,  6,   0,   7,   0,   0,   0,   10, 0,   0,   0,  0,   0,   0,  0,   0,  0,   0,   0,   0};

void dll_8A_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    Dll8AEffectResourceView* resource = (Dll8AEffectResourceView*)gDll8AEffectResourceData;
    GfxCmd* commands = packet.entries;

    commands[0].layer = 0;
    commands[0].flags = 8;
    commands[0].tex = resource->allVertexIndices;
    commands[0].mode = 2;
    commands[0].x = 0.5f;
    commands[0].y = 0.5f;
    commands[0].z = 0.5f;
    packet.context.modeByte = 0;
    packet.context.attachedSource = sourceObj;
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
    packet.context.initialStateByte = 8;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0x10;
    packet.context.flags = 0x2000492;
    packet.context.commandCount = 1;
    for (s32 paramIndex = 0; paramIndex < ARRAY_COUNT(packet.context.sequenceParams); paramIndex++) {
        packet.context.sequenceParams[paramIndex] = fhReadBES16(&resource->sequenceParams[paramIndex]);
    }
    packet.context.commands = packet.entries;
    packet.context.flags |= spawnFlags;
    if ((packet.context.flags & 1) != 0) {
        if (sourceObj != NULL) {
            GameObject* anchorObj = sourceObj;
            packet.context.position[0] += anchorObj->anim.worldPosX;
            packet.context.position[1] += anchorObj->anim.worldPosY;
            packet.context.position[2] += anchorObj->anim.worldPosZ;
        } else {
            PartFxSpawnParams* anchorParams = spawnParams;
            packet.context.position[0] += anchorParams->posX;
            packet.context.position[1] += anchorParams->posY;
            packet.context.position[2] += anchorParams->posZ;
        }
    }
    (*gModgfxInterface)
        ->spawnEffect(&packet.context, 0, ARRAY_COUNT(resource->vertices), resource->vertices,
                      ARRAY_COUNT(resource->triangles), resource->triangles, 0x1FD, NULL);
}

void dll_8A_release(void) {
}

void dll_8A_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gDll8AResourceDescriptorAcquire, dll_8A_initialise)

Dll8AResourceDescriptor gDll8AResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll8AResourceDescriptorAcquire, dll_8A_release },
    NULL,
    dll_8A_spawnEffect,
};
