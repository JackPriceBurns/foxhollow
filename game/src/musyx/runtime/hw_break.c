#include "musyx/hw_break.h"
#include "musyx/dsp_voice_state.h"
#include "musyx/hw_init.h"

void hwBreak(int voiceIndex) {
    DSPvoice* voice = &dspVoice[voiceIndex];

    if (voice->state == DSP_VOICE_STATE_STARTUP && salTimeOffset == 0) {
        voice->startupBreak = 1;
    }
    voice->changed[salTimeOffset] |= DSP_VOICE_CHANGE_BREAK;
}
