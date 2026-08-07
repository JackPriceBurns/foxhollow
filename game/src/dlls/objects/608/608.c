/* DLL 608: ProximityMine-family object callbacks. */
#include "main/dll/partfx_interface.h"
#include "main/dll/objfx_api.h"
#include "main/proximitymine.h"
#include "main/frame_timing.h"
#include "sys/objects.h"
#include "main/object_render.h"
#include "main/objfx.h"
#include "main/objhits.h"
#include "main/objtexture.h"
#include "main/vecmath.h"
#include "main/audio/sfx_trigger_ids.h"
#include "dolphin/MSL_C/PPCEABI/bare/H/math_api.h"
#include "main/lightmap_api.h"
#include "main/track_dolphin_api.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_stop_channel_api.h"
#include "main/audio/sfx_stop_object_api.h"
#include "main/maketex_timer_api.h"
#include "main/obj_path.h"
#include "main/shader_api.h"
#include "sys/objects/lifecycle.h"
#include "main/model_light.h"

s32 gProximityMineLifespanFrames = 40;
f32 gProximityMineGlowScale = 50.0f;
u8 gProximityMineGlowAlpha = 0x80;
f32 gProximityMineResetGlowScale = 50.0f;
u8 gProximityMineResetGlowAlpha = 0x80;
f32 gProximityMineLaunchDistDivisor = 15.0f;
f32 gProximityMineLaunchSpeedBias = 0.5f;
f32 gProximityMineExplosionRadiusScale = 0.5f;

#define PROXIMITYMINE_PARTFX 0x51c

/* the proximity-triggered variant; the same code also drives contact-only mines
   spawned under other ids. retail OBJECTS.bin name "ProximityMi" (DLL 0x260) */
#define PROXIMITYMINE_OBJ 0x789

void ProximityMine_expire(GameObject* obj)
{
    ProximityMineState* state;
    f32 zeroVelocity;

    state = obj->extra;
    Obj_GetPlayerObject();
    Sfx_StopFromObject(obj, SFXTRIG_id_2e9);
    Sfx_StopFromObject(obj, SFXTRIG_id_2e8);
    Sfx_PlayFromObject(obj, SFXTRIG_crthit6);
    zeroVelocity = 0.0f;
    obj->anim.velocityX = zeroVelocity;
    obj->anim.velocityZ = zeroVelocity;
    storeZeroToFloatParam(&state->renderTimer);
    s16toFloat(&state->renderTimer, 10);
    state->mode = PROXIMITYMINE_MODE_EXPIRED;
    ObjHits_EnableObject(obj);
    ObjHits_MarkObjectPositionDirty(&obj->anim);
    storeZeroToFloatParam(&state->resetTimer);
    objfx_shakeCameraByDistance(obj, 200.0f);
    {
        f32 triggerRadiusDelta = state->triggerDistance - 30.0f;
        spawnExplosion(obj, 60.0f + triggerRadiusDelta * gProximityMineExplosionRadiusScale, 1, 1, 0, 1, 0, 1,
                       0);
    }
    ObjHitbox_SetCapsuleBounds(&obj->anim, state->triggerDistance, -5, 10);
    ObjHits_SetHitVolumeSlot(&obj->anim, PROXIMITYMINE_HIT_VOLUME_SLOT, 1, 0);
    ObjHits_EnableObject(obj);
    if (state->effectHandle != NULL)
    {
        modelLightStruct_freeSlot(&state->effectHandle);
    }
}

int ProximityMine_getExtraSize(void)
{
    return sizeof(ProximityMineState);
}

int ProximityMine_getObjectTypeId(void)
{
    return 0;
}

void ProximityMine_free(GameObject* obj)
{
    ProximityMineState* state;

    state = obj->extra;
    if (state->effectHandle != NULL)
    {
        modelLightStruct_freeSlot(&state->effectHandle);
    }
    return;
}

void ProximityMine_render(GameObject* obj, u32 p2, u32 p3, u32 p4, u32 p5)
{
    int mapBlock;
    ModelLightStruct* effect;
    ProximityMineState* state;

    state = obj->extra;
    if (obj->ownerObj != NULL)
    {
        state->targetObj = obj->ownerObj;
        obj->ownerObj = NULL;
    }
    if (timerIsActive(&state->renderTimer) != 0 ||
        (mapBlock = objPosToMapBlockIdx((double)obj->anim.localPosX, (double)obj->anim.localPosY,
                                        (double)obj->anim.localPosZ)) == -1)
    {
        return;
    }
    effect = state->effectHandle;
    if ((effect != NULL) && (effect->glowType != 0) && (effect->enabled != 0))
    {
        queueGlowRender(effect);
    }
    objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
    return;
}

void ProximityMine_hitDetect(GameObject* obj)
{
    f32 zeroVelocity;
    int hit;
    int hitFlag;
    ObjHitsPriorityState* hitState;
    ProximityMineState* state;

    if (timerIsActive(&((ProximityMineState*)obj->extra)->renderTimer) == 0)
    {
        hit = ObjHits_GetPriorityHit(obj, 0, 0, 0);
        hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
        hitFlag = hitState->contactFlags;
        if ((hitFlag != 0) || (hit != 0) || (hitState->lastHitObject != 0))
        {
            state = obj->extra;
            zeroVelocity = 0.0f;
            obj->anim.velocityY = zeroVelocity;
            obj->anim.velocityX = zeroVelocity;
            obj->anim.velocityZ = zeroVelocity;
            state->mode = PROXIMITYMINE_MODE_EXPIRED;
            storeZeroToFloatParam(&state->resetTimer);
            s16toFloat(&state->resetTimer, 1);
            s16toFloat(&state->renderTimer, 10);
        }
    }
    return;
}

void ProximityMine_update(GameObject* obj)
{
    f32 groundY;
    MatrixTransform params;
    ProximityMineState* state;

    state = obj->extra;
    if (state->effectHandle != NULL)
    {
        modelLightStruct_updateGlowAlpha(state->effectHandle);
    }
    if (obj->ownerObj != NULL)
    {
        state->targetObj = obj->ownerObj;
        obj->ownerObj = NULL;
    }
    if (timerIsActive(&state->lifespanTimer) != 0)
    {
        obj->anim.rootMotionScale += state->scaleStep * timeDelta;
        if (state->targetObj != NULL)
        {
            if (objUpdateOpacity(state->targetObj) != 0)
            {
                ObjPath_GetPointWorldPosition(state->targetObj, obj->userData1, &obj->anim.localPosX,
                                              &obj->anim.localPosY, &obj->anim.localPosZ, 0);
            }
            else
            {
                obj->anim.localPosX = state->targetObj->anim.localPosX;
                obj->anim.localPosY = state->targetObj->anim.localPosY;
                obj->anim.localPosZ = state->targetObj->anim.localPosZ;
            }
        }
        if (timerCountDown(&state->lifespanTimer) != 0)
        {
            if (state->mode == PROXIMITYMINE_MODE_ARMED)
            {
                trackGetNearestGroundOffset(obj, obj->anim.localPosX, obj->anim.localPosY,
                                     obj->anim.localPosZ, &groundY, 0);
                obj->anim.localPosY -= groundY;
                Sfx_PlayFromObject(obj, SFXTRIG_id_2e6);
                Sfx_PlayFromObject(obj, SFXTRIG_id_2e8);
            }
            else
            {
                Sfx_PlayFromObject(obj, SFXTRIG_id_2e7);
                Sfx_PlayFromObject(obj, SFXTRIG_id_2e9);
            }
        }
        if (state->effectHandle == NULL)
        {
            int brightness;
            ObjTextureRuntimeSlot* tex;

            state->effectHandle = modelLightStruct_createPointLight(obj, 0xff, 0, 0, 0);
            tex = objFindTexture(obj, 0, 0);
            if (tex != NULL)
            {
                tex->textureId = (tex->textureId + 0x10) % 512;
                brightness = tex->textureId >> 8;
            }
            else
            {
                brightness = 0;
            }
            if (state->effectHandle != NULL)
            {
                state->effectHandle->enabled = brightness;
                modelLightStruct_setupGlow(state->effectHandle, 0, 0xff, 0, 0, gProximityMineGlowAlpha, gProximityMineGlowScale);
                {
                    ModelLightStruct* fx = state->effectHandle;
                    modelLightStruct_setPosition(fx, 0.0f, obj->anim.hitboxScale, 0.0f);
                }
            }
        }
    }
    else
    {
        if (timerIsActive(&state->resetTimer) != 0)
        {
            Sfx_PlayFromObject(obj, SFXTRIG_id_ef);
            if (state->effectHandle == NULL)
            {
                state->effectHandle = modelLightStruct_createPointLight(obj, 0xff, 0, 0, 0);
                if (state->effectHandle != NULL)
                {
                    modelLightStruct_setupGlow(state->effectHandle, 0, 0xff, 0, 0, gProximityMineResetGlowAlpha, gProximityMineResetGlowScale);
                    {
                        ModelLightStruct* fx = state->effectHandle;
                        modelLightStruct_setPosition(fx, 0.0f, obj->anim.hitboxScale, 0.0f);
                    }
                }
            }
            if (timerCountDown(&state->resetTimer) != 0)
            {
                ProximityMine_expire(obj);
                return;
            }
        }
        switch (state->mode)
        {
        case PROXIMITYMINE_MODE_WAITING:
        {
            f32 trigger;
            GameObject* player;

            trigger = ((ProximityMineDef*)obj->anim.placementData)->parameter;
            player = Obj_GetPlayerObject();
            if (Vec_distance(&obj->anim.worldPosX, &player->anim.worldPosX) < trigger)
            {
                state->mode = PROXIMITYMINE_MODE_ARMED;
                s16toFloat(&state->resetTimer, 0x78);
            }
            break;
        }
        case PROXIMITYMINE_MODE_EXPIRED:
            Sfx_StopObjectChannel(obj, 0x40);
            if (timerCountDown(&state->renderTimer) != 0)
            {
                Obj_FreeObject(obj);
                return;
            }
            break;
        case PROXIMITYMINE_MODE_LAUNCHING:
        {
            f32 dist;
            f32 zero;
            GameObject* player;

            player = Obj_GetPlayerObject();
            dist = Vec_xzDistance(&obj->anim.worldPosX, &player->anim.worldPosX);
            state->mode = PROXIMITYMINE_MODE_FLIGHT;
            obj->anim.velocityX = 0.0f;
            obj->anim.velocityY = sqrtf(dist) / gProximityMineLaunchDistDivisor + 1.1f * gProximityMineLaunchSpeedBias;
            obj->anim.velocityZ = -1.1f * gProximityMineLaunchSpeedBias - sqrtf(dist) / gProximityMineLaunchDistDivisor;
            zero = 0.0f;
            params.x = zero;
            params.y = zero;
            params.z = zero;
            params.scale = 1.0f;
            params.rotZ = 0;
            params.rotY = 0;
            params.rotX = obj->anim.rotX;
            vecRotateZXY(&params.rotX, &obj->anim.velocityX);
            Sfx_PlayFromObject(obj, SFXTRIG_id_f0);
        }
        case PROXIMITYMINE_MODE_FLIGHT:
            if (timerCountDown(&state->launchTimer) != 0)
            {
                f32 zero;

                state = obj->extra;
                zero = 0.0f;
                obj->anim.velocityY = zero;
                obj->anim.velocityX = zero;
                obj->anim.velocityZ = zero;
                state->mode = PROXIMITYMINE_MODE_EXPIRED;
                storeZeroToFloatParam(&state->resetTimer);
                s16toFloat(&state->resetTimer, 1);
                s16toFloat(&state->renderTimer, 10);
                return;
            }
            if (obj->anim.velocityY > -10.0f)
            {
                obj->anim.velocityY += -0.12f * timeDelta;
            }
            obj->anim.rotX += framesThisStep << 10;
            obj->anim.rotY += framesThisStep * 0x700;
            obj->anim.localPosX += obj->anim.velocityX * timeDelta;
            obj->anim.localPosY += obj->anim.velocityY * timeDelta;
            obj->anim.localPosZ += obj->anim.velocityZ * timeDelta;
            obj->anim.worldPosX = obj->anim.localPosX;
            obj->anim.worldPosY = obj->anim.localPosY;
            obj->anim.worldPosZ = obj->anim.localPosZ;
        case PROXIMITYMINE_MODE_ARMED:
            (*gPartfxInterface)->spawnObject(obj, PROXIMITYMINE_PARTFX, NULL, 1, -1, NULL);
            if (timerCountDown(&state->bounceTimer) != 0)
            {
                ObjHits_EnableObject(obj);
            }
            ObjHits_SetHitVolumeSlot(&obj->anim, PROXIMITYMINE_HIT_VOLUME_SLOT, 1, 0);
            if (state->effectHandle != NULL)
            {
                if ((state->effectHandle->enabled != 0) && (state->effectVisible == 0))
                {
                    Sfx_PlayFromObject(obj, SFXTRIG_gal_prophitbird);
                }
                state->effectVisible = state->effectHandle->enabled;
            }
            else
            {
                state->effectVisible = 0;
            }
            break;
        }
        if (timerIsActive(&state->renderTimer) == 0)
        {
            if (objPosToMapBlockIdx((double)obj->anim.localPosX, (double)obj->anim.localPosY,
                                    (double)obj->anim.localPosZ) == -1)
            {
                f32 zero;

                state = obj->extra;
                zero = 0.0f;
                obj->anim.velocityY = zero;
                obj->anim.velocityX = zero;
                obj->anim.velocityZ = zero;
                state->mode = PROXIMITYMINE_MODE_EXPIRED;
                storeZeroToFloatParam(&state->resetTimer);
                s16toFloat(&state->resetTimer, 1);
                s16toFloat(&state->renderTimer, 10);
            }
        }
    }
}

void ProximityMine_init(GameObject* obj, ProximityMineDef* def)
{
    s8 mode;
    ProximityMineState* state;

    state = obj->extra;
    if (obj->anim.romDefNo == PROXIMITYMINE_OBJ)
    {
        def->mode = PROXIMITYMINE_SPAWN_PROXIMITY;
    }
    obj->anim.rotX = 0;
    ObjHits_DisableObject(obj);
    state->mode = PROXIMITYMINE_MODE_EXPIRED;
    storeZeroToFloatParam(&state->renderTimer);
    storeZeroToFloatParam(&state->resetTimer);
    storeZeroToFloatParam(&state->bounceTimer);
    s16toFloat(&state->bounceTimer, 0x14);
    storeZeroToFloatParam(&state->launchTimer);
    storeZeroToFloatParam(&state->initTimer);
    s16toFloat(&state->initTimer, 5);
    obj->anim.rotX = def->angleSeed << 8;
    storeZeroToFloatParam(&state->lifespanTimer);
    s16toFloat(&state->lifespanTimer, (s16)gProximityMineLifespanFrames);
    state->flashMode = 0;
    state->triggerDistance = 30.0f;
    state->effectVisible = 0;
    mode = def->mode;
    switch (mode)
    {
    case PROXIMITYMINE_SPAWN_TIMED:
        s16toFloat(&state->resetTimer, def->parameter);
        state->mode = PROXIMITYMINE_MODE_ARMED;
        Obj_SetActiveModelIndex(obj, 1);
        obj->anim.rootMotionScale *= 0.25f;
        break;
    case PROXIMITYMINE_SPAWN_LAUNCHED:
        s16toFloat(&state->launchTimer, 800);
        s16toFloat(&state->resetTimer, 800);
        obj->anim.rotX = def->parameter;
        state->mode = PROXIMITYMINE_MODE_LAUNCHING;
        obj->anim.rootMotionScale *= 0.25f;
        break;
    case PROXIMITYMINE_SPAWN_PROXIMITY:
        storeZeroToFloatParam(&state->lifespanTimer);
        state->mode = PROXIMITYMINE_MODE_WAITING;
        ObjHits_EnableObject(obj);
        state->triggerDistance = (f32)(s32)def->parameter;
        storeZeroToFloatParam(&state->bounceTimer);
        break;
    }
    state->scaleStep = (3.0f * obj->anim.rootMotionScale) / gProximityMineLifespanFrames;
    state->targetObj = NULL;
    state->effectHandle = NULL;
    return;
}

void ProximityMine_release(void)
{
    return;
}

void ProximityMine_initialise(void)
{
    return;
}

ObjectDescriptor gProximityMineObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)ProximityMine_initialise,
    (ObjectDescriptorCallback)ProximityMine_release,
    0,
    (ObjectDescriptorCallback)ProximityMine_init,
    (ObjectDescriptorCallback)ProximityMine_update,
    (ObjectDescriptorCallback)ProximityMine_hitDetect,
    (ObjectDescriptorCallback)ProximityMine_render,
    (ObjectDescriptorCallback)ProximityMine_free,
    (ObjectDescriptorCallback)ProximityMine_getObjectTypeId,
    ProximityMine_getExtraSize,
};
