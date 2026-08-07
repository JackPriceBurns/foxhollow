#ifndef DLLS_OBJECTS_257_TRICKYGUARD_H_
#define DLLS_OBJECTS_257_TRICKYGUARD_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Known setup-record prefix; the complete retail record width is not proven. */
typedef struct TrickyGuardPlacement {
    ObjPlacement base; /* 0x00 */
    u8 rotXByte;       /* 0x18: high byte of anim.rotX */
    u8 pad19;          /* 0x19 */
    s16 armingGameBit; /* 0x1A: -1 = always armed */
} TrickyGuardPlacement;

STATIC_ASSERT(offsetof(TrickyGuardPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(TrickyGuardPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(TrickyGuardPlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(TrickyGuardPlacement, armingGameBit) == 0x1A);

void TrickyGuard_update(GameObject* obj);
void TrickyGuard_init(GameObject* obj, TrickyGuardPlacement* placement);

extern ObjectDescriptor gTrickyGuardObjDescriptor;

#endif /* DLLS_OBJECTS_257_TRICKYGUARD_H_ */
