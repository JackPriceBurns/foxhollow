#ifndef DLLS_OBJECTS_232_CHECKPOINT4_H_
#define DLLS_OBJECTS_232_CHECKPOINT4_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define CHECKPOINT4_RANDOM_HEADING_COUNT 4

typedef struct Checkpoint4Placement {
    ObjPlacement base;  /* 0x00 */
    u8 pad18[0x10];     /* 0x18 */
    s8 checkpointIndex; /* 0x28 */
    u8 rotX;            /* 0x29 */
    u8 radius;          /* 0x2A */
    u8 pad2B[0x15];     /* 0x2B */
} Checkpoint4Placement;

typedef struct Checkpoint4State {
    u8 pad00[0x10];                                       /* 0x00 */
    f32 planeNormalX;                                     /* 0x10 */
    f32 planeNormalY;                                     /* 0x14 */
    f32 planeNormalZ;                                     /* 0x18 */
    f32 planeDistance;                                    /* 0x1C */
    f32 triggerRadius;                                    /* 0x20 */
    u8 pad24[0x10];                                       /* 0x24 */
    s16 randomHeadings[CHECKPOINT4_RANDOM_HEADING_COUNT]; /* 0x34 */
    u8 pad3C[4];                                          /* 0x3C */
} Checkpoint4State;

STATIC_ASSERT(offsetof(Checkpoint4Placement, base) == 0x0);
STATIC_ASSERT(offsetof(Checkpoint4Placement, pad18) == 0x18);
STATIC_ASSERT(offsetof(Checkpoint4Placement, checkpointIndex) == 0x28);
STATIC_ASSERT(offsetof(Checkpoint4Placement, rotX) == 0x29);
STATIC_ASSERT(offsetof(Checkpoint4Placement, radius) == 0x2A);
STATIC_ASSERT(offsetof(Checkpoint4Placement, pad2B) == 0x2B);
STATIC_ASSERT(sizeof(Checkpoint4Placement) == 0x40);

STATIC_ASSERT(offsetof(Checkpoint4State, pad00) == 0x0);
STATIC_ASSERT(offsetof(Checkpoint4State, planeNormalX) == 0x10);
STATIC_ASSERT(offsetof(Checkpoint4State, planeNormalY) == 0x14);
STATIC_ASSERT(offsetof(Checkpoint4State, planeNormalZ) == 0x18);
STATIC_ASSERT(offsetof(Checkpoint4State, planeDistance) == 0x1C);
STATIC_ASSERT(offsetof(Checkpoint4State, triggerRadius) == 0x20);
STATIC_ASSERT(offsetof(Checkpoint4State, pad24) == 0x24);
STATIC_ASSERT(offsetof(Checkpoint4State, randomHeadings) == 0x34);
STATIC_ASSERT(offsetof(Checkpoint4State, pad3C) == 0x3C);
STATIC_ASSERT(sizeof(Checkpoint4State) == 0x40);

void checkpoint4_func0A(void);
int checkpoint4_getExtraSize(void);
int checkpoint4_getObjectTypeId(void);
void checkpoint4_free(GameObject* obj);
void checkpoint4_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void checkpoint4_hitDetect(GameObject* obj);
void checkpoint4_update(GameObject* obj);
void checkpoint4_init(GameObject* obj, Checkpoint4Placement* placement);
void checkpoint4_release(void);
void checkpoint4_initialise(void);

extern ObjectDescriptor11WithPadding gCheckpoint4ObjDescriptor;

#endif /* DLLS_OBJECTS_232_CHECKPOINT4_H_ */
