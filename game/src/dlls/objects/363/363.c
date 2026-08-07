#include "dlls/objects/363.h"

#include "main/dll/expgfx_interface.h"
#include "main/objseq.h"
#include "main/object_render.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "main/render_lactions_api.h"

#define MAGIC_LIGHT_SEQ_GLOW              0x172
#define MAGIC_LIGHT_SEQ_PROXIMITY         0x16B
#define MAGIC_LIGHT_SEQUENCE_STARTED(obj) ((obj)->userData1)

int MagicLight_sequenceCallback(GameObject* obj) {
    MagicLightState* state;
    GameObject* player;
    f32 distance;

    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQ_GLOW) {
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
    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQ_GLOW) {
        return 0;
    }

    return sizeof(MagicLightState);
}

int MagicLight_getObjectTypeId(void) {
    return 0;
}

void MagicLight_free(GameObject* obj) {
    MagicLightState* state = obj->extra;

    if (obj->anim.romDefNo != MAGIC_LIGHT_SEQ_GLOW) {
        if (state->inRange != 0) {
            getLActions(obj, obj, (u16)state->leaveAction, 0, 0, 0);
        }
        (*gExpgfxInterface)->freeSource2((u32)obj);
    }
}

void MagicLight_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQ_GLOW && visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void MagicLight_hitDetect(void) {
}

void MagicLight_update(GameObject* obj) {
    if (obj->anim.romDefNo != MAGIC_LIGHT_SEQ_GLOW && MAGIC_LIGHT_SEQUENCE_STARTED(obj) == 0) {
        obj->anim.rotX = 0;
        obj->anim.rotY = 0;
        obj->anim.rotZ = 0;
        (*gObjectTriggerInterface)->runSequence(0, obj, -1);
        MAGIC_LIGHT_SEQUENCE_STARTED(obj) = 1;
    }
}

void MagicLight_init(GameObject* obj, const MagicLightPlacement* placement) {
    MagicLightState* state;

    MAGIC_LIGHT_SEQUENCE_STARTED(obj) = 0;
    obj->anim.rotX = (s16)(placement->initialRotX << 8);
    obj->animEventCallback = MagicLight_sequenceCallback;
    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQ_GLOW) {
        return;
    }
    state = obj->extra;
    state->lifetime = randomGetRange(0xC8, 0x258);
    state->subtype = (s8)placement->subtype;
    state->inRange = 0;
    if (obj->anim.romDefNo == MAGIC_LIGHT_SEQ_PROXIMITY) {
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
        state->unknown10 = 0x12D;
    } else {
        state->unknown10 = 0x12D;
    }
}

void MagicLight_release(void) {
}

void MagicLight_initialise(void) {
}

ObjectDescriptor gMagicLightObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)MagicLight_initialise,
    (ObjectDescriptorCallback)MagicLight_release,
    0,
    (ObjectDescriptorCallback)MagicLight_init,
    (ObjectDescriptorCallback)MagicLight_update,
    (ObjectDescriptorCallback)MagicLight_hitDetect,
    (ObjectDescriptorCallback)MagicLight_render,
    (ObjectDescriptorCallback)MagicLight_free,
    (ObjectDescriptorCallback)MagicLight_getObjectTypeId,
    (ObjectDescriptorExtraSizeCallback)MagicLight_getExtraSize,
};
