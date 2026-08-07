#ifndef DLLS_OBJECTS_394_CCPEDSTAL_H_
#define DLLS_OBJECTS_394_CCPEDSTAL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct CCPedestalPlacement {
    ObjPlacement base;
    u8 unknown18[0x1A - 0x18];
    u8 rotXByte;
    u8 unknown1B[0x20 - 0x1B];
} CCPedestalPlacement;

STATIC_ASSERT(sizeof(CCPedestalPlacement) == 0x20);
STATIC_ASSERT(offsetof(CCPedestalPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CCPedestalPlacement, base.ident) == 0x14);
STATIC_ASSERT(offsetof(CCPedestalPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(CCPedestalPlacement, rotXByte) == 0x1A);
STATIC_ASSERT(offsetof(CCPedestalPlacement, unknown1B) == 0x1B);

typedef struct CCPedestalState CCPedestalState;
typedef void (*CCPedestalVariantUpdateFn)(GameObject* obj, CCPedestalState* state);

struct CCPedestalState {
    CCPedestalVariantUpdateFn variantUpdate;
    s16 activationGameBit;
    u8 pendingGameBitFlags;
    u8 unknown07;
};

STATIC_ASSERT(sizeof(CCPedestalState) == 0x08);
STATIC_ASSERT(offsetof(CCPedestalState, variantUpdate) == 0x00);
STATIC_ASSERT(offsetof(CCPedestalState, activationGameBit) == 0x04);
STATIC_ASSERT(offsetof(CCPedestalState, pendingGameBitFlags) == 0x06);
STATIC_ASSERT(offsetof(CCPedestalState, unknown07) == 0x07);

extern ObjectDescriptor gCCPedestalObjDescriptor;

int ccPedestal_getExtraSize(void);
void ccPedestal_updateFireGemGate(GameObject* obj, CCPedestalState* state);
void ccPedestal_updateFireGemSource(GameObject* obj, CCPedestalState* state);
void ccPedestal_update(GameObject* obj);
void ccPedestal_init(GameObject* obj, const CCPedestalPlacement* placement);

#endif /* DLLS_OBJECTS_394_CCPEDSTAL_H_ */
