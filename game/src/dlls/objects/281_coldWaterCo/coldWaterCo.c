#include "dlls/objects/281_coldWaterCo.h"

#include "main/dll/player.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/objhits.h"
#include "main/objseq.h"
#include "sys/objects.h"

#define COLD_WATER_HIT_PRIORITY  0x1C
#define COLD_WATER_TIMER_INITIAL -30.0f
#define COLD_WATER_DAMAGE_PERIOD 240.0f

int ColdWaterControl_getExtraSize(void) {
    return sizeof(ColdWaterControlState);
}

void ColdWaterControl_update(GameObject* obj) {
    ColdWaterControlState* state = obj->extra;
    if (mainGetBit(GAMEBIT_IM_TriggerSlippy) != 0 && mainGetBit(GAMEBIT_IM_SlippyWarnedCold) == 0) {
        (*gObjectTriggerInterface)->runSequence(0, obj, -1);
        mainSetBits(GAMEBIT_IM_SlippyWarnedCold, 1);
        return;
    }

    if (state->cachedPlayer == NULL) {
        state->cachedPlayer = Obj_GetPlayerObject();
        return;
    }

    if (playerIsInWater(state->cachedPlayer) == 0) {
        state->damageTimer = COLD_WATER_TIMER_INITIAL;
        return;
    }

    if (state->damageTimer == COLD_WATER_TIMER_INITIAL) {
        ObjHits_RecordObjectHit(state->cachedPlayer, obj, COLD_WATER_HIT_PRIORITY, 0, 1);
    }

    state->damageTimer += timeDelta;
    if (state->damageTimer > COLD_WATER_DAMAGE_PERIOD) {
        ObjHits_RecordObjectHit(state->cachedPlayer, obj, COLD_WATER_HIT_PRIORITY, 1, 1);
        state->damageTimer -= COLD_WATER_DAMAGE_PERIOD;
    }
}

void ColdWaterControl_init(GameObject* obj) {
    ColdWaterControlState* state = obj->extra;
    state->damageTimer = COLD_WATER_TIMER_INITIAL;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

OBJECT_INIT_ADAPTER(gColdWaterControlObjDescriptorInitAdapter, ColdWaterControl_init, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gColdWaterControlObjDescriptorExtraSizeAdapter, ColdWaterControl_getExtraSize)

ObjectDescriptor gColdWaterControlObjDescriptor = {
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
    gColdWaterControlObjDescriptorInitAdapter,
    ColdWaterControl_update,
    0,
    0,
    0,
    0,
    gColdWaterControlObjDescriptorExtraSizeAdapter,
};
