#ifndef DLLS_OBJECTS_243_FLAMEBLAST_H_
#define DLLS_OBJECTS_243_FLAMEBLAST_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct FlameblastPlacement {
    ObjPlacement base;
    u8 pad18[2];
    s16 streamIndex;
    u8 pad1C[8];
} FlameblastPlacement;

STATIC_ASSERT(sizeof(FlameblastPlacement) == 0x24);
STATIC_ASSERT(offsetof(FlameblastPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(FlameblastPlacement, streamIndex) == 0x1A);

void flameblast_requestFree(GameObject* obj);

extern ObjectDescriptor gFlameblastObjDescriptor;

#endif
