#ifndef MUSYX_HW_VOICE_PARAMS_H_
#define MUSYX_HW_VOICE_PARAMS_H_

#include "types.h"

void hwSetPitch(u32 voice, u16 pitch);
void hwSetSRCType(u32 slot, u8 value);
void hwSetPolyPhaseFilter(u32 slot, u8 value);
void hwSetITDMode(u32 slot, u8 value);

#endif /* MUSYX_HW_VOICE_PARAMS_H_ */
