/*
 * DLL 90 / 0x5A - a staff-collision particle spawner.
 */
#include "main/dll/dll_005A_staffcollision.h"
#include "game/objects/object.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/modgfx_types.h"
#include "main/vecmath.h"

typedef struct StaffCollisionEffectResourceView {
    ModgfxEffectVertex defaultVertices[3];
    u8 pad1E[2];
    ModgfxEffectVertex alternateVertices[4];
    s16 alternateColors[6];
    s16 sequenceParams[7];
    u8 pad62[2];
} StaffCollisionEffectResourceView;

STATIC_ASSERT(offsetof(StaffCollisionEffectResourceView, defaultVertices) == 0x00);
STATIC_ASSERT(offsetof(StaffCollisionEffectResourceView, alternateVertices) == 0x20);
STATIC_ASSERT(offsetof(StaffCollisionEffectResourceView, alternateColors) == 0x48);
STATIC_ASSERT(offsetof(StaffCollisionEffectResourceView, sequenceParams) == 0x54);
STATIC_ASSERT(sizeof(StaffCollisionEffectResourceView) == 0x64);

typedef union StaffCollisionEffectResource {
    u8 bytes[0x64];
    StaffCollisionEffectResourceView view;
} StaffCollisionEffectResource;

STATIC_ASSERT(sizeof(StaffCollisionEffectResource) == 0x64);

u8 gStaffCollisionDefaultColorData[8] = {0, 0, 0, 1, 0, 2, 0, 0};
u8 gStaffCollisionDefaultIndices[8] = {0, 0, 0, 1, 0, 2, 0, 0};
u8 gStaffCollisionAlternateIndices[8] = {0, 0, 0, 1, 0, 2, 0, 3};

StaffCollisionEffectResource gStaffCollisionEffectResourceData = {
    {0,   30, 0, 0,  0, 0, 0, 0,   0,  31, 255, 226, 0,   0,   0, 0, 0,  15,  0,   31, 0, 0, 0, 0, 3,
     232, 0,  8, 0,  0, 0, 0, 0,   15, 0,  0,   0,   0,   0,   0, 0, 31, 255, 241, 0,  0, 0, 0, 0, 15,
     0,   31, 0, 15, 0, 0, 7, 208, 0,  8,  0,   0,   255, 241, 0, 0, 7,  208, 0,   8,  0, 0, 0, 0, 0,
     1,   0,  2, 0,  1, 0, 3, 0,   2,  0,  0,   0,   80,  0,   0, 0, 0,  0,   0,   0,  0, 0, 0, 0, 0}};

void StaffCollision_spawn(GameObject* sourceObj, int mode, PartFxSpawnParams* spawnParams, u32 spawnFlags,
                          int unusedModelId, const StaffCollisionColorArgs* colorArgs) {
    MatrixTransform transform;
    ModgfxSpawnContext packet;
    GfxCmd commandStorage[32];
    GfxCmd* commands = commandStorage;
    int spawnCount;
    StaffCollisionEffectResourceView* resource = &gStaffCollisionEffectResourceData.view;
    s16 colorR, colorG, colorB;
    int spawnIndex;
    colorR = 0xff;
    colorG = 0xff;
    colorB = 0xff;
    spawnCount = 1;
    if (colorArgs != NULL) {
        spawnCount = colorArgs->count;
        colorR = colorArgs->red;
        colorG = colorArgs->green;
        colorB = colorArgs->blue;
    }
    for (spawnIndex = 0; spawnIndex < spawnCount; spawnIndex++) {
        f32 rotationX, rotationY;
        if (mode == 0) {
            colorR += randomGetRange(-0x1b, 0x1b);
            if (colorR > 0xff) {
                colorR = 0xff;
            } else if (colorR < 0) {
                colorR = 0;
            }
            colorG += randomGetRange(-0x1b, 0x1b);
            if (colorG > 0xff) {
                colorG = 0xff;
            } else if (colorG < 0) {
                colorG = 0;
            }
            colorB += randomGetRange(-0x1b, 0x1b);
            if (colorB > 0xff) {
                colorB = 0xff;
            } else if (colorB < 0) {
                colorB = 0;
            }
        }
        commands[0].layer = 0;
        commands[0].flags = mode != 0 ? 4 : 3;
        commands[0].tex = mode != 0 ? gStaffCollisionAlternateIndices : gStaffCollisionDefaultIndices;
        commands[0].mode = 8;
        commands[0].x = colorR;
        commands[0].y = colorG;
        commands[0].z = colorB;
        rotationX = (f32)(int)randomGetRange(0, 0xfffe);
        rotationY = (f32)(int)randomGetRange(-0xbb8, -0x2ee0);
        commands[1].layer = 0;
        commands[1].flags = 0;
        commands[1].tex = NULL;
        commands[1].mode = 0x80;
        commands[1].x = 0.0f;
        commands[1].y = rotationY;
        commands[1].z = rotationX;
        commands[2].layer = 0;
        commands[2].flags = mode != 0 ? 4 : 3;
        commands[2].tex = mode != 0 ? gStaffCollisionAlternateIndices : gStaffCollisionDefaultIndices;
        commands[2].mode = 2;
        commands[2].x = 1.0f;
        commands[2].y = 0.5f;
        commands[2].z = 1.5f;
        commands[3].layer = 1;
        commands[3].flags = 0;
        commands[3].tex = NULL;
        commands[3].mode = 0x400000;
        commands[3].x = 0.0f;
        commands[3].y = 0.0f;
        commands[3].z = 400.0f;
        transform.x = 0.0f;
        transform.y = 0.0f;
        transform.z = 0.0f;
        transform.scale = 1.0f;
        transform.rotZ = 0;
        transform.rotY = rotationY;
        transform.rotX = rotationX;
        vecRotateZXY(&transform.rotX, &commands[3].x);
        packet.modeByte = 0;
        packet.attachedSource = sourceObj;
        packet.sourceMode = mode;
        packet.position[0] = 0.0f;
        packet.position[1] = 0.0f;
        packet.position[2] = 0.0f;
        packet.velocity[0] = 0.0f;
        packet.velocity[1] = 0.0f;
        packet.velocity[2] = 0.0f;
        packet.scale = 1.0f;
        packet.drawGroupCount = 1;
        packet.drawGroupStride = 0;
        packet.initialStateByte = mode != 0 ? 4 : 3;
        packet.byte5A = 0;
        packet.textureFrameTimer = 0x10;
        packet.commandCount = 4;
        packet.sequenceParams[0] = fhReadBES16(&resource->sequenceParams[0]);
        packet.sequenceParams[1] = fhReadBES16(&resource->sequenceParams[1]);
        packet.sequenceParams[2] = fhReadBES16(&resource->sequenceParams[2]);
        packet.sequenceParams[3] = fhReadBES16(&resource->sequenceParams[3]);
        packet.sequenceParams[4] = fhReadBES16(&resource->sequenceParams[4]);
        packet.sequenceParams[5] = fhReadBES16(&resource->sequenceParams[5]);
        packet.sequenceParams[6] = fhReadBES16(&resource->sequenceParams[6]);
        packet.commands = commandStorage;
        packet.flags = 0x2000490;
        packet.flags |= spawnFlags;
        if ((packet.flags & 1) != 0) {
            if (packet.attachedSource != NULL && spawnParams != NULL) {
                packet.position[0] += packet.attachedSource->anim.worldPosX + spawnParams->posX;
                packet.position[1] += packet.attachedSource->anim.worldPosY + spawnParams->posY;
                packet.position[2] += packet.attachedSource->anim.worldPosZ + spawnParams->posZ;
            } else if (packet.attachedSource != NULL) {
                packet.position[0] += packet.attachedSource->anim.worldPosX;
                packet.position[1] += packet.attachedSource->anim.worldPosY;
                packet.position[2] += packet.attachedSource->anim.worldPosZ;
            } else if (spawnParams != NULL) {
                packet.position[0] += spawnParams->posX;
                packet.position[1] += spawnParams->posY;
                packet.position[2] += spawnParams->posZ;
            }
        }
        (*gModgfxInterface)
            ->spawnEffect(&packet, 0, mode != 0 ? 4 : 3,
                          mode != 0 ? (void*)resource->alternateVertices : (void*)resource->defaultVertices,
                          mode != 0 ? 2 : 1,
                          mode != 0 ? (void*)resource->alternateColors : (void*)gStaffCollisionDefaultColorData, 0, 0);
    }
}

StaffCollisionResourceDescriptor gStaffCollisionResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, NULL, NULL },
    NULL,
    StaffCollision_spawn,
    0x00000000,
};
