/*
 * SkeetlaWall object (DLL slot 212).
 *
 * Expands six placement extents into world-space bounds for the collision
 * layer and renders the inactive wall shape.
 */
#include "dlls/objects/212_SkeetlaWall.h"
#include "main/object_render.h"

void SkeetlaWall_getBounds(GameObject* obj, f32* outBounds, u8* outShapeFlag) {
    SkeetlaWallState* state = obj->extra;
    outBounds[0] = obj->anim.worldPosX - state->negXExtent;
    outBounds[1] = obj->anim.worldPosX + state->posXExtent;
    outBounds[2] = obj->anim.worldPosZ + state->posZExtent;
    outBounds[3] = obj->anim.worldPosZ - state->negZExtent;
    outBounds[4] = obj->anim.worldPosY + state->posYExtent;
    outBounds[5] = obj->anim.worldPosY - state->negYExtent;
    *outShapeFlag = state->shapeFlag;
}

int SkeetlaWall_getExtraSize(void) {
    return sizeof(SkeetlaWallState);
}

int SkeetlaWall_getObjectTypeId(void) {
    return 0;
}

void SkeetlaWall_free(GameObject* obj) {
    (void)obj;
}

void SkeetlaWall_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    switch (obj->userData1) {
    case 0:
        objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
        break;
    }
}

void SkeetlaWall_hitDetect(GameObject* obj) {
}

void SkeetlaWall_update(GameObject* obj) {
}

void SkeetlaWall_init(GameObject* obj, SkeetlaWallPlacement* placement) {
    SkeetlaWallState* state = obj->extra;
    state->negXExtent = placement->negXExtent;
    state->posXExtent = placement->posXExtent;
    state->posZExtent = placement->posZExtent;
    state->negZExtent = placement->negZExtent;
    state->posYExtent = placement->posYExtent;
    state->negYExtent = placement->negYExtent;
    state->shapeFlag = placement->shapeFlag;
}

void SkeetlaWall_release(void) {
}

void SkeetlaWall_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSkeetlaWallObjDescriptorInitAdapter, SkeetlaWall_init, obj, placement)
OBJECT_FREE_ADAPTER(gSkeetlaWallObjDescriptorFreeAdapter, SkeetlaWall_free, obj)
OBJECT_TYPE_ID_ADAPTER(gSkeetlaWallObjDescriptorTypeIdAdapter, SkeetlaWall_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSkeetlaWallObjDescriptorExtraSizeAdapter, SkeetlaWall_getExtraSize)

typedef struct SkeetlaWallObjDescriptorTypeInterface {
    OBJECT_INTERFACE_FIELDS;
    __typeof__(SkeetlaWall_getBounds)* SkeetlaWall_getBounds;
} SkeetlaWallObjDescriptorTypeInterface;

typedef struct SkeetlaWallObjDescriptorTypeCore {
    ObjectDescriptorHeader header;
    SkeetlaWallObjDescriptorTypeInterface interface;
} SkeetlaWallObjDescriptorTypeCore;

struct SkeetlaWallObjDescriptorType {
    SkeetlaWallObjDescriptorTypeCore descriptor;
    u32 padding;
};

RESOURCE_ACQUIRE_ADAPTER(gSkeetlaWallObjDescriptorAcquire, SkeetlaWall_initialise)

struct SkeetlaWallObjDescriptorType gSkeetlaWallObjDescriptor = {
    {
        {
            {
                0,
                0,
                0,
                OBJECT_DESCRIPTOR_FLAGS_11_SLOTS,
            },
            gSkeetlaWallObjDescriptorAcquire,
            SkeetlaWall_release,
        },
        {
            0,
            gSkeetlaWallObjDescriptorInitAdapter,
            SkeetlaWall_update,
            SkeetlaWall_hitDetect,
            SkeetlaWall_render,
            gSkeetlaWallObjDescriptorFreeAdapter,
            gSkeetlaWallObjDescriptorTypeIdAdapter,
            gSkeetlaWallObjDescriptorExtraSizeAdapter,
            SkeetlaWall_getBounds,
        },
    },
    0,
};
