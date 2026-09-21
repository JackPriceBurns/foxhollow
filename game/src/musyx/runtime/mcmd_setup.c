#include "types.h"
#include "musyx/mcmd.h"
#include "musyx/mcmd_exec.h"
#include "musyx/snd_service.h"
#include "musyx/voice_conv.h"
#include "musyx/data_tables.h"
#include "musyx/hw_init.h"
#include "musyx/mcmd_setup.h"
#include "musyx/adsr_setup.h"
#include "musyx/synth_delay.h"
#include "musyx/synth_voice.h"
#include "musyx/adsr.h"
#include "musyx/hw_adsr.h"

extern u8 lbl_8032EDD0[]; /* pitch ratio table (u16[13]) heads the macro data tables */

typedef union McmdAdsrData
{
    struct
    {
        u16 atime;
        u16 dtime;
        u16 slevel;
        u16 rtime;
    } linear;

    struct
    {
        s32 atime;
        s32 dtime;
        u16 slevel;
        u16 rtime;
    } dls;
} McmdAdsrData;

/* 64-bit control-flag word overlaying cFlagsHi(hi)/cFlagsLo(lo). */
#define MAC_CFLAGS(sv)     (*(u64*)&(sv)->cFlagsHi)
#define MAC_FLAG64(hi, lo) (((u64)(hi) << 32) | (u64)(lo))

/*
 * Spawn a child macro voice, key-shifted relative to this voice, and link
 * it into the voice's child chain.
 */
void mcmdPlayMacro(McmdVoiceState* svoice, McmdCommandArgs* cstep)
{
    s32 key;
    u32 new_child;

    key = svoice->orgNote + (s8)(u8)(cstep->flags >> 8);
    key = (key < 0) ? 0 : key > 0x7f ? 0x7f : key;

    if (svoice->fxFlag != 0)
    {
        key |= 0x80;
    }

    svoice->block = 1;
    new_child = macStart(cstep->flags >> 0x10, (u8)(cstep->value >> 0x10), (u8)(cstep->value >> 0x18),
                         svoice->allocId, key, (u8)(svoice->volume >> 0x10), (u8)(svoice->panning[0] >> 0x10),
                         svoice->midi, svoice->midiSet, svoice->section, cstep->value, svoice->track, 0,
                         svoice->vGroup, svoice->studio, svoice->itdMode == 0);
    svoice->block = 0;
    if (new_child != 0xFFFFFFFF)
    {
        svoice->lastVID = synthVoice[(u8)new_child].vidList->vid;
        synthVoice[(u8)new_child].parent = svoice->id;
        if (svoice->child != -1)
        {
            synthVoice[(u8)new_child].child = svoice->child;
            synthVoice[(u8)svoice->child].parent = new_child;
        }
        svoice->child = new_child;
        if (svoice->fxFlag != 0)
        {
            synthFXCloneMidiSetup(&synthVoice[(u8)new_child], svoice);
        }
    }
    else
    {
        svoice->lastVID = 0xFFFFFFFF;
    }
}

/*
 * Resolve a sample descriptor and start hardware playback for a voice.
 */
void mcmdStartSample(McmdVoiceState* svoice, McmdCommandArgs* cstep)
{
    static SAMPLE_INFO newsmp;
    u16 smp;

    smp = cstep->flags >> 8;

    if (dataGetSample(smp, &newsmp) != 0)
    {
        return;
    }
    switch ((u8)(cstep->flags >> 0x18))
    {
    case 0:
        newsmp.offset = cstep->value;
        break;
    case 1:
        newsmp.offset = ((u8)(0x7f - (svoice->volume >> 0x10)) * cstep->value) / 0x7f;
        break;
    case 2:
        newsmp.offset = ((u8)(svoice->volume >> 0x10) * cstep->value) / 0x7f;
        break;
    default:
        newsmp.offset = 0;
        break;
    }

    {
        u32 length = newsmp.length;
        u32* offset = &newsmp.offset;
        if (*offset >= length)
        {
            *offset = length - 1;
        }
    }

    hwInitSamplePlayback(svoice->id & 0xFF, smp, &newsmp,
                         (MAC_CFLAGS(svoice) & MAC_FLAG64(0, 0x100)) == 0,
                         ((u32)svoice->prio << 24) | (svoice->age >> 15), svoice->id,
                         (MAC_CFLAGS(svoice) & MAC_FLAG64(0x800, 0)) == 0, svoice->itdMode);

    svoice->sInfo = newsmp.info;

    if (svoice->playFrq != -1)
    {
        DoSetPitch(svoice);
    }
    MAC_CFLAGS(svoice) |= MAC_FLAG64(0, 0x20);
    synthKeyStateUpdate(svoice);
}

/*
 * Configure the voice vibrato ramp and curve flags.
 */
void mcmdVibrato(McmdVoiceState* svoice, McmdCommandArgs* cstep)
{
    u32 time;
    s8 kr;
    s8 cr;

    if ((u8)(cstep->flags >> 0x18) & 3)
    {
        MAC_CFLAGS(svoice) |= MAC_FLAG64(0, 0x4000);
    }
    else
    {
        MAC_CFLAGS(svoice) &= ~MAC_FLAG64(0, 0x4000);
    }

    time = (u16)(cstep->value >> 0x10);
    if ((u8)(cstep->value >> 8) & 1)
    {
        sndConvertMs(&time);
    }
    else
    {
        sndConvertTicks(&time, svoice);
    }

    if (time)
    {
        MAC_CFLAGS(svoice) |= MAC_FLAG64(0, 0x2000);
        svoice->vibPeriod = time;

        kr = (s8)(cstep->flags >> 8);
        cr = (s8)(cstep->flags >> 16);

        if (kr < 0)
        {
            if (cr < 0)
            {
                svoice->vibCentRange = -cr;
            }
            else
            {
                svoice->vibCentRange = cr;
            }

            svoice->vibKeyRange = -kr;
            svoice->vibCurTime = svoice->vibPeriod / 2;
        }
        else
        {
            if (cr < 0)
            {
                if (kr == 0)
                {
                    svoice->vibCentRange = -cr;
                    svoice->vibCurTime = svoice->vibPeriod / 2;
                }
                else
                {
                    --kr;
                    svoice->vibCentRange = 100 - cr;
                    svoice->vibCurTime = 0;
                }
            }
            else
            {
                svoice->vibCentRange = cr;
                svoice->vibCurTime = 0;
            }
            svoice->vibKeyRange = kr;
        }
    }
    else
    {
        MAC_CFLAGS(svoice) &= ~MAC_FLAG64(0, 0x2000);
    }
}

/*
 * Map the previous sample pitch toward the requested pitch, splitting the
 * result into key and fine-tune cents.
 */
void DoSetPitch(McmdVoiceState* svoice)
{
    u32 ratio;
    u32 ratioInt;
    u32 i;
    u32 frq;
    u32 ofrq;
    u32 octave;
    s32 key;
    u16* kf = (u16*)lbl_8032EDD0;

    frq = svoice->playFrq & 0xFFFFFF;
    ofrq = svoice->sInfo & 0xFFFFFF;

    if (ofrq == frq)
    {
        svoice->curNote = (u8)(svoice->sInfo >> 24);
        svoice->curDetune = 0;
    }
    else if (ofrq < frq)
    {
        ratio = (frq << 12) / ofrq;
        ratioInt = ratio >> 12;

        for (octave = 0; octave < 11; octave++)
        {
            if (ratioInt < (1 << (octave + 1)))
            {
                break;
            }
        }

        ratio /= (1 << octave);

        for (i = 11;; i--)
        {
            if (ratio > kf[i])
            {
                break;
            }
        }

        svoice->curNote = (svoice->sInfo >> 24) + (octave * 12) + i;
        svoice->curDetune = ((ratio - kf[i]) * 100) / (kf[i + 1] - kf[i]);
    }
    else
    {
        ratio = (ofrq << 12) / frq;
        ratioInt = ratio >> 12;

        for (octave = 0; octave < 11; octave++)
        {
            if (ratioInt < (1 << (octave + 1)))
            {
                break;
            }
        }

        ratio /= (1 << octave);

        for (i = 11;; i--)
        {
            if (ratio > kf[i])
            {
                break;
            }
        }

        key = octave * 12 + i;
        if (key > (s32)(svoice->sInfo >> 24))
        {
            svoice->curNote = svoice->curDetune = 0;
        }
        else
        {
            svoice->curNote = (svoice->sInfo >> 24) - key;
            svoice->curDetune = ((kf[i] - ratio) * 100) / (kf[i + 1] - kf[i]);
        }
    }
}

/*
 * Resolve ADSR parameters and send them to the hardware voice.
 */
void mcmdSetADSR(McmdVoiceState* svoice, McmdCommandArgs* cstep)
{
    f32 attackProd;
    f32 decayProd;
    s32 ascale;
    s32 dscale;
    McmdAdsrData adsr;
    ADSR_INFO* adsr_ptr;

    if ((adsr_ptr = (ADSR_INFO*)dataGetCurve(cstep->flags >> 8)) != NULL)
    {
        if (!(u8)(cstep->flags >> 24))
        {
            adsr.linear.atime = adsr_ptr->data.linear.atime >> 8 | adsr_ptr->data.linear.atime << 8;
            adsr.linear.dtime = adsr_ptr->data.linear.dtime >> 8 | adsr_ptr->data.linear.dtime << 8;
            adsr.linear.slevel = adsr_ptr->data.linear.slevel >> 8 | adsr_ptr->data.linear.slevel << 8;
            adsr.linear.rtime = adsr_ptr->data.linear.rtime >> 8 | adsr_ptr->data.linear.rtime << 8;
            hwSetADSR(svoice->id & 0xFF, &adsr, 0);
        }
        else
        {
            f32 sScale = voiceAdsrSustainTable[(u16)(adsr_ptr->data.dls.slevel >> 8 | adsr_ptr->data.dls.slevel << 8) >> 5];
            adsr.dls.atime = ((u8*)&adsr_ptr->data.dls.atime)[0] << 0 | ((u8*)&adsr_ptr->data.dls.atime)[1] << 8 |
                             ((u8*)&adsr_ptr->data.dls.atime)[2] << 16 | ((u8*)&adsr_ptr->data.dls.atime)[3] << 24;
            adsr.dls.dtime = ((u8*)&adsr_ptr->data.dls.dtime)[0] << 0 | ((u8*)&adsr_ptr->data.dls.dtime)[1] << 8 |
                             ((u8*)&adsr_ptr->data.dls.dtime)[2] << 16 | ((u8*)&adsr_ptr->data.dls.dtime)[3] << 24;
            adsr.dls.slevel = 4096.0f * sScale;
            adsr.dls.rtime = adsr_ptr->data.dls.rtime >> 8 | adsr_ptr->data.dls.rtime << 8;
            ascale = ((u8*)&adsr_ptr->data.dls.ascale)[0] << 0 | ((u8*)&adsr_ptr->data.dls.ascale)[1] << 8 |
                     ((u8*)&adsr_ptr->data.dls.ascale)[2] << 16 | ((u8*)&adsr_ptr->data.dls.ascale)[3] << 24;
            dscale = ((u8*)&adsr_ptr->data.dls.dscale)[0] << 0 | ((u8*)&adsr_ptr->data.dls.dscale)[1] << 8 |
                     ((u8*)&adsr_ptr->data.dls.dscale)[2] << 16 | ((u8*)&adsr_ptr->data.dls.dscale)[3] << 24;

            if (ascale != 0x80000000)
            {
                attackProd = 1.1920928955078125e-7f * svoice->orgVolume;
                adsr.dls.atime += (s32)(attackProd * ascale);
            }

            if (dscale != 0x80000000)
            {
                decayProd = 0.0078125f * svoice->orgNote;
                adsr.dls.dtime += (s32)(decayProd * dscale);
            }

            hwSetADSR(svoice->id & 0xFF, &adsr, 1);
        }

        MAC_CFLAGS(svoice) |= MAC_FLAG64(0, 0x100);
    }
}

/*
 * Configure the per-voice pitch envelope state from a DLS ADSR table.
 */
void mcmdSetPitchADSR(McmdVoiceState* svoice, McmdCommandArgs* cstep)
{
    McmdAdsrData adsr;
    ADSR_INFO* adsr_ptr;
    u32 sl;
    s32 ascale;
    s32 dscale;

    if ((adsr_ptr = (ADSR_INFO*)dataGetCurve(cstep->flags >> 8)) == NULL)
    {
        return;
    }

    svoice->pitchADSRRange = (s8)cstep->value << 8;

    if (svoice->pitchADSRRange >= 0)
    {
        svoice->pitchADSRRange += ((s16)(s8)(cstep->value >> 8) << 8) / 100;
    }
    else
    {
        svoice->pitchADSRRange -= ((s16)(s8)(cstep->value >> 8) << 8) / 100;
    }

    adsr.dls.atime = ((u8*)&adsr_ptr->data.dls.atime)[0] << 0 | ((u8*)&adsr_ptr->data.dls.atime)[1] << 8 |
                     ((u8*)&adsr_ptr->data.dls.atime)[2] << 16 | ((u8*)&adsr_ptr->data.dls.atime)[3] << 24;
    adsr.dls.dtime = ((u8*)&adsr_ptr->data.dls.dtime)[0] << 0 | ((u8*)&adsr_ptr->data.dls.dtime)[1] << 8 |
                     ((u8*)&adsr_ptr->data.dls.dtime)[2] << 16 | ((u8*)&adsr_ptr->data.dls.dtime)[3] << 24;
    adsr.dls.slevel = adsr_ptr->data.dls.slevel >> 8 | adsr_ptr->data.dls.slevel << 8;
    adsr.dls.rtime = adsr_ptr->data.dls.rtime >> 8 | adsr_ptr->data.dls.rtime << 8;
    ascale = ((u8*)&adsr_ptr->data.dls.ascale)[0] << 0 | ((u8*)&adsr_ptr->data.dls.ascale)[1] << 8 |
             ((u8*)&adsr_ptr->data.dls.ascale)[2] << 16 | ((u8*)&adsr_ptr->data.dls.ascale)[3] << 24;
    dscale = ((u8*)&adsr_ptr->data.dls.dscale)[0] << 0 | ((u8*)&adsr_ptr->data.dls.dscale)[1] << 8 |
             ((u8*)&adsr_ptr->data.dls.dscale)[2] << 16 | ((u8*)&adsr_ptr->data.dls.dscale)[3] << 24;

    if (ascale != 0x80000000)
    {
        f32 prod = 1.1920928955078125e-7f * svoice->orgVolume;
        adsr.dls.atime += (s32)(prod * ascale);
    }
    if (dscale != 0x80000000)
    {
        f32 prod = 0.0078125f * svoice->orgNote;
        adsr.dls.dtime += (s32)(prod * dscale);
    }

    svoice->pitchADSR.mode = 1;
    svoice->pitchADSR.aMode = 0;
    svoice->pitchADSR.aTime = voiceConvertTimeCentsToMs(adsr.dls.atime);
    svoice->pitchADSR.dTime = voiceConvertTimeCentsToMs(adsr.dls.dtime);
    if ((sl = adsr.dls.slevel >> 2) > 0x3ff)
    {
        sl = 0x3ff;
    }
    svoice->pitchADSR.sLevel = 0xc1 - voiceAdsrDecayTable[sl];
    svoice->pitchADSR.rTime = adsr.dls.rtime;
    adsrSetup(&svoice->pitchADSR);
    MAC_CFLAGS(svoice) |= MAC_FLAG64(0x200, 0);
}

/*
 * Configure a panning/surround-panning parameter ramp for the voice.
 */
void voiceConfigureParamRamp(McmdVoiceState* svoice, McmdCommandArgs* cstep, u8 pi)
{
    s32 mstime;
    s32 width;

    width = cstep->flags >> 16;
    svoice->panTime[pi] = width;
    sndConvertMs(&svoice->panTime[pi]);
    mstime = (s8)cstep->value;
    svoice->panning[pi] = ((u8)(cstep->flags >> 8)) << 16;
    svoice->panTarget[pi] = svoice->panning[pi] + (mstime << 16);
    if (svoice->panTime[pi] != 0)
    {
        svoice->panDelta[pi] = (s32)(mstime << 16) / width;
    }
    else
    {
        svoice->panDelta[pi] = (s32)(mstime << 16);
    }

    MAC_CFLAGS(svoice) |= MAC_FLAG64(0x2000, 0);
}
