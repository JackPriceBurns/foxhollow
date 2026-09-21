/*
 * Shared texture animation for the DFSH_Door2S, DFSH_Door3S, and
 * DFSH_Door4S object definitions.
 */

#include "dlls/objects/375.h"

#include "dolphin/math.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/object_render.h"
#include "main/objtexture.h"

#define DLL_177_TEXTURE_VALUE_MAXIMUM   256.0f
#define DLL_177_TEXTURE_PULSE_AMPLITUDE 50.0f
#define DLL_177_UNIT_VALUE              1.0f
#define DLL_177_PI                      3.1415927f
#define DLL_177_HALF_CYCLE_UNITS        32768.0f

int dll_177_updateTextureAnimation(GameObject* obj) {
    ObjTextureRuntimeSlot* texture;
    Dll177State* state;
    const Dll177Placement* placement;
    int alpha;
    u32 phaseStep;
    f32 phase;

    state = obj->extra;
    placement = (const Dll177Placement*)obj->anim.placementData;
    switch (state->textureState) {
    case DLL_177_TEXTURE_STATE_WAIT_FOR_GAME_BIT:
        if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBit)) != 0) {
            state->textureState = DLL_177_TEXTURE_STATE_FADE_IN;
        }
        break;
    case DLL_177_TEXTURE_STATE_FADE_IN:
        texture = objFindTexture(obj, 0, 0);
        if (texture != NULL) {
            alpha = texture->textureId + framesThisStep * 0x10;
            if (alpha > 0x100) {
                alpha = 0x100;
                state->textureState = DLL_177_TEXTURE_STATE_PULSE;
            }
            texture->textureId = alpha;
        }
        break;
    case DLL_177_TEXTURE_STATE_PULSE:
    default:
        texture = objFindTexture(obj, 0, 0);
        if (texture != NULL) {
            phaseStep = (state->pulsePhase + framesThisStep * 800) & 0xFFFF;
            state->pulsePhase = phaseStep;
            phase = (DLL_177_PI * (f32)(u32)state->pulsePhase) / DLL_177_HALF_CYCLE_UNITS;
            texture->textureId = (s32)(DLL_177_TEXTURE_VALUE_MAXIMUM -
                                       DLL_177_TEXTURE_PULSE_AMPLITUDE * (DLL_177_UNIT_VALUE - mathCosf(phase)));
        }
        break;
    }
    return 0;
}

int dll_177_getExtraSize(void) {
    return sizeof(Dll177State);
}

int dll_177_getObjectTypeId(void) {
    return 0;
}

void dll_177_free(void) {
}

void dll_177_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, DLL_177_UNIT_VALUE);
    }
}

void dll_177_hitDetect(void) {
}

void dll_177_update(void) {
}

void dll_177_init(GameObject* obj, const Dll177Placement* placement) {
    Dll177State* state;
    ObjTextureRuntimeSlot* texture;

    state = obj->extra;
    obj->animEventCallback = dll_177_updateTextureAnimation;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->gameBit))) != 0) {
        state->textureState = DLL_177_TEXTURE_STATE_PULSE;
    } else {
        state->textureState = DLL_177_TEXTURE_STATE_WAIT_FOR_GAME_BIT;
    }
    texture = objFindTexture(obj, 0, 0);
    if (texture != NULL) {
        if (state->textureState == DLL_177_TEXTURE_STATE_PULSE) {
            texture->textureId = 1;
        } else {
            texture->textureId = 0;
        }
    }
    state->pulsePhase = 0;
}

void dll_177_release(void) {
}

void dll_177_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll177ObjDescriptorInitAdapter, dll_177_init, obj, placement)
OBJECT_UPDATE_ADAPTER(gDll177ObjDescriptorUpdateAdapter, dll_177_update)
OBJECT_HIT_DETECT_ADAPTER(gDll177ObjDescriptorHitDetectAdapter, dll_177_hitDetect)
OBJECT_FREE_ADAPTER(gDll177ObjDescriptorFreeAdapter, dll_177_free)
OBJECT_TYPE_ID_ADAPTER(gDll177ObjDescriptorTypeIdAdapter, dll_177_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll177ObjDescriptorExtraSizeAdapter, dll_177_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll177ObjDescriptorAcquire, dll_177_initialise)

ObjectDescriptor gDll177ObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll177ObjDescriptorAcquire,
        dll_177_release,
    },
    0,
    gDll177ObjDescriptorInitAdapter,
    gDll177ObjDescriptorUpdateAdapter,
    gDll177ObjDescriptorHitDetectAdapter,
    dll_177_render,
    gDll177ObjDescriptorFreeAdapter,
    gDll177ObjDescriptorTypeIdAdapter,
    gDll177ObjDescriptorExtraSizeAdapter,
};
