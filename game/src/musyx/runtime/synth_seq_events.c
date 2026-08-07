#include "musyx/synth_seq_events.h"

#define SYNTH_TRACK_COMMAND_END  0xFFFF
#define SYNTH_TRACK_COMMAND_JUMP 0xFFFE

#define TRACK_CMD(cursor) ((SynthTrackCommand*)(cursor)->current)

SynthSequenceEvent* GenerateNextTrackEvent(u8 channel)
{
    u32 trackId;
    SynthTrackCursor* track;
    SynthSequenceEvent* ev;
    SynthSequenceState* pattern;
    u32 patternTime;
    u32 pitchTime;
    u32 modTime;

    trackId = channel;
    track = &cseq->track[channel];
    pattern = &cseq->pattern[trackId];

    if (track->current != 0)
    {
        ev = &cseq->channelEvents[trackId];
        ev->trackId = channel;
        ev->state = pattern;

        for (;;)
        {
            if (pattern->noteData == 0)
            {
            process_track_command:
                if (TRACK_CMD(track)->command == SYNTH_TRACK_COMMAND_END)
                {
                    track->current = 0;
                    return 0;
                }

                if (TRACK_CMD(track)->command == SYNTH_TRACK_COMMAND_JUMP)
                {
                    if (cseq->keyGroupMap == 0)
                    {
                        if (cseq->section[0].loopDisable)
                        {
                            track->current = 0;
                            return 0;
                        }
                    }
                    else if (cseq->section[cseq->keyGroupMap[trackId]].loopDisable)
                    {
                        track->current = 0;
                        return 0;
                    }

                    ev->type = 3;
                    ev->time = TRACK_CMD(track)->value0;
                    track->current = track->base + TRACK_CMD(track)->arg * sizeof(SynthTrackCommand);
                    return ev;
                }

                ev->type = 4;
                ev->time = TRACK_CMD(track)->value0;
                ev->data = track->current;
                track->current = TRACK_CMD(track) + 1;
                return ev;
            }

            pitchTime = pattern->pitchBend.nextTime;
            modTime = pattern->modulation.nextTime;

            for (;;)
            {
                patternTime = *(u16*)pattern->noteData + pattern->lastTime;
                if (patternTime < pitchTime)
                {
                    if (patternTime >= modTime)
                    {
                        goto modulation_event;
                    }
                    if (pattern->noteData[2] == 0xFF && pattern->noteData[3] == 0xFF)
                    {
                        pattern->noteData = 0;
                        goto process_track_command;
                    }

                    ev->data = pattern->noteData;
                    pattern->lastTime = patternTime;

                    if ((pattern->noteData[2] & 0x80) != 0)
                    {
                        pattern->noteData += 4;
                    }
                    else if ((pattern->noteData[2] | pattern->noteData[3]) == 0)
                    {
                        pattern->noteData += 4;
                        continue;
                    }
                    else
                    {
                        pattern->noteData += 6;
                    }
                    ev->type = 0;
                    ev->time = patternTime + pattern->baseTime;
                }
                else if (pitchTime < modTime)
                {
                    ev->time = pitchTime + pattern->baseTime;
                    ev->type = 2;
                }
                else
                {
                modulation_event:
                    ev->time = modTime + pattern->baseTime;
                    ev->type = 1;
                }
                return ev;
            }
        }
    }

    return 0;
}

/*
 * Sorted-by-time insert into a channel event queue.
 */
void InsertGlobalEvent(SynthSequenceQueue* queue, SynthSequenceEvent* event)
{
    SynthSequenceEvent* current;
    SynthSequenceEvent* prev;

    prev = 0;
    current = queue->eventList;
    while (current != 0)
    {
        if (current->time > event->time)
        {
            event->next = current;
            event->prev = prev;
            if (prev != 0)
            {
                prev->next = event;
            }
            else
            {
                queue->eventList = event;
            }
            current->prev = event;
            return;
        }

        prev = current;
        current = current->next;
    }

    event->prev = prev;
    if (prev != 0)
    {
        prev->next = event;
    }
    else
    {
        queue->eventList = event;
    }
    event->next = 0;
}
