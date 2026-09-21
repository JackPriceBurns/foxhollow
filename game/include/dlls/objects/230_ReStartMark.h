#ifndef DLLS_OBJECTS_230_RESTARTMARK_H_
#define DLLS_OBJECTS_230_RESTARTMARK_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct ReStartMarkPlacement {
    ObjPlacement base; /* 0x00 */
    u8 rotXByte;       /* 0x18: high byte of the initial X rotation */
    u8 pad19[3];       /* 0x19 */
} ReStartMarkPlacement;

STATIC_ASSERT(offsetof(ReStartMarkPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(ReStartMarkPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(ReStartMarkPlacement, pad19) == 0x19);
STATIC_ASSERT(sizeof(ReStartMarkPlacement) == 0x1C);

void ReStartMark_init(GameObject* obj, ReStartMarkPlacement* placement);

extern ObjectDescriptor gReStartMarkObjDescriptor;

#endif /* DLLS_OBJECTS_230_RESTARTMARK_H_ */
