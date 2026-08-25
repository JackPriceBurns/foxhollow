/*
 * SPitembeam (DLL 649) - the glowing "for sale" beam that marks a
 * purchasable item on a SnowHorn shop stall.
 *
 * Each beam latches onto the nearest shop object (object group 9, the
 * same group the shopkeeper and scarab coins look up) and tracks one
 * item slot (placement->itemIndex). While that item is still for sale
 * the beam scrolls its texture; once the shop reports the item is no
 * longer available, or has already been bought, the beam hides and
 * despawns itself.
 */
#include "main/objtexture.h"
#include "main/objtype.h"
#include "main/dll/SP/dll_0285_spshop.h"
#include "main/dll/SP/dll_0289_spitembeam.h"
#include "dlls/object_descriptor.h"

/* texture-scroll wrap (1/4 of the 0x1000 fixed-point texcoord range) */
#define SPITEMBEAM_SCROLL_WRAP     0x400
#define SPITEMBEAM_TARGET_OBJGROUP 9

int spitembeam_getExtraSize(void) {
    return 0x0;
}
int spitembeam_getObjectTypeId(void) {
    return 0x0;
}

void spitembeam_free(void) {
}

void spitembeam_render(void) {
}

void spitembeam_hitDetect(void) {
}

void spitembeam_update(GameObject* obj) {

    GameObject* shop = (GameObject*)obj->userData1;
    SpitembeamPlacement* def = (SpitembeamPlacement*)obj->anim.placementData;
    if (shop == NULL) {
        f32 searchRadius = 10000.0f;
        obj->userData1 = (intptr_t)objGetNearestTypeTo(SPITEMBEAM_TARGET_OBJGROUP, obj, &searchRadius);
        return;
    }

    if (SHOP_INTERFACE(shop)->isItemAvailable(shop, ObjAnim_ReadPlacementS16(&obj->anim, &def->itemIndex)) == 0 ||
        SHOP_INTERFACE(shop)->isItemBought(shop, ObjAnim_ReadPlacementS16(&obj->anim, &def->itemIndex)) != 0) {
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        obj->objectFlags |= OBJECT_OBJFLAG_UPDATE_DISABLED;
    }

    ObjTextureRuntimeSlot* tex = objFindTexture(obj, 0, 0);
    if (tex == NULL) {
        return;
    }

    tex->offsetS += 8;
    if (tex->offsetS > SPITEMBEAM_SCROLL_WRAP) {
        tex->offsetS -= SPITEMBEAM_SCROLL_WRAP;
    }
}

void spitembeam_init(GameObject* obj) {
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void spitembeam_release(void) {
}

void spitembeam_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSPitembeamObjDescriptorInitAdapter, spitembeam_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gSPitembeamObjDescriptorHitDetectAdapter, spitembeam_hitDetect)
OBJECT_RENDER_ADAPTER(gSPitembeamObjDescriptorRenderAdapter, spitembeam_render)
OBJECT_FREE_ADAPTER(gSPitembeamObjDescriptorFreeAdapter, spitembeam_free)
OBJECT_TYPE_ID_ADAPTER(gSPitembeamObjDescriptorTypeIdAdapter, spitembeam_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSPitembeamObjDescriptorExtraSizeAdapter, spitembeam_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gSPitembeamObjDescriptorAcquire, spitembeam_initialise)

ObjectDescriptor gSPitembeamObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gSPitembeamObjDescriptorAcquire,
        spitembeam_release,
    },
    0,
    gSPitembeamObjDescriptorInitAdapter,
    spitembeam_update,
    gSPitembeamObjDescriptorHitDetectAdapter,
    gSPitembeamObjDescriptorRenderAdapter,
    gSPitembeamObjDescriptorFreeAdapter,
    gSPitembeamObjDescriptorTypeIdAdapter,
    gSPitembeamObjDescriptorExtraSizeAdapter,
};
