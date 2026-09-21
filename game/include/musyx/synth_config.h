#ifndef MUSYX_SYNTH_CONFIG_H_
#define MUSYX_SYNTH_CONFIG_H_

#include "types.h"

typedef struct SynthInfo {
    u32 sampleRate;
    u32 numSamples;
    u8 playbackInfo[0x208];
    u8 voiceCount;
    u8 musicVoiceCount;
    u8 fxVoiceCount;
    u8 studioCount;
} SynthInfo;

typedef SynthInfo SynthConfiguration;

typedef struct SynthITDInfo {
    u8 music;
    u8 sfx;
} SynthITDInfo;

extern SynthITDInfo synthITDDefault[8];

extern SynthInfo synthInfo;

extern u32 synthFlags;

#define SYNTH_CONFIGURATION (&synthInfo)

#endif /* MUSYX_SYNTH_CONFIG_H_ */
