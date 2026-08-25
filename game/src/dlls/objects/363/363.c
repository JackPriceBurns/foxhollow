#include "dlls/objects/363.h"

#include "main/dll/expgfx_interface.h"
#include "main/objseq.h"
#include "main/object_render.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "main/render_lactions.h"

typedef enum MagicLightSequenceId {
    MAGIC_LIGHT_SEQUENCE_PROXIMITY = 0x16B,
    MAGIC_LIGHT_SEQUENCE_GLOW = 0x172,
} MagicLightSequenceId;

typedef struct MagicLightState {
    f32 triggerRadius;
    s16 lifetime;
    s16 enterAction;
    s16 leaveAction;
    u8 pad0A;
    s8 inRange;
    s8 subtype;
    u8 pad0D[0x03];
    s16 unknown10;
    u8 pad12[0x02];
} MagicLightState;

STATIC_ASSERT(sizeof(MagicLightState) == 0x14);
STATIC_ASSERT(offsetof(MagicLightState, triggerRadius) == 0x00);
STATIC_ASSERT(offsetof(MagicLightState, lifetime) == 0x04);
STATIC_ASSERT(offsetof(MagicLightState, enterAction) == 0x06);
STATIC_ASSERT(offsetof(MagicLightState, leaveAction) == 0x08);
STATIC_ASSERT(offsetof(MagicLightState, inRange) == 0x0B);
STATIC_ASSERT(offsetof(MagicLightState, subtype) == 0x0C);
STATIC_ASSERT(offsetof(MagicLightState, unknown10) == 0x10);

int MagicLight_sequenceCallback(GameObject* obj) {
    MagicLightState* state;
    GameObject* player;
    f32 distance;

    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQUENCE_GLOW) {
        return 0;
    }

    state = obj->extra;
    player = Obj_GetPlayerObject();
    distance = Vec_distance(&player->anim.worldPosX, &obj->anim.worldPosX);

    if (distance < state->triggerRadius && state->inRange == 0) {
        state->inRange = 1;
        getLActions(obj, obj, (u16)state->enterAction, 0, 0, 0);
    } else if (distance > 10.0f + state->triggerRadius && state->inRange != 0) {
        state->inRange = 0;
        getLActions(obj, obj, (u16)state->leaveAction, 0, 0, 0);
    }
    return 0;
}

int MagicLight_getExtraSize(GameObject* obj) {
    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQUENCE_GLOW) {
        return 0;
    }

    return sizeof(MagicLightState);
}

int MagicLight_getObjectTypeId(void) {
    return 0;
}

void MagicLight_free(GameObject* obj) {
    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQUENCE_GLOW) {
        return;
    }

    MagicLightState* state = obj->extra;
    if (state->inRange != 0) {
        getLActions(obj, obj, (u16)state->leaveAction, 0, 0, 0);
    }
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void MagicLight_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQUENCE_GLOW && visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void MagicLight_hitDetect(void) {
}

void MagicLight_update(GameObject* obj) {
    if (obj->anim.romDefNo != MAGIC_LIGHT_SEQUENCE_GLOW && obj->userData1 == 0) {
        obj->anim.rotX = 0;
        obj->anim.rotY = 0;
        obj->anim.rotZ = 0;
        (*gObjectTriggerInterface)->runSequence(0, obj, -1);
        obj->userData1 = 1;
    }
}

void MagicLight_init(GameObject* obj, const MagicLightPlacement* placement) {
    MagicLightState* state;

    obj->userData1 = 0;
    obj->anim.rotX = (s16)(placement->initialRotX << 8);
    obj->animEventCallback = MagicLight_sequenceCallback;
    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQUENCE_GLOW) {
        return;
    }
    state = obj->extra;
    state->lifetime = randomGetRange(0xC8, 0x258);
    state->subtype = (s8)ObjAnim_ReadPlacementS16(&obj->anim, &(placement->subtype));
    state->inRange = 0;
    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQUENCE_PROXIMITY) {
        switch (state->subtype) {
        case 0:
            state->enterAction = 0x90;
            state->leaveAction = 0x91;
            state->triggerRadius = 100.0f;
            break;
        case 1:
            state->enterAction = 0x92;
            state->leaveAction = 0x93;
            state->triggerRadius = 100.0f;
            break;
        default:
            state->enterAction = 0x94;
            state->leaveAction = 0x95;
            state->triggerRadius = 300.0f;
            break;
        case 3:
            state->enterAction = 0x187;
            state->leaveAction = 0x5;
            state->triggerRadius = 100.0f;
            break;
        }
    }
    state->unknown10 = 0x12D;
}

void MagicLight_release(void) {
}

void MagicLight_initialise(void) {
}

OBJECT_INIT_ADAPTER(gMagicLightObjDescriptorInitAdapter, MagicLight_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gMagicLightObjDescriptorHitDetectAdapter, MagicLight_hitDetect)
OBJECT_FREE_ADAPTER(gMagicLightObjDescriptorFreeAdapter, MagicLight_free, obj)
OBJECT_TYPE_ID_ADAPTER(gMagicLightObjDescriptorTypeIdAdapter, MagicLight_getObjectTypeId)

RESOURCE_ACQUIRE_ADAPTER(gMagicLightObjDescriptorAcquire, MagicLight_initialise)

ObjectDescriptor gMagicLightObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gMagicLightObjDescriptorAcquire,
        MagicLight_release,
    },
    0,
    gMagicLightObjDescriptorInitAdapter,
    MagicLight_update,
    gMagicLightObjDescriptorHitDetectAdapter,
    MagicLight_render,
    gMagicLightObjDescriptorFreeAdapter,
    gMagicLightObjDescriptorTypeIdAdapter,
    MagicLight_getExtraSize,
};
