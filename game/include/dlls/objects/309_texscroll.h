#ifndef DLLS_OBJECTS_309_TEXSCROLL_H_
#define DLLS_OBJECTS_309_TEXSCROLL_H_

#include "dlls/object_descriptor.h"
#include "dlls/objects/texscroll_types.h"
#include "game/objects/object_fwd.h"

/* TexScroll_getExtraSize proves the complete 0x1C-byte allocation. */
typedef struct TexScrollState {
    u8 pad00[2];    /* 0x00 */
    s16 initLock;   /* 0x02 */
    s16 stepX;      /* 0x04 */
    s16 stepY;      /* 0x06 */
    s16 offsetX;    /* 0x08 */
    s16 offsetY;    /* 0x0A */
    s32 scrollSlot; /* 0x0C */
    u8 pad10[4];    /* 0x10 */
    s16 gameBit;    /* 0x14 */
    u8 pad16[2];    /* 0x16 */
    u8 flags;       /* 0x18 */
    u8 pad19[3];    /* 0x19 */
} TexScrollState;

STATIC_ASSERT(offsetof(TexScrollState, pad00) == 0x00);
STATIC_ASSERT(offsetof(TexScrollState, initLock) == 0x02);
STATIC_ASSERT(offsetof(TexScrollState, stepX) == 0x04);
STATIC_ASSERT(offsetof(TexScrollState, stepY) == 0x06);
STATIC_ASSERT(offsetof(TexScrollState, offsetX) == 0x08);
STATIC_ASSERT(offsetof(TexScrollState, offsetY) == 0x0A);
STATIC_ASSERT(offsetof(TexScrollState, scrollSlot) == 0x0C);
STATIC_ASSERT(offsetof(TexScrollState, pad10) == 0x10);
STATIC_ASSERT(offsetof(TexScrollState, gameBit) == 0x14);
STATIC_ASSERT(offsetof(TexScrollState, pad16) == 0x16);
STATIC_ASSERT(offsetof(TexScrollState, flags) == 0x18);
STATIC_ASSERT(offsetof(TexScrollState, pad19) == 0x19);
STATIC_ASSERT(sizeof(TexScrollState) == 0x1C);

int TexScroll_getExtraSize(void);
int TexScroll_getObjectTypeId(void);
void TexScroll_free(void);
void TexScroll_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void TexScroll_hitDetect(void);
void TexScroll_update(void);
void TexScroll_init(GameObject* obj, TexScrollPlacement* placement, int loadFlags);
void TexScroll_release(void);
void TexScroll_initialise(void);

extern ObjectDescriptor gTexscrollObjDescriptor;

#endif /* DLLS_OBJECTS_309_TEXSCROLL_H_ */
