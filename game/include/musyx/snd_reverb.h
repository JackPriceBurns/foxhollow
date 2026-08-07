#ifndef MUSYX_SND_REVERB_H_
#define MUSYX_SND_REVERB_H_

#include "dolphin/axfx.h"
#include "musyx/snd_types.h"

typedef AXFX_REVERBSTD ReverbState;

void salFree(void *ptr);
void sndAuxCallbackReverbSTD(u8 mode, SynthAuxInfo* info, void* user);
void sndAuxCallbackUpdateSettingsReverbSTD(ReverbState *state);

#endif /* MUSYX_SND_REVERB_H_ */
