#ifndef MAIN_DLL_DFP_TYPES_H_
#define MAIN_DLL_DFP_TYPES_H_

#include "global.h"
#include "types.h"
#include "main/gamebit_latch.h"

typedef struct DfpFlags7 {
    u8 b80 : 1;
    u8 b40 : 1;
    u8 b20 : 1;
    u8 rest : 5;
} DfpFlags7;

typedef struct DfpLevelControlSfxState {
    u8 triggerD5d : 1;
    u8 triggerD59 : 1;
    u8 triggerD5a : 1;
    u8 unused : 5;
} DfpLevelControlSfxState;

typedef struct DfpLevelControlState {
    s16 zappedTimer;
    s16 mode;
    u16 unused04;
    u8 previousPuzzlePadState;
    DfpLevelControlSfxState previousSfxState;
    int musicLatch;
} DfpLevelControlState;

typedef struct DfpSeqPointState {
    f32 triggerRadius;    /* def+0x1A */
    s16 conditionGameBit; /* 0x04: def+0x1E */
    s16 disableGameBit;   /* 0x06: def+0x20 */
    s16 sequenceId;       /* 0x08: def+0x1C */
    u8 unk0A[3];
    u8 doneLatch;   /* 0x0D */
    u8 triggerMode; /* 0x0E: def+0x19 */
    DfpFlags7 flags0F;
} DfpSeqPointState;

STATIC_ASSERT(sizeof(DfpLevelControlState) == 0xC);

#endif
