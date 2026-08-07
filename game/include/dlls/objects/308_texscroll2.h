#ifndef DLLS_OBJECTS_308_TEXSCROLL2_H_
#define DLLS_OBJECTS_308_TEXSCROLL2_H_

#include "dlls/object_descriptor.h"
#include "dlls/objects/texscroll_types.h"
#include "game/objects/object_fwd.h"

/* TexScroll2_getExtraSize proves the complete 0x18-byte allocation. */
typedef struct TexScroll2State {
    u8 pad00[8];              /* 0x00 */
    s32 gameBit;              /* 0x08 */
    s32 previousGameBitValue; /* 0x0C */
    u8 needsApply;            /* 0x10 */
    s8 stepX;                 /* 0x11 */
    s8 stepY;                 /* 0x12 */
    s8 secondaryStepX;        /* 0x13 */
    s8 secondaryStepY;        /* 0x14 */
    u8 pad15[3];              /* 0x15 */
} TexScroll2State;

STATIC_ASSERT(offsetof(TexScroll2State, pad00) == 0x00);
STATIC_ASSERT(offsetof(TexScroll2State, gameBit) == 0x08);
STATIC_ASSERT(offsetof(TexScroll2State, previousGameBitValue) == 0x0C);
STATIC_ASSERT(offsetof(TexScroll2State, needsApply) == 0x10);
STATIC_ASSERT(offsetof(TexScroll2State, stepX) == 0x11);
STATIC_ASSERT(offsetof(TexScroll2State, stepY) == 0x12);
STATIC_ASSERT(offsetof(TexScroll2State, secondaryStepX) == 0x13);
STATIC_ASSERT(offsetof(TexScroll2State, secondaryStepY) == 0x14);
STATIC_ASSERT(offsetof(TexScroll2State, pad15) == 0x15);
STATIC_ASSERT(sizeof(TexScroll2State) == 0x18);

void TexScroll2_setStepY(GameObject* obj, s8 stepY);
void TexScroll2_applyMapTextureScroll(GameObject* obj, TexScroll2State* state);
int TexScroll2_getExtraSize(void);
int TexScroll2_getObjectTypeId(void);
void TexScroll2_free(void);
void TexScroll2_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void TexScroll2_hitDetect(void);
void TexScroll2_update(GameObject* obj);
void TexScroll2_init(GameObject* obj, TexScrollPlacement* placement, int loadFlags);
void TexScroll2_release(void);
void TexScroll2_initialise(void);

extern ObjectDescriptor11WithPadding gTexscroll2ObjDescriptor;

#endif /* DLLS_OBJECTS_308_TEXSCROLL2_H_ */
