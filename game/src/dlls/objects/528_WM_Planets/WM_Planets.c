/*
 * WM_Planets (DLL 0x0210) - orbiting planet models.
 *
 * Each planet circles its spawn point while rotating at independently
 * randomized rates.
 */
#include "main/dll/WM/dll_0210_wmplanets.h"
#include "dlls/object_descriptor.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/vec_types.h"
#include "main/vecmath.h"
#include "sys/objects.h"

int WM_Planets_getExtraSize(void) {
    return sizeof(WmPlanetsState);
}

int WM_Planets_getObjectTypeId(void) {
    return 0x0;
}

void WM_Planets_free(void) {
}

void WM_Planets_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
}

void WM_Planets_hitDetect(void) {
}

void WM_Planets_update(GameObject* obj) {
    WmPlanetsState* state = obj->extra;

    Vec3f vec;
    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = state->orbitRadius;

    state->orbitYaw += state->orbitYawStep;

    MatrixTransform rotate;
    rotate.x = 0.0f;
    rotate.y = 0.0f;
    rotate.z = 0.0f;
    rotate.scale = 1.0f;
    rotate.rotX = state->orbitYaw;
    rotate.rotY = 0;
    rotate.rotZ = 0;
    vecRotateZXY(&rotate.rotX, &vec.x);

    rotate.x = 0.0f;
    rotate.y = 0.0f;
    rotate.z = 0.0f;
    rotate.scale = 1.0f;
    rotate.rotX = 0;
    rotate.rotY = state->orbitPitch;
    rotate.rotZ = 0;
    vecRotateZXY(&rotate.rotX, &vec.x);

    obj->anim.localPosX = vec.x + state->baseX;
    obj->anim.localPosY = vec.y + state->baseY;
    obj->anim.localPosZ = vec.z + state->baseZ;
    obj->anim.rotX += state->yawStep * timeDelta;
}

void WM_Planets_init(GameObject* obj, WmPlanetsMapData* mapData) {
    WmPlanetsState* inner = obj->extra;
    obj->anim.rootMotionScale = 0.1f * obj->anim.modelInstance->rootMotionScaleBase * (1.0f + mapData->scaleByte);
    inner->orbitRadius = mapData->base.objectId != 0 ? -(mapData->radiusByte << 4) : 0.0f;
    inner->orbitYawStep = randomGetRange(0x64, 0xc8);
    inner->yawStep = randomGetRange(0xc8, 0x190);
    inner->orbitYaw = 0;
    inner->orbitPitch = randomGetRange(0, 0x960);
    inner->baseX = obj->anim.localPosX;
    inner->baseY = obj->anim.localPosY;
    inner->baseZ = obj->anim.localPosZ;
    Obj_SetActiveModelIndex(obj, ObjAnim_ReadPlacementS16(&obj->anim, &mapData->modelIndex));
    obj->anim.localPosZ = mapData->base.posZ + inner->orbitRadius;
}

void WM_Planets_release(void) {
}

void WM_Planets_initialise(void) {
}

ObjectDescriptor gWM_PlanetsObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)WM_Planets_initialise,
    (ObjectDescriptorCallback)WM_Planets_release,
    0,
    (ObjectDescriptorCallback)WM_Planets_init,
    (ObjectDescriptorCallback)WM_Planets_update,
    (ObjectDescriptorCallback)WM_Planets_hitDetect,
    (ObjectDescriptorCallback)WM_Planets_render,
    (ObjectDescriptorCallback)WM_Planets_free,
    (ObjectDescriptorCallback)WM_Planets_getObjectTypeId,
    (ObjectDescriptorExtraSizeCallback)WM_Planets_getExtraSize,
};
