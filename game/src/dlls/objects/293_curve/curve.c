/*
 * Curve placement object (DLL slot 293 / 0x125).
 *
 * Curve metadata supplies the object's initial orientation and selects either
 * a type-specific scale or the model's default scale.
 */
#include "dlls/objects/293_curve.h"
#include "main/dll/rom_curve_def.h"
#include "main/object_render.h"

int curve_func0B(void) {
    return 0;
}

void curve_func0A(void) {
}

int curve_getExtraSize(void) {
    return 0;
}

int curve_getObjectTypeId(void) {
    return 0;
}

void curve_free(void) {
}

void curve_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (!visible) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void curve_init(GameObject* obj, RomCurveDef* placement) {
    obj->anim.rotX = placement->yaw << 8;
    obj->anim.rotY = placement->pitch << 8;
    if (placement->type == ROMCURVE_TYPE_SPECIAL_ANGLE_8 || placement->type == ROMCURVE_TYPE_SPECIAL_ANGLE_1A) {
        obj->anim.rotZ = ObjAnim_ReadPlacementS16(&obj->anim, &placement->roll);
    }
    if (placement->type == ROMCURVE_TYPE_SCALE_OVERRIDE_15) {
        obj->anim.rootMotionScale = 1.25f;
    } else if (placement->type == ROMCURVE_TYPE_SCALE_OVERRIDE_16) {
        obj->anim.rootMotionScale = 1.1f;
    } else {
        obj->anim.rootMotionScale = obj->anim.modelInstance->rootMotionScaleBase;
    }
}

OBJECT_INIT_ADAPTER(gCurveObjDescriptorInitAdapter, curve_init, obj, placement)
OBJECT_FREE_ADAPTER(gCurveObjDescriptorFreeAdapter, curve_free)
OBJECT_TYPE_ID_ADAPTER(gCurveObjDescriptorTypeIdAdapter, curve_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gCurveObjDescriptorExtraSizeAdapter, curve_getExtraSize)

typedef struct CurveObjDescriptorTypeInterface {
    OBJECT_INTERFACE_FIELDS;
    __typeof__(curve_func0A)* curve_func0A;
    __typeof__(curve_func0B)* curve_func0B;
} CurveObjDescriptorTypeInterface;

struct CurveObjDescriptorType {
    ObjectDescriptorHeader header;
    CurveObjDescriptorTypeInterface interface;
};

struct CurveObjDescriptorType gCurveObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_12_SLOTS,
        },
        0,
        0,
    },
    {
        0,
        gCurveObjDescriptorInitAdapter,
        0,
        0,
        curve_render,
        gCurveObjDescriptorFreeAdapter,
        gCurveObjDescriptorTypeIdAdapter,
        gCurveObjDescriptorExtraSizeAdapter,
        curve_func0A,
        curve_func0B,
    },
};
