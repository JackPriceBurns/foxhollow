#ifndef MAIN_DLL_DFP_TYPES_H_
#define MAIN_DLL_DFP_TYPES_H_

#include "types.h"

typedef struct DfpFlags7
{
    u8 b80 : 1;
    u8 b40 : 1;
    u8 b20 : 1;
    u8 rest : 5;
} DfpFlags7;

typedef struct DfpLevelControlState
{
    s16 timer; /* counts down by timeDelta; set 300 on gamebit 1509 */
    s16 mode; /* 1..2, from def+0x1A */
    u8 unk04[2];
    u8 sfxLatch; /* gamebit-1589 one-shot latch */
    DfpFlags7 flags07;
    u8 gameBitLatches[4]; /* persistent latch state for the 3 GameBitLatch_Update calls in update */
} DfpLevelControlState;

typedef struct DfpSeqPointState
{
    f32 triggerRadius; /* def+0x1A */
    s16 conditionGameBit; /* 0x04: def+0x1E */
    s16 disableGameBit; /* 0x06: def+0x20 */
    s16 sequenceId; /* 0x08: def+0x1C */
    u8 unk0A[3];
    u8 doneLatch; /* 0x0D */
    u8 triggerMode; /* 0x0E: def+0x19 */
    DfpFlags7 flags0F;
} DfpSeqPointState;

#endif
