/*
 * MMSH_WaterS (DLL 0x18E) - water-spike hazard in Moon Mountain Pass.
 *
 * Each spike follows the Y coordinate of a shared XYZ Animator. If that
 * animator is unavailable, the spike falls back to nearby water surfaces.
 */
#include "dlls/objects/398_MMSH_WaterS.h"

#include "dlls/objects/316_XYZAnimator.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/debug.h"
#include "main/dll/player_state.h"
#include "main/dll/waterfx_interface.h"
#include "main/frame_timing.h"
#include "main/objhits.h"
#include "main/track_dolphin.h"
#include "main/vecmath.h"
#include "sys/objects.h"

typedef struct MmshWaterSpikePlacement {
    ObjPlacement base;
    u8 pad18[2];
    s16 xyzAnimatorObjectIdLow;
    s16 xyzAnimatorObjectIdHigh;
    u8 pad1E[6];
} MmshWaterSpikePlacement;

STATIC_ASSERT(sizeof(MmshWaterSpikePlacement) == 0x24);
STATIC_ASSERT(offsetof(MmshWaterSpikePlacement, xyzAnimatorObjectIdLow) == 0x1A);
STATIC_ASSERT(offsetof(MmshWaterSpikePlacement, xyzAnimatorObjectIdHigh) == 0x1C);

static char sMmshWaterSpikeInvalidXyzAnimatorIdWarning[] = "WARNING Water Spike [%d] as invalid xyzAnim ID\n";

static int mmshWaterSpike_getExtraSize(void) {
    return 0;
}

static int mmshWaterSpike_getObjectTypeId(void) {
    return 0;
}

static void mmshWaterSpike_free(GameObject* obj) {
}

static void mmshWaterSpike_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                  s8 visible) {
    if (visible == 0) {
        return;
    }
}

static void mmshWaterSpike_hitDetect(void) {
}

static void mmshWaterSpike_update(GameObject* obj) {
    GameObject* xyzAnimator;
    int hitCount;
    f32 surfaceDelta;
    f32 targetY;
    f32 maxHeight;
    f32 riseDelta;
    TrackGroundHit** hitList;
    const MmshWaterSpikePlacement* placement;

    placement = (const MmshWaterSpikePlacement*)obj->anim.placementData;
    ObjHits_SetHitVolumeSlot(&obj->anim, 9, OBJHITS_ACTIVE_HITBOX_MODE, 0);
    xyzAnimator = ObjList_FindObjectById((u32)obj->userData2);
    if (xyzAnimator != NULL) {
        riseDelta = XyzAnimator_getCoordinate(xyzAnimator, XYZ_ANIMATOR_COORD_WORLD_Y) - obj->anim.localPosY;
    } else {
        logPrintf(sMmshWaterSpikeInvalidXyzAnimatorIdWarning, placement->base.ident);
        hitCount = trackGetHeight(obj, obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ, &hitList, 0, 0);
        if (hitCount != 0) {
            riseDelta = -9999.0f;
            for (int hitIndex = 0; hitIndex < hitCount; hitIndex++) {
                TrackGroundHit* hit = hitList[hitIndex];

                if ((s8)hit->surfaceType == SURFACE_WATER) {
                    surfaceDelta = hit->height - obj->anim.localPosY;
                    if (surfaceDelta > riseDelta) {
                        riseDelta = surfaceDelta;
                    }
                }
            }
        }
    }
    targetY = obj->anim.localPosY + riseDelta;
    maxHeight = placement->base.posY;
    if (targetY > maxHeight) {
        obj->anim.localPosY = maxHeight;
    } else {
        obj->anim.localPosY = targetY;
        obj->userData1 -= framesThisStep;
        if (obj->userData1 <= 0) {
            obj->userData1 = randomGetRange(0x3C, 0xF0);
            if (riseDelta == 0.0f) {
                (*gWaterfxInterface)
                    ->spawnRipple(obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ, 0, 0.5f, 3);
            }
        }
    }
}

static void mmshWaterSpike_init(GameObject* obj, const MmshWaterSpikePlacement* placement) {
    u32 xyzAnimatorObjectIdHigh;
    u32 xyzAnimatorObjectIdLow;

    ObjHits_EnableObject(obj);
    obj->userData1 = 0;
    xyzAnimatorObjectIdHigh = (u16)ObjAnim_ReadPlacementS16(&obj->anim, &placement->xyzAnimatorObjectIdHigh);
    xyzAnimatorObjectIdLow = (u16)ObjAnim_ReadPlacementS16(&obj->anim, &placement->xyzAnimatorObjectIdLow);
    obj->userData2 = (xyzAnimatorObjectIdHigh << 16) | xyzAnimatorObjectIdLow;
}

static void mmshWaterSpike_release(void) {
}

static void mmshWaterSpike_initialise(void) {
}

OBJECT_INIT_ADAPTER(gMMSHWaterSpikeObjDescriptorInitAdapter, mmshWaterSpike_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gMMSHWaterSpikeObjDescriptorHitDetectAdapter, mmshWaterSpike_hitDetect)
OBJECT_FREE_ADAPTER(gMMSHWaterSpikeObjDescriptorFreeAdapter, mmshWaterSpike_free, obj)
OBJECT_TYPE_ID_ADAPTER(gMMSHWaterSpikeObjDescriptorTypeIdAdapter, mmshWaterSpike_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gMMSHWaterSpikeObjDescriptorExtraSizeAdapter, mmshWaterSpike_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gMMSHWaterSpikeObjDescriptorAcquire, mmshWaterSpike_initialise)

ObjectDescriptor gMMSHWaterSpikeObjDescriptor = {
    .header =
        {
            .metadata = {0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS},
            .acquire = gMMSHWaterSpikeObjDescriptorAcquire,
            .release = mmshWaterSpike_release,
        },
    .init = gMMSHWaterSpikeObjDescriptorInitAdapter,
    .update = mmshWaterSpike_update,
    .hitDetect = gMMSHWaterSpikeObjDescriptorHitDetectAdapter,
    .render = mmshWaterSpike_render,
    .free = gMMSHWaterSpikeObjDescriptorFreeAdapter,
    .getObjectTypeId = gMMSHWaterSpikeObjDescriptorTypeIdAdapter,
    .getExtraSize = gMMSHWaterSpikeObjDescriptorExtraSizeAdapter,
};
;
