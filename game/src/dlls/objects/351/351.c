/* Target marker used by CFAttractor and DBAttractor. */

#include "dlls/objects/351.h"
#include "main/object_render.h"
#include "sys/objects.h"
#include "main/objtype.h"
#include "main/vecmath.h"

#define ATTRACTOR_OBJECT_GROUP 0x1E

void attractor_getTarget(GameObject* obj, GameObject** outTarget) {
    GameObject* target = NULL;
    AttractorPlacement* placement = (AttractorPlacement*)obj->anim.placementData;

    switch (placement->mode) {
    case ATTRACTOR_MODE_NONE:
        break;
    case ATTRACTOR_MODE_RETURN_SELF:
        target = obj;
        break;
    case ATTRACTOR_MODE_FACE_PLAYER: {
        GameObject* player = Obj_GetPlayerObject();
        int angle = atan2i(player->anim.localPosX - obj->anim.localPosX, player->anim.localPosZ - obj->anim.localPosZ);
        obj->anim.rotX = angle + 0x8000;
        target = obj;
        break;
    }
    }
    *outTarget = target;
}

int attractor_getScale(GameObject* obj) {
    AttractorPlacement* placement = (AttractorPlacement*)obj->anim.placementData;
    if (placement->mode != ATTRACTOR_MODE_NONE) {
        return ObjAnim_ReadPlacementS16(&obj->anim, &placement->unknown1A);
    }

    return 0;
}

int attractor_getExtraSize(void) {
    return 0;
}

int attractor_getObjectTypeId(void) {
    return 0;
}

void attractor_free(GameObject* obj) {
    objFreeObjectType(obj, ATTRACTOR_OBJECT_GROUP);
}

void attractor_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void attractor_hitDetect(void) {
}

void attractor_update(void) {
}

void attractor_init(GameObject* obj, AttractorPlacement* placement) {
    objAddObjectType(obj, ATTRACTOR_OBJECT_GROUP);
    obj->anim.rotX = placement->rotXByte << 8;
}

void attractor_release(void) {
}

void attractor_initialise(void) {
}

OBJECT_INIT_ADAPTER(gAttractorObjDescriptorInitAdapter, attractor_init, obj, placement)
OBJECT_UPDATE_ADAPTER(gAttractorObjDescriptorUpdateAdapter, attractor_update)
OBJECT_HIT_DETECT_ADAPTER(gAttractorObjDescriptorHitDetectAdapter, attractor_hitDetect)
OBJECT_FREE_ADAPTER(gAttractorObjDescriptorFreeAdapter, attractor_free, obj)
OBJECT_TYPE_ID_ADAPTER(gAttractorObjDescriptorTypeIdAdapter, attractor_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gAttractorObjDescriptorExtraSizeAdapter, attractor_getExtraSize)

typedef struct AttractorObjDescriptorTypeInterface {
    OBJECT_INTERFACE_FIELDS;
    __typeof__(attractor_getScale)* attractor_getScale;
    __typeof__(attractor_getTarget)* attractor_getTarget;
} AttractorObjDescriptorTypeInterface;

struct AttractorObjDescriptorType {
    ObjectDescriptorHeader header;
    AttractorObjDescriptorTypeInterface interface;
};

RESOURCE_ACQUIRE_ADAPTER(gAttractorObjDescriptorAcquire, attractor_initialise)

struct AttractorObjDescriptorType gAttractorObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_12_SLOTS,
        },
        gAttractorObjDescriptorAcquire,
        attractor_release,
    },
    {
        0,
        gAttractorObjDescriptorInitAdapter,
        gAttractorObjDescriptorUpdateAdapter,
        gAttractorObjDescriptorHitDetectAdapter,
        attractor_render,
        gAttractorObjDescriptorFreeAdapter,
        gAttractorObjDescriptorTypeIdAdapter,
        gAttractorObjDescriptorExtraSizeAdapter,
        attractor_getScale,
        attractor_getTarget,
    },
};
