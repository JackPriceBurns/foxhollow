#ifndef DLLS_OBJECTS_463_H_
#define DLLS_OBJECTS_463_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * Slot 0x1CF has no active-target OBJECTS.bin definition. This is the complete
 * 0x20-byte prefix interpreted by its init callback, not a claimed whole
 * retail placement record.
 */
typedef struct Dll1CFPlacementView {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19;
    s16 rotationYDegrees;
    u8 unknown1C[2];
    s16 gateGameBit;
} Dll1CFPlacementView;

STATIC_ASSERT(offsetof(Dll1CFPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dll1CFPlacementView, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(Dll1CFPlacementView, unknown19) == 0x19);
STATIC_ASSERT(offsetof(Dll1CFPlacementView, rotationYDegrees) == 0x1A);
STATIC_ASSERT(offsetof(Dll1CFPlacementView, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(Dll1CFPlacementView, gateGameBit) == 0x1E);

int dll_1CF_getExtraSize(void);
int dll_1CF_getObjectTypeId(void);
void dll_1CF_free(void);
void dll_1CF_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_1CF_hitDetect(void);
void dll_1CF_update(void);
void dll_1CF_init(GameObject* obj, const Dll1CFPlacementView* placement);
void dll_1CF_release(void);
void dll_1CF_initialise(void);

extern ObjectDescriptor gDll1CFObjDescriptor;

#endif /* DLLS_OBJECTS_463_H_ */
