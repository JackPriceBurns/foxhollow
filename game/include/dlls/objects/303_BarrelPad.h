#ifndef DLLS_OBJECTS_303_BARRELPAD_H_
#define DLLS_OBJECTS_303_BARRELPAD_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * All seven retail EN placements across the BarrelPad and NWCallOFEld
 * families use this complete fixed-width 0x20-byte record.
 */
typedef struct BarrelPadPlacement {
    ObjPlacement base; /* 0x00 */
    u8 initialRotZ;    /* 0x18: shifted left by eight */
    u8 initialRotY;    /* 0x19: shifted left by eight */
    u8 initialRotX;    /* 0x1A: shifted left by eight */
    u8 scale;          /* 0x1B: zero preserves the existing scale */
    u8 pad1C[4];       /* 0x1C */
} BarrelPadPlacement;

STATIC_ASSERT(offsetof(BarrelPadPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(BarrelPadPlacement, initialRotZ) == 0x18);
STATIC_ASSERT(offsetof(BarrelPadPlacement, initialRotY) == 0x19);
STATIC_ASSERT(offsetof(BarrelPadPlacement, initialRotX) == 0x1A);
STATIC_ASSERT(offsetof(BarrelPadPlacement, scale) == 0x1B);
STATIC_ASSERT(offsetof(BarrelPadPlacement, pad1C) == 0x1C);
STATIC_ASSERT(sizeof(BarrelPadPlacement) == 0x20);

int BarrelPad_getExtraSize(void);
int BarrelPad_getObjectTypeId(void);
void BarrelPad_free(void);
void BarrelPad_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void BarrelPad_hitDetect(void);
void BarrelPad_update(GameObject* obj);
void BarrelPad_init(GameObject* obj, BarrelPadPlacement* placement);
void BarrelPad_release(void);
void BarrelPad_initialise(void);

extern ObjectDescriptor gBarrelPadObjDescriptor;

#endif /* DLLS_OBJECTS_303_BARRELPAD_H_ */
