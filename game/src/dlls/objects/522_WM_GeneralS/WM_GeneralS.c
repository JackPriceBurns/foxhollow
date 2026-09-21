/*
 * WM_GeneralS (DLL 0x020A) - General Scales at Krazoa Palace, the
 * cutscene actor driven entirely by sequence events (his appearance in
 * the final spirit ceremony).
 *
 * The sequence callback fades the model in or out through state->fadeAlpha,
 * spawns impact particles and SFX on the slam events, and attaches or detaches
 * his scalessword child on demand. He starts hidden, so the hidden phase skips
 * rendering.
 */
#include "dlls/objects/522_WM_GeneralS.h"

#include "dlls/objects/298_CFCrate.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/obj_link.h"
#include "main/objseq.h"
#include "main/object_render.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

typedef enum WmGeneralScalesSequenceEvent {
    WM_GENERAL_SCALES_SEQUENCE_EVENT_HIDE = 1,
    WM_GENERAL_SCALES_SEQUENCE_EVENT_SLAM_A = 2,
    WM_GENERAL_SCALES_SEQUENCE_EVENT_SLAM_B = 3,
    WM_GENERAL_SCALES_SEQUENCE_EVENT_IDLE = 4,
    WM_GENERAL_SCALES_SEQUENCE_EVENT_DRAW_SWORD = 5,
    WM_GENERAL_SCALES_SEQUENCE_EVENT_SHEATHE_SWORD = 6,
    WM_GENERAL_SCALES_SEQUENCE_EVENT_BEGIN_FADE = 7,
    WM_GENERAL_SCALES_SEQUENCE_EVENT_END_FADE = 8,
} WmGeneralScalesSequenceEvent;

#define WM_GENERAL_SCALES_PARTFX_ID            0x556
#define WM_GENERAL_SCALES_PARTFX_MODE          2
#define WM_GENERAL_SCALES_PARTFX_MODEL_NONE    -1
#define WM_GENERAL_SCALES_PARTFX_OUTPUT_SIZE   16
#define WM_GENERAL_SCALES_SWORD_SETUP_FLAGS    5
#define WM_GENERAL_SCALES_SWORD_MAP_LAYER_NONE -1
#define WM_GENERAL_SCALES_SWORD_OBJECT_NONE    -1
#define WM_GENERAL_SCALES_SWORD_COLOR_RED      0x20
#define WM_GENERAL_SCALES_SWORD_COLOR_GREEN    0x04
#define WM_GENERAL_SCALES_SWORD_COLOR_ALPHA    0xFF
#define WM_GENERAL_SCALES_SWORD_SCALE          1.1f
#define WM_GENERAL_SCALES_OBJECT_TYPE_ID       0x9
#define WM_GENERAL_SCALES_MAX_ALPHA            0xFF
#define WM_GENERAL_SCALES_FADE_START_ALPHA     1
#define WM_GENERAL_SCALES_SLAM_A_VALUE         0.0f
#define WM_GENERAL_SCALES_SLAM_B_VALUE         800.0f
#define WM_GENERAL_SCALES_RENDER_SCALE         1.0f

/*
 * Retail symbol order places this exact descriptor before the switch table
 * generated for WM_GeneralScales_sequenceCallback().
 */

int WM_GeneralScales_sequenceCallback(GameObject* obj, int unusedArg2, ObjSeqState* animUpdate) {
    WmGeneralScalesState* state;
    int eventIndex;
    u8 partfxOutput[WM_GENERAL_SCALES_PARTFX_OUTPUT_SIZE];

    state = obj->extra;
    if (state->fadeAlpha != 0) {
        int alpha = state->fadeAlpha + framesThisStep;
        if (alpha < 0) {
            alpha = 0;
        } else if (alpha > WM_GENERAL_SCALES_MAX_ALPHA) {
            alpha = WM_GENERAL_SCALES_MAX_ALPHA;
        }
        state->fadeAlpha = alpha;
        Obj_SetModelRenderOpAlpha(obj, (u8)alpha);
    } else {
        Obj_SetModelRenderOpAlpha(obj, 0);
    }

    for (eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++) {
        switch (animUpdate->eventIds[eventIndex]) {
        case WM_GENERAL_SCALES_SEQUENCE_EVENT_HIDE:
            state->phase = WM_GENERAL_SCALES_PHASE_HIDDEN;
            break;
        case WM_GENERAL_SCALES_SEQUENCE_EVENT_SLAM_A:
            state->phase = WM_GENERAL_SCALES_PHASE_SLAM_A;
            (*gPartfxInterface)
                ->spawnObject(obj, WM_GENERAL_SCALES_PARTFX_ID, NULL, WM_GENERAL_SCALES_PARTFX_MODE,
                              WM_GENERAL_SCALES_PARTFX_MODEL_NONE, partfxOutput);
            Sfx_PlayFromObject(obj, SFXTRIG_id_7b);
            Sfx_PlayFromObject(obj, SFXTRIG_id_7c);
            state->unknown00 = WM_GENERAL_SCALES_SLAM_A_VALUE;
            break;
        case WM_GENERAL_SCALES_SEQUENCE_EVENT_SLAM_B:
            state->phase = WM_GENERAL_SCALES_PHASE_SLAM_B;
            (*gPartfxInterface)
                ->spawnObject(obj, WM_GENERAL_SCALES_PARTFX_ID, NULL, WM_GENERAL_SCALES_PARTFX_MODE,
                              WM_GENERAL_SCALES_PARTFX_MODEL_NONE, NULL);
            Sfx_PlayFromObject(obj, SFXTRIG_id_7b);
            Sfx_PlayFromObject(obj, SFXTRIG_id_7c);
            state->unknown00 = WM_GENERAL_SCALES_SLAM_B_VALUE;
            break;
        case WM_GENERAL_SCALES_SEQUENCE_EVENT_IDLE:
            state->phase = WM_GENERAL_SCALES_PHASE_IDLE;
            break;
        case WM_GENERAL_SCALES_SEQUENCE_EVENT_DRAW_SWORD:
            if (obj->childObjs[0] == NULL && Obj_IsLoadingLocked() != 0) {
                CFCratePlacement* setup =
                    (CFCratePlacement*)Obj_AllocObjectSetup(sizeof(CFCratePlacement), CFCRATE_OBJ_SCALESSWORD);
                setup->base.posX = obj->anim.localPosX;
                setup->base.posY = obj->anim.localPosY;
                setup->base.posZ = obj->anim.localPosZ;
                setup->base.color[0] = WM_GENERAL_SCALES_SWORD_COLOR_RED;
                setup->base.color[1] = WM_GENERAL_SCALES_SWORD_COLOR_GREEN;
                setup->base.color[3] = WM_GENERAL_SCALES_SWORD_COLOR_ALPHA;
                ObjLink_AttachChild(obj,
                                    objSetupObject(&setup->base, WM_GENERAL_SCALES_SWORD_SETUP_FLAGS,
                                                   WM_GENERAL_SCALES_SWORD_MAP_LAYER_NONE,
                                                   WM_GENERAL_SCALES_SWORD_OBJECT_NONE, NULL),
                                    0);
                ((GameObject*)obj->childObjs[0])->anim.rootMotionScale *= WM_GENERAL_SCALES_SWORD_SCALE;
            }
            break;
        case WM_GENERAL_SCALES_SEQUENCE_EVENT_SHEATHE_SWORD: {
            GameObject* child = obj->childObjs[0];
            if (child != NULL) {
                ObjLink_DetachChild(obj, child);
            }
            break;
        }
        case WM_GENERAL_SCALES_SEQUENCE_EVENT_BEGIN_FADE: {
            ObjDef* model = obj->anim.modelInstance;
            model->renderFlags |= OBJDEF_RENDERFLAG_DEFERRED_RENDER;
            state->fadeAlpha = WM_GENERAL_SCALES_FADE_START_ALPHA;
            break;
        }
        case WM_GENERAL_SCALES_SEQUENCE_EVENT_END_FADE: {
            ObjDef* model = obj->anim.modelInstance;
            model->renderFlags &= ~OBJDEF_RENDERFLAG_DEFERRED_RENDER;
            Obj_SetModelRenderOpAlpha(obj, 0);
            state->fadeAlpha = 0;
            break;
        }
        }
        animUpdate->eventIds[eventIndex] = 0;
    }
    return 0;
}

int WM_GeneralScales_getExtraSize(void) {
    return sizeof(WmGeneralScalesState);
}

int WM_GeneralScales_getObjectTypeId(void) {
    return WM_GENERAL_SCALES_OBJECT_TYPE_ID;
}

void WM_GeneralScales_free(GameObject* obj) {
    GameObject* child = obj->childObjs[0];
    if (child != NULL) {
        ObjLink_DetachChild(obj, child);
    }
}

void WM_GeneralScales_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible) {
    WmGeneralScalesState* state = obj->extra;
    if (state->phase == WM_GENERAL_SCALES_PHASE_HIDDEN) {
        return;
    }
    if (visible == 0) {
        return;
    }
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, WM_GENERAL_SCALES_RENDER_SCALE);
}

void WM_GeneralScales_hitDetect(void) {
}

void WM_GeneralScales_update(void) {
}

void WM_GeneralScales_init(GameObject* obj) {
    WmGeneralScalesState* state = obj->extra;
    obj->animEventCallback = WM_GeneralScales_sequenceCallback;
    state->unknown00 = WM_GENERAL_SCALES_SLAM_A_VALUE;
    state->phase = WM_GENERAL_SCALES_PHASE_HIDDEN;
    obj->childObjs[0] = NULL;
}

void WM_GeneralScales_release(void) {
}

void WM_GeneralScales_initialise(void) {
}

OBJECT_INIT_ADAPTER(gWM_GeneralScalesObjDescriptorInitAdapter, WM_GeneralScales_init, obj)
OBJECT_UPDATE_ADAPTER(gWM_GeneralScalesObjDescriptorUpdateAdapter, WM_GeneralScales_update)
OBJECT_HIT_DETECT_ADAPTER(gWM_GeneralScalesObjDescriptorHitDetectAdapter, WM_GeneralScales_hitDetect)
OBJECT_FREE_ADAPTER(gWM_GeneralScalesObjDescriptorFreeAdapter, WM_GeneralScales_free, obj)
OBJECT_TYPE_ID_ADAPTER(gWM_GeneralScalesObjDescriptorTypeIdAdapter, WM_GeneralScales_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gWM_GeneralScalesObjDescriptorExtraSizeAdapter, WM_GeneralScales_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gWM_GeneralScalesObjDescriptorAcquire, WM_GeneralScales_initialise)

ObjectDescriptor gWM_GeneralScalesObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gWM_GeneralScalesObjDescriptorAcquire,
        WM_GeneralScales_release,
    },
    0,
    gWM_GeneralScalesObjDescriptorInitAdapter,
    gWM_GeneralScalesObjDescriptorUpdateAdapter,
    gWM_GeneralScalesObjDescriptorHitDetectAdapter,
    WM_GeneralScales_render,
    gWM_GeneralScalesObjDescriptorFreeAdapter,
    gWM_GeneralScalesObjDescriptorTypeIdAdapter,
    gWM_GeneralScalesObjDescriptorExtraSizeAdapter,
};
