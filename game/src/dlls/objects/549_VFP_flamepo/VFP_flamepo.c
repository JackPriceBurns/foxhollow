/* VFP_flamepo (DLL 0x0225) */
#include "dlls/object_descriptor.h"
#include "main/dll/dll_00C4_tricky.h"
#include "main/gamebits.h"
#include "main/objprint_render.h"
#include "main/objtype.h"
#include "sys/objects/lifecycle.h"

typedef enum VfpFlamePointFlags {
    VFP_FLAME_POINT_FLAG_NO_CHECK = 1 << 6,
    VFP_FLAME_POINT_FLAG_DONE = 1 << 7,
} VfpFlamePointFlags;

typedef struct VfpFlamePointState {
    s16 showGameBit;
    s16 checkGameBit;
    s8 counter;
    u8 flags;
    u8 pad06[2];
} VfpFlamePointState;

typedef struct VfpFlamePointPlacement {
    ObjPlacement base;
    u8 pad18[2];
    s16 counterInit;
    s16 noCheck;
    s16 showGameBit;
    s16 checkGameBit;
} VfpFlamePointPlacement;

STATIC_ASSERT(sizeof(VfpFlamePointState) == 0x08);
STATIC_ASSERT(offsetof(VfpFlamePointState, showGameBit) == 0x00);
STATIC_ASSERT(offsetof(VfpFlamePointState, checkGameBit) == 0x02);
STATIC_ASSERT(offsetof(VfpFlamePointState, counter) == 0x04);
STATIC_ASSERT(offsetof(VfpFlamePointState, flags) == 0x05);
STATIC_ASSERT(offsetof(VfpFlamePointPlacement, counterInit) == 0x1A);
STATIC_ASSERT(offsetof(VfpFlamePointPlacement, noCheck) == 0x1C);
STATIC_ASSERT(offsetof(VfpFlamePointPlacement, showGameBit) == 0x1E);
STATIC_ASSERT(offsetof(VfpFlamePointPlacement, checkGameBit) == 0x20);

int vfpflamepoint_countdownCallback(GameObject* obj, int x)
{
    VfpFlamePointState* state = obj->extra;

    if (state != NULL)
    {
        state->counter -= x;
        return state->counter <= 0;
    }
    return 0;
}

int VFP_flamepoint_getExtraSize(void)
{
    return sizeof(VfpFlamePointState);
}

void VFP_flamepoint_update(GameObject* obj)
{
    VfpFlamePointState* state = obj->extra;

    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    if ((state->flags & VFP_FLAME_POINT_FLAG_DONE) == 0 &&
        (state->checkGameBit == -1 || mainGetBit(state->checkGameBit) != 0))
    {
        if (state->counter <= 0)
        {
            if (state->showGameBit != -1)
            {
                mainSetBits(state->showGameBit, 1);
                state->flags |= VFP_FLAME_POINT_FLAG_DONE;
            }
        }
        else
        {
            GameObject* tricky = getTrickyObject();
            if (tricky != NULL)
            {
                f32 dist = 35.0f;

                if ((state->flags & VFP_FLAME_POINT_FLAG_NO_CHECK) != 0 ||
                    objGetNearestTypeTo(5, obj, &dist) == NULL)
                {
                    if (obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE)
                    {
                        TRICKY_INTERFACE(tricky)->sideCommandEnable(tricky, obj, 1, 4);
                    }
                    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
                    objUpdateHitVolumeTransforms(obj);
                }
            }
        }
    }
    else
    {
        if (mainGetBit(state->showGameBit) != 0)
        {
            state->flags |= VFP_FLAME_POINT_FLAG_DONE;
        }
        else
        {
            const VfpFlamePointPlacement* placement =
                (const VfpFlamePointPlacement*)obj->anim.placementData;
            state->flags &= (u8)~VFP_FLAME_POINT_FLAG_DONE;
            state->counter = (s8)ObjAnim_ReadPlacementS16(&obj->anim, &placement->counterInit);
        }
    }
}

void VFP_flamepoint_init(GameObject* obj, const VfpFlamePointPlacement* placement)
{
    VfpFlamePointState* state = obj->extra;

    state->counter = (s8)ObjAnim_ReadPlacementS16(&obj->anim, &placement->counterInit);
    state->flags = (ObjAnim_ReadPlacementS16(&obj->anim, &placement->noCheck) & 1) != 0
                       ? VFP_FLAME_POINT_FLAG_NO_CHECK
                       : 0;
    state->showGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->showGameBit);
    state->checkGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->checkGameBit);
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

OBJECT_INIT_ADAPTER(gVFP_flamepointObjDescriptorInitAdapter, VFP_flamepoint_init, obj, placement)
OBJECT_EXTRA_SIZE_ADAPTER(gVFP_flamepointObjDescriptorExtraSizeAdapter, VFP_flamepoint_getExtraSize)

ObjectDescriptor gVFP_flamepointObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        0,
        0,
    },
    0,
    gVFP_flamepointObjDescriptorInitAdapter,
    VFP_flamepoint_update,
    0,
    0,
    0,
    0,
    gVFP_flamepointObjDescriptorExtraSizeAdapter,
};
