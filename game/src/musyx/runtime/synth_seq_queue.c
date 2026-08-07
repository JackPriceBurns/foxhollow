#include "src/musyx/runtime/synth_internal.h"
#include "musyx/synth_seq_events.h"

void synthInitChannelEventQueues(void)
{
    SynthSequenceEvent* event;
    SynthSequenceQueue* queue;
    SynthVoice* voice;
    u8* keyGroupMap;
    u8 channel;

    voice = cseq;
    keyGroupMap = voice->keyGroupMap;
    if (keyGroupMap == 0)
    {
        queue = &voice->section[0];
        for (channel = 0; channel < SYNTH_SEQUENCE_TRACK_COUNT; channel++)
        {
            event = GenerateNextTrackEvent(channel);
            if (event != 0)
            {
                InsertGlobalEvent(queue, event);
            }
        }
        return;
    }

    for (channel = 0; channel < SYNTH_SEQUENCE_TRACK_COUNT; channel++)
    {
        event = GenerateNextTrackEvent(channel);
        if (event != 0)
        {
            InsertGlobalEvent(&voice->section[keyGroupMap[channel]], event);
        }
    }
}

void synthRefreshChannelEventQueue(u8 groupIndex)
{
    SynthSequenceEvent* event;
    SynthSequenceQueue* queue;
    SynthVoice* voice;
    u8* keyGroupMap;
    u8 channel;

    voice = cseq;
    keyGroupMap = voice->keyGroupMap;
    if (keyGroupMap == 0)
    {
        queue = &voice->section[0];
        for (channel = 0; channel < SYNTH_SEQUENCE_TRACK_COUNT; channel++)
        {
            event = GenerateNextTrackEvent(channel);
            if (event != 0)
            {
                InsertGlobalEvent(queue, event);
            }
        }
        return;
    }

    queue = &voice->section[groupIndex];
    for (channel = 0; channel < SYNTH_SEQUENCE_TRACK_COUNT; channel++)
    {
        if (keyGroupMap[channel] == groupIndex)
        {
            event = GenerateNextTrackEvent(channel);
            if (event != 0)
            {
                InsertGlobalEvent(queue, event);
            }
        }
    }
}
