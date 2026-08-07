#ifndef DLLS_OBJECTS_393_CCSHARPCLAW_H_
#define DLLS_OBJECTS_393_CCSHARPCLAW_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct CCSharpClawPadPlacement {
    ObjPlacement base;
    u8 rotXByte;
    u8 unknown19;
    s16 activationGameBit;
} CCSharpClawPadPlacement;

STATIC_ASSERT(sizeof(CCSharpClawPadPlacement) == 0x1C);
STATIC_ASSERT(offsetof(CCSharpClawPadPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CCSharpClawPadPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(CCSharpClawPadPlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(CCSharpClawPadPlacement, activationGameBit) == 0x1A);

typedef struct CCSharpClawPadState {
    f32 helpTimer;
} CCSharpClawPadState;

STATIC_ASSERT(sizeof(CCSharpClawPadState) == 0x04);
STATIC_ASSERT(offsetof(CCSharpClawPadState, helpTimer) == 0x00);

extern ObjectDescriptor gCCSharpClawPadObjDescriptor;

int ccSharpClawPad_getExtraSize(void);
void ccSharpClawPad_update(GameObject* obj);
void ccSharpClawPad_init(GameObject* obj, const CCSharpClawPadPlacement* placement);

#endif /* DLLS_OBJECTS_393_CCSHARPCLAW_H_ */
