/* Falling-rock and icicle controller. */
#include "dlls/objects/362_CRrockfall.h"

#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/objfx.h"
#include "main/object_render.h"
#include "main/resource.h"
#include "main/track_dolphin.h"
#include "main/vecmath_distance.h"
#include "sys/objects.h"
#include "main/audio/sfx.h"
#include "main/objhits.h"

/* anim.romDefNo variants shared by CRrockfall and IMIcicle. */
#define CR_ROCKFALL_SEQ_BIG        0x600
#define CR_ROCKFALL_SEQ_QUARRY     0x67
#define CR_ROCKFALL_RESOURCE_ID    91
#define CR_ROCKFALL_HIT_MASK       0x10
#define CR_ROCKFALL_HIT_VOLUME     1
#define CR_ROCKFALL_HIT_PRIORITY   13
#define CR_ROCKFALL_SCRAPE_CHANNEL 8

void* gCrRockfallResource;

static int crrockfall_isTriggerNear(f32 xz, f32 dy, u32 range) {
    return xz > 0.0f && xz < 4.0f * (f32)range && dy < 300.0f;
}

f32 crrockfall_findFloorY(GameObject* obj) {
    CrRockfallState* state = obj->extra;
    TrackGroundHit** list;
    TrackGroundHit** p;
    int count;
    int i;
    int bestIdx;
    f32 bestDist;

    count = trackGetHeight(obj, obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ, &list, 0, 0);
    bestDist = 100000.0f;
    bestIdx = -1;
    for (i = 0, p = list; i < count; i++) {
        f32 dy;
        if ((dy = obj->anim.localPosY - (*p)->height) > 20.0f && dy < bestDist) {
            bestDist = dy;
            bestIdx = i;
        }
        p++;
    }
    if (bestIdx != -1) {
        state->floorFound = 1;
        return list[bestIdx]->height;
    }
    return obj->anim.localPosY;
}

int crrockfall_getExtraSize(void) {
    return sizeof(CrRockfallState);
}

int crrockfall_getObjectTypeId(void) {
    return 0;
}

void crrockfall_free(void) {
}

void crrockfall_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    CrRockfallState* state = obj->extra;

    if (state->mode != CR_ROCKFALL_MODE_SHATTERED && visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void crrockfall_hitDetect(void) {
}

void crrockfall_update(GameObject* obj) {
    CrRockfallState* state = obj->extra;
    ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
    ObjModelState* phaseData = obj->anim.modelState;
    const CrRockfallPlacement* placement = (const CrRockfallPlacement*)obj->anim.placementData;

    if (gCrRockfallResource == NULL) {
        gCrRockfallResource = Resource_Acquire(CR_ROCKFALL_RESOURCE_ID, 1);
    }

    if (state->floorFound == 0) {
        state->floorY = crrockfall_findFloorY(obj);
        if (state->floorFound != 0 && phaseData != NULL) {
            phaseData->overrideWorldPosY = state->floorY;
            objShadowInvalidate(obj);
        }
        return;
    } else {
        if (phaseData != NULL) {
            f32 heightFraction;
            f32 height;
            f32 playerDistance;
            int alphaScale;
            GameObject* player;

            heightFraction = (obj->anim.localPosY - state->floorY) / (state->startY - state->floorY);
            if (heightFraction > 1.0f) {
                heightFraction = 1.0f;
            } else if (heightFraction < 0.0f) {
                heightFraction = 0.0f;
            }
            height = 1.0f - heightFraction;
            player = Obj_GetPlayerObject();
            if (player != NULL) {
                playerDistance = Vec_distance(&obj->anim.worldPosX, &player->anim.worldPosX);
                if (playerDistance > 350.0f) {
                    playerDistance = 350.0f;
                } else if (playerDistance < 250.0f) {
                    playerDistance = 250.0f;
                }
            } else {
                playerDistance = 350.0f;
            }
            playerDistance = (playerDistance - 250.0f) / 100.0f;
            playerDistance = 1.0f - playerDistance;
            alphaScale = (int)(120.0f * height) + 0x40;
            phaseData->shadowAlpha =
                (int)(((f32)(u32)obj->anim.renderAlpha / 255.0f) * ((f32)alphaScale * playerDistance));
        }

        if (ObjAnim_ReadPlacementS16(&obj->anim, &(placement->gameBitId)) != -1 && mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->gameBitId))) == 0) {
            return;
        }

        switch (state->mode) {
        case CR_ROCKFALL_MODE_ARMED: {
            f32 xzDistance;
            f32 verticalDistance;
            int inRange;
            GameObject* player = Obj_GetPlayerObject();

            if (player == NULL) {
                inRange = 0;
            } else {
                phaseData = (ObjModelState*)obj->anim.placementData;
                xzDistance = Vec_xzDistance(&obj->anim.worldPosX, &player->anim.worldPosX);
                verticalDistance = obj->anim.localPosY - player->anim.localPosY;
                if (verticalDistance < 0.0f) {
                    verticalDistance = 0.0f;
                }
                if (xzDistance < 4.0f * (f32)(u32)((const CrRockfallPlacement*)phaseData)->triggerRange &&
                    verticalDistance < 300.0f) {
                    inRange = 1;
                } else {
                    inRange = 0;
                }
            }
            if (inRange != 0) {
                if ((state->fallDelay -= framesThisStep) <= 0) {
                    state->mode = CR_ROCKFALL_MODE_FALLING;
                }
            }
            break;
        }
        case CR_ROCKFALL_MODE_FALLING:
            if (state->fallStarted == 0) {
                state->fallStarted = 1;
                obj->anim.velocityY = 0.0f;
                if (obj->anim.romDefNo == CR_ROCKFALL_SEQ_QUARRY) {
                    Sfx_PlayFromObject(obj, SFXTRIG_dn_boar1_c_155);
                }
                Sfx_PlayFromObject(obj, SFXTRIG_wp_swdwood16);
                hitState->flags |= OBJHITS_PRIORITY_STATE_ENABLED;
            }
            hitState->objectHitMask = CR_ROCKFALL_HIT_MASK;
            hitState->skeletonHitMask = CR_ROCKFALL_HIT_MASK;
            hitState->hitVolumeId = CR_ROCKFALL_HIT_VOLUME;
            hitState->hitVolumePriority = CR_ROCKFALL_HIT_PRIORITY;
            obj->anim.velocityY = -0.15f * timeDelta + obj->anim.velocityY;
            obj->anim.localPosY = obj->anim.velocityY * timeDelta + obj->anim.localPosY;
            if (obj->anim.localPosY < state->floorY + state->config->restOffsetY) {
                obj->anim.localPosY = state->config->restOffsetY * obj->anim.rootMotionScale + state->floorY;
                state->mode = CR_ROCKFALL_MODE_RESTING;
                if (state->config->landSfx != 0) {
                    Sfx_PlayFromObject(obj, (u16)state->config->landSfx);
                }
            }
            break;
        case CR_ROCKFALL_MODE_RESTING:
            hitState->objectHitMask = CR_ROCKFALL_HIT_MASK;
            hitState->skeletonHitMask = CR_ROCKFALL_HIT_MASK;
            hitState->hitVolumeId = CR_ROCKFALL_HIT_VOLUME;
            hitState->hitVolumePriority = CR_ROCKFALL_HIT_PRIORITY;
            break;
        case CR_ROCKFALL_MODE_SHATTERED:
            break;
        }

        if (hitState->lastHitObject != 0) {
            hitState->flags &= ~OBJHITS_PRIORITY_STATE_ENABLED;
            state->mode = CR_ROCKFALL_MODE_SHATTERED;
            Sfx_StopObjectChannel(obj, CR_ROCKFALL_SCRAPE_CHANNEL);
            if (obj->anim.romDefNo == CR_ROCKFALL_SEQ_QUARRY) {
                Sfx_PlayFromObject(obj, SFXTRIG_mv_dinostomp1);
            } else {
                Sfx_PlayFromObject(obj, SFXTRIG_jbike_bombbeep);
                spawnExplosion(obj, (f32)(u32)placement->scaleByte, 1, 1, 0, 1, 1, 1, 1);
            }
        }
    }

    {
        f32 z = 0.0f;
        obj->anim.velocityX = z;
        obj->anim.velocityZ = z;
    }
}

void crrockfall_init(GameObject* obj, const CrRockfallPlacement* placement) {
    CrRockfallState* state = obj->extra;
    ObjHitsPriorityState* hitState;
    ObjModelState* modelState;

    state->mode = CR_ROCKFALL_MODE_ARMED;
    state->startY = obj->anim.localPosY;
    state->fallDelay = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->fallDelay));
    obj->anim.rootMotionScale = (f32)(u32)placement->scaleByte / 127.0f;

    hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
    if (hitState != NULL) {
        f32 scale = obj->anim.rootMotionScale;
        ObjHitbox_SetCapsuleBounds((ObjAnimComponent*)obj, (int)((f32)hitState->primaryRadius * scale),
                                   (int)((f32)hitState->primaryCapsuleOffsetA * scale),
                                   (int)((f32)hitState->primaryCapsuleOffsetB * scale));
        ObjHits_DisableObject(obj);
    }

    modelState = obj->anim.modelState;
    if (modelState != NULL) {
        modelState->flags |= (OBJ_MODEL_STATE_UNREAD_0080 | OBJ_MODEL_STATE_SHADOW_POS_OVERRIDE | OBJ_MODEL_STATE_UNREAD_0010);
        modelState->flags |= (OBJ_MODEL_STATE_UNREAD_0800 | OBJ_MODEL_STATE_UNREAD_0400);
        modelState->overrideWorldPosX = obj->anim.localPosX;
        modelState->overrideWorldPosZ = obj->anim.localPosZ;
        modelState->shadowScale *= obj->anim.rootMotionScale;
    }

    if (obj->anim.romDefNo == CR_ROCKFALL_SEQ_BIG) {
        state->config = &gCrRockfallConfigTable[1];
    } else {
        state->config = &gCrRockfallConfigTable[0];
    }
}

void crrockfall_release(void) {
    if (gCrRockfallResource != NULL) {
        Resource_Release(gCrRockfallResource);
    }
    gCrRockfallResource = NULL;
}

void crrockfall_initialise(void) {
    gCrRockfallResource = NULL;
}

CrRockfallConfig gCrRockfallConfigTable[CR_ROCKFALL_CONFIG_COUNT] = {
    {CR_ROCKFALL_SEQ_QUARRY, 0, 20.0f},
    {CR_ROCKFALL_SEQ_BIG, 0x3E3, 30.0f},
};

OBJECT_INIT_ADAPTER(gCRrockfallObjDescriptorInitAdapter, crrockfall_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gCRrockfallObjDescriptorHitDetectAdapter, crrockfall_hitDetect)
OBJECT_FREE_ADAPTER(gCRrockfallObjDescriptorFreeAdapter, crrockfall_free)
OBJECT_TYPE_ID_ADAPTER(gCRrockfallObjDescriptorTypeIdAdapter, crrockfall_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gCRrockfallObjDescriptorExtraSizeAdapter, crrockfall_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gCRrockfallObjDescriptorAcquire, crrockfall_initialise)

ObjectDescriptor gCRrockfallObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gCRrockfallObjDescriptorAcquire,
        crrockfall_release,
    },
    0,
    gCRrockfallObjDescriptorInitAdapter,
    crrockfall_update,
    gCRrockfallObjDescriptorHitDetectAdapter,
    crrockfall_render,
    gCRrockfallObjDescriptorFreeAdapter,
    gCRrockfallObjDescriptorTypeIdAdapter,
    gCRrockfallObjDescriptorExtraSizeAdapter,
};
