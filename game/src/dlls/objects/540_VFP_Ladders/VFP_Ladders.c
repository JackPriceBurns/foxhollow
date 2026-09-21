/*
 * vfpladders (DLL 0x21C, VFP_Ladders) - a retractable ladder / climbable
 * prop in the Volcano Force Point Temple.
 *
 * Two behaviours, selected by the object's seq id:
 *  - the trigger variant (seq 0x548) plays raise (sequence 0) / lower
 *    (sequence 1) animations driven by the trigger vs base game bits;
 *  - the sliding variant waits for its trigger bit, then after a short
 *    delay drops from its placed height down by a fixed offset (with a
 *    buzzing sfx) and latches at the bottom.
 */
#include "main/audio/sfx.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/objseq.h"
#include "main/dll/expgfx_interface.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/VF/dll_021C_vfpladders.h"

typedef enum VfpLaddersSequenceId {
    VFP_LADDERS_SEQUENCE_TRIGGER = 0x548,
} VfpLaddersSequenceId;

typedef enum VfpLaddersPhase {
    VFP_LADDERS_PHASE_WAIT,
    VFP_LADDERS_PHASE_DROPPING,
    VFP_LADDERS_PHASE_SETTLED,
} VfpLaddersPhase;

typedef struct VfpLaddersState {
    s16 baseGameBit;
    s16 triggerGameBit;
    s16 phase;
    s16 delayTimer;
} VfpLaddersState;

STATIC_ASSERT(sizeof(VfpLaddersState) == 0x08);
STATIC_ASSERT(offsetof(VfpLaddersState, baseGameBit) == 0x00);
STATIC_ASSERT(offsetof(VfpLaddersState, triggerGameBit) == 0x02);
STATIC_ASSERT(offsetof(VfpLaddersState, phase) == 0x04);
STATIC_ASSERT(offsetof(VfpLaddersState, delayTimer) == 0x06);

int vfpladders_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate)
{
    (void)obj;
    (void)unused;
    (void)animUpdate;
    return 0;
}

int VFP_Ladders_getExtraSize(void)
{
    return sizeof(VfpLaddersState);
}

int VFP_Ladders_getObjectTypeId(void)
{
    return 0;
}

void VFP_Ladders_free(GameObject* obj)
{
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void VFP_Ladders_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible)
{
}

void VFP_Ladders_hitDetect(GameObject* obj)
{
}

void VFP_Ladders_update(GameObject* obj)
{
    const VfpLaddersPlacement* placement = (const VfpLaddersPlacement*)obj->anim.placementData;
    VfpLaddersState* state = obj->extra;

    if (obj->anim.romDefNo == VFP_LADDERS_SEQUENCE_TRIGGER)
    {
        if (mainGetBit(state->triggerGameBit) != 0 && mainGetBit(state->baseGameBit) == 0)
        {
            (*gObjectTriggerInterface)->runSequence(0, obj, -1);
        }
        if (mainGetBit(state->triggerGameBit) == 0 && mainGetBit(state->baseGameBit) != 0)
        {
            (*gObjectTriggerInterface)->runSequence(1, obj, -1);
        }
    }
    else
    {
        if (state->delayTimer != 0)
        {
            state->delayTimer -= (s16)timeDelta;
            if (state->delayTimer <= 0)
            {
                state->phase = VFP_LADDERS_PHASE_DROPPING;
                Sfx_PlayFromObject(obj, SFXTRIG_mv_bodyf4_c);
                state->delayTimer = 0;
            }
        }
        else
        {
            if (state->phase == VFP_LADDERS_PHASE_WAIT && mainGetBit(state->triggerGameBit) != 0)
            {
                state->delayTimer = 90;
            }
            f32 settledY = placement->base.posY - 150.0f;
            if (state->phase == VFP_LADDERS_PHASE_DROPPING && obj->anim.localPosY > settledY)
            {
                obj->anim.localPosY -= 2.0f * timeDelta;
                if (obj->anim.localPosY < settledY)
                {
                    obj->anim.localPosY = settledY;
                    state->phase = VFP_LADDERS_PHASE_SETTLED;
                }
            }
        }
    }
}

void VFP_Ladders_init(GameObject* obj, const VfpLaddersPlacement* placement)
{
    VfpLaddersState* state = obj->extra;
    obj->anim.rotX = (s16)(placement->rotXByte << 8);
    state->triggerGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->triggerGameBit);
    state->baseGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->baseGameBit);
    obj->objectFlags |= (OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED);
    obj->animEventCallback = vfpladders_SeqFn;
}

void VFP_Ladders_release(void)
{
}

void VFP_Ladders_initialise(void)
{
}

OBJECT_INIT_ADAPTER(gVFP_LaddersObjDescriptorInitAdapter, VFP_Ladders_init, obj, placement)
OBJECT_FREE_ADAPTER(gVFP_LaddersObjDescriptorFreeAdapter, VFP_Ladders_free, obj)
OBJECT_TYPE_ID_ADAPTER(gVFP_LaddersObjDescriptorTypeIdAdapter, VFP_Ladders_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gVFP_LaddersObjDescriptorExtraSizeAdapter, VFP_Ladders_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gVFP_LaddersObjDescriptorAcquire, VFP_Ladders_initialise)

ObjectDescriptor gVFP_LaddersObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gVFP_LaddersObjDescriptorAcquire,
        VFP_Ladders_release,
    },
    0,
    gVFP_LaddersObjDescriptorInitAdapter,
    VFP_Ladders_update,
    VFP_Ladders_hitDetect,
    VFP_Ladders_render,
    gVFP_LaddersObjDescriptorFreeAdapter,
    gVFP_LaddersObjDescriptorTypeIdAdapter,
    gVFP_LaddersObjDescriptorExtraSizeAdapter,
};
