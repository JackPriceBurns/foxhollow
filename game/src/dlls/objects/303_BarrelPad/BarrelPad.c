/*
 * Barrel-launcher pad family. Its launch sequences spawn arced particle
 * bursts; placement data supplies the initial rotation and optional scale.
 */
#include "dlls/objects/303_BarrelPad.h"
#include "main/dll/partfx_interface.h"
#include "main/objfx.h"
#include "main/object_render.h"

#define BARRELPAD_SEQ_LAUNCH_ACTIVE    0x79
#define BARRELPAD_SEQ_LAUNCH_SECONDARY 0x748

int BarrelPad_getExtraSize(void) {
    return 0;
}

int BarrelPad_getObjectTypeId(void) {
    return 0;
}

void BarrelPad_free(void) {
}

void BarrelPad_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void BarrelPad_hitDetect(void) {
}

void BarrelPad_update(GameObject* obj) {
    if (obj->anim.romDefNo == BARRELPAD_SEQ_LAUNCH_ACTIVE) {
        PartFxSpawnParams particleArgs;
        particleArgs.posX = 0.0f;
        particleArgs.posY = 8.0f;
        particleArgs.posZ = 0.0f;
        objfx_spawnArcedBurst(obj, 5, 0.75f, 5, 2, 0x19, 12.0f, 12.0f, 2.0f, &particleArgs, 0);
    }

    if (obj->anim.romDefNo == BARRELPAD_SEQ_LAUNCH_SECONDARY) {
        PartFxSpawnParams particleArgs;
        particleArgs.posX = 0.0f;
        particleArgs.posY = 6.0f;
        particleArgs.posZ = 0.0f;
        objfx_spawnArcedBurst(obj, 5, 0.25f, 5, 2, 5, 7.0f, 7.0f, 2.0f, &particleArgs, 0);
    }
}

void BarrelPad_init(GameObject* obj, BarrelPadPlacement* placement) {
    obj->anim.rotZ = placement->initialRotZ << 8;
    obj->anim.rotY = placement->initialRotY << 8;
    obj->anim.rotX = placement->initialRotX << 8;

    if (placement->scale == 0) {
        obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
        return;
    }

    obj->anim.rootMotionScale = placement->scale / 255.0f;
    if (!obj->anim.rootMotionScale) {
        obj->anim.rootMotionScale = 1.0f;
    }

    obj->anim.rootMotionScale *= obj->anim.modelInstance->rootMotionScaleBase;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void BarrelPad_release(void) {
}

void BarrelPad_initialise(void) {
}

OBJECT_INIT_ADAPTER(gBarrelPadObjDescriptorInitAdapter, BarrelPad_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gBarrelPadObjDescriptorHitDetectAdapter, BarrelPad_hitDetect)
OBJECT_FREE_ADAPTER(gBarrelPadObjDescriptorFreeAdapter, BarrelPad_free)
OBJECT_TYPE_ID_ADAPTER(gBarrelPadObjDescriptorTypeIdAdapter, BarrelPad_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gBarrelPadObjDescriptorExtraSizeAdapter, BarrelPad_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gBarrelPadObjDescriptorAcquire, BarrelPad_initialise)

ObjectDescriptor gBarrelPadObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gBarrelPadObjDescriptorAcquire,
        BarrelPad_release,
    },
    0,
    gBarrelPadObjDescriptorInitAdapter,
    BarrelPad_update,
    gBarrelPadObjDescriptorHitDetectAdapter,
    BarrelPad_render,
    gBarrelPadObjDescriptorFreeAdapter,
    gBarrelPadObjDescriptorTypeIdAdapter,
    gBarrelPadObjDescriptorExtraSizeAdapter,
};
