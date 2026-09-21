#include "dlls/objects/296_KT_Torch.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/object_render.h"

typedef struct KTTorchPlacement {
    ObjPlacement base;
    u8 modelBankIndex;
    u8 animationIndex;
    u8 initialAnimationProgress;
    u8 animationSpeed;
    u8 scaleMultiplier;
    u8 initialYaw;
    u8 pad1E[2];
    s16 visibilityGameBit;
} KTTorchPlacement;

STATIC_ASSERT(sizeof(KTTorchPlacement) == 0x24);
STATIC_ASSERT(offsetof(KTTorchPlacement, modelBankIndex) == 0x18);
STATIC_ASSERT(offsetof(KTTorchPlacement, animationIndex) == 0x19);
STATIC_ASSERT(offsetof(KTTorchPlacement, initialAnimationProgress) == 0x1A);
STATIC_ASSERT(offsetof(KTTorchPlacement, animationSpeed) == 0x1B);
STATIC_ASSERT(offsetof(KTTorchPlacement, scaleMultiplier) == 0x1C);
STATIC_ASSERT(offsetof(KTTorchPlacement, initialYaw) == 0x1D);
STATIC_ASSERT(offsetof(KTTorchPlacement, visibilityGameBit) == 0x20);

static void ktTorch_updateVisibility(GameObject* obj, const KTTorchPlacement* placement) {
    s16 visibilityGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->visibilityGameBit);

    if (visibilityGameBit != -1) {
        obj->anim.alpha = mainGetBit(visibilityGameBit) != 0 ? 0xFF : 0;
    }
}

static int ktTorch_getExtraSize(void) {
    return 0;
}

static int ktTorch_getObjectTypeId(void) {
    return 0;
}

static void ktTorch_free(void) {
}

static void ktTorch_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                           s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void ktTorch_hitDetect(void) {
}

static void ktTorch_update(GameObject* obj) {
    const KTTorchPlacement* placement = (const KTTorchPlacement*)obj->anim.placementData;

    ObjAnim_AdvanceCurrentMove(obj, placement->animationSpeed / 10000.0f, timeDelta, NULL);
    ktTorch_updateVisibility(obj, placement);
}

static void ktTorch_init(GameObject* obj, const KTTorchPlacement* placement) {
    ObjAnimComponent* objAnim = &obj->anim;
    f32 scale = placement->scaleMultiplier;

    objAnim->flags |= 2;
    if (scale < 10.0f) {
        scale = 10.0f;
    }
    scale *= 0.015625f;

    objAnim->rootMotionScale = objAnim->modelInstance->rootMotionScaleBase * scale;
    objAnim->rotX = (s16)((placement->initialYaw & 0x3F) * 0x400);
    if (objAnim->modelState != NULL) {
        objAnim->modelState->shadowScale = objAnim->modelInstance->shadowScaleBase * scale;
    }

    objAnim->bankIndex = (s8)placement->modelBankIndex;
    if (objAnim->bankIndex >= objAnim->modelInstance->modelCount) {
        objAnim->bankIndex = 0;
    }
    ObjAnim_SetCurrentMove(obj, placement->animationIndex, placement->initialAnimationProgress * 0.00390625f, 0);
    ktTorch_updateVisibility(obj, placement);
}

static void ktTorch_release(void) {
}

static void ktTorch_initialise(void) {
}

OBJECT_INIT_ADAPTER(gKT_TorchObjDescriptorInitAdapter, ktTorch_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gKT_TorchObjDescriptorHitDetectAdapter, ktTorch_hitDetect)
OBJECT_FREE_ADAPTER(gKT_TorchObjDescriptorFreeAdapter, ktTorch_free)
OBJECT_TYPE_ID_ADAPTER(gKT_TorchObjDescriptorTypeIdAdapter, ktTorch_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gKT_TorchObjDescriptorExtraSizeAdapter, ktTorch_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gKT_TorchObjDescriptorAcquire, ktTorch_initialise)

ObjectDescriptor gKT_TorchObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gKT_TorchObjDescriptorAcquire,
        .release = ktTorch_release,
    },
    .init = gKT_TorchObjDescriptorInitAdapter,
    .update = ktTorch_update,
    .hitDetect = gKT_TorchObjDescriptorHitDetectAdapter,
    .render = ktTorch_render,
    .free = gKT_TorchObjDescriptorFreeAdapter,
    .getObjectTypeId = gKT_TorchObjDescriptorTypeIdAdapter,
    .getExtraSize = gKT_TorchObjDescriptorExtraSizeAdapter,
};;
