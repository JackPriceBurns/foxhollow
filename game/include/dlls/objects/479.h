#ifndef DLLS_OBJECTS_479_H_
#define DLLS_OBJECTS_479_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * Only the fields consumed by this DLL are modeled. The active-target retail
 * placement files are unavailable, so this does not claim a complete record
 * width.
 */
typedef struct Dll1DFPlacementView {
    ObjPlacement base;
    u8 rotationZByte;
    u8 rotationYByte;
    u8 rotationXByte;
    u8 scaleByte;
} Dll1DFPlacementView;

/* dll_1DF_getExtraSize() allocates the complete 0x28-byte block. */
typedef struct Dll1DFState {
    u8 unknown00[0x10];
    f32 unknown10;
    u8 unknown14[0x10];
    f32 spawnTimer;
} Dll1DFState;

STATIC_ASSERT(offsetof(Dll1DFPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dll1DFPlacementView, rotationZByte) == 0x18);
STATIC_ASSERT(offsetof(Dll1DFPlacementView, rotationYByte) == 0x19);
STATIC_ASSERT(offsetof(Dll1DFPlacementView, rotationXByte) == 0x1A);
STATIC_ASSERT(offsetof(Dll1DFPlacementView, scaleByte) == 0x1B);

STATIC_ASSERT(offsetof(Dll1DFState, unknown00) == 0x00);
STATIC_ASSERT(offsetof(Dll1DFState, unknown10) == 0x10);
STATIC_ASSERT(offsetof(Dll1DFState, unknown14) == 0x14);
STATIC_ASSERT(offsetof(Dll1DFState, spawnTimer) == 0x24);
STATIC_ASSERT(sizeof(Dll1DFState) == 0x28);

int dll_1DF_getExtraSize(void);
int dll_1DF_getObjectTypeId(void);
void dll_1DF_free(void);
void dll_1DF_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_1DF_hitDetect(void);
void dll_1DF_update(GameObject* obj);
void dll_1DF_init(GameObject* obj, const Dll1DFPlacementView* placement);
void dll_1DF_release(void);
void dll_1DF_initialise(void);

extern ObjectDescriptor gDll1DFObjDescriptor;

#endif /* DLLS_OBJECTS_479_H_ */
