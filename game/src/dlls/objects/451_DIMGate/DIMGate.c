/*
 * DIMGate (DLL 0x1C3) opens when an object with sequence ID 399 enters its
 * contact list and persists the open state through a game bit.
 */

#include "dlls/objects/451_DIMGate.h"
#include "main/gamebits.h"
#include "main/object_render.h"
#include "main/objhits.h"

#define DIM_GATE_TRIGGER_SEQUENCE_ID 399

int dimgate_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    return 0;
}

int dimgate_getExtraSize(void) {
    return sizeof(DimGateState);
}

int dimgate_getObjectTypeId(void) {
    return 0;
}

void dimgate_free(void) {
}

void dimgate_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void dimgate_hitDetect(void) {
}

void dimgate_update(GameObject* obj) {
    DimGateState* state = obj->extra;
    DimGatePlacement* placement = (DimGatePlacement*)obj->anim.placementData;

    switch (state->mode) {
    case DIM_GATE_MODE_CLOSED: {
        if (*(s8*)&((ObjHitsPriorityState*)obj->anim.hitReactState)->stateIndex != DIM_GATE_MODE_OPENING) {
            ObjHitbox_SetStateIndex(obj, obj->anim.hitReactState, DIM_GATE_MODE_OPENING);
        }

        int triggerFound = 0;
        for (int i = 0; i < obj->anim.hitboxTransformState->contactObjectCount; i++) {
            GameObject* contactObject = obj->anim.hitboxTransformState->contactObjects[i];

            if (contactObject->anim.romDefNo == DIM_GATE_TRIGGER_SEQUENCE_ID) {
                triggerFound = 1;
                break;
            }
        }

        if (triggerFound) {
            mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->openGameBit), 1);
            if (*(s8*)&((ObjHitsPriorityState*)obj->anim.hitReactState)->stateIndex != DIM_GATE_MODE_OPEN) {
                ObjHitbox_SetStateIndex(obj, obj->anim.hitReactState, DIM_GATE_MODE_OPEN);
            }
            state->mode = DIM_GATE_MODE_OPEN;
        }
        break;
    }
    case DIM_GATE_MODE_OPENING:
        break;
    case DIM_GATE_MODE_OPEN: {
        if (*(s8*)&((ObjHitsPriorityState*)obj->anim.hitReactState)->stateIndex != DIM_GATE_MODE_OPEN) {
            ObjHitbox_SetStateIndex(obj, obj->anim.hitReactState, DIM_GATE_MODE_OPEN);
        }
        break;
    }
    }
}

void dimgate_init(GameObject* obj, DimGatePlacement* unusedPlacement) {
    DimGatePlacement* placement = (DimGatePlacement*)obj->anim.placementData;
    DimGateState* state = obj->extra;

    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->openGameBit)) != 0) {
        state->mode = DIM_GATE_MODE_OPEN;
        obj->anim.currentMoveProgress = 1.0f;
    } else {
        state->mode = DIM_GATE_MODE_CLOSED;
    }

    obj->animEventCallback = dimgate_SeqFn;
    obj->anim.rotX = placement->rotationXByte << 8;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void dimgate_release(void) {
}

void dimgate_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDIMGateObjDescriptorInitAdapter, dimgate_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDIMGateObjDescriptorHitDetectAdapter, dimgate_hitDetect)
OBJECT_FREE_ADAPTER(gDIMGateObjDescriptorFreeAdapter, dimgate_free)
OBJECT_TYPE_ID_ADAPTER(gDIMGateObjDescriptorTypeIdAdapter, dimgate_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDIMGateObjDescriptorExtraSizeAdapter, dimgate_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDIMGateObjDescriptorAcquire, dimgate_initialise)

ObjectDescriptor gDIMGateObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDIMGateObjDescriptorAcquire,
        dimgate_release,
    },
    0,
    gDIMGateObjDescriptorInitAdapter,
    dimgate_update,
    gDIMGateObjDescriptorHitDetectAdapter,
    dimgate_render,
    gDIMGateObjDescriptorFreeAdapter,
    gDIMGateObjDescriptorTypeIdAdapter,
    gDIMGateObjDescriptorExtraSizeAdapter,
};
