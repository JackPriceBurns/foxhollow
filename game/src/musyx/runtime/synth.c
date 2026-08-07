#include "global.h"
#include "musyx/synth_voice.h"
#include "musyx/synth_job_queue.h"
#include "musyx/synth_config.h"
#include "musyx/mcmd.h"
#include "musyx/synth_master_fader.h"
#include "util/carry.h"
#include "musyx/synth_channel.h"
#include "musyx/synth_channel_scale.h"
#include "musyx/synth_callback.h"
#include "musyx/hw_samplemem.h"
#include "musyx/voice_id.h"
#include "musyx/synth_queue.h"
#include "musyx/hw_init.h"
#include "musyx/hw_voice_params.h"
#include "musyx/hw_voice_start.h"
#include "musyx/hw_volume.h"
#include "musyx/adsr_handle.h"
#include "musyx/adsr_lowprec.h"
#include "musyx/data_tables.h"
#include "musyx/sal_dsp.h"
#include "musyx/snd_reverb.h"
#include "musyx/synth_delay.h"
#include "musyx/synth_virtual_sample.h"
#include "musyx/vid_init.h"
#include "musyx/inp_ctrl.h"
#include "musyx/snd_service.h"
#include "musyx/hw_keyoff.h"
#include "musyx/inp_midi.h"
#include "musyx/mcmd_exec.h"
#include "musyx/voice_conv.h"
#include "musyx/voice_manage.h"
#include "musyx/snd_core.h"
#include "string.h"
#include "musyx/synth_volume.h"
#include "musyx/synth_control.h"


struct SynthDelayedNode
{
    struct SynthDelayedNode* next;
    struct SynthDelayedNode* prev;
    u8 voiceIndex;
    u8 jobTabIndex;
    u8 pad[2];
};

typedef void (*SynthDelayedBucketCallback)(int voiceIndex);

#define SYNTH_FADE_COUNT                     0x20
#define SND_SEQVOL_STOP  1
#define SND_SEQVOL_PAUSE 2
#define SND_SEQVOL_MUTE    3
#define SYNTH_FADE_ACTION_DISABLED           4
#define SYNTH_INVALID_LINK_ID                0xffffffff

u64 synthRealTime;
u8 synthIdleWaitActive;
SynthMessageCallback synthMessageCallback;
McmdVoiceState* synthVoice;
u32 synthFlags;
u32 synthMasterFaderActiveFlags;
u32 synthMasterFaderPauseActiveFlags;
u8 synthAuxAMIDI[8];
u8 synthAuxAMIDISet[8];
u8 synthAuxBMIDI[8];
u8 synthAuxBMIDISet[8];
u8 synthJobTableIndex;
u8 sndActive;

#define HWVOICE(i)        (&synthVoice[i])
#define HWVOICE_FLAGS(sv) (*(u64*)&(sv)->cFlagsHi)

static u32 synthTicksPerSecond[9][16];
static SynthJobTab synthJobTable[32];
McmdInputSlot inpAuxA[8][4];
McmdInputSlot inpAuxB[8][4];
s32 synthGlobalVariable[16];
SynthITDInfo synthITDDefault[8];
SynthAuxCallback synthAuxBCallback[8];
void* synthAuxBUser[8];
SynthAuxCallback synthAuxACallback[8];
void* synthAuxAUser[8];
u8 synthTrackVolume[64];
SynthMasterFader synthMasterFader[32];
SynthInfo synthInfo;


typedef struct LAYER
{
    u16 id;
    u8 keyLow;
    u8 keyHigh;
    s8 transpose;
    u8 volume;
    s16 prioOffset;
    u8 panning;
    u8 reserved[3];
} LAYER;

static u32 StartKeymap(u16 id, s16 prio, u8 maxVoices, u16 allocId, u8 key, u8 vol, u8 pan, u8 midi, u8 midiSet,
                       u8 section, u16 step, u16 trackid, u32 vidFlag, u8 vGroup, u8 studio, u32 itd);

void synthSetBpm(int bpm, u8 set, u8 section);
int synthGetTicksPerSecond(McmdVoiceState* state);
void HandleJobQueue(SynthDelayedNode** head, SynthDelayedBucketCallback callback);

void synthSetBpm(int bpm, u8 set, u8 section)
{
    if (set == 0xff)
    {
        set = 8;
    }
    synthTicksPerSecond[set][section] = (u32)((bpm << 3) * 0x600) / 0xf0;
}

int synthGetTicksPerSecond(McmdVoiceState* state)
{
    McmdVoiceState* v = state;
    u32 set;
    int section;
    if ((set = v->midiSet) == 0xff)
        set = 8;
    section = v->section;
    return synthTicksPerSecond[set][section];
}

/*
 * Flag-check and conditional store.
 */
void synthInitPortamento(McmdVoiceState* state)
{
    McmdVoiceState* v = state;
    u64 flags;

    flags = *(u64*)&v->cFlagsHi;
    if ((flags & 0x20000) != 0)
    {
        return;
    }
    if (v->portType == 1)
    {
        if ((flags & 0x1000) == 0)
        {
            v->portTime = 0;
        }
        else
        {
            v->portTime = v->portDuration;
        }
    }
    else
    {
        v->portTime = v->portDuration;
    }
    v->portCurPitch = v->lastNote << 0x10;
}

/*
 * Reuse an active voice matching the requested MIDI slot/channel.
 */
static u32 do_voice_portamento(u8 key, u8 midi, u8 midiSet, u32 isMaster, u32* rejected)
{
    u32 i;
    u32 result;
    u32 previousId;
    McmdVoiceState* voice;
    McmdVoiceState* selectedVoice;
    u32 sawHeldVoice;

    sawHeldVoice = 0;
    result = -1;
    for (i = 0, voice = (McmdVoiceState*)synthVoice; i < SYNTH_CONFIGURATION->voiceCount; ++i, ++voice)
    {
        if (voice->block == 0 && voice->id != 0xffffffff && voice->midi == midi &&
            voice->midiSet == midiSet)
        {
            if ((*(u64*)&voice->cFlagsHi & 2) != 0)
            {
                sawHeldVoice = 1;
            }
            if ((*(u64*)&voice->cFlagsHi & 0x10) != 0 && (*(u64*)&voice->cFlagsHi & 0x10000000008) != 8 &&
                hwIsActive(i) != 0)
            {
                if (result == 0xffffffff && (*(u64*)&voice->cFlagsHi & 0x20002) == 0x20002)
                {
                    *rejected = 1;
                    return -1;
                }

                selectedVoice = voice;
                voice->portCurPitch = ((u32)voice->curNote << 16) + ((s32)voice->curDetune << 16) / 100;
                voice->lastNote = voice->curNote;
                voice->curNote = key + ((voice->curNote & 0xff) - voice->orgNote);
                voice->orgNote = key;
                voice->curDetune = 0;
                voice->portTime = 0;
                voice->cFlagsLo = voice->cFlagsLo | 0x20000LL;
                vidRemoveVoiceReferences(&synthVoice[i]);
                if (result == 0xffffffff)
                {
                    voice->child = 0xffffffff;
                    voice->parent = 0xffffffff;
                    result = vidMakeNew(&synthVoice[i], isMaster);
                    previousId = voice->id;
                }
                else
                {
                    ((McmdVoiceState*)synthVoice)[previousId & 0xff].child = voice->id;
                    voice->parent = previousId;
                    previousId = voice->id;
                    vidMakeNew(&synthVoice[i], 0);
                }
            }
        }
    }

    if (result != 0xffffffff)
    {
        voiceSetLastStarted(selectedVoice);
        inpSetMidiLastNote(selectedVoice->midi, selectedVoice->midiSet, selectedVoice->curNote & 0xff);
        *rejected = 0;
    }
    else
    {
        *rejected = sawHeldVoice;
    }
    return result;
}

static inline u32 check_portamento(u8 key, u8 midi, u8 midiSet, u32 newVID, u32* vid) {
    u32 rejected;

    if (inpGetMidiCtrl(MCMD_CTRL_PORTAMENTO, midi, midiSet) > 0x1F80) {
        *vid = do_voice_portamento(key & 0x7F, midi, midiSet, newVID, &rejected);
        return !rejected;
    }
    *vid = 0xFFFFFFFF;
    return 1;
}

static u32 StartLayer(u16 layerID, s16 prio, u8 maxVoices, u16 allocId, u8 key, u8 vol, u8 panning, u8 midi,
                      u8 midiSet, u8 section, u16 step, u16 trackid, u32 vidFlag, u8 vGroup, u8 studio, u32 itd) {
    u16 count;
    u32 vid;
    u32 new_id;
    u32 id;
    LAYER* l;
    s32 pan;
    s32 note;
    u8 scaledVol;
    u8 mKey;

    vid = 0xFFFFFFFF;
    if ((l = dataGetLayer(layerID, &count)) == NULL) {
        goto end;
    }

    mKey = key & 0x7f;
    for (; count != 0; --count, l++) {
        if (l->id == 0xffff || l->keyLow > mKey || l->keyHigh < mKey) {
            continue;
        }

        note = mKey + l->transpose;
        note = note > 127 ? 127 : note < 0 ? 0 : note;

        if ((l->id & 0xC000) == 0) {
            if (check_portamento(note, midi, midiSet, 0, &new_id)) {
                if (new_id != 0xFFFFFFFF) {
                    goto apply_new_id;
                } else {
                    goto start_new_id;
                }
            }
            continue;
        }

    start_new_id:
        if ((l->panning & 0x80) == 0) {
            pan = l->panning - 0x40;
            pan += panning;
            pan = pan < 0 ? 0 : pan > 0x7f ? 0x7f : pan;
        } else {
            pan = 0x80;
        }

        scaledVol = (vol * l->volume) / 0x7f;
        prio += l->prioOffset;
        prio = prio > 0xff ? 0xff : prio < 0 ? 0 : prio;

        switch (l->id & 0xC000) {
        case 0:
            new_id = macStart(l->id, prio, maxVoices, allocId, note | (key & 0x80), scaledVol, pan, midi, midiSet,
                              section, step, trackid, 0, vGroup, studio, itd);
            break;
        case 0x4000:
            new_id = StartKeymap(l->id, prio, maxVoices, allocId, note | (key & 0x80), scaledVol, pan, midi, midiSet,
                                 section, step, trackid, 0, vGroup, studio, itd);
            break;
        case 0x8000:
            new_id = StartLayer(l->id, prio, maxVoices, allocId, note | (key & 0x80), scaledVol, pan, midi, midiSet,
                                section, step, trackid, 0, vGroup, studio, itd);
            break;
        }

        if (new_id != 0xFFFFFFFF) {
        apply_new_id:
            if (vid == 0xFFFFFFFF) {
                if (vidFlag != 0) {
                    vid = vidMakeRoot(&synthVoice[new_id & 0xff]);
                } else {
                    vid = new_id;
                }
            } else {
                synthVoice[id & 0xff].child = new_id;
                synthVoice[new_id & 0xff].parent = id;
            }
            id = new_id;
            while (synthVoice[id & 0xff].child != 0xFFFFFFFF) {
                synthVoice[id & 0xff].block = 1;
                id = synthVoice[id & 0xff].child;
            }
            synthVoice[id & 0xff].block = 1;
        }
    }

end:
    return vid;
}

typedef struct KeymapEntry
{
    u16 id;         /* 0x0 */
    s8 transpose;   /* 0x2 */
    u8 panning;     /* 0x3 */
    s16 prioOffset; /* 0x4 */
    u8 reserved[2]; /* 0x6 */
} KeymapEntry;      /* size 0x8, MP4 musyx/synthdata.h KEYMAP */

/*
 * Resolve an indirection-table sample entry, then dispatch the resolved
 * sample or nested sample group.
 */
static u32 StartKeymap(u16 id, s16 prio, u8 maxVoices, u16 allocId, u8 key, u8 vol, u8 pan, u8 midi, u8 midiSet,
                       u8 section, u16 step, u16 trackid, u32 vidFlag, u8 vGroup, u8 studio, u32 itd)
{
    u8 o;
    KeymapEntry* keymap;
    s32 p;
    s32 k;
    u32 handle;

    if ((keymap = (KeymapEntry*)dataGetKeymap(id)) != 0)
    {
        o = key & 0x7F;
        if (keymap[o].id != 0xFFFF)
        {
            if ((keymap[o].id & 0xC000) != 0x4000)
            {
                if ((keymap[o].panning & 0x80) == 0)
                {
                    p = keymap[key].panning - 0x40;
                    p += pan;
                    if (p < 0)
                    {
                        pan = 0;
                    }
                    else if (p > 0x7F)
                    {
                        pan = 0x7F;
                    }
                    else
                    {
                        pan = p;
                    }
                }
                else
                {
                    pan = 0x80;
                }

                k = (key & 0x7F) + keymap[o].transpose;
                k = k > 0x7F ? 0x7F : k < 0 ? 0 : k;

                prio += keymap[o].prioOffset;
                prio = prio > 0xFF ? 0xFF : prio < 0 ? 0 : prio;

                if ((keymap[o].id & 0xC000) == 0)
                {
                    if (!check_portamento(k & 0xFF, midi, midiSet, vidFlag, &handle))
                    {
                        return -1;
                    }
                    if (handle != 0xFFFFFFFF)
                    {
                        return handle;
                    }
                    return macStart(keymap[o].id, prio, maxVoices, allocId, k | (key & 0x80),
                                    vol, pan, midi, midiSet, section, step, trackid, vidFlag, vGroup, studio, itd);
                }
                return StartLayer(keymap[o].id, prio, maxVoices, allocId, k | (key & 0x80), vol, pan, midi, midiSet,
                                  section, step, trackid, vidFlag & 0xff, vGroup, studio, itd);
            }
        }
    }
    return -1;
}

/*
 * Start a sample/FX id, handling direct samples, table-expanded sample
 * groups, and already-linked voice chains.
 */
static inline void unblockAllAllocatedVoices(u32 vid)
{
    u32 vi;

    vi = vidGetInternalId(vid);
    while (vi != 0xFFFFFFFF)
    {
        HWVOICE(vi & 0xFF)->block = 0;
        vi = HWVOICE(vi & 0xFF)->child;
    }
}

u32 synthStartSound(u16 id, u8 prio, u8 maxVoices, u8 key, u8 vol, u8 pan, u8 midi, u8 midiSet, u8 section, u16 step,
                    u16 trackid, u8 vGroup, s16 prioOffset, u8 studio, u32 itd)
{
    prio += prioOffset;
    prio = prio > 0xFF ? 0xFF : prio;

    switch (id & 0xC000)
    {
    case 0:
    {
        u32 handle;
        if (!check_portamento(key, midi, midiSet, 1, &handle))
        {
            return -1;
        }
        if (handle != 0xFFFFFFFF)
        {
            return handle;
        }
        return macStart(id, prio, maxVoices, id, key, vol, pan, midi, midiSet, section, step, trackid, 1, vGroup, studio,
                        itd);
    }
    case 0x4000:
    {
        u32 vid = StartKeymap(id, prio, maxVoices, id, key, vol, pan, midi, midiSet, section, step, trackid, 1, vGroup,
                              studio, itd);
        if (vid != 0xFFFFFFFF)
        {
            unblockAllAllocatedVoices(vid);
        }
        return vid;
    }
    case 0x8000:
    {
        u32 vid =
            StartLayer(id, prio, maxVoices, id, key, vol, pan, midi, midiSet, section, step, trackid, 1, vGroup, studio, itd);
        if (vid != 0xFFFFFFFF)
        {
            unblockAllAllocatedVoices(vid);
        }
        return vid;
    }
    }
    return -1;
}

/*
 * Low-precision per-voice update: LFOs, vibrato, pitch sweeps, pan ramps,
 * pitch bend/portamento and final pitch computation.
 */
static inline u32 apply_portamento(McmdVoiceState* svoice, u32 ccents, u32 deltaTime)
{
    u32 old_portCurPitch;

    if ((HWVOICE_FLAGS(svoice) & 0x400) != 0 && (s32)((svoice->portDuration - svoice->portTime) >> 8) > 0)
    {
        old_portCurPitch = svoice->portCurPitch;
        svoice->portCurPitch += (s32)deltaTime * ((s32)(ccents - svoice->portCurPitch) >> 8) /
                                (s32)((svoice->portDuration - svoice->portTime) >> 8);
        if ((old_portCurPitch < ccents && svoice->portCurPitch < ccents) ||
            (old_portCurPitch > ccents && svoice->portCurPitch > ccents))
        {
            ccents = svoice->portCurPitch;
            svoice->portTime += deltaTime;
        }
        else
        {
            svoice->portTime = svoice->portDuration;
        }
    }
    return ccents;
}

static inline u32 convert_cents(McmdVoiceState* svoice, u32 ccents)
{
    u32 curDetune;
    u32 cpitch;

    cpitch = voiceGetPitchRatio(ccents >> 16, svoice->sInfo) << 16;
    if ((curDetune = ccents & 0xFFFF) != 0)
    {
        cpitch += curDetune * ((u16)voiceScaleSampleRate(cpitch >> 16) - (cpitch >> 16));
    }
    return cpitch;
}

static inline void UpdateTimeMIDICtrl(McmdVoiceState* sv)
{
    if (sv->timeUsedByInput != 0)
    {
        sv->timeUsedByInput = 0;
        sv->midiDirtyFlags = 0x1FFF;
    }
}

static void LowPrecisionHandler(int voice)
{
    u32 j;
    s32 pbend;
    u32 ccents;
    u32 cpitch;
    u16 Modulation;
    u16 portamentoRaw;
    u32 lowDeltaTime;
    McmdVoiceState* sv;
    u32 cntDelta;
    u32 addFactor;
    u16 adsr_start;
    u16 adsr_delta;
    s32 vrange;
    s32 voff;
    sv = HWVOICE(voice);
    if (hwIsActive(voice) || sv->addr != 0)
    {

        lowDeltaTime = (u32)(synthRealTime - *(u64*)&sv->lastLowCallTimeHi);
        *(u64*)&sv->lastLowCallTimeHi = synthRealTime;

        for (j = 0; j < 2; ++j)
        {
            if (sv->lfo[j].period == 0)
            {
                continue;
            }
            sv->lfo[j].time += lowDeltaTime;
            sv->lfo[j].value = sndSin((u16)((sv->lfo[j].time % sv->lfo[j].period * 16) / (sv->lfo[j].period / 256)));
            if (sv->lfo[j].value != sv->lfo[j].lastValue)
            {
                sv->lfo[j].lastValue = sv->lfo[j].value;
                if (sv->lfoUsedByInput[j])
                {
                    sv->lfoUsedByInput[j] = 0;
                    sv->midiDirtyFlags |= 0x1FFF;
                }
            }
        }

        if ((HWVOICE_FLAGS(sv) & 0x2000) != 0)
        {
            sv->vibCurTime += lowDeltaTime;
            sv->vibCurOffset = sndSin((u16)((sv->vibCurTime % sv->vibPeriod * 16) / (sv->vibPeriod / 256)));
        }

        if (sv->sweepNum[0] | sv->sweepNum[1])
        {
            cntDelta = (lowDeltaTime << 8) >> 4;
            addFactor = (lowDeltaTime << 4) >> 4;
            for (j = 0; j < 2; ++j)
            {
                if (sv->sweepNum[j] == 0)
                {
                    continue;
                }
                sv->sweepCnt[j] -= cntDelta;
                if (sv->sweepCnt[j] <= 0)
                {
                    sv->sweepCnt[j] = sv->sweepNum[j] << 16;
                    sv->sweepOff[j] = 0;
                }
                else
                {
                    sv->sweepOff[j] += (sv->sweepAdd[j] >> 12) * addFactor;
                }
            }
        }

        for (j = 0; j < 2; ++j)
        {
            u32 panVal;
            if (sv->panning[j] == sv->panTarget[j])
            {
                continue;
            }
            sv->panTime[j] -= lowDeltaTime;
            if ((s32)sv->panTime[j] <= 0)
            {
                sv->panning[j] = sv->panTarget[j];
                sv->panTime[j] = 0;
            }
            else
            {
                sv->panning[j] = sv->panTarget[j] - (sv->panTime[j] / 256) * sv->panDelta[j];
                panVal = sv->panning[j];
                sv->panning[j] = (s32)panVal < 0 ? 0 : panVal > 0x7F0000 ? 0x7F0000 : panVal;
            }
            HWVOICE_FLAGS(sv) |= 0x200000000000ULL;
        }

        if ((HWVOICE_FLAGS(sv) & 0x20000000000ULL) != 0 && adsrHandleLowPrecision(&sv->pitchADSR, &adsr_start, &adsr_delta))
        {
            HWVOICE_FLAGS(sv) &= ~0x20000000000ULL;
        }

        ccents = (sv->curNote << 16) + (sv->curDetune * 0x10000) / 100;
        do
        {
            if ((HWVOICE_FLAGS(sv) & 0x10030) != 0)
            {
                if (sv->midi == 0xFF)
                {
                    continue;
                }
                pbend = inpGetPitchBend(sv);
                sv->pbLast = pbend;
            }
            else
            {
                pbend = sv->pbLast;
            }
            if (pbend != 0x2000)
            {
                pbend -= 0x2000;
                if (pbend < 0)
                {
                    ccents += sv->pbLowerKeyRange * pbend * 8;
                }
                else
                {
                    ccents += sv->pbUpperKeyRange * pbend * 8;
                }
            }
        } while (0);

        if ((HWVOICE_FLAGS(sv) & 0x2000) != 0)
        {
            Modulation = inpGetModulation(sv);
            vrange = sv->vibKeyRange * 256 + (sv->vibCentRange * 256) / 100;
            if (sv->vibModAddScale != 0)
            {
                vrange += (sv->vibModAddScale * ((Modulation >> 7) & 0x1FF)) >> 7;
            }
            if ((HWVOICE_FLAGS(sv) & 0x4000) != 0)
            {
                voff = (sv->vibCurOffset * ((Modulation >> 7) & 0x1FF)) >> 7;
            }
            else
            {
                voff = sv->vibCurOffset;
            }
            ccents += (vrange * voff) >> 4;
        }

        if (sv->midi != 0xFF)
        {
            portamentoRaw = inpGetMidiCtrl(MCMD_CTRL_PORTAMENTO, sv->midi, sv->midiSet);
            if (portamentoRaw != sv->portLastCtrlState || (HWVOICE_FLAGS(sv) & 0x21000) == 0x20000)
            {
                if (portamentoRaw <= 0x1F80)
                {
                    HWVOICE_FLAGS(sv) &= ~0x400;
                }
                else
                {
                    if ((HWVOICE_FLAGS(sv) & 0x400) == 0)
                    {
                        if ((HWVOICE_FLAGS(sv) & 0x20000) == 0)
                        {
                            if (sv->portType == 1)
                            {
                                if ((HWVOICE_FLAGS(sv) & 0x1000) == 0)
                                {
                                    sv->portTime = 0;
                                }
                                else
                                {
                                    sv->portTime = sv->portDuration;
                                }
                            }
                            else
                            {
                                sv->portTime = sv->portDuration;
                            }
                            sv->portCurPitch = sv->lastNote << 16;
                        }
                    }
                    HWVOICE_FLAGS(sv) |= 0x400;
                }
                HWVOICE_FLAGS(sv) |= 0x1000;
                sv->portLastCtrlState = portamentoRaw;
            }
        }

        ccents = apply_portamento(sv, ccents, lowDeltaTime);

        if ((HWVOICE_FLAGS(sv) & 0x20000000000ULL) != 0)
        {
            ccents += sv->pitchADSRRange * ((s32)sv->pitchADSR.currentVolume >> 16) >> 7;
        }

        cpitch = convert_cents(sv, ccents);
        cpitch += sv->sweepOff[0] + sv->sweepOff[1];
        hwSetPitch(voice, sv->curPitch = ((cpitch >> 16) * inpGetDoppler(sv)) >> 13);
        synthAddJob((SynthDelayedNode*)sv, 0, 0xF00);

    }
    UpdateTimeMIDICtrl(sv);
}

/*
 * Zero-offset per-voice update: volume envelope, tremolo, panning and final
 * volume/aux sends.
 */
static void ZeroOffsetHandler(int voice)
{
    McmdVoiceState* sv;
    u32 lowDeltaTime;
    u16 Modulation;
    f32 vol;
    f32 auxa;
    f32 auxb;
    f32 faderVol;
    f32 voiceVol;
    u32 volUpdate;
    f32 lfo;
    f32 scale;
    s32 pan;
    f32 preVol;
    f32 postVol;

    sv = HWVOICE(voice);
    if (hwIsActive(voice) || sv->addr != 0)
    {

        lowDeltaTime = (u32)(synthRealTime - *(u64*)&sv->lastZeroCallTimeHi);
        *(u64*)&sv->lastZeroCallTimeHi = synthRealTime;

        if ((HWVOICE_FLAGS(sv) & 0x8000) != 0)
        {
            sv->envCurrent += sv->envDelta * (lowDeltaTime >> 8);
            if (sv->envDelta < 0)
            {
                if (sv->envTarget >= sv->envCurrent)
                {
                    sv->envCurrent = sv->envTarget;
                    HWVOICE_FLAGS(sv) &= ~0x8000;
                }
            }
            else if (sv->envTarget <= sv->envCurrent)
            {
                sv->envCurrent = sv->envTarget;
                HWVOICE_FLAGS(sv) &= ~0x8000;
            }
            sv->volume = sv->envCurrent;
            volUpdate = 1;
        }
        else
        {
            volUpdate = (HWVOICE_FLAGS(sv) & 0x100000000000ULL) != 0;
        }

        HWVOICE_FLAGS(sv) &= ~0x100000000000ULL;

        faderVol = synthMasterFader[sv->vGroup].pauseVol * synthMasterFader[sv->vGroup].volume *
                   synthMasterFader[sv->fxFlag ? 22 : 21].volume;

        if (sv->track != 0xFF)
        {
            vol = (1.f / 127.f) * (faderVol * (f32)synthTrackVolume[sv->track]);
        }
        else
        {
            vol = faderVol;
        }

        if (vol != sv->lastVolFaderScale)
        {
            sv->lastVolFaderScale = vol;
            volUpdate = 1;
        }

        voiceVol = (1.f / (8192.f * 1016.f)) * (f32)sv->volume;

        if ((sv->treScale | sv->treModAddScale) != 0)
        {
            Modulation = inpGetModulation(sv);
            lfo = (1.f / 8192.f) *
                  (f32)(0x2000 - ((0x2000 - ((s16)inpGetTremolo(sv) - 0x2000)) >> 1));
            {
                f32 modScale = 1.490207e-08f * ((f32)Modulation * (f32)(0x1000 - sv->treModAddScale));
                scale = (1.f / 4096.f) * ((f32)sv->treScale * (1.f - modScale));
            }
            if (sv->treCurScale < scale)
            {
                if ((sv->treCurScale += 0.2f) > scale)
                {
                    sv->treCurScale = scale;
                }
            }
            else if (sv->treCurScale > scale)
            {
                if ((sv->treCurScale -= 0.2f) < scale)
                {
                    sv->treCurScale = scale;
                }
            }
            {
                f32 tmp = lfo * (1.f - sv->treCurScale);
                voiceVol = voiceVol * (1.f - tmp);
            }
            volUpdate = 1;
        }

        if ((synthFlags & 1) == 0)
        {
            if ((HWVOICE_FLAGS(sv) & 0x200000000000ULL) != 0 || (sv->midiDirtyFlags & 0x6) != 0)
            {
                HWVOICE_FLAGS(sv) &= ~0x200000000000ULL;
                pan = sv->panning[0] + (inpGetPanning(sv) - 0x2000) * 0x200;
                sv->lastPan = pan < 0 ? 0 : (pan > 0x7F0000 ? 0x7F0000 : pan);

                if ((synthFlags & 2) != 0)
                {
                    if ((sv->lastSPan = sv->panning[1] + inpGetSurPanning(sv) * 0x200) > 0x7F0000)
                    {
                        sv->lastSPan = 0x7F0000;
                    }
                }
                else
                {
                    sv->lastSPan = 0;
                }
                volUpdate = 1;
            }
            else if ((synthFlags & 2) == 0)
            {
                sv->lastSPan = 0;
            }
        }
        else
        {
            sv->lastPan = 0x400000;
            sv->lastSPan = 0;
            volUpdate |= (HWVOICE_FLAGS(sv) & 0x200000000000ULL) != 0;
            HWVOICE_FLAGS(sv) &= ~0x200000000000ULL;
        }

        if (volUpdate || (sv->midiDirtyFlags & 0xF01) != 0)
        {
            preVol = voiceVol;
            postVol = (1.f / 16383.f) * (voiceVol * vol * (f32)inpGetVolume(sv));
            auxa = (1.f / 127.f) * (f32)sv->revVolOffset +
                   ((1.f / 16383.f) * (preVol * (f32)inpGetPreAuxA(sv)) +
                    (1.f / 127.f) *
                        ((f32)sv->revVolScale *
                         ((1.f / 16383.f) * (postVol * (f32)inpGetReverb(sv)))));
            auxb = (1.f / 16383.f) * (preVol * (f32)inpGetPreAuxB(sv)) +
                   (1.f / 16383.f) * (postVol * (f32)inpGetPostAuxB(sv));
            sv->curOutputVolume = (u16)(32767.f * postVol);
            hwSetVolume(voice, sv->volTable, postVol, sv->lastPan, sv->lastSPan, auxa, auxb);
        }

        if (sv->age != 0)
        {
            if ((s32)(sv->age -= sv->ageSpeed * lowDeltaTime) < 0)
            {
                sv->age = 0;
            }
            hwSetPriority(voice, sv->prio << 24 | sv->age >> 15);
        }

        synthAddJob((SynthDelayedNode*)sv, 1, (5 - hwGetTimeOffset()) * 256);

    }
    UpdateTimeMIDICtrl(sv);
}

/*
 * Event per-voice update: pedal state, deferred hardware start and key-off.
 */
static void EventHandler(int voice)
{
    McmdVoiceState* sv;

    sv = HWVOICE(voice);
    if (hwIsActive(voice) || sv->addr != 0)
    {
        macSetPedalState(sv, inpGetPedal(sv) > 0x1F80);

        if ((HWVOICE_FLAGS(sv) & 0x20) != 0)
        {
            HWVOICE_FLAGS(sv) &= ~0x20;
            HWVOICE_FLAGS(sv) |= 0x10;
            hwStart(voice, sv->studio);
        }

        if ((HWVOICE_FLAGS(sv) & 0x10000000090ULL) == 0x90)
        {
            HWVOICE_FLAGS(sv) &= ~0x90;
            hwKeyOff(voice);
            if ((HWVOICE_FLAGS(sv) & 0x20000000000ULL) != 0 &&
                (u32)adsrRelease(&sv->pitchADSR) != 0)
            {
                HWVOICE_FLAGS(sv) &= ~0x20000000000ULL;
            }
        }
    }

    UpdateTimeMIDICtrl(sv);
}

void synthAddJob(SynthDelayedNode* fade, int mode, u32 delay)
{
    SynthDelayedNode* newJq;
    SynthDelayedNode** root;
    u8 jobTabIndex;
    SynthJobTab* jobTab;

    jobTabIndex = ((delay / 256) + synthJobTableIndex) & 0x1F;
    jobTab = &synthJobTable[jobTabIndex];

    switch (mode)
    {
    case 0:
        newJq = fade;
        if (newJq->jobTabIndex != 0xFF)
        {
            if (newJq->jobTabIndex == jobTabIndex)
            {
                return;
            }
            if (newJq->next != 0)
            {
                newJq->next->prev = newJq->prev;
            }
            if (newJq->prev != 0)
            {
                newJq->prev->next = newJq->next;
            }
            else
            {
                synthJobTable[newJq->jobTabIndex].lowPrecision = newJq->next;
            }
        }
        root = &jobTab->lowPrecision;
        break;
    case 1:
        newJq = fade + 1;
        if (newJq->jobTabIndex != 0xFF)
        {
            if (newJq->jobTabIndex == jobTabIndex)
            {
                return;
            }
            if (newJq->next != 0)
            {
                newJq->next->prev = newJq->prev;
            }
            if (newJq->prev != 0)
            {
                newJq->prev->next = newJq->next;
            }
            else
            {
                synthJobTable[newJq->jobTabIndex].zeroOffset = newJq->next;
            }
        }
        root = &jobTab->zeroOffset;
        break;
    case 2:
        newJq = fade + 2;
        if (newJq->jobTabIndex != 0xFF)
        {
            return;
        }
        root = &jobTab->event;
        break;
    default:
        break;
    }

    newJq->jobTabIndex = jobTabIndex;
    if ((newJq->next = *root) != 0)
    {
        (*root)->prev = newJq;
    }
    newJq->prev = 0;
    *root = newJq;
}

/*
 * Reset four pos/timer fields on the handle, then advance both
 * channels (modes 0 and 1).
 */
void synthStartSynthJobHandling(McmdVoiceState* voice)
{
    *(u64*)&voice->lastLowCallTimeHi = synthRealTime;
    *(u64*)&voice->lastZeroCallTimeHi = synthRealTime;
    synthAddJob((SynthDelayedNode*)voice, 0, 0);
    synthAddJob((SynthDelayedNode*)voice, 1, 0);
}

void synthForceLowPrecisionUpdate(McmdVoiceState* voice)
{
    synthAddJob((SynthDelayedNode*)voice, 0, 0);
    synthAddJob((SynthDelayedNode*)voice, 1, 0);
}

void synthKeyStateUpdate(McmdVoiceState* voice)
{
    synthAddJob((SynthDelayedNode*)voice, 2, 0);
}

void HandleJobQueue(SynthDelayedNode** head, SynthDelayedBucketCallback callback)
{
    SynthDelayedNode* node = *head;
    while (node != 0)
    {
        SynthDelayedNode* next = node->next;
        node->jobTabIndex = 0xff;
        {
            if (synthVoice[node->voiceIndex].block == 0)
            {
                callback(node->voiceIndex);
            }
        }
        node = next;
    }
    *head = 0;
}

static inline void HandleVoices(void)
{
    SynthJobTab* jobTab = &synthJobTable[synthJobTableIndex];
    HandleJobQueue(&jobTab->lowPrecision, LowPrecisionHandler);
    HandleJobQueue(&jobTab->event, EventHandler);
    HandleJobQueue(&jobTab->zeroOffset, ZeroOffsetHandler);
    synthJobTableIndex = (synthJobTableIndex + 1) & 0x1f;
}

void HandleFaderTermination(SynthMasterFader* fade)
{
    switch (fade->seqMode)
    {
    case SND_SEQVOL_STOP:
        seqStop(fade->seqId);
        break;
    case SND_SEQVOL_PAUSE:
        seqPause(fade->seqId);
        break;
    case SND_SEQVOL_MUTE:
        seqMute(fade->seqId, 0, 0);
        break;
    }
}

/*
 * Periodic synth tick: drains delayed-action buckets, advances fade ramps,
 * runs AUX callbacks, and advances the global synth timer.
 */
void synthHandle(u32 deltaTime)
{
    u32 i;
    u32 s;
    SynthMasterFader* fade;
    u32 mask;

    if (synthInfo.numSamples == 0)
    {
        return;
    }

    macHandle(deltaTime);
    HandleVoices();
    if (hwGetTimeOffset() == 0)
    {
        if ((synthMasterFaderActiveFlags | synthMasterFaderPauseActiveFlags) != 0)
        {
            for (i = 0, fade = synthMasterFader, mask = 1; i < SYNTH_FADE_COUNT; mask <<= 1, ++i, ++fade)
            {
                if ((synthMasterFaderActiveFlags & mask) != 0)
                {
                    fade->volume = fade->target - fade->time * (fade->target - fade->start);
                    if ((fade->time -= fade->deltaTime) <= 0.f)
                    {
                        fade->volume = fade->target;
                        HandleFaderTermination(fade);
                        if (((synthMasterFaderActiveFlags &= ~mask) == 0) &&
                            (synthMasterFaderPauseActiveFlags == 0))
                        {
                            break;
                        }
                    }
                }
                if ((synthMasterFaderPauseActiveFlags & mask) != 0)
                {
                    fade->pauseVol = fade->pauseTarget - fade->pauseTime * (fade->pauseTarget - fade->pauseStart);
                    if ((fade->pauseTime -= fade->pauseDeltaTime) <= 0.f)
                    {
                        fade->pauseVol = fade->pauseTarget;
                        if (((synthMasterFaderPauseActiveFlags &= ~mask) == 0) &&
                            (synthMasterFaderActiveFlags == 0))
                        {
                            break;
                        }
                    }
                }
            }
        }
        for (s = 0; s < 8; ++s)
        {
            if (synthAuxAMIDI[s] != 0xff)
            {
                SynthAuxInfo info;
                for (i = 0; i < 4; ++i)
                {
                    info.data.parameterUpdate.para[i] = inpGetAuxA(s, i, synthAuxAMIDI[s], synthAuxAMIDISet[s]);
                }
                synthAuxACallback[s](1, &info, synthAuxAUser[s]);
            }
            if (synthAuxBMIDI[s] != 0xff)
            {
                SynthAuxInfo info;
                for (i = 0; i < 4; ++i)
                {
                    info.data.parameterUpdate.para[i] = inpGetAuxB(s, i, synthAuxBMIDI[s], synthAuxBMIDISet[s]);
                }
                synthAuxBCallback[s](1, &info, synthAuxBUser[s]);
            }
        }
    }
    hwFrameDone();
    synthRealTime += deltaTime;
}

/*
 * Start an FX sample by id, applying default volume/pan sentinels.
 */
u32 synthFXStart(u16 fxId, u8 volume, u8 pan, u8 studio, u32 studioAux)
{
    FX_TAB* fx;
    u32 handle;

    handle = 0xFFFFFFFF;
    fx = dataGetFX(fxId);
    if (fx != NULL)
    {
        if (volume == 0xff)
        {
            volume = fx->volume;
        }
        if (pan == 0xff)
        {
            pan = fx->panning;
        }
        handle = synthStartSound(fx->macro, fx->priority, fx->maxVoices, fx->key | 0x80,
                                 volume, pan, 0xff, 0xff, 0, 0, 0xff, fx->vGroup,
                                 0, studio, studioAux);
    }
    return handle;
}

#define SND_USERMUSIC_VOLGROUPS 0xfa
#define SND_USERFX_VOLGROUPS    0xfb
#define SND_USERALL_VOLGROUPS   0xfc
#define SND_MUSIC_VOLGROUPS     0xfd
#define SND_FX_VOLGROUPS        0xfe
#define SND_ALL_VOLGROUPS       0xff
#define SYNTH_FADE_TYPE_MUSIC     0
#define SYNTH_FADE_TYPE_FX        1
#define SYNTH_FADE_TYPE_USERMUSIC 2
#define SYNTH_FADE_TYPE_USERFX    3

/*
 * synthFXSetCtrl - sndFXCtrl underlying impl.
 * Walks the handle's voice-slot chain, dispatching inpSetMidiCtrl per slot.
 */
u32 synthFXSetCtrl(u32 handle, u8 controller, u8 value)
{
    u32 found;
    u8 idx;
    McmdVoiceState* slot;

    found = 0;
    handle = vidGetInternalId(handle);
    while (handle != 0xFFFFFFFFu)
    {
        idx = handle;
        if (handle == synthVoice[idx].id)
        {
            slot = &synthVoice[idx];
            if ((*(u64*)&slot->cFlagsHi & 2) != 0)
            {
                inpSetMidiCtrl(controller, idx, slot->setup.midiSet, value);
            }
            else
            {
                inpSetMidiCtrl(controller, idx, slot->midiSet, value);
            }
            found = 1;
            handle = synthVoice[idx].child;
        }
        else
        {
            return found;
        }
    }
    return found;
}

/*
 * synthFXSetCtrl14 - sndFXCtrl14 underlying impl.
 */
u32 synthFXSetCtrl14(u32 handle, u8 controller, u16 value)
{
    u32 found;
    u8 idx;
    McmdVoiceState* slot;

    found = 0;
    handle = vidGetInternalId(handle);
    while (handle != 0xFFFFFFFFu)
    {
        idx = handle;
        if (handle == synthVoice[idx].id)
        {
            slot = &synthVoice[idx];
            if ((*(u64*)&slot->cFlagsHi & 2) != 0)
            {
                inpSetMidiCtrl14(controller, idx, slot->setup.midiSet, value);
            }
            else
            {
                inpSetMidiCtrl14(controller, idx, slot->midiSet, value);
            }
            found = 1;
            handle = synthVoice[idx].child;
        }
        else
        {
            return found;
        }
    }
    return found;
}

/*
 * synthFXCloneMidiSetup - copies the five FX-stage controllers
 * (volume, pan, expression, reverb, chorus) between two handles.
 */
void synthFXCloneMidiSetup(McmdVoiceState* dstVoice, McmdVoiceState* srcVoice)
{
    inpFXCopyCtrl(0x07, dstVoice, srcVoice);
    inpFXCopyCtrl(0x0A, dstVoice, srcVoice);
    inpFXCopyCtrl(0x5B, dstVoice, srcVoice);
    inpFXCopyCtrl(0x80, dstVoice, srcVoice);
    inpFXCopyCtrl(0x84, dstVoice, srcVoice);
}

/*
 * synthSendKeyOff - sndFXKeyOff underlying impl.
 * Walks the handle's voice-slot chain and signals key-off on each slot.
 */
u32 synthSendKeyOff(u32 handle)
{
    u32 found;
    u32 idx;

    found = 0;
    if (sndActive != 0)
    {
        handle = vidGetInternalId(handle);
        while (handle != 0xFFFFFFFFu)
        {
            idx = (u8)handle;
            if (handle == synthVoice[idx].id)
            {
                macSetExternalKeyoff(&synthVoice[idx]);
                found = 1;
            }
            handle = synthVoice[idx].child;
        }
    }
    return found;
}

/*
 * Route synth fade commands to one slot or to the broadcast pseudo-slots
 * 0xfa through 0xff.
 */
static inline void SetupFader(SynthMasterFader* fade, u8 volume, u32 time, u8 seqMode, u32 seqId)
{
    fade->seqMode = seqMode;
    fade->seqId = seqId;
    if (time != 0)
    {
        fade->start = fade->volume;
        fade->target = (f32)volume * (1.f / 127.f);
        fade->time = 1.f;
        fade->deltaTime = 1280.f / (f32)time;
    }
    else
    {
        fade->volume = fade->target = (f32)volume * (1.f / 127.f);
        if (fade->seqId != SYNTH_INVALID_LINK_ID)
        {
            HandleFaderTermination(fade);
        }
    }
}

void synthVolume(u8 volume, u16 time, u8 vGroup, u8 seqMode, u32 seqId)
{
    u32 convertedTime;
    u32 i;
    u8 matchType;
    SynthMasterFader* fade;

    if ((convertedTime = time) != 0)
    {
        sndConvertMs(&convertedTime);
    }

    do
    {
        switch (vGroup)
        {
        case SND_ALL_VOLGROUPS:
            for (fade = synthMasterFader, i = 0; i < SYNTH_FADE_COUNT; ++i, ++fade)
            {
                if (fade->type == SYNTH_FADE_TYPE_MUSIC || fade->type == SYNTH_FADE_TYPE_FX)
                {
                    SetupFader(fade, volume, convertedTime, seqMode, SYNTH_INVALID_LINK_ID);
                    synthMasterFaderActiveFlags |= 1U << i;
                }
            }
            return;

        case SND_USERALL_VOLGROUPS:
            for (fade = synthMasterFader, i = 0; i < SYNTH_FADE_COUNT; ++i, ++fade)
            {
                if (fade->type == SYNTH_FADE_TYPE_USERMUSIC || fade->type == SYNTH_FADE_TYPE_USERFX)
                {
                    SetupFader(fade, volume, convertedTime, seqMode, SYNTH_INVALID_LINK_ID);
                    synthMasterFaderActiveFlags |= 1U << i;
                }
            }
            return;

        case SND_USERMUSIC_VOLGROUPS:
            matchType = SYNTH_FADE_TYPE_USERMUSIC;
            break;

        case SND_USERFX_VOLGROUPS:
            matchType = SYNTH_FADE_TYPE_USERFX;
            break;

        case SND_MUSIC_VOLGROUPS:
            matchType = SYNTH_FADE_TYPE_MUSIC;
            break;

        case SND_FX_VOLGROUPS:
            matchType = SYNTH_FADE_TYPE_FX;
            break;

        default:
            continue;
        }

        for (fade = synthMasterFader, i = 0; i < SYNTH_FADE_COUNT; ++i, ++fade)
        {
            if (fade->type == matchType)
            {
                SetupFader(fade, volume, convertedTime, seqMode, SYNTH_INVALID_LINK_ID);
                synthMasterFaderActiveFlags |= 1U << i;
            }
        }
        return;
    } while (0);

    SetupFader(&synthMasterFader[vGroup], volume, convertedTime, seqMode, seqId);
    synthMasterFaderActiveFlags |= 1U << vGroup;
}
int synthIsFadeOutActive(u8 voiceIdx)
{
    if (((synthMasterFader[voiceIdx].type != SYNTH_FADE_ACTION_DISABLED) &&
         ((synthMasterFaderActiveFlags & (1U << voiceIdx)) != 0)) &&
        (synthMasterFader[voiceIdx].start > synthMasterFader[voiceIdx].target))
    {
        return 1;
    }
    return 0;
}

/*
 * Set a single byte field on a voice slot.
 */
void synthSetMusicVolumeType(u8 voiceIdx, u8 value)
{
    if (sndActive == 0)
    {
        return;
    }
    synthMasterFader[voiceIdx].type = value;
}

/*
 * Voice command dispatcher: runs different actions per command code.
 *   0 -> validate current sample and mark the slot active
 *   1 -> voiceKill
 *   2 -> claim virtual sample slot
 *   3 -> simple vacate via hwGetVirtualSampleID + vsSampleEndNotify
 */
u32 synthHWMessageHandler(u32 mode, u32 arg)
{
    u32 result = 0;

    switch (mode)
    {
    case 0:
    {
        if (synthVoice[arg & 0xff].block != 0)
        {
            break;
        }
        vsSampleEndNotify(hwGetVirtualSampleID(arg & 0xff));
        if (arg != synthVoice[arg & 0xff].id)
        {
            break;
        }
        macSampleEndNotify(&synthVoice[arg & 0xff]);
        break;
    }
    case 1:
        voiceKill(arg & 0xff);
        break;
    case 2:
        result = vsSampleStartNotify(arg & 0xff);
        break;
    case 3:
    {
        vsSampleEndNotify(hwGetVirtualSampleID(arg & 0xff));
        break;
    }
    }
    return result;
}

static inline void synthInitJobQueue(void)
{
    u8 i;

    for (i = 0; i < 32; ++i)
    {
        synthJobTable[i].lowPrecision = 0;
        synthJobTable[i].event = 0;
        synthJobTable[i].zeroOffset = 0;
    }

    synthJobTableIndex = 0;
}

void synthInit(u32 sampleRate, u32 voiceCount)
{
    u32 voiceIndex;
    u32 fadeIndex;
    u32 auxIndex;
    f32 unusedA[2];

    synthRealTime = 0;
    synthInfo.sampleRate = sampleRate;
    synthSetBpm(120, 255, 0);
    synthFlags = 0;
    synthMessageCallback = 0;

    synthVoice = salMalloc(voiceCount * SYNTH_VOICE_STRIDE);
    memset(synthVoice, 0, voiceCount * SYNTH_VOICE_STRIDE);

    for (voiceIndex = 0; voiceIndex < voiceCount; voiceIndex++)
    {
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0xF4) = SYNTH_INVALID_LINK_ID;
        *(u64*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x114) = 0;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x110) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x10C) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x121) = 0xFF;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x154) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x192) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x190) = 0x80;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x191) = 0;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x180) = 0x400000;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x170) = 0x400000;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x184) = 0;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x174) = 0;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x1A0) = 0;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x1A4) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x1B8) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x1B9) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x11C) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x11E) = 0x17;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x104) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x193) = 1;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x1C0) = 0;
        *(u16*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x1C4) = 0;
        *(u16*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x1C6) = 0x7FFF;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x1CC) = 0;
        *(u16*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x1D0) = 0;
        *(u16*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x1D2) = 0x7FFF;
        *(u32*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x13C) = 0x6400;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x131) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x11F) = 0;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x08) = (u8)voiceIndex;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x09) = 0xFF;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x14) = (u8)voiceIndex;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x15) = 0xFF;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x20) = (u8)voiceIndex;
        *(u8*)((u8*)synthVoice + voiceIndex * SYNTH_VOICE_STRIDE + 0x21) = 0xFF;
    }

    {
        SynthMasterFader* fade = synthMasterFader;
        u32 pass;

        for (pass = 0; pass < 2; pass++)
        {
            fade[0].volume = 0.f;
            fade[0].pauseVol = 1.f;
            fade[0].type = SYNTH_FADE_ACTION_DISABLED;
            fade[1].volume = 0.f;
            fade[1].pauseVol = 1.f;
            fade[1].type = SYNTH_FADE_ACTION_DISABLED;
            fade[2].volume = 0.f;
            fade[2].pauseVol = 1.f;
            fade[2].type = SYNTH_FADE_ACTION_DISABLED;
            fade[3].volume = 0.f;
            fade[3].pauseVol = 1.f;
            fade[3].type = SYNTH_FADE_ACTION_DISABLED;
            fade[4].volume = 0.f;
            fade[4].pauseVol = 1.f;
            fade[4].type = SYNTH_FADE_ACTION_DISABLED;
            fade[5].volume = 0.f;
            fade[5].pauseVol = 1.f;
            fade[5].type = SYNTH_FADE_ACTION_DISABLED;
            fade[6].volume = 0.f;
            fade[6].pauseVol = 1.f;
            fade[6].type = SYNTH_FADE_ACTION_DISABLED;
            fade[7].volume = 0.f;
            fade[7].pauseVol = 1.f;
            fade[7].type = SYNTH_FADE_ACTION_DISABLED;
            fade[8].volume = 0.f;
            fade[8].pauseVol = 1.f;
            fade[8].type = SYNTH_FADE_ACTION_DISABLED;
            fade[9].volume = 0.f;
            fade[9].pauseVol = 1.f;
            fade[9].type = SYNTH_FADE_ACTION_DISABLED;
            fade[10].volume = 0.f;
            fade[10].pauseVol = 1.f;
            fade[10].type = SYNTH_FADE_ACTION_DISABLED;
            fade[11].volume = 0.f;
            fade[11].pauseVol = 1.f;
            fade[11].type = SYNTH_FADE_ACTION_DISABLED;
            fade[12].volume = 0.f;
            fade[12].pauseVol = 1.f;
            fade[12].type = SYNTH_FADE_ACTION_DISABLED;
            fade[13].volume = 0.f;
            fade[13].pauseVol = 1.f;
            fade[13].type = SYNTH_FADE_ACTION_DISABLED;
            fade[14].volume = 0.f;
            fade[14].pauseVol = 1.f;
            fade[14].type = SYNTH_FADE_ACTION_DISABLED;
            fade[15].volume = 0.f;
            fade[15].pauseVol = 1.f;
            fade[15].type = SYNTH_FADE_ACTION_DISABLED;
            fade += 16;
        }
    }

    synthMasterFaderActiveFlags = 0;
    synthMasterFaderPauseActiveFlags = 0;
    synthMasterFader[31].type = 1;
    for (fadeIndex = 0; fadeIndex < 8; fadeIndex++)
    {
        synthMasterFader[fadeIndex + 23].type = 0;
    }
    synthMasterFader[21].volume = 1.f;
    synthMasterFader[22].volume = 1.f;

    inpInit(NULL);

    for (auxIndex = 0; auxIndex < 8; auxIndex++)
    {
        synthAuxACallback[auxIndex] = 0;
        synthAuxAMIDI[auxIndex] = 0xFF;
        synthAuxBCallback[auxIndex] = 0;
        synthAuxBMIDI[auxIndex] = 0xFF;
        synthITDDefault[auxIndex].sfx = 0;
        synthITDDefault[auxIndex].music = 0;
    }

    macInit();
    vidInit();
    synthInitAllocationAids();

    for (auxIndex = 0; auxIndex < 16; auxIndex++)
    {
        synthGlobalVariable[auxIndex] = 0;
    }

    voiceInitLastStarted();

    synthInitJobQueue();
    hwSetMesgCallback(synthHWMessageHandler);
}

void synthExit(void)
{
    salFree(synthVoice);
}
