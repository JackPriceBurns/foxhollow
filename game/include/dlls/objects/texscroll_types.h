#ifndef DLLS_OBJECTS_TEXSCROLL_TYPES_H_
#define DLLS_OBJECTS_TEXSCROLL_TYPES_H_

#include "game/objects/object_setup.h"

/*
 * All 226 active retail EN placements shared by texscroll2 and texscroll
 * use this complete fixed-width 0x24-byte record.
 */
typedef struct TexScrollPlacement {
    ObjPlacement base;     /* 0x00 */
    s16 textureTableIndex; /* 0x18: negated index into table 0x0E */
    s16 gameBit;           /* 0x1A */
    s8 secondaryStepX;     /* 0x1C */
    s8 secondaryStepY;     /* 0x1D */
    s8 stepX;              /* 0x1E */
    s8 stepY;              /* 0x1F */
    u8 pad20[4];           /* 0x20 */
} TexScrollPlacement;

STATIC_ASSERT(offsetof(TexScrollPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(TexScrollPlacement, textureTableIndex) == 0x18);
STATIC_ASSERT(offsetof(TexScrollPlacement, gameBit) == 0x1A);
STATIC_ASSERT(offsetof(TexScrollPlacement, secondaryStepX) == 0x1C);
STATIC_ASSERT(offsetof(TexScrollPlacement, secondaryStepY) == 0x1D);
STATIC_ASSERT(offsetof(TexScrollPlacement, stepX) == 0x1E);
STATIC_ASSERT(offsetof(TexScrollPlacement, stepY) == 0x1F);
STATIC_ASSERT(offsetof(TexScrollPlacement, pad20) == 0x20);
STATIC_ASSERT(sizeof(TexScrollPlacement) == 0x24);

#endif /* DLLS_OBJECTS_TEXSCROLL_TYPES_H_ */
