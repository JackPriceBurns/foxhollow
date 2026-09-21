/*
 * CNTcounter (DLL 0x2B4) - a generic countdown object.
 * Reads an initial count and two game bits from placement data: one bit
 * that, when set, decrements the counter (the bit's value is used as the
 * decrement amount and then cleared), and one bit set when the counter
 * reaches zero. Optionally shows the current count on the HUD.
 */
#include "main/dll/dll_02B4_cntcounter.h"
#include "main/gamebits.h"
#include "main/model_engine.h"

int CntCounter_getExtraSize(void) {
    return 8;
}

int CntCounter_getObjectTypeId(void) {
    return 0;
}

void CntCounter_free(GameObject* obj) {
    CntCounterState* state = obj->extra;
    if (state->displayHud != 0) {
        hudNumberSet(-1);
    }
}

void CntCounter_render(void) {
}

void CntCounter_hitDetect(void) {
}

void CntCounter_update(GameObject* obj) {
    CntCounterState* state = obj->extra;
    CntCounterSetup* setup = (CntCounterSetup*)obj->anim.placementData;

    if (state->remainingCount == 0) {
        if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(setup->decrementGameBit))) != 0) {
            state->displayHud = setup->displayHud;
            state->remainingCount = ObjAnim_ReadPlacementS16(&obj->anim, &(setup->initialCount));
        }
        return;
    }

    if (state->displayHud != 0) {
        hudNumberSet(state->remainingCount);
    }

    int bit = mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &setup->decrementGameBit));
    if (bit == 0) {
        return;
    }

    mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &setup->decrementGameBit), 0);
    state->remainingCount -= bit;
    if (state->remainingCount > 0) {
        return;
    }

    state->remainingCount = 0;
    mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &setup->doneGameBit), 1);
    if (state->displayHud != 0) {
        hudNumberSet(-1);
    }
    state->displayHud = 0;
}

void CntCounter_init(GameObject* obj) {
    CntCounterState* state = obj->extra;
    state->displayHud = 0;
    state->remainingCount = 0;
}

void CntCounter_release(void) {
}

void CntCounter_initialise(void) {
}

OBJECT_INIT_ADAPTER(gCNTcounterObjDescriptorInitAdapter, CntCounter_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gCNTcounterObjDescriptorHitDetectAdapter, CntCounter_hitDetect)
OBJECT_RENDER_ADAPTER(gCNTcounterObjDescriptorRenderAdapter, CntCounter_render)
OBJECT_FREE_ADAPTER(gCNTcounterObjDescriptorFreeAdapter, CntCounter_free, obj)
OBJECT_TYPE_ID_ADAPTER(gCNTcounterObjDescriptorTypeIdAdapter, CntCounter_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gCNTcounterObjDescriptorExtraSizeAdapter, CntCounter_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gCNTcounterObjDescriptorAcquire, CntCounter_initialise)

ObjectDescriptor gCNTcounterObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gCNTcounterObjDescriptorAcquire,
        CntCounter_release,
    },
    0,
    gCNTcounterObjDescriptorInitAdapter,
    CntCounter_update,
    gCNTcounterObjDescriptorHitDetectAdapter,
    gCNTcounterObjDescriptorRenderAdapter,
    gCNTcounterObjDescriptorFreeAdapter,
    gCNTcounterObjDescriptorTypeIdAdapter,
    gCNTcounterObjDescriptorExtraSizeAdapter,
};
