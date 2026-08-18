#include "src/musyx/runtime/synth_internal.h"

void synthInitChannelEventQueues(void) {
    SynthVoice* voice = cseq;
    if (voice->keyGroupMap == 0) {
        SynthSequenceQueue* queue = &voice->section[0];
        for (u8 i = 0; i < SYNTH_SEQUENCE_TRACK_COUNT; i++) {
            SynthSequenceEvent* event = GenerateNextTrackEvent(i);
            if (event != 0) {
                InsertGlobalEvent(queue, event);
            }
        }

        return;
    }

    for (u8 i = 0; i < SYNTH_SEQUENCE_TRACK_COUNT; i++) {
        SynthSequenceEvent* event = GenerateNextTrackEvent(i);
        if (event != 0) {
            InsertGlobalEvent(&voice->section[voice->keyGroupMap[i]], event);
        }
    }
}

void synthRefreshChannelEventQueue(u8 groupIndex) {
    if (cseq->keyGroupMap == 0) {
        SynthSequenceQueue* queue = &cseq->section[0];
        for (u8 i = 0; i < SYNTH_SEQUENCE_TRACK_COUNT; i++) {
            SynthSequenceEvent* event = GenerateNextTrackEvent(i);
            if (event != 0) {
                InsertGlobalEvent(queue, event);
            }
        }

        return;
    }

    SynthSequenceQueue* queue = &cseq->section[groupIndex];
    for (u8 i = 0; i < SYNTH_SEQUENCE_TRACK_COUNT; i++) {
        if (cseq->keyGroupMap[i] == groupIndex) {
            SynthSequenceEvent* event = GenerateNextTrackEvent(i);
            if (event != 0) {
                InsertGlobalEvent(queue, event);
            }
        }
    }
}
