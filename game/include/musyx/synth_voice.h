#ifndef MUSYX_SYNTH_VOICE_H_
#define MUSYX_SYNTH_VOICE_H_

#include "global.h"
#include "musyx/mcmd.h"
#include "musyx/synth_master_fader.h"

typedef struct SynthDelayedNode SynthDelayedNode;

void synthAddJob(SynthDelayedNode* fade, int mode, u32 delay);
void synthStartSynthJobHandling(McmdVoiceState *voice);
void synthForceLowPrecisionUpdate(McmdVoiceState *voice);
void synthKeyStateUpdate(McmdVoiceState *voice);
void HandleFaderTermination(SynthMasterFader* fade);
void synthHandle(u32 deltaTime);
u32 synthStartSound(u16 id, u8 priority, u8 maxVoices, u8 key, u8 volume, u8 pan,
                    u8 midi, u8 midiSet, u8 section, u16 step, u16 trackId,
                    u8 volumeGroup, s16 priorityOffset, u8 studio, u32 itd);
u32 synthFXStart(u16 fxId, u8 volume, u8 pan, u8 studio, u32 studioAux);

#endif /* MUSYX_SYNTH_VOICE_H_ */
