#include "foxhollow_compat.h"
#include "main/unknown/autos/musyx_dsp.h"
#include "musyx/dsp_voice_state.h"
#include "musyx/endian.h"
#include "musyx/hw_init.h"
#include <dolphin/ar.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MUSYX_MIX_FRAMES 160
#define MUSYX_MIX_VOICES 64

typedef struct MusyxMixVoice
{
    u8 active;
    u8 compType;
    u8 looped;
    u16 sampleId;
    u32 callbackId;
    u32 aramBase;
    u64 phase;
    u32 decodedFrames;
    u32 cachedFrame;
    s16 history1;
    s16 history2;
    s16 cache[14];
    s16* decodedSamples;
    u32 decodedLength;
    s32 volumeL;
    s32 volumeR;
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
    s16 coefficient1 = fhReadBES16(info + 8 + predictor * 4);
    s16 coefficient2 = fhReadBES16(info + 10 + predictor * 4);
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
    if (state->decodedSamples != NULL && sampleIndex < state->decodedLength)
    {
        return state->decodedSamples[sampleIndex];
    }
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

static void musyxDecodeSample(MusyxMixVoice* state, const DSPvoice* voice)
{
    s16* decoded;
    u32 frameIndex;
    u32 sampleIndex;

    state->decodedSamples = NULL;
    state->decodedLength = 0;
    if ((voice->smp_info.compType != SAMPLE_TYPE_ADPCM &&
         voice->smp_info.compType != SAMPLE_TYPE_ADPCM_PLUS) ||
        voice->smp_info.length == 0 || voice->smp_info.extraData == NULL ||
        ARGetStorageAddress() == NULL)
    {
        return;
    }
    decoded = malloc((size_t)voice->smp_info.length * sizeof(*decoded));
    if (decoded == NULL)
    {
        return;
    }
    musyxResetDecoder(state);
    for (frameIndex = 0, sampleIndex = 0; sampleIndex < voice->smp_info.length; frameIndex++)
    {
        u32 i;
        musyxDecodeFrame(state, voice, frameIndex);
        for (i = 0; i < 14 && sampleIndex < voice->smp_info.length; i++, sampleIndex++)
        {
            decoded[sampleIndex] = state->cache[i];
        }
    }
    state->decodedSamples = decoded;
    state->decodedLength = voice->smp_info.length;
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
        return fhReadBES16(aram + state->aramBase + sampleIndex * 2);
    case SAMPLE_TYPE_PCM8:
        return (s16)((s8)aram[state->aramBase + sampleIndex] * 256);
    default:
        return 0;
    }
}

static u32 musyxOffsetSampleIndex(const MusyxMixVoice* state, const DSPvoice* voice, u32 sampleIndex,
                                  s32 offset)
{
    s64 index = (s64)sampleIndex + offset;

    if (voice->smp_info.loopLength != 0)
    {
        s64 loopStart = voice->smp_info.loop;
        s64 loopLength = voice->smp_info.loopLength;
        s64 loopEnd = loopStart + loopLength;
        if (index >= loopEnd)
        {
            index = loopStart + (index - loopEnd) % loopLength;
        }
        else if (index < loopStart && state->looped != 0)
        {
            index = loopEnd - 1 - (loopStart - 1 - index) % loopLength;
        }
    }
    if (index < 0)
    {
        return 0;
    }
    if (index >= voice->smp_info.length)
    {
        return voice->smp_info.length - 1;
    }
    return (u32)index;
}

static s16 musyxCubicSample(MusyxMixVoice* state, const DSPvoice* voice, u32 sampleIndex, u32 fraction)
{
    s32 p0 = musyxReadSample(state, voice, musyxOffsetSampleIndex(state, voice, sampleIndex, -1));
    s32 p1 = musyxReadSample(state, voice, sampleIndex);
    s32 p2 = musyxReadSample(state, voice, musyxOffsetSampleIndex(state, voice, sampleIndex, 1));
    s32 p3 = musyxReadSample(state, voice, musyxOffsetSampleIndex(state, voice, sampleIndex, 2));
    s64 value;

    value = ((s64)-p0 + 3 * p1 - 3 * p2 + p3) * fraction >> 16;
    value = (value + 2 * p0 - 5 * p1 + 4 * p2 - p3) * fraction >> 16;
    value = (value - p0 + p2) * fraction >> 16;
    value = (value + 2 * p1) / 2;
    return musyxClampSample((s32)value);
}

static void musyxStartVoice(MusyxMixVoice* state, const DSPvoice* voice)
{
    free(state->decodedSamples);
    state->active = 1;
    state->compType = voice->smp_info.compType;
    state->looped = 0;
    state->sampleId = voice->smp_id;
    state->callbackId = voice->mesgCallBackUserValue;
    state->aramBase = (u32)(uintptr_t)voice->smp_info.addr;
    state->phase = (u64)voice->smp_info.offset << 16;
    state->volumeL = (s32)((u32)voice->volL << 16);
    state->volumeR = (s32)((u32)voice->volR << 16);
    musyxResetDecoder(state);
    musyxDecodeSample(state, voice);
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
        s32 volumeL;
        s32 volumeR;
        s32 volumeDeltaL;
        s32 volumeDeltaR;
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
        pitch = voice->srcTypeSelect == 2 ? 0x10000 : voice->playInfo.pitch;
        if (pitch == 0)
        {
            pitch = 0x10000;
        }
        volumeL = state->volumeL;
        volumeR = state->volumeR;
        volumeDeltaL = (s32)((((s64)voice->volL << 16) - volumeL) / MUSYX_MIX_FRAMES);
        volumeDeltaR = (s32)((((s64)voice->volR << 16) - volumeR) / MUSYX_MIX_FRAMES);
        for (frame = 0; frame < MUSYX_MIX_FRAMES; frame++)
        {
            u32 sampleIndex = state->phase >> 16;
            u32 fraction = state->phase & 0xffff;
            s32 envelope = voice->pb->ve.currentVolume + (s16)voice->pb->ve.currentDelta * (s32)frame;
            s16 sample;
            s64 scaled;
            if (voice->smp_info.loopLength != 0)
            {
                u32 loopEnd = voice->smp_info.loop + voice->smp_info.loopLength;
                if (sampleIndex >= loopEnd)
                {
                    sampleIndex = voice->smp_info.loop + (sampleIndex - loopEnd) % voice->smp_info.loopLength;
                    state->phase = ((u64)sampleIndex << 16) | (state->phase & 0xffff);
                    state->looped = 1;
                }
            }
            else if (sampleIndex >= voice->smp_info.length)
            {
                break;
            }
            sample = musyxReadSample(state, voice, sampleIndex);
            if (fraction != 0 && voice->srcTypeSelect == 0)
            {
                sample = musyxCubicSample(state, voice, sampleIndex, fraction);
            }
            else if (fraction != 0 && voice->srcTypeSelect == 1)
            {
                s16 nextSample = musyxReadSample(
                    state, voice, musyxOffsetSampleIndex(state, voice, sampleIndex, 1));
                sample = musyxClampSample((s32)sample +
                                          ((((s32)nextSample - sample) * (s32)fraction) >> 16));
            }
            if (envelope < 0)
            {
                envelope = 0;
            }
            else if (envelope > 0x7fff)
            {
                envelope = 0x7fff;
            }
            scaled = (s64)sample * envelope;
            mix[frame * 2] += (scaled * (volumeL >> 16)) >> 30;
            mix[frame * 2 + 1] += (scaled * (volumeR >> 16)) >> 30;
            state->phase += pitch;
            volumeL += volumeDeltaL;
            volumeR += volumeDeltaR;
        }
        state->volumeL = (s32)((u32)voice->volL << 16);
        state->volumeR = (s32)((u32)voice->volR << 16);
    }
    for (frame = 0; frame < MUSYX_MIX_FRAMES * 2; frame++)
    {
        destination[frame] = musyxClampSample(mix[frame]);
    }
}
