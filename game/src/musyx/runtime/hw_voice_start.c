#include "musyx/hw_voice_start.h"
#include "musyx/dsp_voice_state.h"
#include "musyx/hw_dspctrl.h"
#include "musyx/hw_init.h"


void hwStart(u32 voice, u8 studio)
{
    dspVoice[voice].singleOffset = salTimeOffset;
    salActivateVoice(&dspVoice[voice], studio);
}
