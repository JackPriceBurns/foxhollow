#ifndef MUSYX_SYNTH_VOLUME_H_
#define MUSYX_SYNTH_VOLUME_H_

#include "types.h"

void synthVolume(u8 volume, u16 timeMs, u8 target, u8 action, u32 handle);
int synthIsFadeOutActive(u8 voiceIdx);
void synthSetMusicVolumeType(u8 voiceIdx, u8 value);
u32 synthHWMessageHandler(u32 message, u32 voiceId);

#endif /* MUSYX_SYNTH_VOLUME_H_ */
