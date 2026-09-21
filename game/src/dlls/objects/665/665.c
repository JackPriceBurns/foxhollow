/*
 * DLL 665 - an ambient particle-effect object.
 *
 * On init it stores a placement-supplied s16 id into its 2-byte extra
 * state, acquires resource 0xA6, and seeds three 0x545 and one 0x546
 * particle bursts. Each update tick it randomly (1-in-3) invokes vtable
 * slot 1 of the acquired resource, then spawns three 0x547 particles.
 * On free it releases the exp/mod-gfx sources and the acquired resource.
 * Render/hitDetect/release/initialise are stubs.
 */
#include "main/dll/modgfx_interface.h"
#include "main/dll/partfx_interface.h"
#include "main/dll/dll_0299.h"
#include "main/dll_000A_expgfx.h"
#include "main/resource.h"
#include "main/vecmath.h"

DllA6Interface** gDll299Resource;

#define DLL299_RESOURCE_ID 0xa6
#define DLL0299_PARTFX_INIT 0x545
#define DLL0299_PARTFX_INIT2 0x546
#define DLL0299_PARTFX_AMBIENT 0x547

int dll_299_getExtraSize_ret_2(void) {
    return 0x2;
}

int dll_299_getObjectTypeId(void) {
    return 0x0;
}

void dll_299_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
    (*gModgfxInterface)->freeSourceEffects(obj);
    Resource_Release(gDll299Resource);
    gDll299Resource = NULL;
}

void dll_299_render_nop(void) {
}

void dll_299_hitDetect_nop(void) {
}

void dll_299_update(GameObject* obj) {
    if (randomGetRange(0, 2) == 0) {
        (*gDll299Resource)->spawn(obj, 1, NULL, 4);
    }
    (*gPartfxInterface)->spawnObject(obj, DLL0299_PARTFX_AMBIENT, NULL, 4, -1, NULL);
    (*gPartfxInterface)->spawnObject(obj, DLL0299_PARTFX_AMBIENT, NULL, 4, -1, NULL);
    (*gPartfxInterface)->spawnObject(obj, DLL0299_PARTFX_AMBIENT, NULL, 4, -1, NULL);
}

void dll_299_init(GameObject* obj, Dll299Setup* setup) {
    ((Dll299State*)obj->extra)->id = ObjAnim_ReadPlacementS16(&obj->anim, &setup->id);
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
    gDll299Resource = Resource_Acquire(DLL299_RESOURCE_ID, 1);
    (*gPartfxInterface)->spawnObject(obj, DLL0299_PARTFX_INIT, NULL, 0x802, -1, NULL);
    (*gPartfxInterface)->spawnObject(obj, DLL0299_PARTFX_INIT, NULL, 0x802, -1, NULL);
    (*gPartfxInterface)->spawnObject(obj, DLL0299_PARTFX_INIT, NULL, 0x802, -1, NULL);
    (*gPartfxInterface)->spawnObject(obj, DLL0299_PARTFX_INIT2, NULL, 0x802, -1, NULL);
}

void dll_299_release_nop(void) {
}

void dll_299_initialise_nop(void) {
}

OBJECT_INIT_ADAPTER(gDll299ObjDescriptorInitAdapter, dll_299_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDll299ObjDescriptorHitDetectAdapter, dll_299_hitDetect_nop)
OBJECT_RENDER_ADAPTER(gDll299ObjDescriptorRenderAdapter, dll_299_render_nop)
OBJECT_FREE_ADAPTER(gDll299ObjDescriptorFreeAdapter, dll_299_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDll299ObjDescriptorTypeIdAdapter, dll_299_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll299ObjDescriptorExtraSizeAdapter, dll_299_getExtraSize_ret_2)

RESOURCE_ACQUIRE_ADAPTER(gDll299ObjDescriptorAcquire, dll_299_initialise_nop)

ObjectDescriptor gDll299ObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll299ObjDescriptorAcquire,
        dll_299_release_nop,
    },
    0,
    gDll299ObjDescriptorInitAdapter,
    dll_299_update,
    gDll299ObjDescriptorHitDetectAdapter,
    gDll299ObjDescriptorRenderAdapter,
    gDll299ObjDescriptorFreeAdapter,
    gDll299ObjDescriptorTypeIdAdapter,
    gDll299ObjDescriptorExtraSizeAdapter,
};
