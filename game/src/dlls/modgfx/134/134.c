#include "main/dll/dll_0086_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"
#include "main/vecmath.h"

typedef struct Dll86SequenceResource {
    s16 sequenceParams[7];
    s16 opaqueTail;
} Dll86SequenceResource;

typedef enum Dll86Variant {
    DLL86_VARIANT_ZERO,
    DLL86_VARIANT_ONE,
    DLL86_VARIANT_TWO,
    DLL86_VARIANT_THREE
} Dll86Variant;

STATIC_ASSERT(offsetof(Dll86SequenceResource, sequenceParams) == 0x00);
STATIC_ASSERT(offsetof(Dll86SequenceResource, opaqueTail) == 0x0E);
STATIC_ASSERT(sizeof(Dll86SequenceResource) == 0x10);

Dll86SequenceResource gDll86SequenceResource = {{0, 255, 0, 0, 0, 0, 0}, 0};

void dll_86_spawnEffect(GameObject* sourceObj, int variant, PartFxSpawnParams* spawnParams, u32 spawnFlags) {
    ModgfxSpawnPacket packet;
    GfxCmd* commands = packet.entries;
    f32 effectWidth = 81.0f;
    f32 effectHeight = 82.0f;
    int commandFlags = 0x64;
    f32 randomX;

    switch (variant) {
    case DLL86_VARIANT_ZERO:
        effectWidth = 18.0f;
        effectHeight = 8.0f;
        commandFlags = 0x410;
        break;
    case DLL86_VARIANT_ONE:
        effectWidth = 19.0f;
        effectHeight = 9.0f;
        commandFlags = 0x410;
        break;
    case DLL86_VARIANT_TWO:
    case DLL86_VARIANT_THREE:
        effectWidth = 20.0f;
        effectHeight = 15.0f;
        commandFlags = 0x410;
        break;
    }

    commands[0].layer = 0;
    commands[0].flags = commandFlags;
    commands[0].tex = NULL;
    commands[0].mode = 0x20000000;
    commands[0].x = 999.0f;
    commands[0].y = effectWidth;
    commands[0].z = effectHeight;
    commands[1].layer = 1;
    commands[1].flags = 0;
    commands[1].tex = NULL;
    commands[1].mode = 0x400000;
    commands[1].x = (f32)randomGetRange(-0x64, 0x64);
    commands[1].y = 0.0f;
    commands[1].z = (f32)randomGetRange(-0x4b0, -0x320);
    commands[2].layer = 1;
    commands[2].flags = 0;
    commands[2].tex = NULL;
    commands[2].mode = 0x40000000;
    commands[2].x = commands[1].x;
    commands[2].y = 0.0f;
    commands[2].z = commands[1].y;
    commands[3].layer = 1;
    commands[3].flags = 0x65;
    commands[3].tex = NULL;
    commands[3].mode = 0x800000;
    commands[3].x = 1.0f;
    commands[3].y = 1.0f;
    commands[3].z = 0.0f;
    commands[4].layer = 2;
    commands[4].flags = 0;
    commands[4].tex = NULL;
    commands[4].mode = 0x20000000;
    commands[4].x = 999.0f;
    commands[4].y = effectWidth;
    commands[4].z = effectHeight;
    packet.context.modeByte = 0;
    packet.context.attachedSource = sourceObj;
    packet.context.sourceMode = variant;
    randomX = (f32)randomGetRange(-0x64, 0x64);
    packet.context.position[0] = randomX;
    packet.context.position[1] = 0.0f;
    packet.context.position[2] = 0.0f;
    packet.context.velocity[0] = 0.0f;
    packet.context.velocity[1] = 0.0f;
    packet.context.velocity[2] = 0.0f;
    packet.context.scale = 1.0f;
    packet.context.drawGroupCount = 0;
    packet.context.drawGroupStride = 0;
    packet.context.initialStateByte = 0;
    packet.context.byte5A = 0;
    packet.context.textureFrameTimer = 0;
    packet.context.commandCount = 5;
    for (s32 paramIndex = 0; paramIndex < ARRAY_COUNT(packet.context.sequenceParams); paramIndex++) {
        packet.context.sequenceParams[paramIndex] = gDll86SequenceResource.sequenceParams[paramIndex];
    }
    packet.context.commands = packet.entries;
    packet.context.flags = 0x10400;
    packet.context.flags |= spawnFlags;
    if ((packet.context.flags & 1) != 0) {
        if (packet.context.attachedSource != NULL) {
            packet.context.position[0] = randomX + packet.context.attachedSource->anim.worldPosX;
            packet.context.position[1] += packet.context.attachedSource->anim.worldPosY;
            packet.context.position[2] += packet.context.attachedSource->anim.worldPosZ;
        } else {
            packet.context.position[0] = randomX + spawnParams->posX;
            packet.context.position[1] += spawnParams->posY;
            packet.context.position[2] += spawnParams->posZ;
        }
    }
    (*gModgfxInterface)->spawnEffect(&packet.context, 0, 0, 0, 0, 0, 0, 0);
}

void dll_86_release(void) {
}

void dll_86_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gDll86ResourceDescriptorAcquire, dll_86_initialise)

Dll86ResourceDescriptor gDll86ResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDll86ResourceDescriptorAcquire, dll_86_release },
    NULL,
    dll_86_spawnEffect,
};
