#ifndef DLLS_OBJECTS_232_CHECKPOINT4_H_
#define DLLS_OBJECTS_232_CHECKPOINT4_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct Checkpoint4Placement {
    ObjPlacement base;
    u8 pad18[0x10];
    s8 checkpointIndex;
    u8 rotX;
    u8 radius;
    u8 pad2B[0x15];
} Checkpoint4Placement;

STATIC_ASSERT(offsetof(Checkpoint4Placement, base) == 0x0);
STATIC_ASSERT(offsetof(Checkpoint4Placement, pad18) == 0x18);
STATIC_ASSERT(offsetof(Checkpoint4Placement, checkpointIndex) == 0x28);
STATIC_ASSERT(offsetof(Checkpoint4Placement, rotX) == 0x29);
STATIC_ASSERT(offsetof(Checkpoint4Placement, radius) == 0x2A);
STATIC_ASSERT(offsetof(Checkpoint4Placement, pad2B) == 0x2B);
STATIC_ASSERT(sizeof(Checkpoint4Placement) == 0x40);

void checkpoint4_func0A(void);
int checkpoint4_getExtraSize(void);
int checkpoint4_getObjectTypeId(void);
void checkpoint4_free(GameObject* obj);
void checkpoint4_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void checkpoint4_hitDetect(GameObject* obj);
void checkpoint4_update(GameObject* obj);
void checkpoint4_init(GameObject* obj, const Checkpoint4Placement* placement);
void checkpoint4_release(void);
void checkpoint4_initialise(void);

extern ObjectDescriptor11WithPadding gCheckpoint4ObjDescriptor;

#endif /* DLLS_OBJECTS_232_CHECKPOINT4_H_ */
