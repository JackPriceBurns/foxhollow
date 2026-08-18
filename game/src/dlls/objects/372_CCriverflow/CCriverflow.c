#include "dlls/objects/372_CCriverflow.h"
#include "game/objects/object.h"
#include "main/gamebits_api.h"
#include "main/objtype.h"

#define CC_RIVER_FLOW_OBJECT_GROUP 0x14

int ccRiverFlow_getExtraSize(void) {
    return sizeof(CCRiverFlowState);
}

void ccRiverFlow_free(GameObject* obj) {
    CCRiverFlowState* state = obj->extra;

    if (state->active != 0) {
        objFreeObjectType(obj, CC_RIVER_FLOW_OBJECT_GROUP);
    }
}

void ccRiverFlow_render(void) {
}

void ccRiverFlow_update(GameObject* obj) {

    CCRiverFlowPlacement* placement = (CCRiverFlowPlacement*)obj->anim.placementData;
    if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBit) == -1) {
        return;
    }

    CCRiverFlowState* state = obj->extra;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBit)) != 0) {
        if (state->active != 0) {
            state->active = 0;
            objFreeObjectType(obj, CC_RIVER_FLOW_OBJECT_GROUP);
        }
        return;
    }

    if (state->active == 0) {
        state->active = 1;
        objAddObjectType(obj, CC_RIVER_FLOW_OBJECT_GROUP);
    }
}

void ccRiverFlow_init(GameObject* obj, CCRiverFlowPlacement* placement) {
    if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBit) == -1) {
        objAddObjectType(obj, CC_RIVER_FLOW_OBJECT_GROUP);
        ((CCRiverFlowState*)obj->extra)->active = 1;
    }

    obj->anim.rotX = placement->angle << 8;
    obj->anim.rootMotionScale = obj->anim.modelInstance->rootMotionScaleBase;
    obj->anim.rootMotionScale += placement->heightOffset / 512.0f;
    if (obj->anim.rootMotionScale < 0.01f) {
        obj->anim.rootMotionScale = 0.01f;
    }
    if (placement->speed == 0) {
        placement->speed = 255;
    }
}

ObjectDescriptor gCCRiverFlowObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)ccRiverFlow_init,
    (ObjectDescriptorCallback)ccRiverFlow_update,
    0,
    (ObjectDescriptorCallback)ccRiverFlow_render,
    (ObjectDescriptorCallback)ccRiverFlow_free,
    0,
    ccRiverFlow_getExtraSize,
};
