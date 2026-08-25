#include "main/mapEvent.h"
#include "main/spellstone.h"
#include "game/objects/object_setup.h"
#include "main/gamebit_ids.h"
#include "main/object_render.h"
#include "sys/objects.h"
#include "main/vecmath.h"
#include "main/gamebits.h"
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

                obj->anim.localPosX = owner->anim.localPosX;
                obj->anim.localPosY = owner->anim.localPosY;
                obj->anim.localPosZ = owner->anim.localPosZ;
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

OBJECT_INIT_ADAPTER(gSpellStoneObjDescriptorInitAdapter, spellstone_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gSpellStoneObjDescriptorHitDetectAdapter, spellstone_hitDetect)
OBJECT_RENDER_ADAPTER(gSpellStoneObjDescriptorRenderAdapter, spellstone_render, obj, arg2, arg3, arg4, arg5, visible)
OBJECT_FREE_ADAPTER(gSpellStoneObjDescriptorFreeAdapter, spellstone_free, obj)
OBJECT_TYPE_ID_ADAPTER(gSpellStoneObjDescriptorTypeIdAdapter, spellstone_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSpellStoneObjDescriptorExtraSizeAdapter, spellstone_getExtraSize)

typedef struct SpellStoneObjDescriptorTypeInterface {
    OBJECT_INTERFACE_FIELDS;
    __typeof__(spellstone_setState)* spellstone_setState;
    __typeof__(spellstone_getState)* spellstone_getState;
} SpellStoneObjDescriptorTypeInterface;

struct SpellStoneObjDescriptorType {
    ObjectDescriptorHeader header;
    SpellStoneObjDescriptorTypeInterface interface;
};

RESOURCE_ACQUIRE_ADAPTER(gSpellStoneObjDescriptorAcquire, spellstone_initialise)

struct SpellStoneObjDescriptorType gSpellStoneObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_12_SLOTS,
        },
        gSpellStoneObjDescriptorAcquire,
        spellstone_release,
    },
    {
        0,
        gSpellStoneObjDescriptorInitAdapter,
        spellstone_update,
        gSpellStoneObjDescriptorHitDetectAdapter,
        gSpellStoneObjDescriptorRenderAdapter,
        gSpellStoneObjDescriptorFreeAdapter,
        gSpellStoneObjDescriptorTypeIdAdapter,
        gSpellStoneObjDescriptorExtraSizeAdapter,
        spellstone_setState,
        spellstone_getState,
    },
};
