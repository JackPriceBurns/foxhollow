#ifndef MAIN_AUDIO_AUDIO_CONTROL_API_H_
#define MAIN_AUDIO_AUDIO_CONTROL_API_H_

#include "types.h"

void audioStopByMask(int mask);
void audioReset(void);
int audioIsResetting(void);
void audioStopAll(void);
void audioUpdate(void);
void audioSetVolumes(int volume, int time, int musicFlag, int fxFlag, int streamFlag);
void audioSetSoundMode(int mode, u8 forceFlag);
int audioInit(void);
void Sfx_SetObjectReverbPreset(u32 preset);
int Music_GetTrackCount(void);
void AudioStream_Nop(int unused);

#endif /* MAIN_AUDIO_AUDIO_CONTROL_API_H_ */
