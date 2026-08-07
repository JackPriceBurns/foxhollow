#ifndef DLLS_OBJECTS_207_CANNONCLAW_H_
#define DLLS_OBJECTS_207_CANNONCLAW_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct CannonClawPlacement {
    ObjPlacement base;     /* 0x00 */
    u8 pad18[0x28 - 0x18]; /* 0x18 */
    s8 rotXScale;          /* 0x28: initial X rotation, scaled by 256 */
    u8 pad29[0x2C - 0x29]; /* 0x29 */
} CannonClawPlacement;

STATIC_ASSERT(offsetof(CannonClawPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(CannonClawPlacement, rotXScale) == 0x28);
STATIC_ASSERT(sizeof(CannonClawPlacement) == 0x2C);

int cannonclaw_getExtraSize(void);
int cannonclaw_getObjectTypeId(void);
void cannonclaw_free(GameObject* obj);
void cannonclaw_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void cannonclaw_hitDetect(GameObject* obj);
void cannonclaw_update(GameObject* obj);
void cannonclaw_init(GameObject* obj, CannonClawPlacement* placement);
void cannonclaw_release(void);
void cannonclaw_initialise(void);

extern ObjectDescriptor gCannonClawObjDescriptor;

#endif /* DLLS_OBJECTS_207_CANNONCLAW_H_ */
