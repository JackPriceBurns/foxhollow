#include "musyx/hw_keyoff.h"
#include "musyx/dsp_voice_state.h"
#include "musyx/hw_init.h"


void hwKeyOff(int slot)
{
    dspVoice[slot].changed[salTimeOffset] |= DSP_VOICE_CHANGE_KEYOFF;
}
