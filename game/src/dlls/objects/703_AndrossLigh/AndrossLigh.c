#include "dolphin/mtx.h"
#include "main/frame_timing.h"
#include "main/lightningeffect.h"
#include "main/mm.h"
#include "sys/objects.h"
#include "main/newclouds.h"
#include "main/shader_api.h"
#include "main/dll/dll_02BF_androssligh.h"
#include "dolphin/mtx/vec.h"
#include "main/camera.h"

enum
{
    ANDROSSLIGH_ANCHOR_OBJ_ID = 0x47dd9
};

typedef struct AndrossLighState {
    GameObject* anchor;
    LightningEffect* bolt;
    f32 boltAge;
    s8 mode;
    u8 previousMode;
} AndrossLighState;

static void androssligh_applyViewOffset(Vec3f* endpoint)
{
    Vec3f offset;

    offset.x = endpoint->x - playerMapOffsetX;
    offset.y = endpoint->y;
    offset.z = endpoint->x - playerMapOffsetZ;
    PSMTXMultVec((MtxP)Camera_GetViewMatrix(), &offset, &offset);
    offset.x = -offset.x;
    offset.y = -offset.y;
    offset.z = -offset.z;
    PSVECScale(&offset, &offset, 0.05f);
    PSMTXMultVec((MtxP)Camera_GetInverseViewRotationMatrix(), &offset, &offset);
    PSVECAdd(endpoint, &offset, endpoint);
}

static void androssligh_updateBeam(GameObject* obj, AndrossLighState* state)
{
    Vec3f start;
    Vec3f end;

    start.x = obj->anim.localPosX - 300.0f;
    start.y = obj->anim.localPosY;
    start.z = obj->anim.localPosZ;
    end.x = obj->anim.localPosX + 300.0f;
    end.y = start.y;
    end.z = start.z;
    androssligh_applyViewOffset(&start);
    androssligh_applyViewOffset(&end);
    if (state->bolt == NULL)
    {
        state->bolt = lightningCreate(&start, &end, 0.025f, 0.1f, 10, 100, 0);
        state->boltAge = 0.0f;
    }
    else
    {
        state->boltAge += timeDelta;
        state->bolt->timer = (int)(0.5f + state->boltAge);
        if (state->bolt->timer >= state->bolt->lifetime)
        {
            mm_free(state->bolt);
            state->bolt = NULL;
        }
    }
}

void androssligh_setState(GameObject* obj, AndrossLighMode newState, u8 force)
{
    AndrossLighState* state;

    if (obj == NULL)
    {
        return;
    }
    state = (obj)->extra;
    if (state->mode == ANDROSSLIGH_DONE)
    {
        if (force == 0)
        {
            return;
        }
    }
    state->mode = newState;
}

int androssligh_getExtraSize(void)
{
    return sizeof(AndrossLighState);
}

int androssligh_getObjectTypeId(void)
{
    return 0;
}

void androssligh_free(void)
{
}

void androssligh_render(GameObject* obj)
{
    LightningEffect* bolt = ((AndrossLighState*)obj->extra)->bolt;

    if (bolt != NULL)
    {
        lightningRender(bolt);
    }
}

void androssligh_hitDetect(void)
{
}

void androssligh_update(GameObject* obj)
{
    AndrossLighState* state = obj->extra;

    if (state->anchor == NULL)
    {
        state->anchor = ObjList_FindObjectById(ANDROSSLIGH_ANCHOR_OBJ_ID);
    }
    if (state->anchor != NULL)
    {
        obj->anim.localPos = state->anchor->anim.localPos;
    }
    state->previousMode = state->mode;
    switch (state->mode)
    {
    case ANDROSSLIGH_IDLE:
        break;
    case ANDROSSLIGH_ACTIVE:
        androssligh_updateBeam(obj, state);
        break;
    case ANDROSSLIGH_DONE:
        break;
    }
}

void androssligh_init(void)
{
}

ObjectDescriptor gAndrossLighObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)androssligh_init,
    (ObjectDescriptorCallback)androssligh_update,
    (ObjectDescriptorCallback)androssligh_hitDetect,
    (ObjectDescriptorCallback)androssligh_render,
    (ObjectDescriptorCallback)androssligh_free,
    (ObjectDescriptorCallback)androssligh_getObjectTypeId,
    (ObjectDescriptorExtraSizeCallback)androssligh_getExtraSize,
};
