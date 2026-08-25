#include "dlls/objects/513_WM_colrise.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/object_render.h"
#include "main/objseq.h"

struct WmColumnRisePlacement {
    ObjPlacement base;
    s8 rotationX;
    u8 pad19[5];
    s16 riseGameBit;
};

typedef struct WmColumnRiseState {
    s16 riseGameBit;
    s8 raiseTimer;
    u8 pad03;
} WmColumnRiseState;

STATIC_ASSERT(offsetof(WmColumnRisePlacement, rotationX) == 0x18);
STATIC_ASSERT(offsetof(WmColumnRisePlacement, riseGameBit) == 0x1E);
STATIC_ASSERT(sizeof(WmColumnRisePlacement) == 0x20);
STATIC_ASSERT(offsetof(WmColumnRiseState, raiseTimer) == 0x02);
STATIC_ASSERT(sizeof(WmColumnRiseState) == 0x04);

int WM_colrise_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    animUpdate->flags = -1;
    animUpdate->movementState = 0;
    return 0;
}

int WM_colrise_getExtraSize(void) {
    return sizeof(WmColumnRiseState);
}

int WM_colrise_getObjectTypeId(void) {
    return 0;
}

void WM_colrise_free(void) {
}

void WM_colrise_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void WM_colrise_hitDetect(void) {
}

void WM_colrise_update(GameObject* obj) {
    const WmColumnRisePlacement* placement;
    WmColumnRiseState* state;
    s8 isMoving;
    f32 targetY;
    int contactIndex;

    placement = (const WmColumnRisePlacement*)obj->anim.placementData;
    state = obj->extra;
    state->raiseTimer--;
    if (state->raiseTimer < 0) {
        state->raiseTimer = 0;
    }
    if (obj->anim.hitboxTransformState->contactObjectCount > 0) {
        for (contactIndex = 0; contactIndex < obj->anim.hitboxTransformState->contactObjectCount; contactIndex++) {
            GameObject* rider = obj->anim.hitboxTransformState->contactObjects[contactIndex];
            if (rider->anim.localPosY - obj->anim.localPosY > 3.0f) {
                state->raiseTimer = 60;
            }
        }
    }
    isMoving = 0;
    if ((state->riseGameBit == -1 || mainGetBit(state->riseGameBit) != 0) && state->raiseTimer != 0) {
        targetY = 20.0f + (100.0f + placement->base.posY);
        if (obj->anim.localPosY > targetY) {
            obj->anim.localPosY = obj->anim.localPosY - 0.5f * timeDelta;
            if (obj->anim.localPosY > targetY) {
                obj->anim.localPosY = targetY;
            }
        } else {
            obj->anim.localPosY = 0.25f * timeDelta + obj->anim.localPosY;
            if (obj->anim.localPosY > targetY) {
                obj->anim.localPosY = targetY;
            } else {
                isMoving = 1;
            }
        }
    } else {
        obj->anim.localPosY = obj->anim.localPosY - 0.125f * timeDelta;
        if (obj->anim.localPosY < placement->base.posY) {
            obj->anim.localPosY = placement->base.posY;
        } else {
            isMoving = 1;
        }
    }
    if (isMoving != 0) {
        Sfx_PlayFromObject(obj, SFXTRIG_en_treedrum16_7d);
    } else {
        Sfx_StopObjectChannel(obj, 8);
    }
}

void WM_colrise_init(GameObject* obj, const WmColumnRisePlacement* placement) {
    WmColumnRiseState* state = obj->extra;

    obj->animEventCallback = WM_colrise_SeqFn;
    obj->anim.rotX = placement->rotationX * 256;
    state->riseGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->riseGameBit));
}

void WM_colrise_release(void) {
}

void WM_colrise_initialise(void) {
}

OBJECT_INIT_ADAPTER(gWM_colriseObjDescriptorInitAdapter, WM_colrise_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gWM_colriseObjDescriptorHitDetectAdapter, WM_colrise_hitDetect)
OBJECT_FREE_ADAPTER(gWM_colriseObjDescriptorFreeAdapter, WM_colrise_free)
OBJECT_TYPE_ID_ADAPTER(gWM_colriseObjDescriptorTypeIdAdapter, WM_colrise_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gWM_colriseObjDescriptorExtraSizeAdapter, WM_colrise_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gWM_colriseObjDescriptorAcquire, WM_colrise_initialise)

ObjectDescriptor gWM_colriseObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gWM_colriseObjDescriptorAcquire,
        WM_colrise_release,
    },
    0,
    gWM_colriseObjDescriptorInitAdapter,
    WM_colrise_update,
    gWM_colriseObjDescriptorHitDetectAdapter,
    WM_colrise_render,
    gWM_colriseObjDescriptorFreeAdapter,
    gWM_colriseObjDescriptorTypeIdAdapter,
    gWM_colriseObjDescriptorExtraSizeAdapter,
};
