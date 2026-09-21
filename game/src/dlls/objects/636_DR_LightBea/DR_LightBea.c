#include "main/audio/sfx.h"
#include "main/dll/dll_002E_moveLib.h"
#include "main/gamebits.h"
#include "main/lightningeffect.h"
#include "main/mm.h"
#include "main/newclouds.h"
#include "main/vecmath.h"
#include "game/objects/object_setup.h"
#include "sys/objects.h"
#include "dlls/object_descriptor.h"

#include "main/audio/sfx_trigger_ids.h"

#include "main/dll/DR/dll_027C_drlightbea.h"
#include "sys/objects/lifecycle.h"

typedef enum DrLightBeamFlags {
    DR_LIGHT_BEAM_ACTIVE = 0x80,
    DR_LIGHT_BEAM_FREE_OBJECT = 0x40
} DrLightBeamFlags;

typedef struct DrLightBeamPlacement {
    ObjPlacement base;
    u8 pad18;
    s8 targetId;
    u8 pad1A[6];
    s16 gameBit;
    u8 pad22[6];
} DrLightBeamPlacement;

typedef struct DrLightBeamState {
    LightningEffect* beam;
    u8 flags;
} DrLightBeamState;

STATIC_ASSERT(offsetof(DrLightBeamPlacement, targetId) == 0x19);
STATIC_ASSERT(offsetof(DrLightBeamPlacement, gameBit) == 0x20);
STATIC_ASSERT(sizeof(DrLightBeamPlacement) == 0x28);

int DR_LightBea_getExtraSize(void)
{
    return sizeof(DrLightBeamState);
}

int DR_LightBea_getObjectTypeId(void)
{
    return 0;
}

void DR_LightBea_free(GameObject* obj)
{
    DrLightBeamState* state = obj->extra;

    if (state->beam != NULL)
    {
        mm_free(state->beam);
        state->beam = NULL;
    }
}

void DR_LightBea_render(GameObject* obj, int p2, int p3, int p4, int p5)
{
    DrLightBeamState* state = obj->extra;
    const DrLightBeamPlacement* placement = (const DrLightBeamPlacement*)obj->anim.placementData;
    GameObject* player;
    MoveLibTarget target;
    Vec3f sourcePos;
    Vec3f targetPos;

    if ((state->flags & DR_LIGHT_BEAM_ACTIVE) != 0)
    {
        state->beam->start[0] = obj->anim.localPosX;
        state->beam->start[1] = obj->anim.localPosY;
        state->beam->start[2] = obj->anim.localPosZ;
        if (placement->targetId == 0)
        {
            player = Obj_GetPlayerObject();
            state->beam->end[0] = player->anim.localPosX;
            state->beam->end[1] = 15.0f + player->anim.localPosY;
            state->beam->end[2] = player->anim.localPosZ;
        }
        lightningRender(state->beam);
        state->beam->timer += 1;
        if (state->beam->timer >= state->beam->lifetime)
        {
            mm_free(state->beam);
            state->beam = NULL;
            state->flags &= ~DR_LIGHT_BEAM_ACTIVE;
            if (placement->base.ident == -1)
            {
                state->flags |= DR_LIGHT_BEAM_FREE_OBJECT;
            }
        }
    }
    else
    {
        if (state->beam != NULL)
        {
            mm_free(state->beam);
            state->beam = NULL;
        }
        if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBit)) != 0)
        {
            state->flags |= DR_LIGHT_BEAM_ACTIVE;
            Sfx_PlayFromObject(obj, SFXTRIG_id_30f);
            sourcePos.x = obj->anim.localPosX;
            sourcePos.y = obj->anim.localPosY;
            sourcePos.z = obj->anim.localPosZ;
            if (placement->targetId != 0 && dll_2E_getCurveActionTarget(placement->targetId, &target) != 0)
            {
                targetPos.x = target.x;
                targetPos.y = target.y;
                targetPos.z = target.z;
            }
            else
            {
                player = Obj_GetPlayerObject();
                targetPos.x = player->anim.localPosX;
                targetPos.y = 15.0f + player->anim.localPosY;
                targetPos.z = player->anim.localPosZ;
            }
            state->beam = lightningCreate(&sourcePos, &targetPos, 0.05f, 0.1f, randomGetRange(5, 0xf), 0x60, 0);
        }
    }
}

void DR_LightBea_hitDetect(void)
{
}

void DR_LightBea_update(GameObject* obj)
{
    DrLightBeamState* state = obj->extra;
    if ((state->flags & DR_LIGHT_BEAM_FREE_OBJECT) != 0)
    {
        Obj_FreeObject(obj);
    }
}

void DR_LightBea_init(GameObject* obj)
{
    DrLightBeamState* state = obj->extra;
    state->flags &= ~(DR_LIGHT_BEAM_ACTIVE | DR_LIGHT_BEAM_FREE_OBJECT);
    state->beam = NULL;
}

void DR_LightBea_release(void)
{
}

void DR_LightBea_initialise(void)
{
}

OBJECT_INIT_ADAPTER(gDrLightBeaObjDescriptorInitAdapter, DR_LightBea_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gDrLightBeaObjDescriptorHitDetectAdapter, DR_LightBea_hitDetect)
OBJECT_RENDER_ADAPTER(gDrLightBeaObjDescriptorRenderAdapter, DR_LightBea_render, obj, arg2, arg3, arg4, arg5)
OBJECT_FREE_ADAPTER(gDrLightBeaObjDescriptorFreeAdapter, DR_LightBea_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDrLightBeaObjDescriptorTypeIdAdapter, DR_LightBea_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDrLightBeaObjDescriptorExtraSizeAdapter, DR_LightBea_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDrLightBeaObjDescriptorAcquire, DR_LightBea_initialise)

ObjectDescriptor gDrLightBeaObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDrLightBeaObjDescriptorAcquire,
        DR_LightBea_release,
    },
    0,
    gDrLightBeaObjDescriptorInitAdapter,
    DR_LightBea_update,
    gDrLightBeaObjDescriptorHitDetectAdapter,
    gDrLightBeaObjDescriptorRenderAdapter,
    gDrLightBeaObjDescriptorFreeAdapter,
    gDrLightBeaObjDescriptorTypeIdAdapter,
    gDrLightBeaObjDescriptorExtraSizeAdapter,
};
