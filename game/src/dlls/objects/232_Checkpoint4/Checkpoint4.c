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

#define CHECKPOINT4_OBJECT_TYPE_ID     0x10
#define CHECKPOINT4_MIN_RADIUS         5.0f
#define CHECKPOINT4_RADIUS_SCALE       0.0078125f
#define CHECKPOINT4_TRIGGER_SCALE      2.0f
#define CHECKPOINT4_RANDOM_HEADING_MAX 0xF0

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

void checkpoint4_init(GameObject* obj, Checkpoint4Placement* placement) {
    f32 radius;
    u32 heading;
    int i;
    f32 normalYContribution;
    Checkpoint4State* state;
    MatrixTransform transform;
    f32 matrix[16];

    state = obj->extra;
    radius = (f32)(int)placement->radius;
    if ((f32)(int)placement->radius < CHECKPOINT4_MIN_RADIUS) {
        radius = CHECKPOINT4_MIN_RADIUS;
    }
    radius *= CHECKPOINT4_RADIUS_SCALE;
    obj->anim.rootMotionScale = radius;
    obj->anim.rotX = (s16)((s16)placement->rotX << 8);
    transform.rotX = obj->anim.rotX;
    transform.rotY = obj->anim.rotY;
    transform.rotZ = obj->anim.rotZ;
    transform.scale = 1.0f;
    transform.x = 0.0f;
    transform.y = 0.0f;
    transform.z = 0.0f;
    setMatrixFromObjectPos(matrix, &transform);
    Matrix_TransformPoint(matrix, 0.0f, 0.0f, 1.0f, &state->planeNormalX, &state->planeNormalY, &state->planeNormalZ);
    normalYContribution = obj->anim.localPosY * state->planeNormalY;
    state->planeDistance =
        -(normalYContribution + obj->anim.localPosX * state->planeNormalX + obj->anim.localPosZ * state->planeNormalZ);
    state->triggerRadius = CHECKPOINT4_TRIGGER_SCALE * obj->anim.rootMotionScale;
    i = 0;
    do {
        heading = randomGetRange(0, CHECKPOINT4_RANDOM_HEADING_MAX);
        state->randomHeadings[i] = heading;
        i++;
    } while (i < CHECKPOINT4_RANDOM_HEADING_COUNT);
    obj->userData1 = placement->checkpointIndex;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED | OBJECT_OBJFLAG_UPDATE_DISABLED;
}

void checkpoint4_release(void) {
}

void checkpoint4_initialise(void) {
}

ObjectDescriptor11WithPadding gCheckpoint4ObjDescriptor = {
    {
        0,                                                     /* reserved0 */
        0,                                                     /* reserved1 */
        0,                                                     /* reserved2 */
        OBJECT_DESCRIPTOR_FLAGS_11_SLOTS,                      /* slotCountAndFlags */
        (ObjectDescriptorCallback)checkpoint4_initialise,      /* initialise */
        (ObjectDescriptorCallback)checkpoint4_release,         /* release */
        0,                                                     /* slot02 */
        (ObjectDescriptorCallback)checkpoint4_init,            /* init */
        (ObjectDescriptorCallback)checkpoint4_update,          /* update */
        (ObjectDescriptorCallback)checkpoint4_hitDetect,       /* hitDetect */
        (ObjectDescriptorCallback)checkpoint4_render,          /* render */
        (ObjectDescriptorCallback)checkpoint4_free,            /* free */
        (ObjectDescriptorCallback)checkpoint4_getObjectTypeId, /* getObjectTypeId */
        checkpoint4_getExtraSize,                              /* getExtraSize */
        (ObjectDescriptorCallback)checkpoint4_func0A,        /* slot0A */
    },
    0, /* padding */
};
