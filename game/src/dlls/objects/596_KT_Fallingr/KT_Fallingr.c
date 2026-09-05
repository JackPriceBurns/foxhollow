/*
 * KT_Fallingr (DLL 0x254) - a one-shot rockfall trigger that rains
 * particle effects down around the player.
 *
 * Each update tick it watches a placement game bit; when the bit is set
 * it snaps to the player's XZ position, spawns ten rock particle effects
 * scattered within +/-200 units, plays the rockfall sfx, then clears the
 * bit so the burst only fires once per trigger.
 */
#include "dlls/object_descriptor.h"
#include "main/audio/sfx.h"
#include "main/dll/partfx_interface.h"
#include "main/dll/expgfx_interface.h"
#include "main/gamebits.h"
#include "sys/objects.h"
#include "main/vecmath.h"

#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/DR/dll_0254_ktfallingrocks.h"

int ktfallingrocks_getExtraSize(void) {
    return 0x0;
}

int ktfallingrocks_getObjectTypeId(void) {
    return 0x0;
}

void ktfallingrocks_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void ktfallingrocks_render(void* obj, u32 p2, u32 p3, u32 p4, u32 p5, char visible) {
}

void ktfallingrocks_hitDetect(void) {
}

void ktfallingrocks_update(GameObject* obj) {
    KtfallingrocksPlacement* placement = (KtfallingrocksPlacement*)obj->anim.placementData;
    MatrixTransform params;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->triggerBit)) == 0) {
        return;
    }

    GameObject* player = Obj_GetPlayerObject();
    if (player == NULL) {
        return;
    }

    obj->anim.localPosX = player->anim.localPosX;
    obj->anim.localPosZ = player->anim.localPosZ;

    for (int i = 0; i < 10; i++) {
        params.x = obj->anim.localPosX + (f32)randomGetRange(-200, 200);
        params.y = obj->anim.localPosY;
        params.z = obj->anim.localPosZ + (f32)randomGetRange(-200, 200);
        (*gPartfxInterface)
            ->spawnObject(obj, ObjAnim_ReadPlacementU16(&obj->anim, &placement->effectId), &params, 0x200001, -1, NULL);
    }

    Sfx_PlayFromObject(obj, SFXTRIG_en_birdynight11);
    mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->triggerBit), 0);
}

void ktfallingrocks_init(GameObject* obj) {
    obj->animEventCallback = NULL;
}

void ktfallingrocks_release(void) {
}

void ktfallingrocks_initialise(void) {
}

OBJECT_INIT_ADAPTER(gKtFallingrocksObjDescriptorInitAdapter, ktfallingrocks_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gKtFallingrocksObjDescriptorHitDetectAdapter, ktfallingrocks_hitDetect)
OBJECT_RENDER_ADAPTER(gKtFallingrocksObjDescriptorRenderAdapter, ktfallingrocks_render, obj, arg2, arg3, arg4, arg5, visible)
OBJECT_FREE_ADAPTER(gKtFallingrocksObjDescriptorFreeAdapter, ktfallingrocks_free, obj)
OBJECT_TYPE_ID_ADAPTER(gKtFallingrocksObjDescriptorTypeIdAdapter, ktfallingrocks_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gKtFallingrocksObjDescriptorExtraSizeAdapter, ktfallingrocks_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gKtFallingrocksObjDescriptorAcquire, ktfallingrocks_initialise)

ObjectDescriptor gKtFallingrocksObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gKtFallingrocksObjDescriptorAcquire,
        ktfallingrocks_release,
    },
    0,
    gKtFallingrocksObjDescriptorInitAdapter,
    ktfallingrocks_update,
    gKtFallingrocksObjDescriptorHitDetectAdapter,
    gKtFallingrocksObjDescriptorRenderAdapter,
    gKtFallingrocksObjDescriptorFreeAdapter,
    gKtFallingrocksObjDescriptorTypeIdAdapter,
    gKtFallingrocksObjDescriptorExtraSizeAdapter,
};
