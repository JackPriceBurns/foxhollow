/*
 * DIMSnowBall (DLL 0x1C2) - timed snowball spawner for Dinosaur Island
 * Mission.  On each timer expiry, if loading is not locked and the player
 * is clear, allocates a 36-byte setup for rolling-snowball sequence 0x196,
 * seeds it from the placement params, and resets the spawn countdown.
 */

#include "dlls/objects/450_DIMSnowBall.h"

#include "dlls/objects/449_DIMSnowBall.h"
#include "main/dll/player_api.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

struct DimSnowBallSpawnerPlacement {
    ObjPlacement base;
    s16 spawnPeriod;
    u8 childRotationParam1A;
    u8 childRotationParam1CBase;
    u8 rotationXByte;
    u8 unknown1D[3];
};

typedef struct DimSnowBallSpawnerState {
    s16 spawnCountdown;
    s16 spawnPeriod;
} DimSnowBallSpawnerState;

STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, spawnPeriod) == 0x18);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, childRotationParam1A) == 0x1A);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, childRotationParam1CBase) == 0x1B);
STATIC_ASSERT(offsetof(DimSnowBallSpawnerPlacement, rotationXByte) == 0x1C);
STATIC_ASSERT(sizeof(DimSnowBallSpawnerPlacement) == 0x20);
STATIC_ASSERT(sizeof(DimSnowBallSpawnerState) == 0x04);

int dimsnowball1c2_getExtraSize(void) {
    return sizeof(DimSnowBallSpawnerState);
}

int dimsnowball1c2_getObjectTypeId(void) {
    return 0;
}

void dimsnowball1c2_free(void) {
}

void dimsnowball1c2_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                           s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void dimsnowball1c2_hitDetect(void) {
}

void dimsnowball1c2_update(GameObject* obj) {
    if (Obj_IsLoadingLocked()) {
        DimSnowBallSpawnerState* state = obj->extra;

        if ((state->spawnCountdown -= framesThisStep) <= 0) {
            if (playerGetFocusObject(Obj_GetPlayerObject()) == NULL) {
                const DimSnowBallSpawnerPlacement* placement =
                    (const DimSnowBallSpawnerPlacement*)obj->anim.placementData;
                DimSnowBallPlacement* setup =
                    (DimSnowBallPlacement*)Obj_AllocObjectSetup(sizeof(DimSnowBallPlacement), DIM_SNOWBALL_SEQUENCE_ID);
                for (int i = 0; i < ARRAY_COUNT(setup->base.color); i++) {
                    setup->base.color[i] = placement->base.color[i];
                }
                setup->base.posX = obj->anim.localPosX;
                setup->base.posY = obj->anim.localPosY;
                setup->base.posZ = obj->anim.localPosZ;
                setup->base.ident = placement->base.ident;
                setup->rotationXByte = (s8)placement->rotationXByte;
                setup->rotationParam1A = placement->childRotationParam1A;
                setup->rotationParam1C = (s16)((f32)placement->childRotationParam1CBase +
                                               (f32)randomGetRange(0, 100) / 100.0f);
                objSetupObject(&setup->base, 5, obj->anim.mapEventSlot, -1, NULL);
                state->spawnCountdown = state->spawnPeriod;
            }
        }
    }
}

void dimsnowball1c2_init(GameObject* obj, const DimSnowBallSpawnerPlacement* placement) {
    DimSnowBallSpawnerState* state = obj->extra;
    s16 spawnPeriod = ObjAnim_ReadPlacementS16(&obj->anim, &placement->spawnPeriod);

    obj->anim.rotX = (s16)((u16)placement->rotationXByte << 8);
    state->spawnPeriod = spawnPeriod;
    state->spawnCountdown = spawnPeriod;
    obj->objectFlags |= (OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED);
}

void dimsnowball1c2_release(void) {
}

void dimsnowball1c2_initialise(void) {
}

ObjectDescriptor gDIMSnowBall1C2ObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)dimsnowball1c2_initialise,
    (ObjectDescriptorCallback)dimsnowball1c2_release,
    0,
    (ObjectDescriptorCallback)dimsnowball1c2_init,
    (ObjectDescriptorCallback)dimsnowball1c2_update,
    (ObjectDescriptorCallback)dimsnowball1c2_hitDetect,
    (ObjectDescriptorCallback)dimsnowball1c2_render,
    (ObjectDescriptorCallback)dimsnowball1c2_free,
    (ObjectDescriptorCallback)dimsnowball1c2_getObjectTypeId,
    dimsnowball1c2_getExtraSize,
};
