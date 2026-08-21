/* Emits one placement-configured particle burst when its trigger bit is set. */
#include "dlls/objects/317_ExplodeAnim.h"
#include "game/objects/object.h"
#include "main/dll/partfx_interface.h"
#include "main/gamebits.h"
#include "main/objtype.h"
#include "main/vecmath.h"

int ExplodeAnimator_getExtraSize(void) {
    return sizeof(ExplodeAnimatorState);
}

int ExplodeAnimator_getObjectTypeId(void) {
    return 0;
}

void ExplodeAnimator_free(GameObject* obj) {
    objFreeObjectType(obj, EXPLODE_ANIMATOR_OBJECT_GROUP);
}

void ExplodeAnimator_render(void) {
}

void ExplodeAnimator_hitDetect(void) {
}

void ExplodeAnimator_update(GameObject* obj) {
    ExplodeAnimatorState* state = obj->extra;
    if ((state->flags & EXPLODE_ANIMATOR_STATE_FIRED) != 0) {
        return;
    }

    ExplodeAnimatorPlacement* placement = (ExplodeAnimatorPlacement*)obj->anim.placementData;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->triggerGameBit)) == 0) {
        return;
    }

    mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->resultGameBit), 1);
    state->flags |= EXPLODE_ANIMATOR_STATE_FIRED;

    for (int i = 0; i < placement->particleCount; i++) {
        f32 velocity[2];
        velocity[0] = 0.01f * randomGetRange(ObjAnim_ReadPlacementS16(&obj->anim, &placement->velXMin),
                                             ObjAnim_ReadPlacementS16(&obj->anim, &placement->velXMax));
        velocity[1] = 0.01f * randomGetRange(ObjAnim_ReadPlacementS16(&obj->anim, &placement->velYMin),
                                             ObjAnim_ReadPlacementS16(&obj->anim, &placement->velYMax));

        PartFxSpawnParams spawnParams;
        spawnParams.posX = randomGetRange(ObjAnim_ReadPlacementS16(&obj->anim, &placement->posXMin),
                                          ObjAnim_ReadPlacementS16(&obj->anim, &placement->posXMax));
        spawnParams.posY = randomGetRange(ObjAnim_ReadPlacementS16(&obj->anim, &placement->posYMin),
                                          ObjAnim_ReadPlacementS16(&obj->anim, &placement->posYMax));
        spawnParams.posZ = randomGetRange(ObjAnim_ReadPlacementS16(&obj->anim, &placement->posZMin),
                                          ObjAnim_ReadPlacementS16(&obj->anim, &placement->posZMax));

        (*gPartfxInterface)
            ->spawnObject(obj, ObjAnim_ReadPlacementS16(&obj->anim, &placement->effectId), &spawnParams,
                          2, -1, velocity);
    }
}

void ExplodeAnimator_init(GameObject* obj, ExplodeAnimatorPlacement* placement) {
    ExplodeAnimatorState* state = obj->extra;

    state->flags = mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->resultGameBit)) != 0
                       ? EXPLODE_ANIMATOR_STATE_FIRED
                       : 0;
    objAddObjectType(obj, EXPLODE_ANIMATOR_OBJECT_GROUP);
}

void ExplodeAnimator_release(void) {
}

void ExplodeAnimator_initialise(void) {
}

ObjectDescriptor gExplodeAnimatorObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)ExplodeAnimator_initialise,
    (ObjectDescriptorCallback)ExplodeAnimator_release,
    0,
    (ObjectDescriptorCallback)ExplodeAnimator_init,
    (ObjectDescriptorCallback)ExplodeAnimator_update,
    (ObjectDescriptorCallback)ExplodeAnimator_hitDetect,
    (ObjectDescriptorCallback)ExplodeAnimator_render,
    (ObjectDescriptorCallback)ExplodeAnimator_free,
    (ObjectDescriptorCallback)ExplodeAnimator_getObjectTypeId,
    ExplodeAnimator_getExtraSize,
};
