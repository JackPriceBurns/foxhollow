#include "dlls/objects/516_WM_Torch.h"

#include "game/objects/object.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/dll_0063_modgfx.h"
#include "main/dll/dll_0069_modgfx.h"
#include "main/dll/expgfx_interface.h"
#include "main/dll/modgfx_interface.h"
#include "main/resource.h"
#include "main/vecmath_distance.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

typedef enum WMTorchType {
    WM_TORCH_TYPE_DEFAULT = 0,
    WM_TORCH_TYPE_SPINNING = 2,
    WM_TORCH_TYPE_ALTERNATE_FLAME = 0x7F
} WMTorchType;

typedef struct WMTorchState {
    GameObject* linkedObject;
    f32 motionRate;
    u8 unknown0C[2];
    s16 colorIndex;
    u8 torchType;
    u8 unknown11[3];
} WMTorchState;

int wmtorch_getExtraSize(void) {
    return sizeof(WMTorchState);
}

int wmtorch_getObjectTypeId(void) {
    return 1;
}

void wmtorch_free(GameObject* obj, int mode) {
    WMTorchState* state = obj->extra;

    if (mode == 0 && state->linkedObject != NULL) {
        Obj_FreeObject(state->linkedObject);
    }
    (*gModgfxInterface)->detachSource(obj);
    (*gExpgfxInterface)->freeSource((uintptr_t)obj);
}

void wmtorch_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }
}

void wmtorch_hitDetect(void) {
}

void wmtorch_update(GameObject* obj) {
    WMTorchState* state = obj->extra;

    if (state->torchType == WM_TORCH_TYPE_SPINNING) {
        obj->anim.rotX += 0x32;
    }
    if (Vec_distance(&Obj_GetPlayerObject()->anim.worldPosX, &obj->anim.worldPosX) < 90.0f) {
        Sfx_PlayFromObject(obj, SFXTRIG_mushdizzylp12);
    } else {
        Sfx_StopObjectChannel(obj, 0x40);
    }
}

void wmtorch_init(GameObject* obj, const WMTorchPlacement* placement) {
    WMTorchState* state = obj->extra;
    s16 motionRate = ObjAnim_ReadPlacementS16(&obj->anim, &placement->motionRate);
    s16 colorIndex = ObjAnim_ReadPlacementS16(&obj->anim, &placement->colorIndex);
    f32 flameParams[5];

    state->motionRate = motionRate != 0 ? motionRate : 75.0f;
    state->colorIndex = colorIndex != 0 ? colorIndex : 0x8C;
    state->torchType = placement->torchType;
    flameParams[4] = -2.0f;
    if (state->torchType == WM_TORCH_TYPE_DEFAULT) {
        Dll69Interface** effectResource = Resource_Acquire(DLL_69_RESOURCE_ID, 1);

        obj->anim.rootMotionScale *= 0.5f;
        (*effectResource)->spawn(obj, 1, flameParams, 0x10004, -1, NULL);
        obj->anim.rootMotionScale *= 2.0f;
        Resource_Release(effectResource);
    } else if (state->torchType == WM_TORCH_TYPE_ALTERNATE_FLAME) {
        Dll69Interface** effectResource = Resource_Acquire(DLL_69_RESOURCE_ID, 1);

        obj->anim.rootMotionScale *= 0.5f;
        (*effectResource)->spawn(obj, 2, flameParams, 0x10004, -1, NULL);
        obj->anim.rootMotionScale *= 2.0f;
        Resource_Release(effectResource);
    } else {
        Dll63Interface** effectResource = Resource_Acquire(0x63, 1);

        obj->anim.rootMotionScale *= 0.5f;
        (*effectResource)->spawn(obj, 2, flameParams, 0x10004, -1, NULL);
        obj->anim.rootMotionScale *= 2.0f;
        Resource_Release(effectResource);
    }
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void wmtorch_release(void) {
}

void wmtorch_initialise(void) {
}

OBJECT_INIT_ADAPTER(gWM_TorchObjDescriptorInitAdapter, wmtorch_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gWM_TorchObjDescriptorHitDetectAdapter, wmtorch_hitDetect)
OBJECT_TYPE_ID_ADAPTER(gWM_TorchObjDescriptorTypeIdAdapter, wmtorch_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gWM_TorchObjDescriptorExtraSizeAdapter, wmtorch_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gWM_TorchObjDescriptorAcquire, wmtorch_initialise)

ObjectDescriptor gWM_TorchObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gWM_TorchObjDescriptorAcquire,
        wmtorch_release,
    },
    0,
    gWM_TorchObjDescriptorInitAdapter,
    wmtorch_update,
    gWM_TorchObjDescriptorHitDetectAdapter,
    wmtorch_render,
    wmtorch_free,
    gWM_TorchObjDescriptorTypeIdAdapter,
    gWM_TorchObjDescriptorExtraSizeAdapter,
};
