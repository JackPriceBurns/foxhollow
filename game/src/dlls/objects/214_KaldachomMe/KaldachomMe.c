/*
 * KaldachomMe object (DLL slot 214).
 *
 * Drives a linked mouth animation toward a requested progress and selects
 * linked mouth objects from the owning Kaldachom's placement ID.
 */
#include "dlls/objects/214_KaldachomMe.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "sys/objects.h"

typedef struct KaldachomMeState {
    f32 progress;
    f32 step;
    f32 targetProgress;
    u8 moveId;
} KaldachomMeState;

static s32 kaldachomme_getLinkedMouthId(s32 ident) {
    switch (ident) {
    case 0x43d14:
        return 0x4b3b5;
    case 0x41be9:
        return 0x4b3f9;
    case 0x41cc4:
        return 0x4b402;
    case 0x41cc5:
        return 0x4b403;
    case 0x41cc6:
        return 0x4b404;
    case 0x41cc7:
        return 0x4b40b;
    case 0x41cc8:
        return 0x4b40c;
    case 0x41cc9:
        return 0x4b40f;
    case 0x41cd2:
        return 0x4b410;
    case 0x41ccc:
        return 0x4b411;
    case 0x41cd5:
        return 0x4b414;
    case 0x41cd6:
        return 0x4b415;
    case 0x41cd9:
        return 0x4b453;
    default:
        return -1;
    }
}

void kaldachomme_setLinkedMouthMode(GameObject* obj, KaldachomMeLinkedMode mode) {
    if (obj == NULL) {
        return;
    }
    s32 linkedMouthId = kaldachomme_getLinkedMouthId(obj->anim.placement->ident);
    if (linkedMouthId == -1) {
        return;
    }
    GameObject* linkedObj = ObjList_FindObjectById(linkedMouthId);
    KaldachomMeState* state = linkedObj->extra;
    if (state == NULL) {
        return;
    }
    switch (mode) {
    case KALDACHOMME_LINKED_MODE_MOVE_0:
        state->moveId = 0;
        break;
    case KALDACHOMME_LINKED_MODE_MOVE_1:
        state->moveId = 1;
        break;
    default:
        return;
    }
    state->targetProgress = 1.0f;
    state->progress = 0.0f;
    state->step = 0.025f;
}

int KaldachomMe_getExtraSize(void) {
    return sizeof(KaldachomMeState);
}

int KaldachomMe_getObjectTypeId(void) {
    return 0;
}

void KaldachomMe_free(GameObject* obj) {
    (void)obj;
}

void KaldachomMe_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
    }
}

void KaldachomMe_hitDetect(GameObject* obj) {
    (void)obj;
}

void KaldachomMe_update(GameObject* obj) {
    KaldachomMeState* state = obj->extra;
    f32 current = state->progress;
    f32 target = state->targetProgress;
    if (current != target) {
        f32 step = state->step;
        if (step > 0.0f) {
            if (current < target) {
                state->progress = current + step * timeDelta;
            } else {
                state->progress = target;
            }
        } else {
            if (current > target) {
                state->progress = current + step * timeDelta;
            } else {
                state->progress = target;
            }
        }
    }
    ObjAnim_SetCurrentMove(obj, state->moveId, state->progress, 0);
}

void KaldachomMe_init(GameObject* obj, KaldachomMePlacement* placement) {
    obj->anim.rotZ = (s16)(placement->rotZByte << 8);
    obj->anim.rotY = (s16)(placement->rotYByte << 8);
    obj->anim.rotX = (s16)(placement->rotXByte << 8);
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
    ObjAnim_SetCurrentMove(obj, 0, 0.0f, 0);
}

void KaldachomMe_release(void) {
}

void KaldachomMe_initialise(void) {
}

OBJECT_INIT_ADAPTER(gKaldachomMeObjDescriptorInitAdapter, KaldachomMe_init, obj, placement)
OBJECT_FREE_ADAPTER(gKaldachomMeObjDescriptorFreeAdapter, KaldachomMe_free, obj)
OBJECT_TYPE_ID_ADAPTER(gKaldachomMeObjDescriptorTypeIdAdapter, KaldachomMe_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gKaldachomMeObjDescriptorExtraSizeAdapter, KaldachomMe_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gKaldachomMeObjDescriptorAcquire, KaldachomMe_initialise)

ObjectDescriptor gKaldachomMeObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gKaldachomMeObjDescriptorAcquire,
        KaldachomMe_release,
    },
    0,
    gKaldachomMeObjDescriptorInitAdapter,
    KaldachomMe_update,
    KaldachomMe_hitDetect,
    KaldachomMe_render,
    gKaldachomMeObjDescriptorFreeAdapter,
    gKaldachomMeObjDescriptorTypeIdAdapter,
    gKaldachomMeObjDescriptorExtraSizeAdapter,
};
