#ifndef DLLS_OBJECTS_198_ANIMATEDOBJ_H_
#define DLLS_OBJECTS_198_ANIMATEDOBJ_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct AnimatedObjPlacement {
    ObjPlacement base;     /* 0x00 */
    s16 animDataIndex;     /* 0x18: animation-data selector; -1 disables the object */
    s16 sequenceGameBit;   /* 0x1A: copied into ObjSeqState.gameBit */
    s16 unk1C;             /* 0x1C */
    s16 unk1E;             /* 0x1E */
    s16 unk20;             /* 0x20 */
    u8 pad22[0x24 - 0x22]; /* 0x22 */
    u8 positionDamping;    /* 0x24: posOffsetDecay = 1 / (1 + positionDamping) */
    u8 pad25[0x2C - 0x25]; /* 0x25 */
    s16 unk2C;             /* 0x2C */
    u8 pad2E[0x30 - 0x2E]; /* 0x2E */
} AnimatedObjPlacement;

typedef struct AnimatedObjState {
    ObjSeqState sequence;     /* 0x000 */
    u8 pad138[0x140 - 0x138]; /* 0x138 */
} AnimatedObjState;

STATIC_ASSERT(offsetof(AnimatedObjPlacement, animDataIndex) == 0x18);
STATIC_ASSERT(offsetof(AnimatedObjPlacement, sequenceGameBit) == 0x1A);
STATIC_ASSERT(offsetof(AnimatedObjPlacement, positionDamping) == 0x24);
STATIC_ASSERT(sizeof(AnimatedObjPlacement) == 0x30);
STATIC_ASSERT(offsetof(AnimatedObjState, sequence) == 0x0);
STATIC_ASSERT(sizeof(AnimatedObjState) == 0x140);

int animatedobj_getExtraSize(void);
void animatedobj_free(GameObject* obj, int clearSequence);
void animatedobj_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 unusedVisible);
void animatedobj_update(GameObject* obj);
void animatedobj_init(GameObject* obj, AnimatedObjPlacement* placement);

extern ObjectDescriptor gAnimatedObjDescriptor;

#endif /* DLLS_OBJECTS_198_ANIMATEDOBJ_H_ */
