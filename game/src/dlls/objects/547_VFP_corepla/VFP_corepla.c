/* VFP_corepla (DLL 0x0223) */
#include "dlls/object_descriptor.h"
#include "main/dll/expgfx_interface.h"
#include "main/gamebits.h"
#include "main/object_render.h"
#include "game/objects/object_setup.h"

typedef struct VfpCorePlatformState {
    s16 gameBitId;
    u8 pad02[2];
} VfpCorePlatformState;

typedef struct VfpCorePlatformPlacement {
    ObjPlacement base;
    s8 rotXByte;
    u8 axisMode;
    u8 pad1A[6];
    s16 gameBitId;
} VfpCorePlatformPlacement;

STATIC_ASSERT(sizeof(VfpCorePlatformState) == 0x4);

int VFP_coreplat_sequenceCallback(void) {
    return 0x0;
}

int VFP_coreplat_getExtraSize(void) {
    return 0x4;
}

int VFP_coreplat_getObjectTypeId(void) {
    return 0x0;
}

void VFP_coreplat_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void VFP_coreplat_render(GameObject* p1, int p2, int p3, int p4, int p5, s8 visible) {
    objRenderModelAndHitVolumes(p1, p2, p3, p4, p5, 1.0f);
}

void VFP_coreplat_hitDetect(void) {
}

void VFP_coreplat_update(void) {
}

void VFP_coreplat_init(GameObject* obj, VfpCorePlatformPlacement* data) {
    VfpCorePlatformState* state = obj->extra;
    obj->anim.rotX = data->rotXByte << 8;
    state->gameBitId = ObjAnim_ReadPlacementS16(&obj->anim, &data->gameBitId);
    obj->animEventCallback = VFP_coreplat_sequenceCallback;

    if (obj->anim.romDefNo == 0x3cb) {
        if (mainGetBit(GAMEBIT_ITEM_SpellStone1_Used) != 0) {
            obj->anim.rootMotionScale = 0.7f * obj->anim.modelInstance->rootMotionScaleBase;
        }
        if (mainGetBit(GAMEBIT_ITEM_SpellStone3_Got) != 0) {
            obj->anim.rootMotionScale = 0.45f * obj->anim.modelInstance->rootMotionScaleBase;
        }
    }

    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void VFP_coreplat_release(void) {
}

void VFP_coreplat_initialise(void) {
}

OBJECT_INIT_ADAPTER(gVFP_coreplatObjDescriptorInitAdapter, VFP_coreplat_init, obj, placement)
OBJECT_UPDATE_ADAPTER(gVFP_coreplatObjDescriptorUpdateAdapter, VFP_coreplat_update)
OBJECT_HIT_DETECT_ADAPTER(gVFP_coreplatObjDescriptorHitDetectAdapter, VFP_coreplat_hitDetect)
OBJECT_FREE_ADAPTER(gVFP_coreplatObjDescriptorFreeAdapter, VFP_coreplat_free, obj)
OBJECT_TYPE_ID_ADAPTER(gVFP_coreplatObjDescriptorTypeIdAdapter, VFP_coreplat_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gVFP_coreplatObjDescriptorExtraSizeAdapter, VFP_coreplat_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gVFP_coreplatObjDescriptorAcquire, VFP_coreplat_initialise)

ObjectDescriptor gVFP_coreplatObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gVFP_coreplatObjDescriptorAcquire,
        VFP_coreplat_release,
    },
    0,
    gVFP_coreplatObjDescriptorInitAdapter,
    gVFP_coreplatObjDescriptorUpdateAdapter,
    gVFP_coreplatObjDescriptorHitDetectAdapter,
    VFP_coreplat_render,
    gVFP_coreplatObjDescriptorFreeAdapter,
    gVFP_coreplatObjDescriptorTypeIdAdapter,
    gVFP_coreplatObjDescriptorExtraSizeAdapter,
};
