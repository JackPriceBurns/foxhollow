/*
 * GPSH_ObjCre (DLL 0x193) - Test of Knowledge symbol creator.
 *
 * The shrine activates six indexed child types. Each creator counts down,
 * emits a hit effect, and spawns its configured child at the creator's
 * position. The shrine reset bit re-arms the creator for another attempt.
 */
#include "dlls/objects/403_GPSH_ObjCre.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/object_render.h"
#include "main/objfx.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

typedef struct GPSHObjCreatorChildSetup {
    ObjPlacement base;
    u8 initialYaw;
    u8 unk19;
    s16 symbolParam;
    u8 unk1C[8];
} GPSHObjCreatorChildSetup;

typedef struct GPSHObjCreatorPlacement {
    ObjPlacement base;
    s16 unk18;
    s16 symbolIndex;
    u8 unk1C[2];
    s8 initialYaw;
    u8 unk1F[5];
} GPSHObjCreatorPlacement;

typedef struct GPSHObjCreatorState {
    f32 spawnTimer;
    u8 symbolIndex;
    u8 flags;
    u8 unk06[2];
} GPSHObjCreatorState;

typedef enum GPSHObjCreatorFlags {
    GPSH_OBJ_CREATOR_CHILD_SPAWNED = 1 << 0,
} GPSHObjCreatorFlags;

STATIC_ASSERT(sizeof(GPSHObjCreatorChildSetup) == 0x24);
STATIC_ASSERT(offsetof(GPSHObjCreatorChildSetup, initialYaw) == 0x18);
STATIC_ASSERT(offsetof(GPSHObjCreatorChildSetup, symbolParam) == 0x1A);
STATIC_ASSERT(sizeof(GPSHObjCreatorPlacement) == 0x24);
STATIC_ASSERT(offsetof(GPSHObjCreatorPlacement, symbolIndex) == 0x1A);
STATIC_ASSERT(offsetof(GPSHObjCreatorPlacement, initialYaw) == 0x1E);
STATIC_ASSERT(sizeof(GPSHObjCreatorState) == 0x08);
STATIC_ASSERT(offsetof(GPSHObjCreatorState, symbolIndex) == 0x04);
STATIC_ASSERT(offsetof(GPSHObjCreatorState, flags) == 0x05);

static const s16 gGPSHObjCreatorSymbolParams[] = {
    0x28, 0x28, 0x30, 0x30, 0x2D, 0x2D,
};

static int gpshObjCreator_getExtraSize(void) {
    return sizeof(GPSHObjCreatorState);
}

static int gpshObjCreator_getObjectTypeId(void) {
    return 0;
}

static void gpshObjCreator_free(void) {
}

static void gpshObjCreator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                  s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void gpshObjCreator_hitDetect(void) {
}

static void gpshObjCreator_update(GameObject* obj) {
    GPSHObjCreatorState* state = obj->extra;

    if (mainGetBit(GAMEBIT_GPSH_ResetSymbolCreators) != 0) {
        obj->userData2 = 0;
        state->flags &= ~GPSH_OBJ_CREATOR_CHILD_SPAWNED;
        obj->anim.renderAlpha = 0xFF;
        obj->anim.alpha = 0xFF;
    }
    if ((state->flags & GPSH_OBJ_CREATOR_CHILD_SPAWNED) != 0) {
        return;
    }
    if (obj->userData2 == 0 && mainGetBit(GAMEBIT_GPSH_ActivateSymbolSpawns) != 0) {
        state->spawnTimer = 100.0f;
        obj->userData2 = 1;
    }
    if (Obj_IsLoadingLocked() == 0) {
        return;
    }
    if (state->spawnTimer == 0.0f) {
        return;
    }

    state->spawnTimer -= timeDelta;
    objfx_spawnHitEffectBurst(obj, 0.6f, 2, 1, 1, NULL);
    if (state->spawnTimer <= 0.0f) {
        Sfx_PlayFromObjectLimited(0, SFXTRIG_wp_hitpos_6_167, 1);
        GPSHObjCreatorChildSetup* childSetup =
            (GPSHObjCreatorChildSetup*)Obj_AllocObjectSetup(sizeof(GPSHObjCreatorChildSetup),
                                                           state->symbolIndex + 0x1F4);

        state->flags |= GPSH_OBJ_CREATOR_CHILD_SPAWNED;
        childSetup->base.color[3] = 0xFF;
        childSetup->base.color[0] = 0x20;
        childSetup->base.color[1] = 2;
        childSetup->base.posX = obj->anim.localPosX;
        childSetup->base.posY = obj->anim.localPosY;
        childSetup->base.posZ = obj->anim.localPosZ;
        childSetup->base.objectId = (s16)(state->symbolIndex + 0x1F4);
        childSetup->initialYaw = (u8)(obj->anim.rotX >> 8);
        childSetup->symbolParam = gGPSHObjCreatorSymbolParams[state->symbolIndex];
        objSetupObject(&childSetup->base, 5, obj->anim.mapEventSlot, -1, obj->anim.parent);
    }
}

static void gpshObjCreator_init(GameObject* obj, const GPSHObjCreatorPlacement* placement) {
    GPSHObjCreatorState* state = obj->extra;

    obj->anim.rotX = (s16)((s32)placement->initialYaw * 0x100);
    obj->userData2 = 0;
    state->symbolIndex = (u8)ObjAnim_ReadPlacementS16(&obj->anim, &placement->symbolIndex);
    state->flags &= ~GPSH_OBJ_CREATOR_CHILD_SPAWNED;
    obj->anim.renderAlpha = 0xFF;
    obj->anim.alpha = 0xFF;
}

static void gpshObjCreator_release(void) {
}

static void gpshObjCreator_initialise(void) {
}

OBJECT_INIT_ADAPTER(gGPSHObjCreatorObjDescriptorInitAdapter, gpshObjCreator_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gGPSHObjCreatorObjDescriptorHitDetectAdapter, gpshObjCreator_hitDetect)
OBJECT_FREE_ADAPTER(gGPSHObjCreatorObjDescriptorFreeAdapter, gpshObjCreator_free)
OBJECT_TYPE_ID_ADAPTER(gGPSHObjCreatorObjDescriptorTypeIdAdapter, gpshObjCreator_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gGPSHObjCreatorObjDescriptorExtraSizeAdapter, gpshObjCreator_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gGPSHObjCreatorObjDescriptorAcquire, gpshObjCreator_initialise)

ObjectDescriptor10WithPadding gGPSHObjCreatorObjDescriptor = {
    .descriptor =
        {
            .header.metadata[3] = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
            .header.acquire = gGPSHObjCreatorObjDescriptorAcquire,
            .header.release = gpshObjCreator_release,
            .init = gGPSHObjCreatorObjDescriptorInitAdapter,
            .update = gpshObjCreator_update,
            .hitDetect = gGPSHObjCreatorObjDescriptorHitDetectAdapter,
            .render = gpshObjCreator_render,
            .free = gGPSHObjCreatorObjDescriptorFreeAdapter,
            .getObjectTypeId = gGPSHObjCreatorObjDescriptorTypeIdAdapter,
            .getExtraSize = gGPSHObjCreatorObjDescriptorExtraSizeAdapter,
        },
};
