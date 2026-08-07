#ifndef DLLS_OBJECTS_302_H_
#define DLLS_OBJECTS_302_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * Retail EN proves this complete 0x20-byte layout for all 145 class-0x61
 * placements handled by slot 302.
 */
typedef struct CFLightWallPlacement {
    ObjPlacement base; /* 0x00 */
    u8 initialRotZ;    /* 0x18: shifted left by eight */
    u8 initialRotY;    /* 0x19: shifted left by eight */
    u8 initialRotX;    /* 0x1A: shifted left by eight */
    u8 scale;          /* 0x1B: zero preserves the existing scale */
    u8 pad1C[4];       /* 0x1C */
} CFLightWallPlacement;

/*
 * The sole retail DBprotectZo placement uses slot 302 with a class-0x30
 * 0x24-byte record. Its first 0x20 bytes use the common layout above.
 */
typedef struct DBProtectZonePlacement {
    CFLightWallPlacement common; /* 0x00 */
    u8 pad20[4];                 /* 0x20 */
} DBProtectZonePlacement;

STATIC_ASSERT(offsetof(CFLightWallPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CFLightWallPlacement, initialRotZ) == 0x18);
STATIC_ASSERT(offsetof(CFLightWallPlacement, initialRotY) == 0x19);
STATIC_ASSERT(offsetof(CFLightWallPlacement, initialRotX) == 0x1A);
STATIC_ASSERT(offsetof(CFLightWallPlacement, scale) == 0x1B);
STATIC_ASSERT(offsetof(CFLightWallPlacement, pad1C) == 0x1C);
STATIC_ASSERT(sizeof(CFLightWallPlacement) == 0x20);

STATIC_ASSERT(offsetof(DBProtectZonePlacement, common) == 0x00);
STATIC_ASSERT(offsetof(DBProtectZonePlacement, pad20) == 0x20);
STATIC_ASSERT(sizeof(DBProtectZonePlacement) == 0x24);

int CFLightWall_getExtraSize(void);
int CFLightWall_getObjectTypeId(void);
void CFLightWall_free(void);
void CFLightWall_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void CFLightWall_hitDetect(void);
void CFLightWall_update(void);
void CFLightWall_init(GameObject* obj, CFLightWallPlacement* placement);
void CFLightWall_release(void);
void CFLightWall_initialise(void);

extern ObjectDescriptor gCFLightWallObjDescriptor;

#endif /* DLLS_OBJECTS_302_H_ */
