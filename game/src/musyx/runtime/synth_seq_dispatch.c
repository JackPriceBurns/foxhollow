#include "types.h"
#include "dolphin/MSL_C/PPCEABI/bare/H/floorf.h"
#include "musyx/inp_midi.h"
#include "musyx/synth_callback.h"
#include "musyx/synth_handle.h"
#include "musyx/synth_queue.h"
#include "musyx/synth_seq_dispatch.h"
#include "musyx/synth_seq_events.h"
#include "musyx/synth_voice.h"
#include "musyx/mcmd.h"
#include "musyx/hw_init.h"
#include "musyx/snd_synth_api.h"
#include "musyx/voice_id.h"
#include "musyx/voice_manage.h"
#include "musyx/synth_config.h"
#include "musyx/synth_job_queue.h"
#include "musyx/synth_channel_scale.h"
#include "dolphin/MSL_C/PPCEABI/bare/H/math_api.h"
#include "musyx/synth_volume.h"
#include "src/musyx/runtime/synth_internal.h"

typedef struct
{
    u32 time;       // 0x0
    u8 prgChange;   // 0x4
    u8 velocity;    // 0x5
    u8 res[2];      // 0x6
    u16 pattern;    // 0x8
    s8 transpose;   // 0xa
    s8 velocityAdd; // 0xb
} SeqTrackEntry;    // size 0xc

typedef struct
{
    u16 time;    // 0x0
    u8 key;      // 0x2
    u8 velocity; // 0x3
    u16 length;  // 0x4
} SeqNoteData;   // size 0x6

/* Standard MIDI controller (CC) numbers dispatched by the sequencer. */

/* Sequencer meta-command sub-codes (carried in the high nibble of a note event). */
#define SEQ_META_KEY_OFF       0x82
#define SEQ_META_START_PENDING 0x68
#define SEQ_META_LOOP_MARK     0x69
#define SEQ_META_LOOP_MARK_HI  0x6a
#define SEQ_META_RESET_CTRL    0x79
#define SEQ_META_ALL_NOTES_OFF 0x7b

/* Empty double-buffered time slot marker. */
#define SEQ_TIME_EMPTY 0x7fffffff


SynthVoice* seqActiveRoot;
SynthVoice* seqPausedRoot;
SynthVoice* seqFreeRoot;
u32 seq_next_id;
u8 curFadeOutState;
u32 curSeqId;
SynthCallbackLink* noteFree;
SynthVoice* cseq;

static inline void InitStream(SynthSequenceStream* stream, u32 streamDataOffset)
{
    u16 delta;

    if (streamDataOffset != 0)
    {
        if ((stream->cursor = GetStreamValue(
                 (u8*)(streamDataOffset + (u32)cseq->arrbase), &delta,
                 &stream->step)) != 0)
        {
            stream->nextTime = delta;
        }
        else
        {
            stream->nextTime = SEQ_TIME_EMPTY;
        }
    }
    else
    {
        stream->nextTime = SEQ_TIME_EMPTY;
    }
}

static inline u16 HandleStream(SynthSequenceStream* stream)
{
    u16 delta;

    stream->value += stream->step;
    if (stream->cursor != 0)
    {
        if ((stream->cursor = GetStreamValue(stream->cursor, &delta, &stream->step)) != 0)
        {
            stream->nextTime += delta;
        }
        else
        {
            stream->nextTime = SEQ_TIME_EMPTY;
        }
    }
    else
    {
        stream->nextTime = SEQ_TIME_EMPTY;
    }
    return stream->value;
}

static inline void DoPrgChange(SynthVoiceRuntime* rt, SynthVoice* voice, u8 program, u32 midi)
{
    rt->voiceNotes[curSeqId][midi] = 0xFFFF;
    if (midi != 9)
    {
        program = voice->normTrans[program];
        if (program == 0xff)
        {
            return;
        }
        voice->prgState[midi].macId = voice->normtab[program].macro;
        voice->prgState[midi].priority = voice->normtab[program].priority;
        voice->prgState[midi].maxVoices = voice->normtab[program].maxVoices;
        return;
    }
    program = voice->drumTrans[program];
    if (program == 0xff)
    {
        return;
    }
    voice->prgState[midi].macId = voice->drumtab[program].macro;
    voice->prgState[midi].priority = voice->drumtab[program].priority;
    voice->prgState[midi].maxVoices = voice->drumtab[program].maxVoices;
}

/*
 * Dispatch a queued voice/MIDI channel event by type, then pull the next
 * event for the channel.
 */
SynthSequenceEvent* HandleEvent(SynthSequenceEvent* event, u8 voice, u32* flag)
{
    SynthSequenceState* pa;
    SeqNoteData* pe;
    int velocity;
    int key;
    u32 midi;
    u16 macId;
    SynthCallbackLink* note;
    SeqTrackEntry* tEntry;
    SynthSequenceState* pattern;
    SynthVoiceRuntime* rt;

    rt = SYNTH_VOICE_RUNTIME();
    switch (event->type)
    {
    case 4:
    {
        SynthVoice* sv;
        u8* seq;
        SynthSeqPattern* pat;
        u8 prog;

        tEntry = (SeqTrackEntry*)event->data;
        sv = cseq;
        seq = sv->arrbase;
        pattern = &sv->pattern[event->trackId];
        pat = (SynthSeqPattern*)(*(u32*)(((SynthArrangement*)seq)->patternTableOffset + (u32)seq + tEntry->pattern * 4) +
                                 (u32)seq);
        pattern->noteData = (u8*)(pat + 1);
        pattern->lastTime = 0;
        pattern->baseTime = tEntry->time;
        pattern->patternInfo = tEntry;
        InitStream(&pattern->pitchBend, pat->pitchBendOffset);
        pattern->pitchBend.value = 0x2000;
        InitStream(&pattern->modulation, pat->modulationOffset);
        pattern->modulation.value = 0;
        pattern->midi = *(u8*)(((SynthArrangement*)cseq->arrbase)->trackMidiTableOffset +
                               (u32)cseq->arrbase + event->trackId);
        prog = tEntry->prgChange;
        if (prog != 0xff)
        {
            DoPrgChange(rt, cseq, prog, pattern->midi);
        }
        if (tEntry->velocity != 0xff)
        {
            inpSetMidiCtrl(MCMD_CTRL_VOLUME, pattern->midi, curSeqId & 0xff, tEntry->velocity);
        }
        break;
    }
    case 0:
        pe = (SeqNoteData*)event->data;
        pa = event->state;
        key = pe->key;
        velocity = pe->velocity;
        midi = pa->midi;

        if (key & 0x80)
        {
            switch (velocity)
            {
            case 0:
                DoPrgChange(rt, cseq, key & 0x7f, midi);
                break;
            case 1:
                inpSetMidiCtrl(SEQ_META_KEY_OFF, midi, curSeqId & 0xff, key & 0x7f);
                break;
            default:
                if ((velocity & 0x80) == 0x80)
                {
                    switch (velocity & 0x7f)
                    {
                    case SEQ_META_START_PENDING:
                        if (cseq->syncActive != 0)
                        {
                            seqCrossFade(&cseq->syncCrossInfo, cseq->syncSeqIdPtr, 1);
                            cseq->syncActive = 0;
                        }
                        break;
                    case SEQ_META_LOOP_MARK:
                        rt->voiceNotes[curSeqId][midi] = key & 0x7f;
                        break;
                    case SEQ_META_LOOP_MARK_HI:
                        rt->voiceNotes[curSeqId][midi] = (key & 0x7f) + 0x80;
                        break;
                    case SEQ_META_RESET_CTRL:
                        inpResetMidiCtrl(midi, curSeqId & 0xff, 0);
                        break;
                    case SEQ_META_ALL_NOTES_OFF:
                        KeyOffNotes();
                        break;
                    default:
                        inpSetMidiCtrl(velocity & 0x7f, midi, curSeqId & 0xff, key & 0x7f);
                        break;
                    }
                }
                break;
            }
        }
        else
        {
            SynthVoice* sv = cseq;
            if (sv->trackMute[event->trackId / 32] & (1 << (event->trackId & 0x1f)))
            {
                if ((macId = sv->prgState[midi].macId) != 0xFFFF)
                {
                    key += ((SeqTrackEntry*)pa->patternInfo)->transpose;
                    key = key > 0x7f ? 0x7f : key < 0 ? 0 : key;
                    velocity += ((SeqTrackEntry*)pa->patternInfo)->velocityAdd;
                    velocity = velocity > 0x7f ? 0x7f : velocity < 0 ? 0 : velocity;
                    if ((note = AllocateNote(event->time + pe->length, voice)) != NULL)
                    {
                        SynthVoice* sv2;
                        s16 mod;
                        u8 vt;
                        u8 tid;

                        mod = curFadeOutState != 0 ? -1 : 0;
                        sv2 = cseq;
                        tid = event->trackId;
                        vt = sv2->defStudio;
                        if ((note->callbackId =
                                 synthStartSound(macId, sv2->prgState[midi].priority,
                                                 sv2->prgState[midi].maxVoices, key & 0xff,
                                                 velocity & 0xff, 0x40, midi, curSeqId & 0xff,
                                                 voice, 0, tid, sv2->trackVolumeGroup[tid], mod, vt,
                                                 synthITDDefault[vt].music)) == 0xFFFFFFFF)
                        {
                            if (note->next != 0)
                            {
                                note->next->prev = note->prev;
                            }
                            if (note->prev != 0)
                            {
                                note->prev->next = note->next;
                            }
                            else
                            {
                                cseq->callbackLists[note->listIndex] = note->next;
                            }
                            if ((note->next = noteFree) != 0)
                            {
                                noteFree->prev = note;
                            }
                            note->prev = 0;
                            noteFree = note;
                        }
                    }
                }
            }
        }
        break;
    case 2:
        pa = event->state;
        inpSetMidiCtrl14(MCMD_CTRL_PITCH_BEND, pa->midi, curSeqId & 0xff,
                         HandleStream(&pa->pitchBend));
        break;
    case 1:
        pa = event->state;
        inpSetMidiCtrl14(MCMD_CTRL_MODULATION, pa->midi, curSeqId & 0xff,
                         HandleStream(&pa->modulation));
        break;
    case 3:
        *flag |= 1;
        return 0;
    }
    return GenerateNextTrackEvent(event->trackId);
}

/*
 * Queue each MIDI channel's initial event into its mapped sequence section.
 */
void InitTrackEvents(void)
{
    u32 i;
    SynthSequenceEvent* event;

    if (cseq->keyGroupMap == 0)
    {
        for (i = 0; i < 0x40; i++)
        {
            event = GenerateNextTrackEvent((u8)i);
            if (event != 0)
            {
                InsertGlobalEvent(&cseq->section[0], event);
            }
        }
    }
    else
    {
        for (i = 0; i < 0x40; i++)
        {
            event = GenerateNextTrackEvent((u8)i);
            if (event != 0)
            {
                InsertGlobalEvent(&cseq->section[cseq->keyGroupMap[i]], event);
            }
        }
    }
}

/* Queue the next event for every MIDI channel mapped to one sequence section. */
static void InitTrackEventsSection(u8 sectionIndex)
{
    u32 group;
    u32 i;
    SynthSequenceEvent* event;

    if (cseq->keyGroupMap == 0)
    {
        for (i = 0; i < 0x40; i++)
        {
            event = GenerateNextTrackEvent((u8)i);
            if (event != 0)
            {
                InsertGlobalEvent(&cseq->section[0], event);
            }
        }
    }
    else
    {
        group = sectionIndex & 0xff;
        for (i = 0; i < 0x40; i++)
        {
            if (group == cseq->keyGroupMap[i])
            {
                event = GenerateNextTrackEvent((u8)i);
                if (event != 0)
                {
                    InsertGlobalEvent(&cseq->section[group], event);
                }
            }
        }
    }
}

static inline u32 GetNextEventTime(SynthSequenceQueue* section)
{
    return section->eventList == NULL ? 0 : section->eventList->time;
}

static inline SynthSequenceEvent* GetGlobalEvent(SynthSequenceQueue* section)
{
    SynthSequenceEvent* ev;

    ev = section->eventList;
    if (ev != NULL && (section->eventList = ev->next) != NULL)
    {
        section->eventList->prev = NULL;
    }
    return ev;
}

static inline f32 seq_fmod(f32 x, f32 y)
{
    f32 ay;
    f32 ax;

    ay = __fabsf(y);
    ax = __fabsf(x);
    if (ay > ax)
    {
        return x;
    }
    return x - y * (f32)(s64)(u64)(x / y);
}

static inline void SetTickDelta(SynthSequenceQueue* section, u32 deltaTime)
{
    f32 tickDelta;

    tickDelta = (1.f / 40960000.f) * ((f32)section->bpm * deltaTime);
    tickDelta *= (1.f / 256.f) * (f32)section->speed;
    section->tickDelta[section->timeIndex].low = seq_fmod(65536.f * tickDelta, 65536.f);
    section->tickDelta[section->timeIndex].high = (int)floorf(tickDelta);
}

u32 HandleTrackEvents(u8 voice, u32 param)
{
    SynthSequenceQueue* vp;
    SynthSequenceEvent* event;
    SynthSequenceEvent* res;
    u32 flag;
    SynthTimeWord unusedTime;

    flag = 0;
    vp = (SynthSequenceQueue*)((u8*)cseq + voice * 56 + 0x14e8);
    while ((vp->eventList == NULL ? 0 : vp->eventList->time) <= vp->time[vp->timeIndex].high)
    {
        SynthSequenceEvent* ev = vp->eventList;
        if (ev != NULL && (vp->eventList = ev->next) != NULL)
        {
            vp->eventList->prev = NULL;
        }
        if ((event = ev) == NULL)
        {
            if (flag == 0)
            {
                return 0;
            }
            flag = 0;
            vp->timeIndex ^= 1;
            vp->time[vp->timeIndex].high = ((SynthArrangement*)cseq->arrbase)->loopPoint[voice];
            vp->time[vp->timeIndex].low = vp->time[vp->timeIndex ^ 1].low;
            {
                u8* voiceState = (u8*)(voice * 56);
                voiceState += (u32)cseq;
                if (*(void**)(voiceState + 0x14e8) != NULL)
                {
                    *(int*)(voiceState + 0x14ec) = *(int*)(voiceState + 0x14e8);
                    HandleMasterTrack(voice);
                    SetTickDelta((SynthSequenceQueue*)((u8*)cseq + voice * 56 + 0x14e8), param);
                }
            }
            vp->loopCount += 1;
            InitTrackEventsSection(voice);
            continue;
        }
        res = HandleEvent(event, voice, &flag);
        if (res != 0)
        {
            InsertGlobalEvent(vp, res);
        }
    }
    return 1;
}

/*
 * Per-sequence tick and event update pass.
 */
static inline f32 sal_fmod(f32 x, f32 y, f64 absy)
{
    s64 n;

    if (absy > __fabs(x))
    {
        return x;
    }
    n = (s64)(u64)(x / y);
    x = x - y * (f32)n;
    return x;
}

static inline void HandleKeyOffNotes(void)
{
    SynthCallbackLink* node;
    SynthCallbackLink* next;

    if (cseq->keyOffCheck == 0)
    {
        node = cseq->callbackLists[2];
        while (node != NULL)
        {
            next = node->next;
            if ((node->callbackId != 0xffffffff) && (sndFXCheck(node->callbackId) == 0xffffffff))
            {
                seqFreeKeyOffNote(node);
            }
            node = next;
        }
    }
    cseq->keyOffCheck = (cseq->keyOffCheck + 1) % 5;
}

static inline void SetTickDeltaInline(SynthSequenceQueue* section, u32 deltaTime, f32 c0, f32 c1, f32 range,
                                     f64 absRange)
{
    f32 tickDelta = c0 * ((f32)section->bpm * deltaTime);
    tickDelta = tickDelta * (c1 * (f32)(u32)section->speed);

    section->tickDelta[section->timeIndex].low = sal_fmod(range * tickDelta, range, absRange);
    *(int*)&section->tickDelta[section->timeIndex].high = floorf(tickDelta);
}

static inline void HandleMasterTrackInline(u8 secIndex)
{
    SynthSequenceQueue* section;
    u32* evt;

    section = &cseq->section[secIndex];
    if (section->masterTrackBase != NULL)
    {
        while (*(evt = (u32*)section->masterTrackCursor) != 0xffffffff)
        {
            if (*evt > section->time[section->timeIndex].high)
            {
                break;
            }
            if ((((SynthArrangement*)cseq->arrbase)->info & 0x40000000) != 0)
            {
                synthSetBpm((section->bpm = evt[1]) >> 10, curSeqId, secIndex);
            }
            else
            {
                synthSetBpm(evt[1], curSeqId, secIndex);
                section->bpm = ((u32*)section->masterTrackCursor)[1] << 10;
            }
            section->masterTrackCursor += 8;
        }
    }
}

void seqHandle(u32 deltaTime)
{
    u32 tickSum;
    u32 sectionIndex;
    u32 timeIndex;
    u32 eventsActive;
    u32 callbacksActive;
    SynthVoice* song;
    SynthVoice* nextSong;
    f64 absoluteTickRange;
    f32 tickRange;

    if (deltaTime != 0)
    {
        tickRange = 65536.f;
        song = seqActiveRoot;
        absoluteTickRange = __fabs(tickRange);
        for (; song != NULL; song = nextSong)
        {
            nextSong = song->next;
            cseq = song;
            curSeqId = song->slotIndex;
            curFadeOutState = synthIsFadeOutActive(song->defaultVolumeGroup);
            if (cseq->keyGroupMap == NULL)
            {
                HandleMasterTrackInline(0);
                SetTickDeltaInline(cseq->section, deltaTime, (1.f / 40960000.f), 0.00390625f, tickRange,
                                  absoluteTickRange);
                eventsActive = HandleTrackEvents(0, deltaTime);
                callbacksActive = HandleNotes();
                HandleKeyOffNotes();
                for (sectionIndex = 0; sectionIndex < 2; ++sectionIndex)
                {
                    tickSum = cseq->section[0].time[sectionIndex].low +
                              cseq->section[0].tickDelta[sectionIndex].low;
                    cseq->section[0].time[sectionIndex].low = tickSum & 0xffff;
                    tickSum = tickSum >> 16;
                    cseq->section[0].time[sectionIndex].high +=
                        tickSum + cseq->section[0].tickDelta[sectionIndex].high;
                }
            }
            else
            {
                eventsActive = 0;
                for (sectionIndex = 0; sectionIndex < 0x10; sectionIndex++)
                {
                    HandleMasterTrackInline(sectionIndex);
                    SetTickDeltaInline(&cseq->section[sectionIndex], deltaTime, (1.f / 40960000.f),
                                      0.00390625f, tickRange, absoluteTickRange);
                    eventsActive |= HandleTrackEvents(sectionIndex, deltaTime);
                }
                callbacksActive = HandleNotes();
                HandleKeyOffNotes();
                for (sectionIndex = 0; sectionIndex < 16; sectionIndex++)
                {
                    for (timeIndex = 0; timeIndex < 2; ++timeIndex)
                    {
                        tickSum = cseq->section[sectionIndex].time[timeIndex].low +
                                  cseq->section[sectionIndex].tickDelta[timeIndex].low;
                        cseq->section[sectionIndex].time[timeIndex].low = tickSum & 0xffff;
                        tickSum = tickSum >> 16;
                        cseq->section[sectionIndex].time[timeIndex].high +=
                            tickSum + cseq->section[sectionIndex].tickDelta[timeIndex].high;
                    }
                }
            }
            if ((eventsActive == 0) && (callbacksActive == 0))
            {
                if (song->prev != NULL)
                {
                    song->prev->next = nextSong;
                }
                else
                {
                    seqActiveRoot = nextSong;
                }
                if (nextSong != NULL)
                {
                    nextSong->prev = song->prev;
                }
                ResetNotes(song);
                song->state = 0;
                song->prev = NULL;
                if ((song->next = seqFreeRoot) != NULL)
                {
                    seqFreeRoot->prev = song;
                }
                seqFreeRoot = song;
            }
        }
    }
}

static inline void ClearNotes(SynthVoiceRuntime* runtime) {
    SynthCallbackLink* prev;
    SynthCallbackLink* callback;
    u32 i;

    prev = NULL;
    noteFree = &runtime->callbacks[0];
    for (i = 0; i < 0x100; i++) {
        callback = &runtime->callbacks[i];
        callback->prev = prev;
        if (prev != NULL) {
            prev->next = callback;
        }
        prev = callback;
    }
    prev->next = NULL;
}

/*
 * Initialize sequence instances, note priorities, and callback links.
 */
void seqInit(void)
{
    u16* note;
    SynthVoice* voice;
    SynthVoiceRuntime* runtime;
    u32 i;
    int j;

    runtime = SYNTH_VOICE_RUNTIME();
    seqActiveRoot = NULL;
    seqPausedRoot = NULL;
    voice = &runtime->voices[0];
    note = runtime->voiceNotes[0];
    for (i = 0; i < 8; i++)
    {
        if (i == 0)
        {
            seqFreeRoot = voice;
            voice->prev = NULL;
        }
        else
        {
            (voice - 1)->next = voice;
            voice->prev = &SYNTH_VOICE_RUNTIME()->voices[i - 1];
        }
        voice->slotIndex = i;
        voice->state = 0;
        for (j = 0; j < 16; j++)
        {
            note[j] = 0xffff;
        }
        note += 16;
        voice++;
    }
    runtime->voices[i - 1].next = NULL;

    ClearNotes(runtime);
    seq_next_id = 0;
}
