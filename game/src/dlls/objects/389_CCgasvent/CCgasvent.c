#include "dlls/objects/389_CCgasvent.h"

#include <float.h>

#include "game/objects/object.h"
#include "main/dll/partfx_interface.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/objtype.h"

enum CcGasVentPhase {
    CC_GAS_VENT_PHASE_BLOCKED,
    CC_GAS_VENT_PHASE_CLEAR,
};

typedef struct CcGasVentState {
    u8 phase;
} CcGasVentState;

STATIC_ASSERT(sizeof(CcGasVentState) == 1);

static int ccGasVent_getExtraSize(void) {
    return sizeof(CcGasVentState);
}

static void ccGasVent_free(GameObject* obj) {
    objFreeObjectType(obj, CC_GAS_VENT_OBJECT_GROUP);
}

static void ccGasVent_render(void) {
}

static void ccGasVent_update(GameObject* obj) {
    CcGasVentState* state = obj->extra;
    f32 blockerDistance = FLT_MAX;

    if (mainGetBit(GAMEBIT_CC_GasVentActive) == 0) {
        return;
    }

    objGetNearestTypeTo(CC_GAS_VENT_BLOCKER_OBJECT_GROUP, obj, &blockerDistance);
    switch (state->phase) {
    case CC_GAS_VENT_PHASE_BLOCKED:
        if (blockerDistance >= 10.0f) {
            state->phase = CC_GAS_VENT_PHASE_CLEAR;
        }
        break;
    case CC_GAS_VENT_PHASE_CLEAR:
        if (blockerDistance < 10.0f) {
            state->phase = CC_GAS_VENT_PHASE_BLOCKED;
        } else {
            (*gPartfxInterface)->spawnObject(obj, 0x3DF, NULL, 0, -1, NULL);
        }
        break;
    }
}

static void ccGasVent_init(GameObject* obj) {
    objAddObjectType(obj, CC_GAS_VENT_OBJECT_GROUP);
}

ObjectDescriptor gCCGasVentObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .init = (ObjectDescriptorCallback)ccGasVent_init,
    .update = (ObjectDescriptorCallback)ccGasVent_update,
    .render = (ObjectDescriptorCallback)ccGasVent_render,
    .free = (ObjectDescriptorCallback)ccGasVent_free,
    .getExtraSize = ccGasVent_getExtraSize,
};
