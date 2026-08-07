#ifndef DLLS_OBJECTS_236_INFOPOINT_H_
#define DLLS_OBJECTS_236_INFOPOINT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

struct GameTextDef;
struct ObjSeqState;

typedef struct InfoPointRenderBounds {
    s32 x;      /* 0x00 */
    s32 y;      /* 0x04 */
    s32 width;  /* 0x08 */
    s32 height; /* 0x0C */
} InfoPointRenderBounds;

typedef struct InfoPointSharedResources {
    void* fontTexture; /* 0x00 */
    u32 reserved[5];   /* 0x04 */
} InfoPointSharedResources;

typedef struct InfoPointState {
    struct GameTextDef* text;            /* 0x00 */
    char* firstString;                   /* 0x04 */
    InfoPointRenderBounds* renderBounds; /* 0x08 */
    s32 displayTimer;                    /* 0x0C */
    u8 unk10;                            /* 0x10: copied from placement+0x1B */
    u8 pad11[5];                         /* 0x11 */
    s16 sequenceState;                   /* 0x16: controlled by sequence events 1 and 2 */
    s32 unk18;                           /* 0x18: initialized to 2 */
    u8 pad1C[4];                         /* 0x1C */
} InfoPointState;

typedef struct InfoPointPlacement {
    ObjPlacement base; /* 0x00 */
    u16 textId;        /* 0x18 */
    u8 pad1A;          /* 0x1A */
    u8 unk1B;          /* 0x1B */
    u8 rotXByte;       /* 0x1C: rotX in 1/256 turns */
    u8 pad1D[3];       /* 0x1D */
} InfoPointPlacement;

STATIC_ASSERT(offsetof(InfoPointRenderBounds, x) == 0x0);
STATIC_ASSERT(offsetof(InfoPointRenderBounds, y) == 0x4);
STATIC_ASSERT(offsetof(InfoPointRenderBounds, width) == 0x8);
STATIC_ASSERT(offsetof(InfoPointRenderBounds, height) == 0xC);
STATIC_ASSERT(sizeof(InfoPointRenderBounds) == 0x10);

STATIC_ASSERT(offsetof(InfoPointSharedResources, fontTexture) == 0x0);
STATIC_ASSERT(offsetof(InfoPointSharedResources, reserved) == 0x4);
STATIC_ASSERT(sizeof(InfoPointSharedResources) == 0x18);

STATIC_ASSERT(offsetof(InfoPointState, text) == 0x0);
STATIC_ASSERT(offsetof(InfoPointState, firstString) == 0x4);
STATIC_ASSERT(offsetof(InfoPointState, renderBounds) == 0x8);
STATIC_ASSERT(offsetof(InfoPointState, displayTimer) == 0xC);
STATIC_ASSERT(offsetof(InfoPointState, unk10) == 0x10);
STATIC_ASSERT(offsetof(InfoPointState, pad11) == 0x11);
STATIC_ASSERT(offsetof(InfoPointState, sequenceState) == 0x16);
STATIC_ASSERT(offsetof(InfoPointState, unk18) == 0x18);
STATIC_ASSERT(offsetof(InfoPointState, pad1C) == 0x1C);
STATIC_ASSERT(sizeof(InfoPointState) == 0x20);

STATIC_ASSERT(offsetof(InfoPointPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(InfoPointPlacement, textId) == 0x18);
STATIC_ASSERT(offsetof(InfoPointPlacement, pad1A) == 0x1A);
STATIC_ASSERT(offsetof(InfoPointPlacement, unk1B) == 0x1B);
STATIC_ASSERT(offsetof(InfoPointPlacement, rotXByte) == 0x1C);
STATIC_ASSERT(offsetof(InfoPointPlacement, pad1D) == 0x1D);
STATIC_ASSERT(sizeof(InfoPointPlacement) == 0x20);

int InfoPoint_SeqFn(GameObject* obj, int unused, struct ObjSeqState* animUpdate);
int InfoPoint_getExtraSize(void);
int InfoPoint_getObjectTypeId(void);
void InfoPoint_free(GameObject* obj);
void InfoPoint_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void InfoPoint_hitDetect(GameObject* obj);
void InfoPoint_update(GameObject* obj);
void InfoPoint_init(GameObject* obj, InfoPointPlacement* placement);
void InfoPoint_release(void);
void InfoPoint_initialise(void);

extern InfoPointRenderBounds gInfoPointRenderBounds;
extern InfoPointSharedResources gInfoPointSharedResources;
extern ObjectDescriptor gInfoPointObjDescriptor;

#endif /* DLLS_OBJECTS_236_INFOPOINT_H_ */
