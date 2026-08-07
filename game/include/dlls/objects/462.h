#ifndef DLLS_OBJECTS_462_H_
#define DLLS_OBJECTS_462_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * DLL 0x1CE has no active-target OBJECTS.bin definitions. This is the complete
 * placement prefix accessed by its callbacks, not a retail-proven allocation
 * width.
 */
typedef struct Dll1CEPlacementView {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19;
    s16 contentsSpawnBitValue;
    u8 unknown1C[2];
    s16 openedGameBit;
} Dll1CEPlacementView;

typedef struct Dll1CEState {
    f32 openProgress;
    f32 openVelocity;
    u8 opened;
    s8 unlockCountdown;
    u8 unknown0A[2];
} Dll1CEState;

STATIC_ASSERT(offsetof(Dll1CEPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dll1CEPlacementView, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(Dll1CEPlacementView, unknown19) == 0x19);
STATIC_ASSERT(offsetof(Dll1CEPlacementView, contentsSpawnBitValue) == 0x1A);
STATIC_ASSERT(offsetof(Dll1CEPlacementView, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(Dll1CEPlacementView, openedGameBit) == 0x1E);

STATIC_ASSERT(offsetof(Dll1CEState, openProgress) == 0x00);
STATIC_ASSERT(offsetof(Dll1CEState, openVelocity) == 0x04);
STATIC_ASSERT(offsetof(Dll1CEState, opened) == 0x08);
STATIC_ASSERT(offsetof(Dll1CEState, unlockCountdown) == 0x09);
STATIC_ASSERT(offsetof(Dll1CEState, unknown0A) == 0x0A);
STATIC_ASSERT(sizeof(Dll1CEState) == 0x0C);

int dll_1CE_getExtraSize(void);
int dll_1CE_getObjectTypeId(void);
void dll_1CE_free(void);
void dll_1CE_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_1CE_hitDetect(void);
void dll_1CE_update(GameObject* obj);
void dll_1CE_init(GameObject* obj, const Dll1CEPlacementView* placement);
void dll_1CE_release(void);
void dll_1CE_initialise(void);

extern void* gDll1CEResource;
extern ObjectDescriptor gDll1CEObjDescriptor;

#endif /* DLLS_OBJECTS_462_H_ */
