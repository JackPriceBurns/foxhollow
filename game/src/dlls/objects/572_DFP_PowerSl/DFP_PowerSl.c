#include "main/dfppowersl.h"
#include "main/dll/partfx_interface.h"
#include "main/dll_000A_expgfx.h"
#include "main/gamebits.h"
#include "main/objhits.h"
#include "main/objseq.h"

void dfppowersl_spawnSeqObjectsOnHit(GameObject* obj) {
    if (obj == NULL) {
        return;
    }

    GameObject* outObj = NULL;
    int hitPriority = ObjHits_GetPriorityHit(obj, &outObj, 0, 0);
    if (outObj == NULL || hitPriority == 0) {
        return;
    }

    for (int i = 0; i < 20; i++) {
        (*gPartfxInterface)
            ->spawnObject(obj, DFPPOWERSL_SPAWN_OBJECT_ID, 0, 1, 0xffffffff, 0);
    }
}

int dfppowersl_getExtraSize(void) {
    return sizeof(DfpPowerSlState);
}

void dfppowersl_free(GameObject* obj) {
    if (obj == NULL) {
        return;
    }

    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void dfppowersl_render(GameObject* obj) {
    if (obj == NULL) {
        return;
    }

    DfpPowerSlState* state = obj->extra;
    if (mainGetBit(state->eventId) == 0) {
        (*gPartfxInterface)->spawnObject(obj, state->spawnObjectId, 0, 4, 0xffffffff, 0);
        (*gPartfxInterface)->spawnObject(obj, state->spawnObjectId, 0, 1, 0xffffffff, 0);
    }
}

void dfppowersl_update(GameObject* obj) {
    if (obj == NULL) {
        return;
    }

    DfpPowerSlState* state = obj->extra;
    (*gObjectTriggerInterface)->preempt((uintptr_t)obj, state->activateObjectId);
    (*gObjectTriggerInterface)->runSequence(0, obj, 0xffffffff);
}

void dfppowersl_init(GameObject* obj, DfpPowerSlMapData* mapData) {
    if (obj == NULL) {
        return;
    }

    DfpPowerSlState* state = obj->extra;
    s16 activateObjectId = ObjAnim_ReadPlacementS16(&obj->anim, &mapData->activateObjectId);
    if (activateObjectId <= 0) {
        activateObjectId = DFPPOWERSL_DEFAULT_PARAM_OBJECT_ID;
    }

    s16 spawnObjectId = ObjAnim_ReadPlacementS16(&obj->anim, &mapData->spawnObjectId);
    if (spawnObjectId <= 0) {
        spawnObjectId = DFPPOWERSL_DEFAULT_PARAM_OBJECT_ID;
    }

    obj->animEventCallback = dfppowersl_spawnSeqObjectsOnHit;
    state->activateObjectId = activateObjectId;
    state->spawnObjectId = spawnObjectId;
    state->eventId = ObjAnim_ReadPlacementS16(&obj->anim, &mapData->eventId);
    obj->anim.rotX = mapData->mode << DFPPOWERSL_MODE_WORD_SHIFT;
    ObjHits_SetHitVolumeSlot(&obj->anim, DFPPOWERSL_HIT_VOLUME_SLOT, DFPPOWERSL_HIT_VOLUME_ENABLED, 0);
}

OBJECT_INIT_ADAPTER(gDfppowerslObjDescriptorInitAdapter, dfppowersl_init, obj, placement)
OBJECT_RENDER_ADAPTER(gDfppowerslObjDescriptorRenderAdapter, dfppowersl_render, obj)
OBJECT_FREE_ADAPTER(gDfppowerslObjDescriptorFreeAdapter, dfppowersl_free, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gDfppowerslObjDescriptorExtraSizeAdapter, dfppowersl_getExtraSize)

ObjectDescriptor gDfppowerslObjDescriptor = {
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
    gDfppowerslObjDescriptorInitAdapter,
    dfppowersl_update,
    0,
    gDfppowerslObjDescriptorRenderAdapter,
    gDfppowerslObjDescriptorFreeAdapter,
    0,
    gDfppowerslObjDescriptorExtraSizeAdapter,
};
