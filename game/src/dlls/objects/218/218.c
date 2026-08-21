/*
 * DLL 0x00DA - the homing pollen-cloud projectile/fragment
 * spawned by the pollen object. Each fragment picks one of five
 * PollenFragmentConfig presets by its pollen type (0..5), spawns a burst of
 * particle fx and a loop sfx on init, then per-frame steers toward the
 * nearest object in its target group, applies velocity damping/gravity,
 * optionally smooth-turns to face its velocity (or free-spins for the
 * 0x482 fragment object), and bursts (explosion fx + sfx) on contact with a
 * non-owner object. Timed variants fade their alpha out and self-free.
 */
#include "main/dll/partfx_interface.h"
#include "dolphin/mtx/vec.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/dll/dll_0282_barrelgener.h"
#include "main/dll/dll_00DA_pollenfragment_api.h"
#include "main/dll_000A_expgfx.h"
#include "main/obj_path.h"
#include "main/objfx.h"
#include "dlls/object_descriptor.h"
#include "main/model_light.h"
#include "main/objhits.h"
#include "main/objtype.h"
#include "sys/objects.h"
#include "main/audio/sfx_keep_alive_api.h"
#include "main/audio/sfx_limited_object_api.h"
#include "main/maketex_timer_api.h"
#include "main/objseq_api.h"
#include "main/vecmath.h"
#include "sys/objects/lifecycle.h"

typedef enum PollenFragmentConfigFlag
{
    POLLEN_FRAGMENT_CONFIG_NO_VERTICAL = 1 << 0,
    POLLEN_FRAGMENT_CONFIG_TIMED = 1 << 1,
    POLLEN_FRAGMENT_CONFIG_SMOOTH_TURN = 1 << 2,
    POLLEN_FRAGMENT_CONFIG_USE_PATH = 1 << 3,
} PollenFragmentConfigFlag;

typedef struct PollenFragmentConfig
{
    s16 spawnSfxId;
    s16 loopSfxId;
    s16 explodeSfxId;
    s16 initFxId;
    s16 burstFxId;
    s16 auraFxId;
    f32 steerSpeed;
    s16 targetGroup;
    u8 flags;
} PollenFragmentConfig;

static const PollenFragmentConfig sPollenFragmentConfigs[] = {
    {
        .spawnSfxId = 0x0000,
        .loopSfxId = 0x049F,
        .explodeSfxId = 0x00B9,
        .initFxId = 0x04BA,
        .burstFxId = 0x04BA,
        .auraFxId = -1,
        .steerSpeed = 0.2f,
        .targetGroup = 0x0000,
        .flags = POLLEN_FRAGMENT_CONFIG_NO_VERTICAL | POLLEN_FRAGMENT_CONFIG_TIMED,
    },
    {
        .spawnSfxId = 0x02FA,
        .loopSfxId = 0x02FB,
        .explodeSfxId = 0x0496,
        .initFxId = 0x068F,
        .burstFxId = 0x068F,
        .auraFxId = 0x068F,
        .steerSpeed = 0.4f,
        .targetGroup = 0x0026,
        .flags = POLLEN_FRAGMENT_CONFIG_TIMED | POLLEN_FRAGMENT_CONFIG_SMOOTH_TURN |
                 POLLEN_FRAGMENT_CONFIG_USE_PATH,
    },
    {
        .spawnSfxId = 0x02FA,
        .loopSfxId = 0x02FB,
        .explodeSfxId = 0x0496,
        .initFxId = 0x068F,
        .burstFxId = 0x068F,
        .auraFxId = 0x068F,
        .steerSpeed = 0.4f,
        .targetGroup = 0x0026,
        .flags = POLLEN_FRAGMENT_CONFIG_SMOOTH_TURN,
    },
    {
        .spawnSfxId = 0x02FA,
        .loopSfxId = 0x02FB,
        .explodeSfxId = 0x0496,
        .initFxId = 0x068F,
        .burstFxId = 0x068F,
        .auraFxId = -1,
        .steerSpeed = 0.2f,
        .targetGroup = 0x0000,
        .flags = POLLEN_FRAGMENT_CONFIG_SMOOTH_TURN,
    },
    {
        .spawnSfxId = 0x02FA,
        .loopSfxId = 0x02FB,
        .explodeSfxId = 0x0496,
        .initFxId = 0x068F,
        .burstFxId = 0x068F,
        .auraFxId = 0x068F,
        .steerSpeed = 0.4f,
        .targetGroup = 0x0026,
        .flags = POLLEN_FRAGMENT_CONFIG_SMOOTH_TURN | POLLEN_FRAGMENT_CONFIG_USE_PATH,
    },
};

typedef struct PollenFragmentExtra
{
    GameObject* ownerObj;
    f32 speed;
    f32 timer;
    Vec direction;
    ModelLightStruct* modelLight;
    const PollenFragmentConfig* config;
    f32 deathTimer;
    f32 lifetimeTimer;
} PollenFragmentExtra;

int pollenfragment_getExtraSize(void)
{
    return sizeof(PollenFragmentExtra);
}

int pollenfragment_getObjectTypeId(void)
{
    return 0x0;
}

void pollenfragment_free(GameObject* obj)
{
    PollenFragmentExtra* state = obj->extra;
    if (state->modelLight != NULL)
    {
        ModelLightStruct_free(state->modelLight);
        state->modelLight = NULL;
    }
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void pollenfragment_render(GameObject* obj, int p2, int p3, int p4, int p5)
{
    PollenFragmentExtra* state = obj->extra;
    if (timerIsActive(&state->deathTimer) != 0)
        return;
    objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
}

void pollenfragment_hitDetect(GameObject* obj)
{
    PollenFragmentExtra* state = obj->extra;
    ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
    int hitType;
    GameObject* hitObject;

    if (timerIsActive(&state->deathTimer) == 0)
    {
        hitType = ObjHits_GetPriorityHit(obj, &hitObject, 0, 0);
        if (hitType == 0xe || hitType == 0xf)
        {
            if (state->config->explodeSfxId != -1)
            {
                spawnExplosion(obj, 30.0f, 0, 1, 0, 1, 0, 1, 0);
                Sfx_PlayFromObjectLimited(obj, (u16)state->config->explodeSfxId, 3);
            }
            ObjHits_DisableObject(obj);
            s16toFloat(&state->deathTimer, 0x78);
        }
        if (hitState->contactFlags != 0)
        {
            ObjHits_DisableObject(obj);
            state->timer = 0.0f;
            if (state->config->explodeSfxId != -1)
            {
                spawnExplosion(obj, 30.0f, 0, 1, 0, 1, 0, 1, 0);
                Sfx_PlayFromObjectLimited(obj, (u16)state->config->explodeSfxId, 3);
            }
            s16toFloat(&state->deathTimer, 0x78);
        }
    }
}

void pollenfragment_update(GameObject* obj)
{
    PollenFragmentExtra* state = obj->extra;
    const PollenFragmentConfig* config = state->config;
    ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
    GameObject* nearObj;
    GameObject* hit;
    f32 horizDamping;
    f32 t;
    Vec dir;
    Vec sc;
    Vec pos;

    if (getCurSeqNo() != 0)
    {
        Obj_FreeObject(obj);
        return;
    }
    if (timerIsActive(&state->deathTimer) != 0)
    {
        if (timerCountDown(&state->deathTimer) != 0)
        {
            Obj_FreeObject(obj);
        }
        return;
    }
    if (timerCountDown(&state->lifetimeTimer) != 0)
    {
        s16toFloat(&state->deathTimer, 0x78);
    }
    if (obj->ownerObj != NULL)
    {
        state->ownerObj = obj->ownerObj;
        obj->ownerObj = NULL;
    }
    if ((config->flags & POLLEN_FRAGMENT_CONFIG_TIMED) != 0)
    {
        state->timer -= timeDelta;
        if (state->timer <= 0.0f)
        {
            if (obj->anim.alpha == 0xff)
            {
                for (int i = 0; i < 3; i++)
                {
                    (*gPartfxInterface)->spawnObject(obj, (int)config->burstFxId, NULL, 1, -1, NULL);
                }
            }
            state->timer = 0.0f;
            if (obj->anim.alpha >= framesThisStep << 3)
            {
                obj->anim.alpha -= framesThisStep << 3;
            }
            else
            {
                obj->anim.alpha = 0;
                Obj_FreeObject(obj);
                return;
            }
        }
    }
    if (config->auraFxId != -1)
    {
        (*gPartfxInterface)
            ->spawnObject(obj, (int)config->auraFxId, NULL, 1, -1, NULL);
    }
    nearObj = objGetNearestTypeTo((int)config->targetGroup, obj, NULL);
    if (nearObj != NULL &&
        ((config->flags & POLLEN_FRAGMENT_CONFIG_TIMED) == 0 || state->timer < 210.0f))
    {
        if ((config->flags & POLLEN_FRAGMENT_CONFIG_USE_PATH) != 0)
        {
            ObjPath_GetPointWorldPosition(nearObj, 0, &pos.x, &pos.y, &pos.z, 0);
        }
        else
        {
            f32 prod;
            f32 quarter = 0.25f;
            pos.x = nearObj->anim.worldPosX;
            prod = nearObj->anim.hitboxScale * nearObj->anim.rootMotionScale;
            pos.y = prod * quarter + nearObj->anim.worldPosY;
            pos.z = nearObj->anim.worldPosZ;
        }
        PSVECSubtract(&pos, &obj->anim.worldPos, &dir);
        PSVECMag(&dir);
        PSVECNormalize(&dir, &dir);
        PSVECSubtract(&dir, &state->direction, &sc);
        state->direction = dir;
        PSVECScale(&sc, &sc, 30.0f);
        PSVECAdd(&dir, &sc, &dir);
        obj->anim.velocityX =
            obj->anim.velocityX +
            ((30.0f + state->timer) * (dir.x * state->speed)) /
                210.0f;
        obj->anim.velocityZ =
            obj->anim.velocityZ +
            ((30.0f + state->timer) * (dir.z * state->speed)) /
                210.0f;
        if ((config->flags & POLLEN_FRAGMENT_CONFIG_NO_VERTICAL) == 0)
        {
            obj->anim.velocityY =
                obj->anim.velocityY + ((30.0f + state->timer) *
                                                      (2.0f * (dir.y * state->speed))) /
                                                         210.0f;
        }
    }
    obj->anim.velocityX = obj->anim.velocityX * (horizDamping = 0.97f);
    obj->anim.velocityZ = obj->anim.velocityZ * horizDamping;
    obj->anim.velocityY *= 0.95f;
    if ((config->flags & POLLEN_FRAGMENT_CONFIG_NO_VERTICAL) != 0)
    {
        t = 0.04f * timeDelta;
        obj->anim.velocityY =
            obj->anim.velocityY - (t * state->timer) / 300.0f;
    }
    if ((config->flags & POLLEN_FRAGMENT_CONFIG_SMOOTH_TURN) != 0)
    {
        Obj_SmoothTurnAnglesTowardVelocity(obj, &obj->anim.velocity, 10, 0.0f,
                                           1.0f);
        obj->anim.rotZ = obj->anim.rotZ + framesThisStep * 0x500;
    }
    else if (obj->anim.romDefNo == POLLEN_FRAGMENT_OBJECT_ID)
    {
        t = 5.0f * 1024.0f;
        obj->anim.rotX = t * (f32)(u32)framesThisStep + (f32)(int)obj->anim.rotX;
        obj->anim.rotY =
            512.0f * (f32)(u32)framesThisStep + (f32)(int)obj->anim.rotY;
    }
    Sfx_KeepAliveLoopedObjectSound(obj, (u16)config->loopSfxId);
    objMove(obj, obj->anim.velocityX * timeDelta, obj->anim.velocityY * timeDelta,
            obj->anim.velocityZ * timeDelta);
    ObjHits_SetHitVolumeSlot(&obj->anim, 0x16, 1, 0);
    ObjHits_EnableObject(obj);
    hit = (GameObject*)hitState->lastHitObject;
    if (hit != NULL && hit->anim.romDefNo != obj->anim.romDefNo &&
        hit != state->ownerObj)
    {
        state->timer = 0.0f;
        ObjHits_DisableObject(obj);
        if (config->explodeSfxId != -1)
        {
            spawnExplosion(obj, 30.0f, 0, 1, 0, 1, 0, 1, 0);
            Sfx_PlayFromObjectLimited(obj, (u16)config->explodeSfxId, 3);
        }
        s16toFloat(&state->deathTimer, 0x78);
    }
}

void pollenfragment_init(GameObject* obj, PollenFragmentPlacement* setup)
{
    s8 pollenType;
    u32 randomValue;
    PollenFragmentExtra* state;

    state = obj->extra;
    if (setup->pollenType == 1)
    {
        state->timer = 155.0f;
    }
    else
    {
        randomValue = randomGetRange(0xb4, 300);
        state->timer = (f32)(int)randomValue;
    }
    pollenType = setup->pollenType;
    pollenType = (pollenType < 0) ? 0 : ((pollenType > 5u) ? 5 : pollenType);
    setup->pollenType = pollenType;
    state->config = &sPollenFragmentConfigs[setup->pollenType];
    if (state->config->spawnSfxId != 0)
    {
        Sfx_PlayFromObjectLimited(obj, (u16)state->config->spawnSfxId, 3);
    }
    for (int i = 0; i < 5; i++)
    {
        (*gPartfxInterface)->spawnObject(obj, state->config->initFxId, NULL, 1, -1, NULL);
    }
    if ((state->config->flags & POLLEN_FRAGMENT_CONFIG_TIMED) == 0)
    {
        state->timer = 60.0f;
    }
    ObjHits_SetTargetMask(obj, 4);
    state->modelLight = NULL;
    state->speed = state->config->steerSpeed;
    state->ownerObj = NULL;
    s16toFloat(&state->lifetimeTimer, 0xe10);
    storeZeroToFloatParam(&state->deathTimer);
}

void pollenfragment_release(void)
{
}

void pollenfragment_initialise(void)
{
}

ObjectDescriptor gPollenFragmentObjDescriptor = {
    0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)pollenfragment_initialise, (ObjectDescriptorCallback)pollenfragment_release, 0,
    (ObjectDescriptorCallback)pollenfragment_init, (ObjectDescriptorCallback)pollenfragment_update,
    (ObjectDescriptorCallback)pollenfragment_hitDetect, (ObjectDescriptorCallback)pollenfragment_render,
    (ObjectDescriptorCallback)pollenfragment_free, (ObjectDescriptorCallback)pollenfragment_getObjectTypeId,
    pollenfragment_getExtraSize,
};
