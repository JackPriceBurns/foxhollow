/* Animates a map-block texture slot in response to a game bit. */
#include "dlls/objects/319_TexFrameAni.h"

#include "main/debug.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/lightmap_api.h"
#include "main/object_render.h"
#include "main/shader_api.h"
#include "main/shader_map_api.h"

char sTexFrameAnimDebugFormat[] = " TEXFRAMEANIM %i ";

typedef enum TexFrameAnimatorFlags {
    TEX_FRAME_ANIMATOR_FLAG_ACTIVE = 1 << 5,
    TEX_FRAME_ANIMATOR_FLAG_DONE = 1 << 6,
} TexFrameAnimatorFlags;

typedef struct TexFrameAnimatorState {
    int textureSlot;
    u8 speed;
    u8 pad05[3];
    int endFrame;
    int wrapFrame;
    int frame;
    u8 flags;
    u8 pad15[3];
} TexFrameAnimatorState;

STATIC_ASSERT(sizeof(TexFrameAnimatorState) == 0x18);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, textureSlot) == 0x00);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, speed) == 0x04);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, endFrame) == 0x08);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, wrapFrame) == 0x0C);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, frame) == 0x10);
STATIC_ASSERT(offsetof(TexFrameAnimatorState, flags) == 0x14);

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
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void TexFrameAnimator_hitDetect(void) {
}

void TexFrameAnimator_update(GameObject* obj) {
    TexFrameAnimatorState* state = obj->extra;
    const TexFrameAnimatorPlacement* placement =
        (const TexFrameAnimatorPlacement*)obj->anim.placementData;

    if ((state->flags & (TEX_FRAME_ANIMATOR_FLAG_ACTIVE | TEX_FRAME_ANIMATOR_FLAG_DONE)) == 0 &&
        mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->triggerGameBit)) != 0) {
        state->flags |= TEX_FRAME_ANIMATOR_FLAG_ACTIVE;
        state->frame = 0;
    }

    if ((state->flags & TEX_FRAME_ANIMATOR_FLAG_ACTIVE) != 0 && state->textureSlot != 0) {
        MapBlockData* mapBlock =
            mapGetBlock(objPosToMapBlockIdx(obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ));
        if (mapBlock == NULL || !(mapBlock->flags4 & MAP_BLOCK_FLAG_LOADED)) {
            return;
        }
        s16* textureOverrideIndex = mapBlockFindTextureOverrideIndex(mapBlock, state->textureSlot);
        if (textureOverrideIndex != NULL) {
            MapTextureOverride* textureOverride = mapTextureOverrideGetEntry(*textureOverrideIndex);
            state->frame += state->speed * framesThisStep;
            logPrintf(sTexFrameAnimDebugFormat, state->frame);
            if (state->frame < 0) {
                state->frame = 0;
            } else if (state->frame > state->endFrame) {
                s16 completionGameBit =
                    ObjAnim_ReadPlacementS16(&obj->anim, &placement->completionGameBit);
                if (completionGameBit != -1) {
                    mainSetBits(completionGameBit, 1);
                    state->flags &= (u8)~TEX_FRAME_ANIMATOR_FLAG_ACTIVE;
                    state->flags |= TEX_FRAME_ANIMATOR_FLAG_DONE;
                    state->frame = state->endFrame;
                } else {
                    state->frame = state->wrapFrame;
                }
            }
            textureOverride->frame = state->frame;
        }
    }
}

void TexFrameAnimator_init(GameObject* obj, const TexFrameAnimatorPlacement* placement) {
    TexFrameAnimatorState* state = obj->extra;
    state->textureSlot = placement->textureSlot;
    state->endFrame = ObjAnim_ReadPlacementS16(&obj->anim, &placement->endFrame) * 256;
    state->speed = (u8)ObjAnim_ReadPlacementS16(&obj->anim, &placement->speed);
    state->wrapFrame = (int)placement->wrapFrame * 256;
    state->flags = 0;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->completionGameBit)) != 0) {
        state->flags = TEX_FRAME_ANIMATOR_FLAG_DONE | TEX_FRAME_ANIMATOR_FLAG_ACTIVE;
        state->frame = state->endFrame;
    }
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED | OBJECT_OBJFLAG_HIDDEN;
}

void TexFrameAnimator_release(void) {
}

void TexFrameAnimator_initialise(void) {
}

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
