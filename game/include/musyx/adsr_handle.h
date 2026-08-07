#ifndef MUSYX_ADSR_HANDLE_H_
#define MUSYX_ADSR_HANDLE_H_

#include "musyx/adsr.h"

int adsrStartRelease(ADSR_VARS *adsr, u32 divisor);
int adsrRelease(ADSR_VARS *adsr);
u32 adsrHandle(ADSR_VARS *adsr, u16 *out1, u16 *out2);

#endif /* MUSYX_ADSR_HANDLE_H_ */
