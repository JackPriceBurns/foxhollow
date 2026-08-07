/*
 * Map-texture UV scroller. It finds matching shader materials in the
 * current map block and acquires or updates their hardware scroll slots.
 */
#include "dlls/objects/308_texscroll2.h"

#include "main/gamebits.h"
#include "main/lightmap_api.h"
#include "main/map_texscroll.h"
#include "main/model.h"
#include "main/object_render.h"
#include "main/rcp_dolphin.h"
#include "sys/objects/lifecycle.h"

#define TEXSCROLL2_TEXTURE_TABLE_ID 0x0E

#define TEXSCROLL2_GAME_BIT_GATED_IDENT_A 0x49B2F
#define TEXSCROLL2_GAME_BIT_GATED_IDENT_B 0x49B67

#define TEXSCROLL2_TEXTURE_FIXED_SHIFT 6
#define TEXSCROLL2_RENDER_SCALE        1.0f

void TexScroll2_setStepY(GameObject* obj, s8 stepY) {
    TexScroll2State* state = obj->extra;

    if (state->stepY == stepY) {
        return;
    }
    state->stepY = stepY;
    state->needsApply = 1;
}

void TexScroll2_applyMapTextureScroll(GameObject* obj, TexScroll2State* state) {
    Shader* shader;
    int* textureTable;
    Texture* texture;
    int materialIndex;
    int texWidthFixed, texHeightFixed;
    MapBlockData* block;
    int shaderIndex;
    TexScrollPlacement* placement;

    placement = (TexScrollPlacement*)obj->anim.placementData;
    block = mapGetBlock(objPosToMapBlockIdx(obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ));
    if (block == NULL) {
        state->needsApply = 1;
        return;
    }
    textureTable = getTablesBinEntry(TEXSCROLL2_TEXTURE_TABLE_ID);
    if (textureTable == NULL) {
        return;
    }
    texture = getLoadedTexture(-textureTable[(s32)placement->textureTableIndex]);
    if (texture == NULL) {
        return;
    }

    for (shaderIndex = 0; shaderIndex < (s32)block->shaderCount; shaderIndex++) {
        shader = mapBlockGetShader(block, shaderIndex);
        for (materialIndex = 0; materialIndex < (s32)shader->layerCount; materialIndex++) {
            if (shader->layers[materialIndex].texture == texture) {
                texWidthFixed = (s32)(u32)texture->width << TEXSCROLL2_TEXTURE_FIXED_SHIFT;
                texHeightFixed = (s32)(u32)texture->height << TEXSCROLL2_TEXTURE_FIXED_SHIFT;
                if (shader->layers[materialIndex].scrollMtx !=
                    MAP_TEXTURE_SCROLL_SLOT_UNALLOCATED) {
                    int ident = ((TexScrollPlacement*)obj->anim.placementData)->base.ident;
                    if (ident == TEXSCROLL2_GAME_BIT_GATED_IDENT_A || ident == TEXSCROLL2_GAME_BIT_GATED_IDENT_B) {
                        if (mainGetBit(state->gameBit) != 0) {
                            mapTextureScrollSetStep((s32)shader->layers[materialIndex].scrollMtx,
                                                    state->stepX, state->stepY, texWidthFixed, texHeightFixed,
                                                    state->secondaryStepX, state->secondaryStepY, texWidthFixed,
                                                    texHeightFixed);
                        }
                    } else {
                        mapTextureScrollSetStep((s32)shader->layers[materialIndex].scrollMtx,
                                                state->stepX, state->stepY, texWidthFixed, texHeightFixed,
                                                state->secondaryStepX, state->secondaryStepY, texWidthFixed,
                                                texHeightFixed);
                    }
                } else {
                    shader->layers[materialIndex].scrollMtx =
                        mapTextureScrollAcquire(state->stepX, state->stepY, texWidthFixed, texHeightFixed,
                                                state->secondaryStepX, state->secondaryStepY, texWidthFixed,
                                                texHeightFixed);
                }
            }
        }
    }
}

int TexScroll2_getExtraSize(void) {
    return sizeof(TexScroll2State);
}

int TexScroll2_getObjectTypeId(void) {
    return 0;
}

void TexScroll2_free(void) {
}

void TexScroll2_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    s32 visibility = visible;
    if (visibility != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, TEXSCROLL2_RENDER_SCALE);
    }
}

void TexScroll2_hitDetect(void) {
}

void TexScroll2_update(GameObject* obj) {
    TexScroll2State* state;
    MapBlockData* block;
    TexScrollPlacement* placement;
    int ident;

    state = obj->extra;
    block = mapGetBlock(objPosToMapBlockIdx(obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ));
    placement = (TexScrollPlacement*)obj->anim.placementData;
    ident = placement->base.ident;
    if (ident == TEXSCROLL2_GAME_BIT_GATED_IDENT_A || ident == TEXSCROLL2_GAME_BIT_GATED_IDENT_B) {
        if (block != NULL) {
            if (mainGetBit(state->gameBit) != state->previousGameBitValue && state->needsApply == 0) {
                TexScroll2_applyMapTextureScroll(obj, state);
                state->needsApply = 0;
            }
        }
    }
    state->previousGameBitValue = mainGetBit(state->gameBit);
    if (block == NULL) {
        state->needsApply = 1;
    } else {
        if (state->needsApply != 0) {
            TexScroll2_applyMapTextureScroll(obj, state);
            state->needsApply = 0;
        }
    }
}

void TexScroll2_init(GameObject* obj, TexScrollPlacement* placement, int loadFlags) {
    TexScroll2State* state = obj->extra;

    state->stepX = placement->stepX;
    state->stepY = placement->stepY;
    state->secondaryStepX = placement->secondaryStepX;
    state->secondaryStepY = placement->secondaryStepY;
    if (loadFlags == 0) {
        TexScroll2_applyMapTextureScroll(obj, state);
    }
    state->gameBit = placement->gameBit;
    state->previousGameBitValue = -1;
}

void TexScroll2_release(void) {
}

void TexScroll2_initialise(void) {
}

ObjectDescriptor11WithPadding gTexscroll2ObjDescriptor = {
    {
        0,
        0,
        0,
        OBJECT_DESCRIPTOR_FLAGS_11_SLOTS,
        (ObjectDescriptorCallback)TexScroll2_initialise,
        (ObjectDescriptorCallback)TexScroll2_release,
        0,
        (ObjectDescriptorCallback)TexScroll2_init,
        (ObjectDescriptorCallback)TexScroll2_update,
        (ObjectDescriptorCallback)TexScroll2_hitDetect,
        (ObjectDescriptorCallback)TexScroll2_render,
        (ObjectDescriptorCallback)TexScroll2_free,
        (ObjectDescriptorCallback)TexScroll2_getObjectTypeId,
        TexScroll2_getExtraSize,
        (ObjectDescriptorCallback)TexScroll2_setStepY,
    },
    0,
};
