#include "dlls/objects/378_SpiritPrize.h"

#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/objfx_api.h"
#include "main/frame_timing.h"
#include "main/lightmap_api.h"
#include "main/model_light.h"
#include "main/obj_list.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"


#define SPIRIT_PRIZE_DISABLED_MAP_ID       0x4CA62
#define SPIRIT_PRIZE_BOUND_LIGHT_SEQ_ID    0x1D9
#define SPIRIT_PRIZE_SEQUENCE_CLASS_ID     0x10
#define SPIRIT_PRIZE_OBJECT_TYPE_ID        0x8
#define SPIRIT_PRIZE_LIGHT_PARTICLE_TYPE   7
#define SPIRIT_PRIZE_EVENT_BIND_LIGHT      1
#define SPIRIT_PRIZE_EVENT_DETACH_LIGHT    2
#define SPIRIT_PRIZE_AMBIENT_SFX_MIN_DELAY 0xB4
#define SPIRIT_PRIZE_AMBIENT_SFX_MAX_DELAY 0xF0
#define SPIRIT_PRIZE_AMBIENT_SFX_MAP_CELL  0xB

int spiritPrize_getExtraSize(void) {
    return sizeof(SpiritPrizeState);
}

int spiritPrize_getObjectTypeId(void) {
    return SPIRIT_PRIZE_OBJECT_TYPE_ID;
}

void spiritPrize_free(GameObject* obj) {
    SpiritPrizeState* state;
    ModelLightStruct* light;

    state = obj->extra;
    light = state->light;
    if (light != NULL) {
        ModelLightStruct_free(light);
        state->light = NULL;
        state->useDetachedLight = 0;
    }
    (*gObjectTriggerInterface)->freeState((u8*)&state->sequence);
}

void spiritPrize_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    SpiritPrizeState* state;
    s32 isVisible;

    state = obj->extra;
    isVisible = visible;
    if (isVisible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
        if (state->useDetachedLight != 0) {
            objDoParticleFx(obj, 1.0f, SPIRIT_PRIZE_LIGHT_PARTICLE_TYPE, 1.0f, state->light);
        } else {
            objDoParticleFx(obj, 1.0f, SPIRIT_PRIZE_LIGHT_PARTICLE_TYPE, 1.0f, NULL);
        }
    }
}

void spiritPrize_hitDetect(void) {
}

void spiritPrize_update(GameObject* obj) {
    const SpiritPrizePlacement* placement;
    SpiritPrizeState* state;
    GameObject* candidateObj;
    int objectCount;
    int objectIndex;
    GameObject** objects;
    int eventIndex;

    placement = (const SpiritPrizePlacement*)obj->anim.placementData;
    state = obj->extra;
    if (placement == NULL) {
        return;
    }
    if (placement->animDataIndex == -1) {
        return;
    }
    if (placement->base.ident == SPIRIT_PRIZE_DISABLED_MAP_ID) {
        return;
    }

    for (eventIndex = 0; eventIndex < state->sequence.eventCount; eventIndex++) {
        switch (state->sequence.eventIds[eventIndex]) {
        case SPIRIT_PRIZE_EVENT_BIND_LIGHT:
            state->useDetachedLight = 0;
            break;
        case SPIRIT_PRIZE_EVENT_DETACH_LIGHT:
            state->useDetachedLight = 1;
            break;
        }
    }

    objectIndex = (*gObjectTriggerInterface)->update((u8*)obj, (f32)(u32)framesThisStepUnclamped);
    if (objectIndex != 0 && obj->seqIndex == -2) {
        GameObject* matchingObj;
        int slot;
        int scanLimit[1];
        int slotArg[1];
        int duplicateCount[1];

        slot = state->sequence.slot;
        matchingObj = NULL;
        objects = ObjList_GetObjects(&objectIndex, &objectCount);
        scanLimit[0] = 0;
        slotArg[0] = 0;
        duplicateCount[0] = 0;
        objectIndex = duplicateCount[0];
        slotArg[0] = (u32)slot;
        scanLimit[0] = objectCount;
        while (objectIndex < scanLimit[0]) {
            candidateObj = objects[objectIndex];
            if (candidateObj->seqIndex == slot) {
                matchingObj = candidateObj;
            }
            if (candidateObj->seqIndex == -2 && candidateObj->anim.classId == SPIRIT_PRIZE_SEQUENCE_CLASS_ID &&
                slotArg[0] == ((SpiritPrizeState*)candidateObj->extra)->sequence.slot) {
                duplicateCount[0]++;
            }
            objectIndex++;
        }
        if (duplicateCount[0] <= 1 && matchingObj != NULL && matchingObj->seqIndex != -1) {
            matchingObj->seqIndex = -1;
            (*gObjectTriggerInterface)->endSequence(slotArg[0]);
        }
        obj->seqIndex = -1;
        Obj_FreeObject(obj);
    }

    state->ambientSfxTimer -= timeDelta;
    if (state->ambientSfxTimer < 0.0f) {
        GameObject* player;

        player = Obj_GetPlayerObject();
        state->ambientSfxTimer =
            (f32)(s32)randomGetRange(SPIRIT_PRIZE_AMBIENT_SFX_MIN_DELAY, SPIRIT_PRIZE_AMBIENT_SFX_MAX_DELAY);
        if (obj->anim.mapEventSlot == -1 &&
            (player == NULL ||
             coordsToMapCell(player->anim.localPosX, player->anim.localPosZ) == SPIRIT_PRIZE_AMBIENT_SFX_MAP_CELL)) {
            Sfx_PlayFromObject(obj, SFXTRIG_pda);
        }
    }
}

void spiritPrize_init(GameObject* obj, const SpiritPrizePlacement* placement) {
    SpiritPrizeState* state;
    int loadedAnimDataIndexPlusOne;

    state = obj->extra;
    if (placement->base.ident == SPIRIT_PRIZE_DISABLED_MAP_ID) {
        return;
    }
    state->sequence.gameBit = placement->sequenceGameBit;
    state->sequence.flags = -1;
    state->sequence.posOffsetDecay = 1.0f / (1.0f + (f32)(u32)placement->positionDamping);
    state->sequence.curveId = -1;
    loadedAnimDataIndexPlusOne = obj->userData1;
    if (loadedAnimDataIndexPlusOne == 0 && placement->animDataIndex != 1) {
        (*gObjectTriggerInterface)->loadAnimData((u8*)&state->sequence, (u8*)placement);
        obj->userData1 = placement->animDataIndex + 1;
    } else if (loadedAnimDataIndexPlusOne != 0 && placement->animDataIndex != loadedAnimDataIndexPlusOne - 1) {
        (*gObjectTriggerInterface)->freeState((u8*)&state->sequence);
        if (placement->animDataIndex != -1) {
            (*gObjectTriggerInterface)->loadAnimData((u8*)&state->sequence, (u8*)placement);
        }
        obj->userData1 = placement->animDataIndex + 1;
    }
    if (obj->anim.romDefNo != SPIRIT_PRIZE_BOUND_LIGHT_SEQ_ID) {
        state->useDetachedLight = 1;
    }
    if (state->light == NULL) {
        state->light = objCreateLight(state->useDetachedLight != 0 ? NULL : obj, 1);
        if (state->light != NULL) {
            modelLightStruct_setLightKind(state->light, MODEL_LIGHT_KIND_POINT);
            modelLightStruct_setDiffuseColor(state->light, 0x96, 0x32, 0xFF, 0xFF);
            modelLightStruct_setDistanceAttenuation(state->light, 80.0f, 100.0f);
        }
    }
    obj->anim.alpha = 0;
    obj->anim.renderAlpha = 0;
    state->ambientSfxTimer =
        (f32)(s32)randomGetRange(SPIRIT_PRIZE_AMBIENT_SFX_MIN_DELAY, SPIRIT_PRIZE_AMBIENT_SFX_MAX_DELAY);
}

void spiritPrize_release(void) {
}

void spiritPrize_initialise(void) {
}

ObjectDescriptor gSpiritPrizeObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)spiritPrize_initialise,
    (ObjectDescriptorCallback)spiritPrize_release,
    0,
    (ObjectDescriptorCallback)spiritPrize_init,
    (ObjectDescriptorCallback)spiritPrize_update,
    (ObjectDescriptorCallback)spiritPrize_hitDetect,
    (ObjectDescriptorCallback)spiritPrize_render,
    (ObjectDescriptorCallback)spiritPrize_free,
    (ObjectDescriptorCallback)spiritPrize_getObjectTypeId,
    spiritPrize_getExtraSize,
};
