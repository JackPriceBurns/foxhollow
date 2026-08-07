#include "src/musyx/runtime/synth_internal.h"
#include "musyx/synth_delay.h"


#define SYNTH_CALLBACK_ACTIVE_LIST_COUNT    2
#define SYNTH_CALLBACK_COMPLETED_LIST_INDEX 2

void ResetNotes(SynthVoice* voice)
{
    SynthCallbackLink* callback;

    s32 listIndex;

    for (listIndex = 0; listIndex < SYNTH_CALLBACK_ACTIVE_LIST_COUNT; listIndex++)
    {
        if ((callback = voice->callbackLists[listIndex]) != 0)
        {
            while (callback->next != 0)
            {
                callback = callback->next;
            }

            if (noteFree != 0)
            {
                callback->next = noteFree;
                noteFree->prev = callback;
            }

            noteFree = voice->callbackLists[listIndex];
            voice->callbackLists[listIndex] = 0;
        }
    }

    if ((callback = voice->callbackLists[SYNTH_CALLBACK_COMPLETED_LIST_INDEX]) != 0)
    {
        while (callback->next != 0)
        {
            callback = callback->next;
        }

        if (noteFree != 0)
        {
            callback->next = noteFree;
            noteFree->prev = callback;
        }

        noteFree = voice->callbackLists[SYNTH_CALLBACK_COMPLETED_LIST_INDEX];
        voice->callbackLists[SYNTH_CALLBACK_COMPLETED_LIST_INDEX] = 0;
    }
}

SynthCallbackLink* AllocateNote(s32 triggerValue, u8 controllerIndex)
{
    SynthCallbackLink* callback;
    SynthCallbackLink* next;
    register SynthCallbackLink* current;
    register SynthCallbackLink* prev;

    if ((callback = noteFree) != 0)
    {
        noteFree = next = callback->next;
        if (next != 0)
        {
            noteFree->prev = 0;
        }

        callback->triggerValue = triggerValue;
        callback->controllerIndex = controllerIndex;
        prev = 0;
        callback->listIndex = cseq->section[controllerIndex].timeIndex;

        current = cseq->callbackLists[callback->listIndex];
        while (current != 0)
        {
            if (current->triggerValue > callback->triggerValue)
            {
                callback->next = current;
                callback->prev = prev;
                if (prev != 0)
                {
                    prev->next = callback;
                }
                else
                {
                    cseq->callbackLists[callback->listIndex] = callback;
                }
                current->prev = callback;
                return callback;
            }

            prev = current;
            current = current->next;
        }

        callback->prev = prev;
        if (prev != 0)
        {
            prev->next = callback;
        }
        else
        {
            cseq->callbackLists[callback->listIndex] = callback;
        }
        callback->next = 0;
    }

    return callback;
}

s32 HandleNotes(void)
{
    SynthCallbackLink* callback;
    u32 listIndex;
    SynthCallbackLink* next;
    SynthCallbackLink* completed;

    for (listIndex = 0; listIndex < SYNTH_CALLBACK_ACTIVE_LIST_COUNT; listIndex++)
    {
        if ((callback = cseq->callbackLists[listIndex]) != 0)
        {
            while (callback->triggerValue <=
                   (s32)cseq->section[callback->controllerIndex].time[listIndex].high)
            {
                synthSendKeyOff(callback->callbackId);
                next = callback->next;
                cseq->callbackLists[listIndex] = next;
                if (next != 0)
                {
                    cseq->callbackLists[listIndex]->prev = 0;
                }

                completed = cseq->callbackLists[SYNTH_CALLBACK_COMPLETED_LIST_INDEX];
                callback->next = completed;
                if (completed != 0)
                {
                    cseq->callbackLists[SYNTH_CALLBACK_COMPLETED_LIST_INDEX]->prev = callback;
                }
                cseq->callbackLists[SYNTH_CALLBACK_COMPLETED_LIST_INDEX] = callback;
                if ((callback = cseq->callbackLists[listIndex]) == 0)
                {
                    break;
                }
            }
        }
    }

    return cseq->callbackLists[0] != 0 || cseq->callbackLists[1] != 0;
}

void KeyOffNotes(void)
{
    SynthCallbackLink* callback;
    SynthCallbackLink* next;
    u32 listIndex;
    SynthCallbackLink* completed;

    for (listIndex = 0; listIndex < SYNTH_CALLBACK_ACTIVE_LIST_COUNT; listIndex++)
    {
        callback = cseq->callbackLists[listIndex];
        while (callback != 0)
        {
            next = callback->next;
            synthSendKeyOff(callback->callbackId);
            completed = callback->next;
            cseq->callbackLists[listIndex] = completed;
            if (completed != 0)
            {
                cseq->callbackLists[listIndex]->prev = 0;
            }

            completed = cseq->callbackLists[SYNTH_CALLBACK_COMPLETED_LIST_INDEX];
            callback->next = completed;
            if (completed != 0)
            {
                cseq->callbackLists[SYNTH_CALLBACK_COMPLETED_LIST_INDEX]->prev = callback;
            }
            cseq->callbackLists[SYNTH_CALLBACK_COMPLETED_LIST_INDEX] = callback;
            callback = next;
        }
    }
}

void seqFreeKeyOffNote(SynthCallbackLink* callback)
{
    if (callback->next != 0)
    {
        callback->next->prev = callback->prev;
    }

    if (callback->prev != 0)
    {
        callback->prev->next = callback->next;
    }
    else
    {
        cseq->callbackLists[SYNTH_CALLBACK_COMPLETED_LIST_INDEX] = callback->next;
    }

    {
        SynthCallbackLink* freeCallback = noteFree;
        callback->next = freeCallback;
        if (freeCallback != 0)
        {
            noteFree->prev = callback;
        }
    }

    callback->prev = 0;
    noteFree = callback;
}

u32 GetPublicId(s32 voiceIndex)
{
    SynthVoice* queuedVoices;
    SynthVoice* allocatedVoices;
    u32 handle;
    SynthVoice* current;

    queuedVoices = seqActiveRoot;
    allocatedVoices = seqPausedRoot;
    do
    {
        handle = seq_next_id;
        seq_next_id = handle + 1;
        seq_next_id &= SYNTH_HANDLE_ID_MASK;

        for (current = queuedVoices; current != 0; current = current->next)
        {
            if (current->handle == handle)
            {
                handle = SYNTH_HANDLE_INVALID;
                break;
            }
        }

        for (current = allocatedVoices; current != 0; current = current->next)
        {
            if (current->handle == handle)
            {
                handle = SYNTH_HANDLE_INVALID;
                break;
            }
        }
    } while (handle == SYNTH_HANDLE_INVALID);

    seqInstance[voiceIndex].handle = handle;
    return handle;
}

u32 seqGetPrivateId(u32 seqId)
{
    SynthVoice* voice;
    for (voice = seqActiveRoot; voice != 0; voice = voice->next)
    {
        if (voice->handle == (seqId & SYNTH_HANDLE_ID_MASK))
        {
            return voice->slotIndex | (seqId & SYNTH_HANDLE_QUEUED_FLAG);
        }
    }

    for (voice = seqPausedRoot; voice != 0; voice = voice->next)
    {
        if (voice->handle == (seqId & SYNTH_HANDLE_ID_MASK))
        {
            return voice->slotIndex | (seqId & SYNTH_HANDLE_QUEUED_FLAG);
        }
    }

    return SYNTH_HANDLE_INVALID;
}

SynthCallbackLink seqNote[SYNTH_CALLBACK_COUNT];
