/*
 * DLL 0x1FB (slot 507) - an unnamed interaction-controlled animated object.
 */
#include "dlls/objects/507.h"
#include "dolphin/pad.h"
#include "game/objects/object.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/obj_message.h"
#include "main/objseq.h"
#include "main/object_render.h"
#include "main/pad.h"

int dll507_processAnimEvents(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    Dll1FBState* state = obj->extra;

    if (state->triggerMode == 1 || state->triggerMode == 2) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    }

    animUpdate->savedFlags = -1;
    animUpdate->movementState = 0;
    return 0;
}

int dll507_getExtraSize(void) {
    return sizeof(Dll1FBState);
}

int dll507_getObjectTypeId(void) {
    return 0;
}

void dll507_free(void) {
}

void dll507_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    Dll1FBState* state = obj->extra;
    if (visible == 0 || state->hideModel != 0u) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void dll507_hitDetect(void) {
}

void dll507_update(GameObject* obj) {
    Dll1FBState* state = obj->extra;

    if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) == 0 || state->triggerMode != 2 ||
        mainGetBit(GAMEBIT_K1_SHRINE_DOOR_DIALOGUE_DONE) != 0) {
        ObjAnim_AdvanceCurrentMove(obj, 0.01f, timeDelta, NULL);
        return;
    }

    (*gObjectTriggerInterface)->runSequence(4, obj, -1);
    buttonDisable(0, PAD_BUTTON_A);
    mainSetBits(GAMEBIT_K1_SHRINE_DOOR_DIALOGUE_DONE, 1);
    ObjAnim_AdvanceCurrentMove(obj, 0.01f, timeDelta, NULL);
}

void dll507_init(GameObject* obj, const Dll1FBPlacementView* placement) {
    Dll1FBState* state = obj->extra;
    ObjMsg_AllocQueue(obj, 4);
    obj->animEventCallback = dll507_processAnimEvents;
    obj->anim.rotX = placement->rotationXHighByte << 8;
    obj->anim.rotY = ObjAnim_ReadPlacementS16(&obj->anim, &placement->rotationY);
    state->baseMove = placement->baseMove;
    state->triggerMode = ObjAnim_ReadPlacementS16(&obj->anim, &placement->triggerMode);
    ObjAnim_SetCurrentMove(obj, state->baseMove + 0x100, 0.0f, 0);
}

void dll507_release(void) {
}

void dll507_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll1FBObjDescriptorInitAdapter, dll507_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDll1FBObjDescriptorHitDetectAdapter, dll507_hitDetect)
OBJECT_FREE_ADAPTER(gDll1FBObjDescriptorFreeAdapter, dll507_free)
OBJECT_TYPE_ID_ADAPTER(gDll1FBObjDescriptorTypeIdAdapter, dll507_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll1FBObjDescriptorExtraSizeAdapter, dll507_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll1FBObjDescriptorAcquire, dll507_initialise)

ObjectDescriptor gDll1FBObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll1FBObjDescriptorAcquire,
        dll507_release,
    },
    0,
    gDll1FBObjDescriptorInitAdapter,
    dll507_update,
    gDll1FBObjDescriptorHitDetectAdapter,
    dll507_render,
    gDll1FBObjDescriptorFreeAdapter,
    gDll1FBObjDescriptorTypeIdAdapter,
    gDll1FBObjDescriptorExtraSizeAdapter,
};
