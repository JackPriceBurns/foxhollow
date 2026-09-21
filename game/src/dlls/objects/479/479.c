/*
 * DLL 0x1DF (slot 479) - unidentified particle-emitting object.
 * The object applies placement rotation and scale, suppresses its texture
 * color, and periodically emits an effect while the player is nearby.
 */
#include "dlls/objects/479.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/objtexture.h"
#include "main/vecmath_distance.h"
#include "sys/objects.h"

#define DLL_1DF_PARTFX_ID 0x20D

int dll_1DF_getExtraSize(void) {
    return sizeof(Dll1DFState);
}

int dll_1DF_getObjectTypeId(void) {
    return 0;
}

void dll_1DF_free(void) {
}

void dll_1DF_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void dll_1DF_hitDetect(void) {
}

void dll_1DF_update(GameObject* obj) {
    Dll1DFState* state = obj->extra;

    ObjTextureRuntimeSlot* texture = objFindTexture(obj, 0, 0);
    if (texture != NULL) {
        if (obj->anim.romDefNo == 0xD1) {
            texture->colorR = 0.0f;
            texture->colorG = 0.0f;
            texture->colorB = 0.0f;
        } else {
            texture->colorR = 0.0f;
            texture->colorG = 0.0f;
            texture->colorB = 0.0f;
        }
    }

    GameObject* player = Obj_GetPlayerObject();
    if (vec3f_distanceSquared(&player->anim.worldPosX, &obj->anim.worldPosX) >= 90000.0f) {
        return;
    }

    state->spawnTimer -= timeDelta;
    if (state->spawnTimer - timeDelta < 0.0f) {
        (*gPartfxInterface)->spawnObject(obj, DLL_1DF_PARTFX_ID, NULL, 2, -1, NULL);
        state->spawnTimer = 12.0f;
    }
}

void dll_1DF_init(GameObject* obj, const Dll1DFPlacementView* placement) {
    obj->anim.rotZ = placement->rotationZByte << 8;
    obj->anim.rotY = placement->rotationYByte << 8;
    obj->anim.rotX = placement->rotationXByte << 8;

    if (placement->scaleByte != 0) {
        obj->anim.rootMotionScale = obj->anim.modelInstance->rootMotionScaleBase * (placement->scaleByte / 255.0f);
    }

    ((Dll1DFState*)obj->extra)->unknown10 = 0.01f;
    if (obj->anim.modelState != NULL) {
        obj->anim.modelState->flags |= OBJ_MODEL_STATE_UNREAD_0800 | OBJ_MODEL_STATE_UNREAD_0010;
    }
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void dll_1DF_release(void) {
}

void dll_1DF_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll1DFObjDescriptorInitAdapter, dll_1DF_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDll1DFObjDescriptorHitDetectAdapter, dll_1DF_hitDetect)
OBJECT_FREE_ADAPTER(gDll1DFObjDescriptorFreeAdapter, dll_1DF_free)
OBJECT_TYPE_ID_ADAPTER(gDll1DFObjDescriptorTypeIdAdapter, dll_1DF_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll1DFObjDescriptorExtraSizeAdapter, dll_1DF_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll1DFObjDescriptorAcquire, dll_1DF_initialise)

ObjectDescriptor gDll1DFObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll1DFObjDescriptorAcquire,
        dll_1DF_release,
    },
    0,
    gDll1DFObjDescriptorInitAdapter,
    dll_1DF_update,
    gDll1DFObjDescriptorHitDetectAdapter,
    dll_1DF_render,
    gDll1DFObjDescriptorFreeAdapter,
    gDll1DFObjDescriptorTypeIdAdapter,
    gDll1DFObjDescriptorExtraSizeAdapter,
};
