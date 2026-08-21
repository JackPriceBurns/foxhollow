/*
 * Pollen object (DLL slot 217).
 *
 * Simulates a drifting pollen mote, handles its collision response, and
 * spawns pollen fragments when its vertical motion crosses zero.
 */
#include "dlls/objects/217_Pollen.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/camera.h"
#include "main/dll/partfx_interface.h"
#include "main/dll/dll_00DA_pollenfragment_api.h"
#include "main/dll_000A_expgfx.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"
#include "main/audio/sfx_play_api.h"
#include "main/objhits.h"
#include "main/vecmath.h"

int Pollen_getExtraSize(void) {
    return sizeof(PollenState);
}

int Pollen_getObjectTypeId(void) {
    return 0;
}

void Pollen_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void Pollen_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
    }
}

void Pollen_hitDetect(GameObject* obj) {
    ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;

    if (hitState->contactFlags != 0) {
        obj->anim.localPosX = hitState->contactPosX;
        obj->anim.localPosY = hitState->contactPosY;
        obj->anim.localPosZ = hitState->contactPosZ;
        obj->anim.velocityX = 0.0f;
        obj->anim.velocityY = 0.0f;
        obj->anim.velocityZ = 0.0f;
        obj->anim.alpha = 0;
        ObjHits_DisableObject(obj);
    }
}

void Pollen_update(GameObject* obj) {
    PollenState* state = obj->extra;
    ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;

    if (state->despawnTimer != 0) {
        state->despawnTimer -= 1;
    } else {
        f32 previousVelocityY = obj->anim.velocityY;
        obj->anim.velocityY = -(0.045f * timeDelta - previousVelocityY);
        if (previousVelocityY >= 0.0f && obj->anim.velocityY <= 0.0f) {
            Pollen_burst(obj);
            Sfx_PlayFromObject(obj, SFXTRIG_majring2);
            obj->anim.alpha = 0;
        }
        objMove(obj, obj->anim.velocityX * timeDelta, obj->anim.velocityY * timeDelta, obj->anim.velocityZ * timeDelta);
        ObjHits_SetHitVolumeSlot(&obj->anim, 22, 1, 0);
        ObjHitbox_SetSphereRadius(&obj->anim, 7);
        ObjHits_EnableObject(obj);
        GameObject* lastHitObject = (GameObject*)hitState->lastHitObject;
        if (lastHitObject != NULL &&
            (lastHitObject == Obj_GetPlayerObject() || lastHitObject == getTrickyObject())) {
            CameraShake_Enable();
            CameraShake_SetOffset(1.0f);
            Sfx_PlayFromObject(obj, SFXTRIG_id_b6);
            obj->anim.alpha = 0;
            state->despawnTimer = 60;
            ObjHits_DisableObject(obj);
        }
        if (obj->anim.alpha == 0xff) {
            for (int i = 0; i < 3; i++) {
                (*gPartfxInterface)->spawnObject(obj, 0x4ba, NULL, 1, -1, NULL);
            }
        }
    }
    if (obj->anim.alpha == 0 && state->despawnTimer == 0) {
        Obj_FreeObject(obj);
    }
}

void Pollen_burst(GameObject* obj) {
    PollenState* state = obj->extra;

    if (Obj_IsLoadingLocked() == 0) {
        return;
    }
    for (int i = 0; i < 6; i++) {
        PollenFragmentPlacement* fragmentSetup =
            (PollenFragmentPlacement*)Obj_AllocObjectSetup(sizeof(*fragmentSetup), POLLEN_FRAGMENT_OBJECT_ID);
        fragmentSetup->base.posX = obj->anim.localPosX;
        fragmentSetup->base.posY = obj->anim.localPosY;
        fragmentSetup->base.posZ = obj->anim.localPosZ;
        fragmentSetup->base.color[0] = 1;
        fragmentSetup->base.color[1] = 1;
        fragmentSetup->base.color[2] = 0xff;
        fragmentSetup->base.color[3] = 0xff;
        GameObject* fragment = objSetupObject(&fragmentSetup->base, 5, -1, -1, NULL);
        if (fragment != NULL) {
            fragment->anim.rotY = 0;
            fragment->anim.rotX = randomGetRange(0, 0xffff);
            fragment->anim.velocityX =
                0.03f * (f32)(s32)randomGetRange(-50, 50) +
                obj->anim.velocityX;
            fragment->anim.velocityY =
                0.01f * (f32)(s32)randomGetRange(-50, 50) +
                obj->anim.velocityY;
            fragment->anim.velocityZ =
                0.03f * (f32)(s32)randomGetRange(-50, 50) +
                obj->anim.velocityZ;
            fragment->ownerObj = obj;
        }
    }
    state->despawnTimer = 60;
}

void Pollen_init(GameObject* obj) {
    PollenState* state = obj->extra;
    state->phaseX = randomGetRange(-0x8000, 0x7fff);
    state->driftVelocity = 0.01f * (f32)(s32)randomGetRange(4000, 5000);
    state->phaseY = randomGetRange(-0x8000, 0x7fff);
    state->settleVelocity = 0.0f;
    state->phaseSpeed = randomGetRange(230, 500);
    state->unk10 = 0;
    state->despawnTimer = 0;
    obj->anim.alpha = 0xff;
    ObjHits_DisableObject(obj);
    ObjModelState* modelState = obj->anim.modelState;
    if (modelState != NULL) {
        modelState->flags |= OBJ_MODEL_STATE_UNREAD_0800 | OBJ_MODEL_STATE_UNREAD_0010;
    }
}

void Pollen_release(void) {
}

void Pollen_initialise(void) {
}

ObjectDescriptor gPollenObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)Pollen_initialise,
    (ObjectDescriptorCallback)Pollen_release,
    0,
    (ObjectDescriptorCallback)Pollen_init,
    (ObjectDescriptorCallback)Pollen_update,
    (ObjectDescriptorCallback)Pollen_hitDetect,
    (ObjectDescriptorCallback)Pollen_render,
    (ObjectDescriptorCallback)Pollen_free,
    (ObjectDescriptorCallback)Pollen_getObjectTypeId,
    Pollen_getExtraSize,
};
