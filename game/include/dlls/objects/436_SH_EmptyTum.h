#ifndef DLLS_OBJECTS_436_SH_EMPTY_TUM_H_
#define DLLS_OBJECTS_436_SH_EMPTY_TUM_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* The accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct ShEmptyTumbleweedPlacement {
    ObjPlacement base;
    u8 rotZByte;
    u8 rotYByte;
    u8 rotXByte;
    u8 unknown1B;
    f32 scale;
} ShEmptyTumbleweedPlacement;

STATIC_ASSERT(offsetof(ShEmptyTumbleweedPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ShEmptyTumbleweedPlacement, rotZByte) == 0x18);
STATIC_ASSERT(offsetof(ShEmptyTumbleweedPlacement, rotYByte) == 0x19);
STATIC_ASSERT(offsetof(ShEmptyTumbleweedPlacement, rotXByte) == 0x1A);
STATIC_ASSERT(offsetof(ShEmptyTumbleweedPlacement, unknown1B) == 0x1B);
STATIC_ASSERT(offsetof(ShEmptyTumbleweedPlacement, scale) == 0x1C);

void SH_EmptyTumbleW_update(GameObject* obj);
void SH_EmptyTumbleW_init(GameObject* obj, ShEmptyTumbleweedPlacement* placement);

extern ObjectDescriptor gSH_EmptyTumbleWObjDescriptor;

#endif /* DLLS_OBJECTS_436_SH_EMPTY_TUM_H_ */
