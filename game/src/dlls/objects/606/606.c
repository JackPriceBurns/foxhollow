#include "main/mapEvent.h"
#include "main/spellstone.h"
#include "game/objects/object_setup.h"
#include "main/gamebit_ids.h"
#include "main/object_render.h"
#include "sys/objects.h"
#include "main/vecmath.h"
#include "main/gamebits_api.h"
#include "main/object_update_list.h"
#include "main/objhits.h"
#include "main/objtype.h"
#include "main/spellstone_idle.h"
#include "main/mapEventTypes.h"

typedef struct SpellStoneState {
    u8 state;
} SpellStoneState;

typedef struct SpellStonePlacement {
    ObjPlacement base;
    u8 unknown18;
    s8 eventIndex;
    u8 unknown1A[4];
    s16 completeEvent;
    s16 activeEvent;
} SpellStonePlacement;

STATIC_ASSERT(offsetof(SpellStonePlacement, eventIndex) == 0x19);
STATIC_ASSERT(offsetof(SpellStonePlacement, completeEvent) == 0x1E);
STATIC_ASSERT(offsetof(SpellStonePlacement, activeEvent) == 0x20);
STATIC_ASSERT(sizeof(SpellStonePlacement) == 0x22);

int spellstone_idleCallback(void)
{
    return 0;
}

const s16 gSpellStoneEventGameBits[2] = {
    GAMEBIT_SpellStoneRelated049A,
    GAMEBIT_SpellStoneRelated049A
};

int spellstone_getState(GameObject* obj)
{
    return ((SpellStoneState*)obj->extra)->state != SPELLSTONE_STATE_ACTIVE;
}

int spellstone_setState(GameObject* obj, int state)
{
    SpellStoneState* extra = obj->extra;
    u8 oldState = extra->state;

    extra->state = (u8)state;
    if (state == SPELLSTONE_STATE_ACTIVE)
    {
        obj->anim.localPosY += 4.0f;
    }
    return oldState != SPELLSTONE_STATE_IDLE;
}

int spellstone_getExtraSize(void)
{
    return sizeof(SpellStoneState);
}

int spellstone_getObjectTypeId(void)
{
    return 0;
}

void spellstone_free(GameObject* obj)
{
    objFreeObjectType(obj, 0x1E);
}

void spellstone_render(GameObject* obj, u32 p2, u32 p3, u32 p4, u32 p5, s8 visible)
{
    const SpellStoneState* state = obj->extra;

    if (visible != 0 && state->state != SPELLSTONE_STATE_HIDDEN)
    {
        objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
    }
}

void spellstone_hitDetect(void)
{
    return;
}

void spellstone_update(GameObject* obj)
{
    SpellStoneState* state = obj->extra;
    const SpellStonePlacement* placement = obj->anim.placementData;

    if (state->state == SPELLSTONE_STATE_ACTIVE)
    {
        obj->anim.rotY = 0;
        obj->anim.rotX += 0x100;
        obj->anim.rotZ = 0;
    }
    s32 eventActive = mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->completeEvent));
    if (eventActive != 0)
    {
        mainSetBits(gSpellStoneEventGameBits[placement->eventIndex], 1);
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        Obj_RemoveFromUpdateList(obj);
        (*gMapEventInterface)->setMapAct(0x1d, 2);
    }
    else
    {
        eventActive = mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->activeEvent));
        if (eventActive != 0)
        {
            obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
            Obj_RemoveFromUpdateList(obj);
        }
        if (state->state == SPELLSTONE_STATE_ACTIVE)
        {
            GameObject* playerObj = Obj_GetPlayerObject();

            if (Vec_distance(&obj->anim.worldPosX, &playerObj->anim.worldPosX) < 105.0f)
            {
                mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->completeEvent), 1);
            }
        }
        if (state->state == SPELLSTONE_STATE_HIDDEN)
        {
            ObjHits_DisableObject(obj);
            if (obj->ownerObj != NULL)
            {
                GameObject* owner = obj->ownerObj;

                obj->anim.localPos = owner->anim.localPos;
            }
        }
        else
        {
            ObjHits_EnableObject(obj);
        }
    }
}

void spellstone_init(GameObject* obj)
{
    SpellStoneState* state = obj->extra;

    objAddObjectType(obj, 0x1E);
    state->state = SPELLSTONE_STATE_IDLE;
    obj->animEventCallback = spellstone_idleCallback;
}

void spellstone_release(void)
{
}

void spellstone_initialise(void)
{
}

ObjectDescriptor12 gSpellStoneObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_12_SLOTS,
    (ObjectDescriptorCallback)spellstone_initialise,
    (ObjectDescriptorCallback)spellstone_release,
    0,
    (ObjectDescriptorCallback)spellstone_init,
    (ObjectDescriptorCallback)spellstone_update,
    (ObjectDescriptorCallback)spellstone_hitDetect,
    (ObjectDescriptorCallback)spellstone_render,
    (ObjectDescriptorCallback)spellstone_free,
    (ObjectDescriptorCallback)spellstone_getObjectTypeId,
    spellstone_getExtraSize,
    (ObjectDescriptorCallback)spellstone_setState,
    (ObjectDescriptorCallback)spellstone_getState,
};
