/*
 * MikaBomb object (DLL slot 219).
 *
 * Simulates a thrown bomb until it hits the player or its sampled ground
 * plane, then spawns an explosion effect and fades out.
 */
#include "dlls/objects/219_MikaBomb.h"
#include "dlls/objects/220_MikaBombShadow.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/camera_shake.h"
#include "main/dll/dll_005B_modgfx.h"
#include "main/dll/modgfx_interface.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/resource.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"
#include "main/track_dolphin.h"
#include "main/audio/sfx.h"
#include "main/objhits.h"
#include "main/vecmath.h"

typedef struct MikaBombState {
    GameObject* shadowObj;
    f32 groundY;
    Dll5BInterface** effectResource;
    u8 exploded;
} MikaBombState;

static void MikaBomb_explode(GameObject* obj, MikaBombState* state) {
    Dll5BSpawnCountRange spawnCountRange = {5, 10};

    Sfx_PlayFromObject(obj, SFXTRIG_dsmk2_c);
    u32 effectId = randomGetRange(0, 2);
    (*state->effectResource)->spawn(obj, effectId, NULL, 2, -1, &spawnCountRange);
    ObjHitbox_SetSphereRadius(&obj->anim,
                              (s32)(5.0f * (f32)(u32)obj->anim.modelInstance->primaryHitboxRadius));
    CameraShake_StartDampened(3.0f, 10.0f, 6.0f);
    obj->anim.alpha = 0xfe;
    Obj_FreeObject(state->shadowObj);
    state->shadowObj = NULL;
}

int MikaBomb_getExtraSize(void) {
    return sizeof(MikaBombState);
}

int MikaBomb_getObjectTypeId(void) {
    return 0;
}

void MikaBomb_free(GameObject* obj, int mode) {
    MikaBombState* state = obj->extra;
    if (state->shadowObj != NULL && mode == 0) {
        Obj_FreeObject(state->shadowObj);
        state->shadowObj = NULL;
    }
    (*gModgfxInterface)->detachSource(obj);
}

void MikaBomb_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
    }
}

void MikaBomb_hitDetect(GameObject* obj) {
    (void)obj;
}

void MikaBomb_update(GameObject* obj) {
    MikaBombState* state = obj->extra;
    ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
    u32 alpha = obj->anim.alpha;

    if (alpha < 0xff) {
        f32 alphaFloat = alpha;
        f32 fadeStep = 4.0f * timeDelta;
        if (alphaFloat - fadeStep > 0.0f) {
            obj->anim.alpha = alpha - fadeStep;
        } else {
            Sfx_StopObjectChannel(obj, 0x7f);
            obj->anim.alpha = 0;
            Obj_FreeObject(obj);
            return;
        }
    } else {
        obj->anim.velocityY -= 0.01f * timeDelta;
        if (obj->anim.velocityY < -2.5f) {
            obj->anim.velocityY = -2.5f;
        }
        objMove(obj, obj->anim.velocityX * timeDelta, obj->anim.velocityY * timeDelta, obj->anim.velocityZ * timeDelta);
    }

    if (obj->anim.alpha == 0xff || state->exploded != 0) {
        ObjHits_SetHitVolumeSlot(&obj->anim, 5, 1, 0);
        ObjHits_EnableObject(obj);
        GameObject* lastHitObject = (GameObject*)hitState->lastHitObject;
        if (lastHitObject != NULL && lastHitObject == Obj_GetPlayerObject()) {
            if (obj->anim.alpha == 0xff) {
                MikaBomb_explode(obj, state);
            }
            ObjHits_DisableObject(obj);
        } else if (obj->anim.localPosY <= state->groundY && obj->anim.alpha == 0xff) {
            MikaBomb_explode(obj, state);
            state->exploded = 1;
        }
    }
}

void MikaBomb_init(GameObject* obj) {
    MikaBombState* state = obj->extra;
    f32 groundDistance;

    ObjHits_DisableObject(obj);
    obj->anim.alpha = 0xff;
    obj->anim.velocityX = 0.0f;
    obj->anim.velocityY = -1.0f;
    obj->anim.velocityZ = 0.0f;
    obj->anim.rotY = -0x4000;
    obj->anim.rotX = 0;
    obj->anim.rotZ = 0;
    trackGetHeightAboveGround(obj, obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ, &groundDistance, 0);
    state->groundY = obj->anim.localPosY - groundDistance;
    if (Obj_IsLoadingLocked() != 0) {
        MikaBombShadowPlacement* shadowSetup =
            (MikaBombShadowPlacement*)Obj_AllocObjectSetup(sizeof(*shadowSetup), MIKA_BOMB_SHADOW_OBJECT_ID);
        shadowSetup->base.posX = obj->anim.localPosX;
        shadowSetup->base.posY = obj->anim.localPosY;
        shadowSetup->base.posZ = obj->anim.localPosZ;
        shadowSetup->base.color[0] = 1;
        shadowSetup->base.color[1] = 1;
        shadowSetup->base.color[2] = 0xff;
        shadowSetup->base.color[3] = 0xff;
        state->shadowObj = loadObjectAtObject(obj, &shadowSetup->base);
        state->shadowObj->ownerObj = obj;
    } else {
        state->shadowObj = NULL;
    }
    state->effectResource = Resource_Acquire(0x5b, 1);
    state->exploded = 0;
}

void MikaBomb_release(void) {
}

void MikaBomb_initialise(void) {
}

OBJECT_INIT_ADAPTER(gMikaBombObjDescriptorInitAdapter, MikaBomb_init, obj)
OBJECT_TYPE_ID_ADAPTER(gMikaBombObjDescriptorTypeIdAdapter, MikaBomb_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gMikaBombObjDescriptorExtraSizeAdapter, MikaBomb_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gMikaBombObjDescriptorAcquire, MikaBomb_initialise)

ObjectDescriptor gMikaBombObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gMikaBombObjDescriptorAcquire,
        MikaBomb_release,
    },
    0,
    gMikaBombObjDescriptorInitAdapter,
    MikaBomb_update,
    MikaBomb_hitDetect,
    MikaBomb_render,
    MikaBomb_free,
    gMikaBombObjDescriptorTypeIdAdapter,
    gMikaBombObjDescriptorExtraSizeAdapter,
};
