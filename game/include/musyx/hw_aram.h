#ifndef MUSYX_HW_ARAM_H_
#define MUSYX_HW_ARAM_H_

#include "types.h"

u32 hwExitStream(u32 value);
void hwInitSampleMem(u32 baseAddr, u32 length);
void hwExitSampleMem(void);
#endif /* MUSYX_HW_ARAM_H_ */
