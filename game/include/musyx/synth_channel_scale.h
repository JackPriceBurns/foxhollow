#ifndef MUSYX_SYNTH_CHANNEL_SCALE_H_
#define MUSYX_SYNTH_CHANNEL_SCALE_H_

#include "types.h"
#include "musyx/mcmd.h"

void seqHandle(u32 deltaTime);
void seqInit(void);
void synthSetStudioChannelScale(int value, u8 bank, u8 key);
int synthGetVoiceSlotChannelScale(McmdVoiceState *state);
void synthInitPortamento(McmdVoiceState *state);

#endif /* MUSYX_SYNTH_CHANNEL_SCALE_H_ */
