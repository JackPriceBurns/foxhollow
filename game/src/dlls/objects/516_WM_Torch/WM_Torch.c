/*
 * WM_Torch (DLL 0x0204) - the lightable torch at Krazoa Palace.
 *
 * Initialization attaches the flame effect for the placement's torch type
 * (two variants from resource 0x69, the third from 0x63) and scales the
 * model. The update spins type-2 torches and runs a proximity sound loop
 * around the player; cleanup releases the flame and optional linked object.
 */
#include "dlls/objects/516_WM_Torch.h"

#include "game/objects/object.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_stop_channel_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/dll_0063_modgfx.h"
#include "main/dll/dll_0069_modgfx.h"
#include "main/dll/expgfx_interface.h"
#include "main/dll/modgfx_interface.h"
#include "main/resource.h"
#include "main/vecmath_distance_api.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

#define WM_TORCH_OBJECT_TYPE_ID       1
#define WM_TORCH_SPINNING_TYPE        2
#define WM_TORCH_ROTATION_STEP        0x32
#define WM_TORCH_SOUND_RADIUS         90.0f
#define WM_TORCH_SOUND_CHANNEL        0x40
#define WM_TORCH_DEFAULT_MOTION_RATE  75.0f
#define WM_TORCH_DEFAULT_COLOR_INDEX  0x8C
#define WM_TORCH_ALTERNATE_FLAME_TYPE 0x7F

int wmtorch_getExtraSize(void) {
    return sizeof(WMTorchState);
}

int wmtorch_getObjectTypeId(void) {
    return WM_TORCH_OBJECT_TYPE_ID;
}

void wmtorch_free(GameObject* obj, int mode) {
    WMTorchState* state = obj->extra;

    if (mode == 0 && state->linkedObject != NULL) {
        Obj_FreeObject(state->linkedObject);
    }
    (*gModgfxInterface)->detachSource(obj);
    (*gExpgfxInterface)->freeSource((u32)obj);
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

    if (state->torchType == WM_TORCH_SPINNING_TYPE) {
        obj->anim.rotX += WM_TORCH_ROTATION_STEP;
    }
    if (Vec_distance(&Obj_GetPlayerObject()->anim.worldPosX, &obj->anim.worldPosX) < WM_TORCH_SOUND_RADIUS) {
        Sfx_PlayFromObject(obj, SFXTRIG_mushdizzylp12);
    } else {
        Sfx_StopObjectChannel(obj, WM_TORCH_SOUND_CHANNEL);
    }
}

void wmtorch_init(GameObject* obj, const WMTorchPlacementView* placement) {
    WMTorchState* state;
    void* effectResource;
    f32 flameParams[5]; /* flame params; only [4] is set, the rest raw on purpose */

    state = obj->extra;
    if (placement->motionRate != 0) {
        state->motionRate = (f32)(s32)placement->motionRate;
    } else {
        state->motionRate = WM_TORCH_DEFAULT_MOTION_RATE;
    }
    if (placement->colorIndex != 0) {
        state->colorIndex = placement->colorIndex;
    } else {
        state->colorIndex = WM_TORCH_DEFAULT_COLOR_INDEX;
    }
    state->torchType = placement->torchType;
    flameParams[4] = -2.0f;
    if (state->torchType == 0) {
        effectResource = Resource_Acquire(0x69, 1);
        obj->anim.rootMotionScale *= 0.5f;
        (*(Dll69Interface**)effectResource)->spawn(obj, 1, flameParams, 0x10004, -1, NULL);
    } else if (state->torchType == WM_TORCH_ALTERNATE_FLAME_TYPE) {
        effectResource = Resource_Acquire(0x69, 1);
        obj->anim.rootMotionScale *= 0.5f;
        (*(Dll69Interface**)effectResource)->spawn(obj, 2, flameParams, 0x10004, -1, NULL);
    } else {
        effectResource = Resource_Acquire(0x63, 1);
        obj->anim.rootMotionScale *= 0.5f;
        (*(Dll63Interface**)effectResource)->spawn(obj, 2, flameParams, 0x10004, -1, NULL);
    }
    obj->anim.rootMotionScale *= 2.0f;
    Resource_Release(effectResource);
    obj->objectFlags = (u16)(obj->objectFlags | OBJECT_OBJFLAG_HITDETECT_DISABLED);
}

void wmtorch_release(void) {
}

void wmtorch_initialise(void) {
}

ObjectDescriptor gWM_TorchObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    wmtorch_initialise,
    wmtorch_release,
    0,
    (ObjectDescriptorCallback)wmtorch_init,
    (ObjectDescriptorCallback)wmtorch_update,
    wmtorch_hitDetect,
    (ObjectDescriptorCallback)wmtorch_render,
    (ObjectDescriptorCallback)wmtorch_free,
    (ObjectDescriptorCallback)wmtorch_getObjectTypeId,
    wmtorch_getExtraSize,
};
