/*
 * DIMbosscrackpar (DLL 0x1E6) - DarkIce Mines boss crack-particle
 * emitters. While a placement-selected game bit is set, each object emits
 * both its crack-specific effect and a fixed glow effect.
 */
#include "dlls/objects/486_DIMbosscrac.h"

#include "game/objects/object.h"
#include "main/dll/expgfx_interface.h"
#include "main/dll/partfx_interface.h"
#include "main/gamebits.h"

#define DIMBOSSCRACKPAR_BASE_PARTICLE_ID 0x4C6
#define DIMBOSSCRACKPAR_GLOW_PARTICLE_ID 0x4C8

int DIMbosscrackpar_SeqFn(GameObject* obj) {
    DIMbosscrackparPlacementView* placement = (DIMbosscrackparPlacementView*)obj->anim.placementData;

    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->triggerGameBit)) == 0) {
        return 0;
    }

    (*gPartfxInterface)
        ->spawnObject(obj,
                      ObjAnim_ReadPlacementS16(&obj->anim, &placement->particleEffectOffset) +
                          DIMBOSSCRACKPAR_BASE_PARTICLE_ID,
                      NULL, 2, -1, NULL);

    (*gPartfxInterface)->spawnObject(obj, DIMBOSSCRACKPAR_GLOW_PARTICLE_ID, NULL, 2, -1, NULL);
    return 0;
}

int DIMbosscrackpar_getExtraSize(void) {
    return 0;
}

int DIMbosscrackpar_getObjectTypeId(void) {
    return 0;
}

void DIMbosscrackpar_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((u32)obj);
}

void DIMbosscrackpar_render(GameObject*, int, int, int, int, s8 visible) {
}

void DIMbosscrackpar_hitDetect(void) {
}

void DIMbosscrackpar_update(GameObject* obj) {
    DIMbosscrackparPlacementView* placement = (DIMbosscrackparPlacementView*)obj->anim.placementData;

    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->triggerGameBit)) != 0) {
        (*gPartfxInterface)
            ->spawnObject(obj,
                          ObjAnim_ReadPlacementS16(&obj->anim, &placement->particleEffectOffset) +
                              DIMBOSSCRACKPAR_BASE_PARTICLE_ID,
                          NULL, 2, -1, NULL);
        (*gPartfxInterface)->spawnObject(obj, DIMBOSSCRACKPAR_GLOW_PARTICLE_ID, NULL, 2, -1, NULL);
    }
}

void DIMbosscrackpar_init(GameObject* obj, DIMbosscrackparPlacementView* placement) {
    obj->anim.rotX = 0;
    obj->anim.rootMotionScale = 0.1f;
    obj->animEventCallback = DIMbosscrackpar_SeqFn;
    obj->anim.rotX = placement->rotationXByte << 8;
    obj->anim.rotY = placement->rotationYByte << 8;
    obj->anim.rotZ = placement->rotationZByte << 8;
}

void DIMbosscrackpar_release(void) {
}

void DIMbosscrackpar_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDIMbosscrackparObjDescriptorInitAdapter, DIMbosscrackpar_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDIMbosscrackparObjDescriptorHitDetectAdapter, DIMbosscrackpar_hitDetect)
OBJECT_FREE_ADAPTER(gDIMbosscrackparObjDescriptorFreeAdapter, DIMbosscrackpar_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDIMbosscrackparObjDescriptorTypeIdAdapter, DIMbosscrackpar_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDIMbosscrackparObjDescriptorExtraSizeAdapter, DIMbosscrackpar_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDIMbosscrackparObjDescriptorAcquire, DIMbosscrackpar_initialise)

ObjectDescriptor gDIMbosscrackparObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDIMbosscrackparObjDescriptorAcquire,
        DIMbosscrackpar_release,
    },
    0,
    gDIMbosscrackparObjDescriptorInitAdapter,
    DIMbosscrackpar_update,
    gDIMbosscrackparObjDescriptorHitDetectAdapter,
    DIMbosscrackpar_render,
    gDIMbosscrackparObjDescriptorFreeAdapter,
    gDIMbosscrackparObjDescriptorTypeIdAdapter,
    gDIMbosscrackparObjDescriptorExtraSizeAdapter,
};
