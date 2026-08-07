#ifndef DLLS_OBJECTS_391_CCQUEEN_H_
#define DLLS_OBJECTS_391_CCQUEEN_H_

#include "dlls/object_descriptor.h"
#include "main/objprint_character_api.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/dll_002E_moveLib.h"

typedef struct CCQueenPlacement {
    ObjPlacement base;
    u8 unknown18[2];
    u8 rotXByte;
    u8 unknown1B[0x20 - 0x1B];
} CCQueenPlacement;

STATIC_ASSERT(sizeof(CCQueenPlacement) == 0x20);
STATIC_ASSERT(offsetof(CCQueenPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CCQueenPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(CCQueenPlacement, rotXByte) == 0x1A);
STATIC_ASSERT(offsetof(CCQueenPlacement, unknown1B) == 0x1B);

typedef struct CCQueenState {
    MoveLibState moveLib;
    CharacterEyeAnimState eyeAnimState;
    u8 pad64C[0x8];
} CCQueenState;

STATIC_ASSERT(sizeof(CCQueenState) == 0x654);
STATIC_ASSERT(offsetof(CCQueenState, moveLib) == 0x000);
STATIC_ASSERT(offsetof(CCQueenState, eyeAnimState) == 0x624);

extern ObjectDescriptor gCCQueenObjDescriptor;

int ccQueen_getExtraSize(void);
void ccQueen_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 unusedVisible);
void ccQueen_update(GameObject* obj);
void ccQueen_init(GameObject* obj, const CCQueenPlacement* placement);

#endif /* DLLS_OBJECTS_391_CCQUEEN_H_ */
