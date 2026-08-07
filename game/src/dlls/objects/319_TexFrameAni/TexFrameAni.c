/* Animates a map-block texture slot in response to a game bit. */
#include "dlls/objects/319_TexFrameAni.h"

#include "main/debug.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/lightmap_api.h"
#include "main/object_render.h"
#include "main/shader_api.h"
#include "main/shader_map_api.h"

ObjectDescriptor gTexFrameAnimatorObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)TexFrameAnimator_initialise,
    (ObjectDescriptorCallback)TexFrameAnimator_release,
    0,
    (ObjectDescriptorCallback)TexFrameAnimator_init,
    (ObjectDescriptorCallback)TexFrameAnimator_update,
    (ObjectDescriptorCallback)TexFrameAnimator_hitDetect,
    (ObjectDescriptorCallback)TexFrameAnimator_render,
    (ObjectDescriptorCallback)TexFrameAnimator_free,
    (ObjectDescriptorCallback)TexFrameAnimator_getObjectTypeId,
    TexFrameAnimator_getExtraSize,
};

char sTexFrameAnimDebugFormat[] = " TEXFRAMEANIM %i ";

int TexFrameAnimator_getExtraSize(void) {
    return sizeof(TexFrameAnimatorState);
}

int TexFrameAnimator_getObjectTypeId(void) {
    return 0;
}

void TexFrameAnimator_free(void) {
}

void TexFrameAnimator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible) {
    s32 isVisible = visible;

    if (isVisible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void TexFrameAnimator_hitDetect(void) {
}

void TexFrameAnimator_update(GameObject* obj) {
    TexFrameAnimatorState* state;
    TexFrameAnimatorPlacement* placement;
    MapBlockData* mapBlock;
    s16* textureOverrideIndex;
    MapTextureOverride* textureOverride;

    state = obj->extra;
    placement = (TexFrameAnimatorPlacement*)obj->anim.placementData;

    if ((state->active == 0) && (mainGetBit(placement->triggerGameBit) != 0) && (state->done == 0)) {
        state->active = 1;
        state->frame = 0;
    }

    if ((state->active != 0) && (state->textureSlot != 0)) {
        mapBlock = mapGetBlock(objPosToMapBlockIdx(obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ));
        if (mapBlock == NULL || !(mapBlock->flags4 & MAP_BLOCK_FLAG_LOADED)) {
            return;
        }
        textureOverrideIndex = mapBlockFindTextureOverrideIndex(mapBlock, state->textureSlot);
        if (textureOverrideIndex != NULL) {
            textureOverride = mapTextureOverrideGetEntry(*textureOverrideIndex);
            state->frame += state->speed * framesThisStep;
            logPrintf(sTexFrameAnimDebugFormat, state->frame);
            if (state->frame < 0) {
                state->frame = 0;
            } else if (state->frame > state->endFrame) {
                if (placement->completionGameBit != -1) {
                    mainSetBits(placement->completionGameBit, 1);
                    state->active = 0;
                    state->done = 1;
                    state->frame = state->endFrame;
                } else {
                    state->frame = state->wrapFrame;
                }
            }
            textureOverride->frame = state->frame;
        }
    }
}

void TexFrameAnimator_init(GameObject* obj, TexFrameAnimatorPlacement* placement) {
    TexFrameAnimatorState* state;
    u8 completionBitValue;

    state = obj->extra;
    state->textureSlot = placement->textureSlot;
    state->endFrame = placement->endFrame << 8;
    state->speed = (u8)placement->speed;
    state->wrapFrame = placement->wrapFrame << 8;
    completionBitValue = mainGetBit(placement->completionGameBit);
    if ((state->done = completionBitValue) != 0) {
        state->frame = state->endFrame;
        state->active = 1;
    }
    obj->objectFlags = (u16)(obj->objectFlags | OBJECT_OBJFLAG_HITDETECT_DISABLED);
    obj->objectFlags = (u16)(obj->objectFlags | OBJECT_OBJFLAG_HIDDEN);
}

void TexFrameAnimator_release(void) {
}

void TexFrameAnimator_initialise(void) {
}
