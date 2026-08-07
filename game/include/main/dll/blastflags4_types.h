#ifndef MAIN_DLL_BLASTFLAGS4_TYPES_H_
#define MAIN_DLL_BLASTFLAGS4_TYPES_H_

#include "types.h"

typedef struct
    {
        u8 b80 : 1;
    } BlastFlags4;

typedef struct GCRobotBlastState
{
    int mode; /* def+0x19 */
    BlastFlags4 flags04;
    u8 unk05[3];
} GCRobotBlastState;

#endif
