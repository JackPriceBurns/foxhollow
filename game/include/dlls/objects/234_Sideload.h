#ifndef DLLS_OBJECTS_234_SIDELOAD_H_
#define DLLS_OBJECTS_234_SIDELOAD_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct SideloadPlacement {
    ObjPlacement base; /* 0x00 */
    s16 armingGameBit; /* 0x18 */
    u8 childRotXByte;  /* 0x1A */
    u8 pad1B;          /* 0x1B */
} SideloadPlacement;

STATIC_ASSERT(offsetof(SideloadPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(SideloadPlacement, armingGameBit) == 0x18);
STATIC_ASSERT(offsetof(SideloadPlacement, childRotXByte) == 0x1A);
STATIC_ASSERT(offsetof(SideloadPlacement, pad1B) == 0x1B);
STATIC_ASSERT(sizeof(SideloadPlacement) == 0x1C);

void sideload_update(GameObject* obj);

extern ObjectDescriptor gSideloadObjDescriptor;

#endif /* DLLS_OBJECTS_234_SIDELOAD_H_ */
