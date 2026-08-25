#include "main/audio/sfx.h"
#include "main/gamebits.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/DR/dll_0268_drcagecontrol.h"

typedef enum DRCageControlSequenceId {
    DR_CAGE_CONTROL_SEQUENCE_CLOSED,
    DR_CAGE_CONTROL_SEQUENCE_OPEN = 2
} DRCageControlSequenceId;

typedef enum DRCageControlFlag {
    DR_CAGE_CONTROL_WATCH_BIT_SET = 1 << 7,
    DR_CAGE_CONTROL_SEQUENCE_STARTED = 1 << 6,
    DR_CAGE_CONTROL_INITIALLY_ARMED = 1 << 5
} DRCageControlFlag;

typedef struct DRCageControlState {
    s32 sequenceId;
    u8 flags;
} DRCageControlState;

int DR_CageControl_SeqFn(GameObject* obj) {
    const CageControlPlacement* placement = obj->anim.placementData;
    DRCageControlState* state = obj->extra;

    if (state->sequenceId == DR_CAGE_CONTROL_SEQUENCE_CLOSED) {
        if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->armGameBit)) != 0) {
            Sfx_StopObjectChannel(obj, 8);
            return 4;
        }

        if (((state->flags & DR_CAGE_CONTROL_WATCH_BIT_SET) != 0) !=
            (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->watchGameBit)) != 0)) {
            Sfx_PlayFromObject(obj, SFXTRIG_mv_blkhit_c);
            Sfx_PlayFromObject(obj, SFXTRIG_mv_persquk2);
            if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->watchGameBit)) != 0) {
                Sfx_PlayFromObject(obj, SFXTRIG_mv_wickpickup16_194);
            } else {
                Sfx_StopObjectChannel(obj, 8);
            }
        }
        if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->watchGameBit)) != 0) {
            state->flags |= DR_CAGE_CONTROL_WATCH_BIT_SET;
        } else {
            state->flags &= ~DR_CAGE_CONTROL_WATCH_BIT_SET;
        }
    }

    if (state->sequenceId == DR_CAGE_CONTROL_SEQUENCE_CLOSED &&
        mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->watchGameBit)) == 0) {
        return 1;
    }

    return 0;
}

int DR_CageControl_getExtraSize(void) {
    return sizeof(DRCageControlState);
}

int DR_CageControl_getObjectTypeId(void) {
    return 0;
}

void DR_CageControl_free(void) {
}

void DR_CageControl_render(GameObject* obj, u32 p2, u32 p3, u32 p4, u32 p5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
    }
}

void DR_CageControl_hitDetect(void) {
}

void DR_CageControl_update(GameObject* obj) {
    const CageControlPlacement* placement = obj->anim.placementData;
    DRCageControlState* state = obj->extra;
    if (state->flags & DR_CAGE_CONTROL_SEQUENCE_STARTED) {
        return;
    }

    if (state->sequenceId == DR_CAGE_CONTROL_SEQUENCE_CLOSED &&
        mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->armGameBit)) != 0) {
        state->flags |= DR_CAGE_CONTROL_SEQUENCE_STARTED;
        state->sequenceId = DR_CAGE_CONTROL_SEQUENCE_OPEN;
    }

    if ((state->flags & DR_CAGE_CONTROL_INITIALLY_ARMED) == 0) {
        (*gObjectTriggerInterface)->runSequence(state->sequenceId, obj, -1);
        return;
    }

    state->flags |= DR_CAGE_CONTROL_SEQUENCE_STARTED;
    (*gObjectTriggerInterface)->preempt((uintptr_t)obj, 0x76c);
    if (mainGetBit(GAMEBIT_DR_EnteredDrakorTower) != 0) {
        (*gObjectTriggerInterface)->runSequence(state->sequenceId, obj, 0x60);
    } else {
        (*gObjectTriggerInterface)->runSequence(state->sequenceId, obj, 0x70);
    }
}

void DR_CageControl_init(GameObject* obj, const CageControlPlacement* placement) {
    DRCageControlState* state = obj->extra;

    obj->animEventCallback = DR_CageControl_SeqFn;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->armGameBit)) != 0) {
        state->flags |= DR_CAGE_CONTROL_INITIALLY_ARMED;
        state->sequenceId = DR_CAGE_CONTROL_SEQUENCE_OPEN;
    } else {
        state->sequenceId = DR_CAGE_CONTROL_SEQUENCE_CLOSED;
    }
}

void DR_CageControl_release(void) {
}

void DR_CageControl_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDrCageControlObjDescriptorInitAdapter, DR_CageControl_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDrCageControlObjDescriptorHitDetectAdapter, DR_CageControl_hitDetect)
OBJECT_RENDER_ADAPTER(gDrCageControlObjDescriptorRenderAdapter, DR_CageControl_render, obj, arg2, arg3, arg4, arg5, visible)
OBJECT_FREE_ADAPTER(gDrCageControlObjDescriptorFreeAdapter, DR_CageControl_free)
OBJECT_TYPE_ID_ADAPTER(gDrCageControlObjDescriptorTypeIdAdapter, DR_CageControl_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDrCageControlObjDescriptorExtraSizeAdapter, DR_CageControl_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDrCageControlObjDescriptorAcquire, DR_CageControl_initialise)

ObjectDescriptor gDrCageControlObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDrCageControlObjDescriptorAcquire,
        DR_CageControl_release,
    },
    0,
    gDrCageControlObjDescriptorInitAdapter,
    DR_CageControl_update,
    gDrCageControlObjDescriptorHitDetectAdapter,
    gDrCageControlObjDescriptorRenderAdapter,
    gDrCageControlObjDescriptorFreeAdapter,
    gDrCageControlObjDescriptorTypeIdAdapter,
    gDrCageControlObjDescriptorExtraSizeAdapter,
};
