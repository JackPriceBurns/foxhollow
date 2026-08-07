#ifndef MAIN_WARPVEC_H_
#define MAIN_WARPVEC_H_

#include "types.h"

typedef struct WarpVec
{
    f32 x;
    f32 y;
    f32 z;
    union {
        f32 pad;
        s32 valid;
    };
} WarpVec;

#endif /* MAIN_WARPVEC_H_ */
