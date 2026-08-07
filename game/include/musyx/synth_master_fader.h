#ifndef MUSYX_SYNTH_MASTER_FADER_H_
#define MUSYX_SYNTH_MASTER_FADER_H_

#include "types.h"

typedef struct SynthMasterFader
{
    f32 volume;
    f32 target;
    f32 start;
    f32 time;
    f32 deltaTime;
    f32 pauseVol;
    f32 pauseTarget;
    f32 pauseStart;
    f32 pauseTime;
    f32 pauseDeltaTime;
    u32 seqId;
    u8 seqMode;
    u8 type;
    u8 pad[2];
} SynthMasterFader;

#endif
