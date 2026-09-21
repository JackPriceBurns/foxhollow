/* VFP_Block1 (DLL 0x021E) */
#include "dlls/object_descriptor.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/expgfx_interface.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "main/audio/sfx.h"
#include "main/dll/VF/dll_021E_vfpblock1.h"

int VFP_Block1_getExtraSize(void) {
    return 0x2;
}

int VFP_Block1_getObjectTypeId(void) {
    return 0x0;
}

void VFP_Block1_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void VFP_Block1_render(void) {
}

void VFP_Block1_hitDetect(void) {
}

void VFP_Block1_update(GameObject* obj) {
    GameObject* player = Obj_GetPlayerObject();
    f32 dist = Vec_distance(&player->anim.worldPosX, &obj->anim.worldPosX);

    if (Sfx_IsPlayingFromObjectChannel(obj, 0x40) != 0) {
        if (dist < 90.0f) {
            Sfx_PlayFromObject(obj, SFXTRIG_mv_mushdizzylp12);
        }
    } else {
        if (dist >= 90.0f) {
            Sfx_StopObjectChannel(obj, 0x40);
        }
    }
}

void VFP_Block1_init(GameObject* obj, VfpBlock1Placement* data) {
    VfpBlock1State* state = obj->extra;
    obj->anim.rotX = data->rotXByte << 8;
    state->gameBitId = ObjAnim_ReadPlacementS16(&obj->anim, &data->gameBitId);
    obj->objectFlags |= (OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED);
}

void VFP_Block1_release(void) {
}

void VFP_Block1_initialise(void) {
}

OBJECT_INIT_ADAPTER(gVFP_Block1ObjDescriptorInitAdapter, VFP_Block1_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gVFP_Block1ObjDescriptorHitDetectAdapter, VFP_Block1_hitDetect)
OBJECT_RENDER_ADAPTER(gVFP_Block1ObjDescriptorRenderAdapter, VFP_Block1_render)
OBJECT_FREE_ADAPTER(gVFP_Block1ObjDescriptorFreeAdapter, VFP_Block1_free, obj)
OBJECT_TYPE_ID_ADAPTER(gVFP_Block1ObjDescriptorTypeIdAdapter, VFP_Block1_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gVFP_Block1ObjDescriptorExtraSizeAdapter, VFP_Block1_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gVFP_Block1ObjDescriptorAcquire, VFP_Block1_initialise)

ObjectDescriptor gVFP_Block1ObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gVFP_Block1ObjDescriptorAcquire,
        VFP_Block1_release,
    },
    0,
    gVFP_Block1ObjDescriptorInitAdapter,
    VFP_Block1_update,
    gVFP_Block1ObjDescriptorHitDetectAdapter,
    gVFP_Block1ObjDescriptorRenderAdapter,
    gVFP_Block1ObjDescriptorFreeAdapter,
    gVFP_Block1ObjDescriptorTypeIdAdapter,
    gVFP_Block1ObjDescriptorExtraSizeAdapter,
};
