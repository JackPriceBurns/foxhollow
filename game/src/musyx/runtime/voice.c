#include "global.h"
#include "types.h"
#include "musyx/mcmd.h"
#include "musyx/vid_init.h"
#include "musyx/vid_get.h"
#include "musyx/hw_init.h"
#include "musyx/synth_jobs.h"
#include "musyx/synth_config.h"
#include "musyx/mcmd_exec.h"
#include "musyx/snd_core.h"
#include "musyx/vidlisttables.h"
#include "musyx/voice_id.h"
#include "musyx/voice_conv.h"
#include "musyx/voice_prio.h"
#include "musyx/voice_alloc.h"
#include "musyx/voice_manage.h"
#include "musyx/voice_unregister.h"
#include "musyx/hw_break.h"

u8 voiceFreeListRoot;
u8 voiceFreeListTail;
u8 voiceFxRunning;
u8 voiceMusicRunning;
u16 voicePrioSortedRoot;
VID_LIST* vidFree;
VID_LIST* vidRoot;
u32 vidCurrentId;

static VID_LIST vidList[128];
static u8 synth_last_started[SYNTH_VOICE_MIDI_CHANNEL_COUNT][SYNTH_VOICE_MIDI_KEY_COUNT];
static u8 synth_last_fxstarted[SYNTH_VOICE_DIRECT_SLOT_COUNT];
static SynthVoiceListNode voicePriorityLinks[0x40];
static u8 voicePriorityGroupHeads[0x100];
static SynthRootListNode voicePrioritySortLinks[0x100];
static SynthVoiceListNode voiceFreeListSlots[64];

void vidInit(void)
{
    int i;
    VID_LIST* prev;

    vidCurrentId = 0;
    vidRoot = 0;
    vidFree = vidList;
    for (prev = NULL, i = 0; i < 128; prev = &vidList[i], ++i)
    {
        vidList[i].prev = prev;
        if (prev != NULL)
        {
            prev->next = &vidList[i];
        }
    }
    prev->next = NULL;
}

VID_LIST* get_vidlist(u32 id)
{
    VID_LIST* node;
    u32 value;

    node = vidRoot;
    while (node != NULL)
    {
        value = node->vid;
        if (value == id)
        {
            return node;
        }
        if (value > id)
        {
            break;
        }
        node = node->next;
    }
    return NULL;
}

#define VID_UNLINK(field)                                                                                              \
    if (s->field->prev != 0)                                                                                           \
    {                                                                                                                  \
        s->field->prev->next = s->field->next;                                                                         \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        vidRoot = s->field->next;                                                                                      \
    }                                                                                                                  \
    if (s->field->next != 0)                                                                                           \
    {                                                                                                                  \
        s->field->next->prev = s->field->prev;                                                                         \
    }                                                                                                                  \
    s->field->next = vidFree;                                                                                          \
    if (vidFree != 0)                                                                                                  \
    {                                                                                                                  \
        vidFree->prev = s->field;                                                                                      \
    }                                                                                                                  \
    s->field->prev = 0;                                                                                                \
    vidFree = s->field

void vidRemoveVoiceReferences(McmdVoiceState* state)
{
    McmdVoiceState* s = state;
    if (s->id != 0xffffffff)
    {
        voiceResetLastStarted(state);
        if (s->parent != 0xffffffff)
        {
            synthVoice[s->parent & 0xff].child = s->child;
            if (s->child != 0xffffffff)
            {
                synthVoice[s->child & 0xff].parent = s->parent;
            }
            VID_UNLINK(vidList);
            s->vidList = 0;
        }
        else if (s->child != 0xffffffff)
        {
            s->vidList->root = s->child;
            synthVoice[s->child & 0xff].parent = 0xffffffff;
            synthVoice[s->child & 0xff].vidMasterList = s->vidMasterList;
            if (s->vidList != s->vidMasterList)
            {
                VID_UNLINK(vidList);
                s->vidList = 0;
            }
            s->vidList = 0;
            s->vidMasterList = 0;
        }
        else if (s->vidList != s->vidMasterList)
        {
            VID_UNLINK(vidList);
            s->vidList = 0;
            VID_UNLINK(vidMasterList);
            s->vidMasterList = 0;
        }
        else
        {
            VID_UNLINK(vidList);
            s->vidList = 0;
            s->vidMasterList = 0;
        }
    }
}

/*
 * Snapshot the current entry's `next` pointer (state->[0xf8]) into the
 * cached field (state->[0xfc]) and return that next entry's id field.
 */
u32 vidMakeRoot(McmdVoiceState* state)
{
    McmdVoiceState* s = state;
    s->vidMasterList = s->vidList;
    return s->vidList->vid;
}

/*
 * Allocate the next unique id from the global counter, walking the
 * sorted-by-id list to skip any already-in-use ids. Used to assign
 * fresh handles to dynamically-allocated voices.
 */
u32 vidMakeNew(McmdVoiceState* state, int returnNewId)
{
    McmdVoiceState* s = state;
    u32 nextId;
    VID_LIST* cursor;
    VID_LIST* node;
    VID_LIST* prev;
    VID_LIST* freeNode;

    do
    {
        nextId = vidCurrentId;
        vidCurrentId = nextId + 1;
    } while (nextId == 0xffffffffU);

    cursor = vidRoot;
    prev = 0;
    while ((node = cursor) != 0)
    {
        if (node->vid > nextId)
        {
            break;
        }
        if (node->vid == nextId)
        {
            do
            {
                nextId = vidCurrentId;
                vidCurrentId = nextId + 1;
            } while (nextId == 0xffffffffU);
        }
        prev = node;
        cursor = node->next;
    }

    if ((freeNode = vidFree) == 0)
    {
        return 0xffffffffU;
    }
    if ((vidFree = vidFree->next) != 0)
    {
        vidFree->prev = NULL;
    }
    if (prev == 0)
    {
        vidRoot = freeNode;
    }
    else
    {
        prev->next = freeNode;
    }
    freeNode->prev = prev;
    freeNode->next = node;
    if (node != 0)
    {
        node->prev = freeNode;
    }
    freeNode->vid = nextId;
    freeNode->root = s->id;
    s->vidMasterList = ((u32)returnNewId != 0) ? freeNode : NULL;
    s->vidList = freeNode;
    if ((u32)returnNewId != 0)
    {
        return nextId;
    }
    return s->id;
}

int vidGetInternalId(u32 id)
{
    VID_LIST* node;

    if (id != 0xffffffffU)
    {
        if ((node = get_vidlist(id)) != NULL)
        {
            return node->root;
        }
    }
    return -1;
}

/*
 * voiceRemovePriority - voice priority-queue removal. Removes the active
 * voice from its group's linked list and from the sorted priority list.
 */
void voiceRemovePriority(McmdVoiceState* state)
{
    McmdVoiceState* s = state;
    VidListTables* vb;
    SynthVoiceListNode* vps;
    SynthRootListNode* pr;

    vb = (VidListTables*)vidList;
    vps = &((SynthVoiceListNode*)((u8*)vb + offsetof(VidListTables, priorityLinks)))[s->id & 0xff];
    if (vps->user != 1)
    {
        return;
    }
    if (vps->prev != 0xff)
    {
        vb->priorityLinks[vps->prev].next = vps->next;
    }
    else
    {
        vb->priorityGroupHeads[s->prio] = vps->next;
    }
    if (vps->next != 0xff)
    {
        vb->priorityLinks[vps->next].prev = vps->prev;
    }
    else if (vps->prev == 0xff)
    {
        u32 prevv;
        pr = &((SynthRootListNode*)((u8*)vb + offsetof(VidListTables, prioritySortLinks)))[s->prio];
        prevv = pr->prev;
        if (prevv != 0xffff)
        {
            vb->prioritySortLinks[prevv].next = pr->next;
        }
        else
        {
            voicePrioSortedRoot = pr->next;
        }
        if (pr->next != 0xffff)
        {
            vb->prioritySortLinks[pr->next].prev = pr->prev;
        }
    }
    vps->user = 0;
}

#define VB_PRIO_HEAD(vb, p)      (*(u8*)((u8*)&(vb)->priorityGroupHeads[0] + (p)))
#define VB_PRIO_LINK(vb, i)      ((SynthVoiceListNode*)((u8*)&(vb)->priorityLinks[0] + (i) * sizeof(SynthVoiceListNode)))
#define VB_PRIO_SORT_NEXT(vb, p) (((SynthRootListNode*)&(vb)->prioritySortLinks[0])[p].next)
#define VB_PRIO_SORT_PREV(vb, p) \
    (((SynthRootListNode*)((u8*)&(vb)->prioritySortLinks[0] + offsetof(SynthRootListNode, prev)))[p].next)

/*
 * Insert the voice into the new priority group's list and keep the global
 * group list sorted by priority.
 */
void voiceSetPriority(McmdVoiceState* svoice, u8 prio)
{
    u32 voiceIdx;
    VidListTables* vb;
    u16 li;
    SynthVoiceListNode* vps;
    u16 root;
    u16 i;

    voiceIdx = (u8)svoice->id;
    vb = (VidListTables*)vidList;
    vps = VB_PRIO_LINK(vb, voiceIdx);
    if (vps->user == 1)
    {
        if (svoice->prio == prio)
        {
            return;
        }

        voiceRemovePriority(svoice);
    }

    vps->user = 1;
    vps->prev = 0xff;
    if ((vps->next = VB_PRIO_HEAD(vb, prio)) != 0xFF)
    {
        VB_PRIO_LINK(vb, VB_PRIO_HEAD(vb, prio))->prev = voiceIdx;
    }
    else if (root = voicePrioSortedRoot, root != 0xFFFF)
    {
        if (prio >= root)
        {
            for (i = root; i != 0xFFFF; i = VB_PRIO_SORT_NEXT(vb, i))
            {
                if (i > prio)
                {
                    break;
                }
                li = i;
            }

            VB_PRIO_SORT_NEXT(vb, li) = prio;
            VB_PRIO_SORT_PREV(vb, prio) = li;
            VB_PRIO_SORT_NEXT(vb, prio) = i;
            if (i != 0xFFFF)
            {
                VB_PRIO_SORT_PREV(vb, i) = prio;
            }
        }
        else
        {
            VB_PRIO_SORT_NEXT(vb, prio) = root;
            VB_PRIO_SORT_PREV(vb, prio) = 0xFFFF;
            VB_PRIO_SORT_PREV(vb, root) = prio;
            voicePrioSortedRoot = prio;
        }
    }
    else
    {
        VB_PRIO_SORT_NEXT(vb, prio) = 0xFFFF;
        VB_PRIO_SORT_PREV(vb, prio) = 0xFFFF;
        voicePrioSortedRoot = prio;
    }

    VB_PRIO_HEAD(vb, prio) = voiceIdx;
    svoice->prio = prio;
    hwSetPriority(svoice->id & 0xFF, ((u32)prio << 24) | (svoice->age >> 15));
}

#define VOICE_CFLAGS(i) (*(u64*)&synthVoice[i].cFlagsHi)

#define VB_PRIO_LINK_NEXT(vb, i) (((SynthVoiceListNode*)((u8*)&(vb)->priorityLinks[0] + (i) * 4))->next)
#define VOICE_PRIORITY_NONE      0xFFFF

/*
 * Allocate a voice id, preferring a free slot but stealing the lowest-priority
 * compatible active voice when limits are exceeded. (musyx synthvoice.c
 * voiceAllocate, pre-2.0.1 variant.)
 */
u32 voiceAllocate(u8 priority, u8 maxInstances, u16 allocId, u8 fxFlag)
{
    s32 i;
    s32 allocationCount;
    s32 selectedVoice;
    u16 prio;
    u32 restrictToStreamKind;
    u16 priorityGroupIndex;
    SynthVoiceListNode* freeSlot;
    SynthVoiceListNode* slotBase;
    VidListTables* voiceLists = (VidListTables*)vidList;

    if (!synthIdleWaitActive)
    {
        if (fxFlag)
        {
            restrictToStreamKind = (voiceFxRunning >= SYNTH_CONFIGURATION->fxVoiceCount &&
                                    SYNTH_CONFIGURATION->voiceCount > SYNTH_CONFIGURATION->fxVoiceCount);

            if (SYNTH_CONFIGURATION->fxVoiceCount <= maxInstances)
            {
                goto steal;
            }
        }
        else
        {
            restrictToStreamKind = (voiceMusicRunning >= SYNTH_CONFIGURATION->musicVoiceCount &&
                                    SYNTH_CONFIGURATION->voiceCount > SYNTH_CONFIGURATION->musicVoiceCount);

            if (SYNTH_CONFIGURATION->musicVoiceCount <= maxInstances)
            {
                goto steal;
            }
        }

        {
            allocationCount = 0;
            selectedVoice = -1;

            prio = voicePrioSortedRoot;
            while (prio != VOICE_PRIORITY_NONE && priority >= prio && selectedVoice == -1)
            {
                u32 group = prio;
                for (i = VB_PRIO_HEAD(voiceLists, group); i != SYNTH_INVALID_VOICE_U8;
                     i = VB_PRIO_LINK_NEXT(voiceLists, i))
                {
                    if (allocId != synthVoice[i].allocId)
                        continue;
                    ++allocationCount;
                    if (synthVoice[i].block)
                        continue;

                    if (!restrictToStreamKind || fxFlag == synthVoice[i].fxFlag)
                    {
                        if (VOICE_CFLAGS(i) & 2)
                            continue;
                        if (selectedVoice != -1)
                        {
                            if (synthVoice[i].age < synthVoice[selectedVoice].age)
                                selectedVoice = i;
                        }
                        else
                            selectedVoice = i;
                    }
                }

                prio = VB_PRIO_SORT_NEXT(voiceLists, group);
            }

            if (allocationCount >= maxInstances)
            {
                goto have_voice;
            }

            while (prio != VOICE_PRIORITY_NONE && allocationCount < maxInstances)
            {
                u32 group = prio;
                i = VB_PRIO_HEAD(voiceLists, group);
                while (i != SYNTH_INVALID_VOICE_U8)
                {
                    if (allocId == synthVoice[i].allocId)
                    {
                        allocationCount++;
                    }

                    i = VB_PRIO_LINK_NEXT(voiceLists, i);
                }

                prio = VB_PRIO_SORT_NEXT(voiceLists, group);
            }

            if (allocationCount >= maxInstances)
            {
                goto have_voice;
            }
        }

    steal:
        {
            selectedVoice = -1;
            if (voiceFreeListRoot != SYNTH_INVALID_VOICE_U8 && restrictToStreamKind == 0)
            {
                selectedVoice = voiceFreeListRoot;
            }
            else
            {
                if (priority < voicePrioSortedRoot)
                {
                    return SYNTH_INVALID_VOICE;
                }

                prio = voicePrioSortedRoot;

                while (prio != VOICE_PRIORITY_NONE && priority >= prio && selectedVoice == -1)
                {
                    priorityGroupIndex = prio;
                    for (i = VB_PRIO_HEAD(voiceLists, priorityGroupIndex); i != SYNTH_INVALID_VOICE_U8;
                         i = VB_PRIO_LINK_NEXT(voiceLists, i))
                    {
                        if ((synthVoice[i].block == 0) &&
                            (!restrictToStreamKind || fxFlag == synthVoice[i].fxFlag))
                        {
                            if ((VOICE_CFLAGS(i) & 2) == 0)
                            {
                                if (selectedVoice != -1)
                                {
                                    if (synthVoice[selectedVoice].age > synthVoice[i].age)
                                        selectedVoice = i;
                                }
                                else
                                {
                                    selectedVoice = i;
                                }
                            }
                        }
                    }
                    prio = VB_PRIO_SORT_NEXT(voiceLists, priorityGroupIndex);
                }

                if (selectedVoice == -1)
                {
                    return SYNTH_INVALID_VOICE;
                }
            }

            if (synthVoice[selectedVoice].prio > priority)
            {
                goto ret_invalid;
            }
        }

    have_voice:
        if (selectedVoice == -1)
        {
            goto ret_invalid;
        }

        slotBase = (SynthVoiceListNode*)((u8*)voiceLists + selectedVoice * 4);
        if (slotBase[offsetof(VidListTables, freeList) / sizeof(*slotBase)].user == 1)
        {
            freeSlot = (SynthVoiceListNode*)((u8*)slotBase + offsetof(VidListTables, freeList));
            i = freeSlot->prev;

            if (i != SYNTH_INVALID_VOICE_U8)
            {
                voiceLists->freeList[i].next = freeSlot->next;
            }
            else
            {
                voiceFreeListRoot = freeSlot->next;
            }

            i = freeSlot->next;
            if (i != SYNTH_INVALID_VOICE_U8)
            {
                voiceLists->freeList[i].prev = freeSlot->prev;
            }

            if (selectedVoice == voiceFreeListTail)
            {
                voiceFreeListTail = freeSlot->prev;
            }

            freeSlot->user = 0;
        }
        else if (synthVoice[selectedVoice].fxFlag)
        {
            voiceFxRunning--;
        }
        else
        {
            voiceMusicRunning--;
        }
        if (fxFlag != 0)
        {
            ++voiceFxRunning;
        }
        else
        {
            ++voiceMusicRunning;
        }
        return selectedVoice;
    }

ret_invalid:
    return SYNTH_INVALID_VOICE;
}

/*
 * Release a voice slot: clear voice flags, unlink from id table,
 * decrement counter, and mark id slot as free (-1).
 */
void voiceFree(McmdVoiceState* voice)
{
    macMakeInactive(voice, 2);
    voiceRemovePriority(voice);
    voice->addr = NULL;
    voice->prio = 0;
    {
        u32 voiceId = voice->id;
        u32 v = voiceId & 0xff;
        SynthVoiceListNode* slot = &voiceFreeListSlots[v];
        if (slot->user == 0)
        {
            slot->user = 1;
            if (voiceFreeListRoot != 0xff)
            {
                slot->next = 0xff;
                slot->prev = voiceFreeListTail;
                voiceFreeListSlots[voiceFreeListTail].next = v;
            }
            else
            {
                slot->next = 0xff;
                slot->prev = 0xff;
                voiceFreeListRoot = v;
            }
            voiceFreeListTail = v;
            if (voice->fxFlag != 0)
            {
                voiceFxRunning--;
            }
            else
            {
                voiceMusicRunning--;
            }
        }
    }
    *(int*)&voice->id = -1;
}

#define SYNTH_VOICE_STATE(voice) (&synthVoice[voice])

static inline void voiceInitFreeList(void)
{
    u32 i;

    for (i = 0; i < synthInfo.voiceCount; i++)
    {
        voiceFreeListSlots[i].prev = i - 1;
        voiceFreeListSlots[i].next = i + 1;
        voiceFreeListSlots[i].user = 1;
    }
    voiceFreeListSlots[0].prev = 0xff;
    voiceFreeListSlots[synthInfo.voiceCount - 1].next = 0xff;
    voiceFreeListRoot = 0;
    voiceFreeListTail = synthInfo.voiceCount - 1;
}

static inline void voiceInitPrioSort(void)
{
    u32 i;

    for (i = 0; i < synthInfo.voiceCount; i++)
    {
        voicePriorityLinks[i].user = 0;
    }
    for (i = 0; i < 0x100; i++)
    {
        voicePriorityGroupHeads[i] = 0xff;
    }
    voicePrioSortedRoot = 0xffff;
}

void synthInitAllocationAids(void)
{
    voiceInitFreeList();
    voiceInitPrioSort();
    voiceFxRunning = 0;
    voiceMusicRunning = 0;
}

/*
 * Voice cleanup: if voice handle is valid, break the active voice and
 * reset its id slot.
 */
void voiceUnblock(u32 voice)
{
    if (voice == SYNTH_INVALID_VOICE)
        return;
    if (hwIsActive(voice) != 0)
    {
        hwBreak(voice);
    }
    synthVoice[voice].id = voice;
    voiceFree(&synthVoice[voice]);
    synthVoice[voice].block = 0;
}

/*
 * Voice teardown: clears state flags then breaks the voice.
 */
void voiceKill(u32 voice)
{
    McmdVoiceState* voiceState = SYNTH_VOICE_STATE(voice);

    if (voiceState->addr != 0)
    {
        vidRemoveVoiceReferences(voiceState);
        *(u64*)&voiceState->cFlagsHi &= ~3;
        voiceState->age = 0;
        voiceFree(voiceState);
    }
    if (voiceState->block != 0)
    {
        streamKill(voice);
    }
    hwBreak(voice);
}

int voiceKillSound(u32 id)
{
    int result = -1;
    u32 next_voiceid;
    u32 i;

    if (sndActive != 0)
    {
        id = vidGetInternalId(id);

        for (; id != SYNTH_INVALID_VOICE; id = next_voiceid)
        {
            i = (u8)id;
            next_voiceid = SYNTH_VOICE_STATE(i)->child;
            if (id == SYNTH_VOICE_STATE(i)->id)
            {
                voiceKill(i);
                result = 0;
            }
        }
    }

    return result;
}

/*
 * Returns 1 if state's voice id is currently registered in the
 * appropriate slot table, else 0.
 */
u32 voiceIsLastStarted(McmdVoiceState* state)
{
    McmdVoiceState* voiceState = state;
    u32 voice = voiceState->id;
    u8 slot;
    u8 channel;
    u8 voiceIdx;
    if (voice != SYNTH_INVALID_VOICE)
    {
        slot = voiceState->midi;
        if (slot != SYNTH_INVALID_VOICE_U8)
        {
            channel = voiceState->midiSet;
            voiceIdx = voice;
            if (channel == SYNTH_INVALID_VOICE_U8)
            {
                if (synth_last_fxstarted[voiceIdx] == voiceIdx)
                    return 1;
            }
            else if (voiceIdx == synth_last_started[channel][slot])
            {
                return 1;
            }
        }
    }
    return 0;
}

/*
 * Register the state's voice id in either the 1D or 2D slot table.
 */
void voiceSetLastStarted(McmdVoiceState* state)
{
    McmdVoiceState* voiceState = state;
    u32 voice = voiceState->id;
    u8 slot;
    u8 channel;
    u8 voiceIdx;
    if (voice == SYNTH_INVALID_VOICE)
        return;
    slot = voiceState->midi;
    if (slot == SYNTH_INVALID_VOICE_U8)
        return;
    channel = voiceState->midiSet;
    voiceIdx = voice;
    if (channel == SYNTH_INVALID_VOICE_U8)
    {
        synth_last_fxstarted[voiceIdx] = voiceIdx;
    }
    else
    {
        synth_last_started[channel][slot] = voiceIdx;
    }
}

void voiceResetLastStarted(McmdVoiceState* voice)
{
    u32 voiceId;
    u32 midi;
    u32 midiSet;
    u32 vid8;
    u8* slot;

    voiceId = voice->id;
    if (voiceId == SYNTH_INVALID_VOICE)
        return;
    midi = voice->midi;
    if (midi == SYNTH_INVALID_VOICE_U8)
        return;
    midiSet = voice->midiSet;
    vid8 = voiceId & 0xff;
    if (midiSet == SYNTH_INVALID_VOICE_U8)
    {
        slot = &synth_last_fxstarted[vid8];
        if (*slot != vid8)
            return;
        *slot = SYNTH_INVALID_VOICE_U8;
    }
    else
    {
        slot = &synth_last_started[midiSet][midi];
        if (vid8 != *slot)
            return;
        *slot = SYNTH_INVALID_VOICE_U8;
    }
}

void voiceInitLastStarted(void)
{
    int channel;
    int key;

    for (channel = 0; channel < SYNTH_VOICE_MIDI_CHANNEL_COUNT; channel++)
    {
        for (key = 0; key < SYNTH_VOICE_MIDI_KEY_COUNT; key++)
        {
            synth_last_started[channel][key] = SYNTH_VOICE_REGISTRATION_FREE;
        }
    }
    for (key = 0; key < SYNTH_VOICE_DIRECT_SLOT_COUNT; key++)
    {
        synth_last_fxstarted[key] = SYNTH_VOICE_REGISTRATION_FREE;
    }
}
