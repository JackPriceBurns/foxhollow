#ifndef MUSYX_SYNTH_HANDLE_H_
#define MUSYX_SYNTH_HANDLE_H_

#include "types.h"

struct SynthStartRequest;

void seqVolume(u8 volume, u16 time, u32 seqId, u8 mode);
void seqCrossFade(struct SynthStartRequest* ci, u32* new_seqId, u8 irq_call);
u8* GetStreamValue(u8* input, u16* value0, s16* value1);

#endif /* MUSYX_SYNTH_HANDLE_H_ */
