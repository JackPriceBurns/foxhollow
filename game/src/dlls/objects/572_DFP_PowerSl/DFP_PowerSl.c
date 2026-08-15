#include "main/dll/partfx_interface.h"
#include "main/dfppowersl.h"
#include "main/dll_000A_expgfx.h"
#include "main/gamebits.h"
#include "main/objhits.h"
#include "main/objseq.h"

static inline DfpPowerSlState* dfppowersl_getState(GameObject* obj)
{
    return obj->extra;
}

int dfppowersl_spawnSeqObjectsOnHit(GameObject* obj)
{
    int i;
    GameObject* outObj;

    outObj = NULL;
    if (obj == 0)
    {
        return 0;
    }
    i = ObjHits_GetPriorityHit(obj, &outObj, 0, 0);
    if ((outObj != NULL) && (i != 0))
    {
        i = 1;
        do
        {
            (*gPartfxInterface)->spawnObject(obj, DFPPOWERSL_SPAWN_OBJECT_ID, 0, DFPPOWERSL_SPAWN_MODE_ACTIVE, 0xffffffff, 0);
        } while (i++ < DFPPOWERSL_SPAWN_COUNT);
    }
    return 0;
}

int dfppowersl_getExtraSize(void)
{
    return sizeof(DfpPowerSlState);
}

void dfppowersl_free(GameObject* obj)
{
    if (obj != 0)
    {
        (*gExpgfxInterface)->freeSource2((u32)obj);
    }
    return;
}

void dfppowersl_render(GameObject* obj)
{
    GameObject* powerSl;
    DfpPowerSlState* state;

    powerSl = obj;
    if ((u32)powerSl != 0)
    {
        state = dfppowersl_getState(powerSl);
        if (mainGetBit(state->eventId) == 0)
        {
            (*gPartfxInterface)
                ->spawnObject(powerSl, state->spawnObjectId, 0, DFPPOWERSL_SPAWN_MODE_PRELOAD, 0xffffffff, 0);
            (*gPartfxInterface)
                ->spawnObject(powerSl, state->spawnObjectId, 0, DFPPOWERSL_SPAWN_MODE_ACTIVE, 0xffffffff, 0);
        }
    }
    return;
}

void dfppowersl_update(GameObject* obj)
{
    GameObject* powerSl;
    DfpPowerSlState* state;

    powerSl = obj;
    if ((u32)powerSl != 0)
    {
        state = dfppowersl_getState(powerSl);
        (*gObjectTriggerInterface)->preempt((uintptr_t)powerSl, state->activateObjectId);
        (*gObjectTriggerInterface)->runSequence(0, powerSl, 0xffffffff);
    }
    return;
}

void dfppowersl_init(GameObject* obj, DfpPowerSlMapData* mapData)
{
    DfpPowerSlState* state;
    s16 activateObjectId;
    s16 spawnObjectId;

    if (obj != 0)
    {
        state = dfppowersl_getState(obj);
        activateObjectId = ObjAnim_ReadPlacementS16(
            &obj->anim, &mapData->activateObjectId);
        spawnObjectId = ObjAnim_ReadPlacementS16(
            &obj->anim, &mapData->spawnObjectId);
        if (activateObjectId <= 0)
        {
            activateObjectId = DFPPOWERSL_DEFAULT_PARAM_OBJECT_ID;
        }
        if (spawnObjectId <= 0)
        {
            spawnObjectId = DFPPOWERSL_DEFAULT_PARAM_OBJECT_ID;
        }
        obj->animEventCallback = dfppowersl_spawnSeqObjectsOnHit;
        state->activateObjectId = activateObjectId;
        state->spawnObjectId = spawnObjectId;
        state->eventId = ObjAnim_ReadPlacementS16(&obj->anim, &mapData->eventId);
        obj->anim.rotX = mapData->mode << DFPPOWERSL_MODE_WORD_SHIFT;
        ObjHits_SetHitVolumeSlot(&obj->anim, DFPPOWERSL_HIT_VOLUME_SLOT, DFPPOWERSL_HIT_VOLUME_ENABLED, 0);
    }
    return;
}

ObjectDescriptor gDfppowerslObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)dfppowersl_init,
    (ObjectDescriptorCallback)dfppowersl_update,
    0,
    (ObjectDescriptorCallback)dfppowersl_render,
    (ObjectDescriptorCallback)dfppowersl_free,
    0,
    dfppowersl_getExtraSize,
};
