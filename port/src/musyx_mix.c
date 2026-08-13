#include "main/unknown/autos/musyx_dsp.h"
#include "musyx/dsp_voice_state.h"
#include "musyx/endian.h"
#include "musyx/hw_init.h"
#include <dolphin/ar.h>
#include <stdint.h>
#include <string.h>

#define MUSYX_MIX_FRAMES 160
#define MUSYX_MIX_VOICES 64

typedef struct MusyxMixVoice
{
    u8 active;
    u8 compType;
    u16 sampleId;
    u32 callbackId;
    u32 aramBase;
    u64 phase;
    u32 decodedFrames;
    u32 cachedFrame;
    s16 history1;
    s16 history2;
    s16 cache[14];
} MusyxMixVoice;

static MusyxMixVoice sMixVoices[MUSYX_MIX_VOICES];

static s16 musyxClampSample(s32 value)
{
    if (value > 32767)
    {
        return 32767;
    }
    if (value < -32768)
    {
        return -32768;
    }
    return value;
}

static void musyxResetDecoder(MusyxMixVoice* state)
{
    state->decodedFrames = 0;
    state->cachedFrame = 0xffffffff;
    state->history1 = 0;
    state->history2 = 0;
}

static void musyxDecodeFrame(MusyxMixVoice* state, const DSPvoice* voice, u32 frameIndex)
{
    const u8* aram = ARGetStorageAddress();
    const u8* info = voice->smp_info.extraData;
    const u8* source = aram + state->aramBase + frameIndex * 8;
    u8 predictor = source[0] >> 4;
    u8 shift = source[0] & 0xf;
    s16 coefficient1 = (s16)musyxReadBE16(info + 8 + predictor * 4);
    s16 coefficient2 = (s16)musyxReadBE16(info + 10 + predictor * 4);
    u32 i;

    for (i = 0; i < 14; i++)
    {
        u8 packed = source[1 + i / 2];
        s32 nibble = (i & 1) != 0 ? packed & 0xf : packed >> 4;
        s32 sample;
        if (nibble >= 8)
        {
            nibble -= 16;
        }
        sample = (((nibble << shift) << 11) + 1024 + coefficient1 * state->history1 +
                  coefficient2 * state->history2) >> 11;
        state->history2 = state->history1;
        state->history1 = musyxClampSample(sample);
        state->cache[i] = state->history1;
    }
    state->cachedFrame = frameIndex;
    state->decodedFrames = frameIndex + 1;
}

static s16 musyxReadAdpcm(MusyxMixVoice* state, const DSPvoice* voice, u32 sampleIndex)
{
    u32 frameIndex = sampleIndex / 14;
    if (voice->smp_info.extraData == NULL || ARGetStorageAddress() == NULL)
    {
        return 0;
    }
    if (frameIndex < state->decodedFrames - (state->decodedFrames != 0))
    {
        musyxResetDecoder(state);
    }
    while (state->decodedFrames <= frameIndex)
    {
        musyxDecodeFrame(state, voice, state->decodedFrames);
    }
    return state->cache[sampleIndex % 14];
}

static s16 musyxReadSample(MusyxMixVoice* state, const DSPvoice* voice, u32 sampleIndex)
{
    const u8* aram = ARGetStorageAddress();
    if (aram == NULL)
    {
        return 0;
    }
    switch (voice->smp_info.compType)
    {
    case SAMPLE_TYPE_ADPCM:
    case SAMPLE_TYPE_ADPCM_PLUS:
        return musyxReadAdpcm(state, voice, sampleIndex);
    case SAMPLE_TYPE_PCM16:
        return (s16)musyxReadBE16(aram + state->aramBase + sampleIndex * 2);
    case SAMPLE_TYPE_PCM8:
        return (s8)aram[state->aramBase + sampleIndex];
    default:
        return 0;
    }
}

static void musyxStartVoice(MusyxMixVoice* state, const DSPvoice* voice)
{
    state->active = 1;
    state->compType = voice->smp_info.compType;
    state->sampleId = voice->smp_id;
    state->callbackId = voice->mesgCallBackUserValue;
    state->aramBase = (u32)(uintptr_t)voice->smp_info.addr;
    state->phase = (u64)voice->smp_info.offset << 16;
    musyxResetDecoder(state);
}

static u32 musyxVoiceMatches(const MusyxMixVoice* state, const DSPvoice* voice)
{
    return state->active != 0 && state->compType == voice->smp_info.compType &&
           state->sampleId == voice->smp_id && state->callbackId == voice->mesgCallBackUserValue &&
           state->aramBase == (u32)(uintptr_t)voice->smp_info.addr;
}

void fhMusyxMix(short* destination)
{
    s32 mix[MUSYX_MIX_FRAMES * 2];
    u32 voiceIndex;
    u32 frame;

    memset(mix, 0, sizeof(mix));
    for (voiceIndex = 0; voiceIndex < salNumVoices && voiceIndex < MUSYX_MIX_VOICES; voiceIndex++)
    {
        DSPvoice* voice = &dspVoice[voiceIndex];
        MusyxMixVoice* state = &sMixVoices[voiceIndex];
        u32 pitch;
        if (voice->state != DSP_VOICE_STATE_ACTIVE)
        {
            state->active = 0;
            continue;
        }
        if (!musyxVoiceMatches(state, voice))
        {
            musyxStartVoice(state, voice);
        }
        pitch = voice->playInfo.pitch;
        if (pitch == 0)
        {
            pitch = 0x10000;
        }
        for (frame = 0; frame < MUSYX_MIX_FRAMES; frame++)
        {
            u32 sampleIndex = state->phase >> 16;
            u32 envelope = voice->pb->ve.currentVolume;
            s16 sample;
            s64 scaled;
            if (voice->smp_info.loopLength != 0)
            {
                u32 loopEnd = voice->smp_info.loop + voice->smp_info.loopLength;
                if (sampleIndex >= loopEnd)
                {
                    sampleIndex = voice->smp_info.loop + (sampleIndex - loopEnd) % voice->smp_info.loopLength;
                    state->phase = ((u64)sampleIndex << 16) | (state->phase & 0xffff);
                }
            }
            else if (sampleIndex >= voice->smp_info.length)
            {
                break;
            }
            sample = musyxReadSample(state, voice, sampleIndex);
            scaled = (s64)sample * envelope;
            mix[frame * 2] += (scaled * voice->volL) >> 30;
            mix[frame * 2 + 1] += (scaled * voice->volR) >> 30;
            state->phase += pitch;
        }
    }
    for (frame = 0; frame < MUSYX_MIX_FRAMES * 2; frame++)
    {
        destination[frame] = musyxClampSample(mix[frame]);
    }
}
