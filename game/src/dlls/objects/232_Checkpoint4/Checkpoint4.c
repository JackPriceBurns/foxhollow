/*
 * Checkpoint4 object (DLL slot 232 / 0xE8).
 *
 * Initialization derives an oriented checkpoint plane from the placement
 * rotation, scales its trigger radius, seeds four random headings, and stores
 * the checkpoint index in the object's class-owned scratch word. Rendering
 * draws the regular model; the remaining runtime callbacks are empty.
 */
#include "dlls/objects/232_Checkpoint4.h"
#include "main/object_render.h"
#include "main/vecmath.h"

enum {
    CHECKPOINT4_RANDOM_HEADING_COUNT = 4,
};

typedef enum Checkpoint4ObjectTypeId {
    CHECKPOINT4_OBJECT_TYPE_ID = 0x10,
} Checkpoint4ObjectTypeId;

typedef struct Checkpoint4State {
    u8 pad00[0x10];
    Vec3f planeNormal;
    f32 planeDistance;
    f32 triggerRadius;
    u8 pad24[0x10];
    s16 randomHeadings[CHECKPOINT4_RANDOM_HEADING_COUNT];
    u8 pad3C[4];
} Checkpoint4State;

STATIC_ASSERT(sizeof(Checkpoint4State) == 0x40);
STATIC_ASSERT(offsetof(Checkpoint4State, planeNormal) == 0x10);
STATIC_ASSERT(offsetof(Checkpoint4State, planeDistance) == 0x1C);
STATIC_ASSERT(offsetof(Checkpoint4State, triggerRadius) == 0x20);
STATIC_ASSERT(offsetof(Checkpoint4State, randomHeadings) == 0x34);

void checkpoint4_func0A(void) {
}

int checkpoint4_getExtraSize(void) {
    return sizeof(Checkpoint4State);
}

int checkpoint4_getObjectTypeId(void) {
    return CHECKPOINT4_OBJECT_TYPE_ID;
}

void checkpoint4_free(GameObject* obj) {
    (void)obj;
}

void checkpoint4_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    (void)visible;

    objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
}

void checkpoint4_hitDetect(GameObject* obj) {
    (void)obj;
}

void checkpoint4_update(GameObject* obj) {
    (void)obj;
}

void checkpoint4_init(GameObject* obj, const Checkpoint4Placement* placement) {
    Checkpoint4State* state = obj->extra;
    f32 matrix[16];

    f32 radius = placement->radius;
    if (radius < 5.0f) {
        radius = 5.0f;
    }
    radius *= 0.0078125f;
    obj->anim.rootMotionScale = radius;
    obj->anim.rotX = (s16)((s16)placement->rotX << 8);
    MatrixTransform transform = {
        .rotX = obj->anim.rotX,
        .rotY = obj->anim.rotY,
        .rotZ = obj->anim.rotZ,
        .scale = 1.0f,
    };
    setMatrixFromObjectPos(matrix, &transform);
    Matrix_TransformPoint(matrix, 0.0f, 0.0f, 1.0f, &state->planeNormal.x,
                          &state->planeNormal.y, &state->planeNormal.z);
    f32 normalYContribution = obj->anim.localPosY * state->planeNormal.y;
    state->planeDistance =
        -(normalYContribution + obj->anim.localPosX * state->planeNormal.x +
          obj->anim.localPosZ * state->planeNormal.z);
    state->triggerRadius = 2.0f * obj->anim.rootMotionScale;
    for (int i = 0; i < CHECKPOINT4_RANDOM_HEADING_COUNT; i++) {
        state->randomHeadings[i] = (s16)randomGetRange(0, 0xF0);
    }
    obj->userData1 = placement->checkpointIndex;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED | OBJECT_OBJFLAG_UPDATE_DISABLED;
}

void checkpoint4_release(void) {
}

void checkpoint4_initialise(void) {
}

OBJECT_INIT_ADAPTER(gCheckpoint4ObjDescriptorInitAdapter, checkpoint4_init, obj, placement)
OBJECT_FREE_ADAPTER(gCheckpoint4ObjDescriptorFreeAdapter, checkpoint4_free, obj)
OBJECT_TYPE_ID_ADAPTER(gCheckpoint4ObjDescriptorTypeIdAdapter, checkpoint4_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gCheckpoint4ObjDescriptorExtraSizeAdapter, checkpoint4_getExtraSize)

typedef struct Checkpoint4ObjDescriptorTypeInterface {
    OBJECT_INTERFACE_FIELDS;
    __typeof__(checkpoint4_func0A)* checkpoint4_func0A;
} Checkpoint4ObjDescriptorTypeInterface;

typedef struct Checkpoint4ObjDescriptorTypeCore {
    ObjectDescriptorHeader header;
    Checkpoint4ObjDescriptorTypeInterface interface;
} Checkpoint4ObjDescriptorTypeCore;

struct Checkpoint4ObjDescriptorType {
    Checkpoint4ObjDescriptorTypeCore descriptor;
    u32 padding;
};

RESOURCE_ACQUIRE_ADAPTER(gCheckpoint4ObjDescriptorAcquire, checkpoint4_initialise)

struct Checkpoint4ObjDescriptorType gCheckpoint4ObjDescriptor = {
    {
        {
            {
                0,
                0,
                0,
                OBJECT_DESCRIPTOR_FLAGS_11_SLOTS,
            },
            gCheckpoint4ObjDescriptorAcquire,
            checkpoint4_release,
        },
        {
            0,
            gCheckpoint4ObjDescriptorInitAdapter,
            checkpoint4_update,
            checkpoint4_hitDetect,
            checkpoint4_render,
            gCheckpoint4ObjDescriptorFreeAdapter,
            gCheckpoint4ObjDescriptorTypeIdAdapter,
            gCheckpoint4ObjDescriptorExtraSizeAdapter,
            checkpoint4_func0A,
        },
    },
    0,
};
