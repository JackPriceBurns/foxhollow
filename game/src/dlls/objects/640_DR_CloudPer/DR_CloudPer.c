#include "main/dll/DR/dll_0280_drcloudper.h"

#include "dolphin/math.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/objseq.h"
#include "main/objtype.h"

typedef enum DrCloudPerObjectGroup {
    DR_CLOUD_PER_OBJECT_GROUP_TRIGGER = 0x13,
    DR_CLOUD_PER_OBJECT_GROUP_SURFACE = 0x39,
} DrCloudPerObjectGroup;

typedef enum DrCloudPerMapEvent {
    DR_CLOUD_PER_MAP_ANIM_EVENT = 0x0C,
} DrCloudPerMapEvent;

typedef enum DrCloudPerSequence {
    DR_CLOUD_PER_SEQUENCE_SELECT = 1,
    DR_CLOUD_PER_SEQUENCE_ACTIVATE = 2,
} DrCloudPerSequence;

typedef struct DrCloudPerPlacement {
    ObjPlacement base;
    s8 yawByte;
    s8 cloudIndex;
    u8 pad1A[4];
    s16 activationGameBit;
} DrCloudPerPlacement;

typedef struct DrCloudPerState {
    Vec3f normal;
    f32 planeDistance;
} DrCloudPerState;

STATIC_ASSERT(sizeof(DrCloudPerPlacement) == 0x20);
STATIC_ASSERT(offsetof(DrCloudPerPlacement, yawByte) == 0x18);
STATIC_ASSERT(offsetof(DrCloudPerPlacement, cloudIndex) == 0x19);
STATIC_ASSERT(offsetof(DrCloudPerPlacement, activationGameBit) == 0x1E);

STATIC_ASSERT(sizeof(DrCloudPerState) == 0x10);
STATIC_ASSERT(offsetof(DrCloudPerState, normal) == 0x00);
STATIC_ASSERT(offsetof(DrCloudPerState, planeDistance) == 0x0C);

static void drCloudPer_enableMapAnimation(GameObject* obj) {
    (*gMapEventInterface)->setObjGroupStatus(obj->anim.mapEventSlot, DR_CLOUD_PER_MAP_ANIM_EVENT, 1);
}

static int drCloudPer_activate(GameObject* obj) {
    const DrCloudPerPlacement* placement = (const DrCloudPerPlacement*)obj->anim.placementData;

    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->activationGameBit)) == 0) {
        return 0;
    }
    mainSetBits(GAMEBIT_DR_ActiveCloud, placement->cloudIndex);
    drCloudPer_enableMapAnimation(obj);
    (*gObjectTriggerInterface)->runSequence(DR_CLOUD_PER_SEQUENCE_ACTIVATE, obj, -1);
    return 1;
}

static int drCloudPer_selectActiveCloud(GameObject* obj) {
    const DrCloudPerPlacement* placement = (const DrCloudPerPlacement*)obj->anim.placementData;

    mainSetBits(GAMEBIT_DR_ActiveCloud, placement->cloudIndex);
    (*gObjectTriggerInterface)->runSequence(DR_CLOUD_PER_SEQUENCE_SELECT, obj, -1);
    return 0;
}

static int drCloudPer_getExtraSize(void) {
    return sizeof(DrCloudPerState);
}

static int drCloudPer_getObjectTypeId(void) {
    return 0;
}

static void drCloudPer_free(GameObject* obj) {
    objFreeObjectType(obj, DR_CLOUD_PER_OBJECT_GROUP_TRIGGER);
    objFreeObjectType(obj, DR_CLOUD_PER_OBJECT_GROUP_SURFACE);
}

static void drCloudPer_render(void) {
}

static void drCloudPer_hitDetect(void) {
}

static void drCloudPer_update(void) {
}

static void drCloudPer_init(GameObject* obj, const DrCloudPerPlacement* placement) {
    DrCloudPerState* state = obj->extra;

    objAddObjectType(obj, DR_CLOUD_PER_OBJECT_GROUP_TRIGGER);
    objAddObjectType(obj, DR_CLOUD_PER_OBJECT_GROUP_SURFACE);
    obj->anim.rotX = (s16)placement->yawByte * 0x100;

    state->normal.x = mathSinf(3.1415927f * obj->anim.rotX / 32768.0f);
    state->normal.y = 0.0f;
    state->normal.z = mathCosf(3.1415927f * obj->anim.rotX / 32768.0f);
    state->planeDistance =
        -(state->normal.z * obj->anim.localPosZ +
          (state->normal.x * obj->anim.localPosX + state->normal.y * obj->anim.localPosY));
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED | OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_UPDATE_DISABLED;
    if (placement->cloudIndex == mainGetBit(GAMEBIT_DR_ActiveCloud)) {
        drCloudPer_enableMapAnimation(obj);
    }
}

static void drCloudPer_release(void) {
}

static void drCloudPer_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDrCloudPerObjDescriptorInitAdapter, drCloudPer_init, obj, placement)
OBJECT_UPDATE_ADAPTER(gDrCloudPerObjDescriptorUpdateAdapter, drCloudPer_update)
OBJECT_HIT_DETECT_ADAPTER(gDrCloudPerObjDescriptorHitDetectAdapter, drCloudPer_hitDetect)
OBJECT_RENDER_ADAPTER(gDrCloudPerObjDescriptorRenderAdapter, drCloudPer_render)
OBJECT_FREE_ADAPTER(gDrCloudPerObjDescriptorFreeAdapter, drCloudPer_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDrCloudPerObjDescriptorTypeIdAdapter, drCloudPer_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDrCloudPerObjDescriptorExtraSizeAdapter, drCloudPer_getExtraSize)

typedef struct DrCloudPerObjDescriptorTypeInterface {
    OBJECT_INTERFACE_FIELDS;
    __typeof__(drCloudPer_activate)* drCloudPer_activate;
    __typeof__(drCloudPer_selectActiveCloud)* drCloudPer_selectActiveCloud;
} DrCloudPerObjDescriptorTypeInterface;

struct DrCloudPerObjDescriptorType {
    ObjectDescriptorHeader header;
    DrCloudPerObjDescriptorTypeInterface interface;
};

RESOURCE_ACQUIRE_ADAPTER(gDrCloudPerObjDescriptorAcquire, drCloudPer_initialise)

struct DrCloudPerObjDescriptorType gDrCloudPerObjDescriptor = {
    {
        .metadata[3] = OBJECT_DESCRIPTOR_FLAGS_12_SLOTS,
        .acquire = gDrCloudPerObjDescriptorAcquire,
        .release = drCloudPer_release,
    },
    {
        .init = gDrCloudPerObjDescriptorInitAdapter,
        .update = gDrCloudPerObjDescriptorUpdateAdapter,
        .hitDetect = gDrCloudPerObjDescriptorHitDetectAdapter,
        .render = gDrCloudPerObjDescriptorRenderAdapter,
        .free = gDrCloudPerObjDescriptorFreeAdapter,
        .getObjectTypeId = gDrCloudPerObjDescriptorTypeIdAdapter,
        .getExtraSize = gDrCloudPerObjDescriptorExtraSizeAdapter,
        .drCloudPer_activate = drCloudPer_activate,
        .drCloudPer_selectActiveCloud = drCloudPer_selectActiveCloud,
    },
};
