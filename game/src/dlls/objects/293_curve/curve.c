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
    if (visible) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void curve_init(GameObject* obj, RomCurveDef* placement) {
    obj->anim.rotX = (s16)(placement->yaw << 8);
    obj->anim.rotY = (s16)(placement->pitch << 8);
    if (placement->type == ROMCURVE_TYPE_SPECIAL_ANGLE_8 ||
        placement->type == ROMCURVE_TYPE_SPECIAL_ANGLE_1A) {
        obj->anim.rotZ = placement->roll;
    }
    if (placement->type == ROMCURVE_TYPE_SCALE_OVERRIDE_15) {
        obj->anim.rootMotionScale = 1.25f;
    } else if (placement->type == ROMCURVE_TYPE_SCALE_OVERRIDE_16) {
        obj->anim.rootMotionScale = 1.1f;
    } else {
        obj->anim.rootMotionScale = obj->anim.modelInstance->rootMotionScaleBase;
    }
}

ObjectDescriptor12 gCurveObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_12_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)curve_init,
    0,
    0,
    (ObjectDescriptorCallback)curve_render,
    (ObjectDescriptorCallback)curve_free,
    (ObjectDescriptorCallback)curve_getObjectTypeId,
    curve_getExtraSize,
    (ObjectDescriptorCallback)curve_func0A,
    (ObjectDescriptorCallback)curve_func0B,
};
