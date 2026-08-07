#include "musyx/snd3d_room.h"
#include "musyx/snd_synth_api.h"
#include "musyx/synth_delay.h"
#include "musyx/snd3d.h"

/* SND_ROOM.flags: room-fade one-shots driven per update tick */
#define S3D_ENTRY_FADE_IN  0x80000000 /* ramp fade up toward full, then clear */
#define S3D_ENTRY_FADE_OUT 0x40000000 /* ramp fade down toward zero, then clear */


/*
 * Update average squared distance from each active spatial entry to all
 * registered listeners.
 */
static void UpdateRoomDistances(void)
{
    SND_LISTENER* listener;
    SND_ROOM* entry;
    u32 listenerCount;

    listenerCount = 0;
    for (listener = s3dListenerRoot; listener != NULL; listener = listener->next)
    {
        listenerCount++;
    }

    if (listenerCount != 0)
    {
        for (entry = s3dRoomRoot; entry != NULL; entry = entry->next)
        {
            f32 distanceSq;
            struct
            {
                f32 x, y, z;
            } d;

            if (entry->studio != 0xff)
            {
                distanceSq = 0.0f;
                for (listener = s3dListenerRoot; listener != NULL; listener = listener->next)
                {
                    d.x = entry->pos.x - listener->pos.x;
                    d.y = entry->pos.y - listener->pos.y;
                    d.z = entry->pos.z - listener->pos.z;

                    distanceSq += d.z * d.z + (d.x * d.x + d.y * d.y);
                }
                entry->distance = distanceSq / listenerCount;
            }
        }
    }
}

/*
 * Allocate scarce studio voices to spatial entries and update their
 * activation fade state.
 */
void CheckRoomStatus(void)
{
    SND_LISTENER* listener;
    SND_EMITTER* voice;
    SND_ROOM* scanEntry;
    SND_ROOM* evictedEntry;
    SND_ROOM* entry;
    struct
    {
        f32 x, y, z;
    } d;
    f32 distanceSq;
    f32 worstDistance;
    u32 listenerCount;
    u32 i;
    u32 mask;
    u8 listenerOwned;

    UpdateRoomDistances();

    listenerCount = 0;
    for (listener = s3dListenerRoot; listener != NULL; listener = listener->next)
    {
        listenerCount++;
    }

    if (listenerCount != 0)
    {
        for (entry = s3dRoomRoot; entry != NULL; entry = entry->next)
        {
            if (entry->studio == 0xff)
            {
                distanceSq = 0.0f;
                for (listener = s3dListenerRoot; listener != NULL; listener = listener->next)
                {
                    d.x = entry->pos.x - listener->pos.x;
                    d.y = entry->pos.y - listener->pos.y;
                    d.z = entry->pos.z - listener->pos.z;

                    distanceSq += d.z * d.z + (d.x * d.x + d.y * d.y);
                }
                listenerOwned = false;
                distanceSq = distanceSq / listenerCount;
                for (listener = s3dListenerRoot; listener != NULL; listener = listener->next)
                {
                    if (listener->room == entry)
                    {
                        listenerOwned = true;
                        break;
                    }
                }

                mask = ~(-1 << snd_max_studios);
                if (mask != (snd_used_studios & mask))
                {
                    for (i = 0; i < snd_max_studios; i++)
                    {
                        if ((snd_used_studios & (1 << i)) == 0)
                        {
                            break;
                        }
                    }
                    snd_used_studios |= 1 << i;
                    entry->studio = i + snd_base_studio;
                }
                else
                {
                    worstDistance = -1.0f;

                    for (scanEntry = s3dRoomRoot; scanEntry != NULL; scanEntry = scanEntry->next)
                    {
                        if (scanEntry->studio != 0xff && worstDistance < scanEntry->distance)
                        {
                            worstDistance = scanEntry->distance;
                            evictedEntry = scanEntry;
                        }
                    }
                    if (!listenerOwned && !(worstDistance > distanceSq))
                    {
                        continue;
                    }
                    for (voice = s3dEmitterRoot; voice != NULL; voice = voice->next)
                    {
                        if (voice->room == evictedEntry)
                        {
                            synthSendKeyOff(voice->vid);
                            voice->flags |= S3D_EMITTER_FLAG_WAITING_FOR_ROOM;
                            voice->vid = 0xffffffff;
                        }
                    }
                    if (evictedEntry->deActivateReverb != NULL)
                    {
                        evictedEntry->deActivateReverb(evictedEntry->studio);
                    }
                    synthDeactivateStudio(evictedEntry->studio);
                    entry->studio = evictedEntry->studio;
                    evictedEntry->studio = 0xff;
                    evictedEntry->flags = 0;
                }

                entry->distance = distanceSq;
                entry->curMVol = listenerOwned ? 0x7f0000 : 0;
                if ((f32)(1.2014794e-07f * entry->curMVol) >= 0.5)
                {
                    synthActivateStudio(entry->studio, 1, 0);
                }
                else
                {
                    synthActivateStudio(entry->studio, 0, 0);
                }
                if (entry->activateReverb != NULL)
                {
                    entry->activateReverb(entry->studio, entry->user);
                }
            }
            else
            {
                if ((entry->flags & S3D_ENTRY_FADE_IN) != 0)
                {
                    entry->curMVol += 0x40000;
                    if (entry->curMVol >= 0x7f0000)
                    {
                        entry->curMVol = 0x7f0000;
                        entry->flags &= ~S3D_ENTRY_FADE_IN;
                    }
                    if ((f32)(1.2014794e-07f * entry->curMVol) >= 0.5)
                    {
                        synthActivateStudio(entry->studio, 1, 0);
                    }
                    else
                    {
                        synthActivateStudio(entry->studio, 0, 0);
                    }
                }
                if ((entry->flags & S3D_ENTRY_FADE_OUT) != 0)
                {
                    entry->curMVol -= 0x40000;
                    if ((s32)entry->curMVol >= 0)
                    {
                        entry->curMVol = 0;
                        entry->flags &= ~S3D_ENTRY_FADE_OUT;
                    }
                    if ((f32)(1.2014794e-07f * entry->curMVol) >= 0.5)
                    {
                        synthActivateStudio(entry->studio, 1, 0);
                    }
                    else
                    {
                        synthActivateStudio(entry->studio, 0, 0);
                    }
                }
            }
        }
    }
}

/*
 * Update studio-input bridges between spatial entries as voices appear
 * and disappear.
 */
void CheckDoorStatus(void)
{
    SND_DOOR* link;
    f32 f;
    f32 v;

    for (link = s3dDoorRoot; link != NULL; link = link->next)
    {
        if ((link->flags & 0x80000000) == 0)
        {
            if (link->a->studio != 0xff)
            {
                if (link->b->studio != 0xff)
                {
                    v = link->open;
                    f = (1.0f - v) * v;
                    link->input.volA = (s32)((f32)link->fxVol * v);
                    link->input.volB = 0;
                    link->input.vol = (s32)(127.0f * v);
                    if ((link->flags & 1) != 0)
                    {
                        link->input.srcStudio = link->b->studio;
                        synthAddStudioInput(link->a->studio, &link->input);
                    }
                    else
                    {
                        link->input.srcStudio = link->a->studio;
                        synthAddStudioInput(link->b->studio, &link->input);
                    }
                    link->flags |= 0x80000000;
                }
            }
        }
        else
        {
            u8 sourceVoice = link->a->studio;

            if (sourceVoice == 0xff || link->b->studio == 0xff)
            {
                if ((sourceVoice != 0xff && sourceVoice == link->destStudio) ||
                    (link->b->studio != 0xff && link->b->studio == link->destStudio))
                {
                    synthRemoveStudioInput(link->destStudio, &link->input);
                }
                link->flags &= 0x7fffffff;
            }
            else
            {
                v = link->open;
                f = (1.0f - v) * v;
                link->input.volA = (s32)((f32)link->fxVol * v);
                link->input.volB = 0;
                link->input.vol = (s32)(127.0f * v);
            }
        }
    }
}
