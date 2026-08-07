#ifndef DLLS_OBJECTS_276_IMMULTISEQ_H_
#define DLLS_OBJECTS_276_IMMULTISEQ_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define IM_MULTI_SEQ_STATE_SIZE 0x2
#define IM_MULTI_SEQ_STEP_COUNT 4

typedef struct ObjSeqState ObjSeqState;

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct IMMultiSeqPlacement {
    ObjPlacement base;                               /* 0x00 */
    s16 completionGameBits[IM_MULTI_SEQ_STEP_COUNT]; /* 0x18 */
    s16 activeGameBits[IM_MULTI_SEQ_STEP_COUNT];     /* 0x20 */
    u8 initialYaw;                                   /* 0x28: rotation in 1/256 turns */
    u8 pad29;                                        /* 0x29 */
    u8 modelBankIndex;                               /* 0x2A */
    u8 pad2B;                                        /* 0x2B */
    s8 sequenceIds[IM_MULTI_SEQ_STEP_COUNT];         /* 0x2C: or -1 */
    u8 polarityMask;                                 /* 0x30 */
} IMMultiSeqPlacement;

typedef struct IMMultiSeqState {
    u8 step;
    u8 flags;
} IMMultiSeqState;

STATIC_ASSERT(offsetof(IMMultiSeqPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(IMMultiSeqPlacement, completionGameBits) == 0x18);
STATIC_ASSERT(offsetof(IMMultiSeqPlacement, activeGameBits) == 0x20);
STATIC_ASSERT(offsetof(IMMultiSeqPlacement, initialYaw) == 0x28);
STATIC_ASSERT(offsetof(IMMultiSeqPlacement, pad29) == 0x29);
STATIC_ASSERT(offsetof(IMMultiSeqPlacement, modelBankIndex) == 0x2A);
STATIC_ASSERT(offsetof(IMMultiSeqPlacement, pad2B) == 0x2B);
STATIC_ASSERT(offsetof(IMMultiSeqPlacement, sequenceIds) == 0x2C);
STATIC_ASSERT(offsetof(IMMultiSeqPlacement, polarityMask) == 0x30);

STATIC_ASSERT(offsetof(IMMultiSeqState, step) == 0x0);
STATIC_ASSERT(offsetof(IMMultiSeqState, flags) == 0x1);
STATIC_ASSERT(sizeof(IMMultiSeqState) == IM_MULTI_SEQ_STATE_SIZE);

int IMMultiSeq_animEventCallback(GameObject* obj, int* unused, ObjSeqState* animUpdate);
int IMMultiSeq_getExtraSize(void);
int IMMultiSeq_getObjectTypeId(void);
void IMMultiSeq_free(GameObject* obj);
void IMMultiSeq_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void IMMultiSeq_hitDetect(void);
void IMMultiSeq_update(GameObject* obj);
void IMMultiSeq_init(GameObject* obj, IMMultiSeqPlacement* placement);
void IMMultiSeq_release(void);
void IMMultiSeq_initialise(void);

extern ObjectDescriptor gIMMultiSeqObjDescriptor;

#endif /* DLLS_OBJECTS_276_IMMULTISEQ_H_ */
