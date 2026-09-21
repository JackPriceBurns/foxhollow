#ifndef DLLS_OBJECTS_199_DIM2ROOFRUB_H_
#define DLLS_OBJECTS_199_DIM2ROOFRUB_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct DIM2RoofRubPlacement {
    ObjPlacement base;     /* 0x00 */
    s16 animDataIndex;     /* 0x18: animation-data selector; -1 skips sequence updates */
    s16 sequenceGameBit;   /* 0x1A: copied into ObjSeqState.gameBit */
    s16 unk1C;             /* 0x1C */
    s16 unk1E;             /* 0x1E */
    s16 unk20;             /* 0x20 */
    u8 pad22[0x24 - 0x22]; /* 0x22 */
    u8 positionDamping;    /* 0x24: posOffsetDecay = 1 / (1 + positionDamping) */
    u8 pad25[0x2C - 0x25]; /* 0x25 */
    s16 unk2C;             /* 0x2C */
    u8 pad2E[0x30 - 0x2E]; /* 0x2E */
} DIM2RoofRubPlacement;

typedef struct DIM2RoofRubState {
    ObjSeqState sequence;     /* 0x000 */
    u8 pad138[0x140 - 0x138]; /* 0x138 */
} DIM2RoofRubState;

STATIC_ASSERT(offsetof(DIM2RoofRubPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(DIM2RoofRubPlacement, animDataIndex) == 0x18);
STATIC_ASSERT(offsetof(DIM2RoofRubPlacement, sequenceGameBit) == 0x1A);
STATIC_ASSERT(offsetof(DIM2RoofRubPlacement, positionDamping) == 0x24);
STATIC_ASSERT(sizeof(DIM2RoofRubPlacement) == 0x30);
STATIC_ASSERT(offsetof(DIM2RoofRubState, sequence) == 0x0);
STATIC_ASSERT(sizeof(DIM2RoofRubState) == 0x140);

void dim2roofrub_spawnEffects(GameObject* obj);
int dim2roofrub_getExtraSize(void);
void dim2roofrub_free(GameObject* obj);
void dim2roofrub_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5);
void dim2roofrub_update(GameObject* obj);
void dim2roofrub_init(GameObject* obj, DIM2RoofRubPlacement* placement);

extern ObjectDescriptor gDIM2RoofRubObjDescriptor;

#endif /* DLLS_OBJECTS_199_DIM2ROOFRUB_H_ */
