#ifndef DLLS_OBJECTS_511_H_
#define DLLS_OBJECTS_511_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * Active EN does not establish a complete retail placement width. This view
 * models only the signed rotation byte consumed by dll_1FF_init().
 */
typedef struct Dll1FFPlacementView {
    ObjPlacement base;
    s8 rotationXByte;
} Dll1FFPlacementView;

/* dll_1FF_getExtraSize() allocates the complete eight-byte state block. */
typedef struct Dll1FFState {
    s16 messageParamLow;
    s16 messageParamHigh;
    u8 unknown04;
    s8 carryState;
    s8 messagePending;
    u8 unknown07;
} Dll1FFState;

STATIC_ASSERT(offsetof(Dll1FFPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dll1FFPlacementView, rotationXByte) == 0x18);

STATIC_ASSERT(offsetof(Dll1FFState, messageParamLow) == 0x00);
STATIC_ASSERT(offsetof(Dll1FFState, messageParamHigh) == 0x02);
STATIC_ASSERT(offsetof(Dll1FFState, unknown04) == 0x04);
STATIC_ASSERT(offsetof(Dll1FFState, carryState) == 0x05);
STATIC_ASSERT(offsetof(Dll1FFState, messagePending) == 0x06);
STATIC_ASSERT(offsetof(Dll1FFState, unknown07) == 0x07);
STATIC_ASSERT(sizeof(Dll1FFState) == 0x08);

int dll_1FF_getExtraSize(void);
int dll_1FF_getObjectTypeId(GameObject* obj);
void dll_1FF_free(void);
void dll_1FF_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_1FF_hitDetect(void);
void dll_1FF_update(GameObject* obj);
void dll_1FF_init(GameObject* obj, const Dll1FFPlacementView* placement);
void dll_1FF_release(void);
void dll_1FF_initialise(void);

extern ObjectDescriptor gDll1FFObjDescriptor;

#endif /* DLLS_OBJECTS_511_H_ */
