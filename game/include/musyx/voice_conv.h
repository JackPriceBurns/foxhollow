#ifndef MUSYX_VOICE_CONV_H_
#define MUSYX_VOICE_CONV_H_

#include "types.h"
#include "musyx/voice_manage.h"

void voiceInitLastStarted(void);
int voiceScaleSampleRate(u16 x);
u32 voiceGetPitchRatio(u8 noteIn, u32 packed);
u32 voiceConvertTimeCentsToMs(u32 timeCents);

#endif /* MUSYX_VOICE_CONV_H_ */
