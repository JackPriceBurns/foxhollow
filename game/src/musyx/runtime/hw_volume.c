#include "musyx/hw_volume.h"
#include "musyx/dsp_voice_state.h"
#include "musyx/hw_dspctrl.h"
#include "musyx/sal_studio.h"
#include "musyx/sal_volume.h"


extern const u16 gItdPanDelayTable[128];

/*
 * hwSetVolume - large mix-volume setter; computes 4-channel pan from
 * 3-axis float input via salCalcVolume, clamps each to s16, and writes
 * back to the voice's pan/volume table.
 */
void hwSetVolume(u32 voiceIndex, u8 volumeTable, f32 volume, u32 pan, u32 surroundPan,
                 f32 auxA, f32 auxB)
{
    f32 out[9];
    u16 il;
    u16 ir;
    u16 is;
    DSPvoice* voice = &dspVoice[voiceIndex];

    if (volume >= 1.0f)
        volume = 1.0f;
    if (auxA >= 1.0f)
        auxA = 1.0f;
    if (auxB >= 1.0f)
        auxB = 1.0f;

    {
        u32 f0w = voice->flags;
        f0w &= DSP_VOICE_ITD_ENABLED_FLAG;
        salCalcVolume(volumeTable, out, pan, surroundPan, f0w != 0,
                            dspStudio[voice->studio].type == SND_STUDIO_TYPE_DPL2, volume, auxA, auxB);
    }

    il = 32767.0f * out[0];
    ir = 32767.0f * out[1];
    is = 32767.0f * out[2];
    if (voice->lastUpdate.vol == 0xff || voice->volL != il || voice->volR != ir || voice->volS != is)
    {
        voice->volL = il;
        voice->volR = ir;
        voice->volS = is;
        voice->changed[0] |= DSP_VOICE_CHANGE_VOLUME;
        voice->lastUpdate.vol = 0;
    }

    il = 32767.0f * out[3];
    ir = 32767.0f * out[4];
    is = 32767.0f * out[5];
    if (voice->lastUpdate.volA == 0xff || voice->volLa != il || voice->volRa != ir || voice->volSa != is)
    {
        voice->volLa = il;
        voice->volRa = ir;
        voice->volSa = is;
        voice->changed[0] |= DSP_VOICE_CHANGE_AUX_A;
        voice->lastUpdate.volA = 0;
    }

    il = 32767.0f * out[6];
    ir = 32767.0f * out[7];
    is = 32767.0f * out[8];
    if (voice->lastUpdate.volB == 0xff || voice->volLb != il || voice->volRb != ir || voice->volSb != is)
    {
        voice->volLb = il;
        voice->volRb = ir;
        voice->volSb = is;
        voice->changed[0] |= DSP_VOICE_CHANGE_AUX_B;
        voice->lastUpdate.volB = 0;
    }

    if (voice->flags & DSP_VOICE_ITD_ENABLED_FLAG)
    {
        const u16* delay = &gItdPanDelayTable[(pan >> 16) & 0xff];
        voice->itdShiftL = *delay;
        voice->itdShiftR = 0x20 - *delay;
        voice->changed[0] |= DSP_VOICE_CHANGE_ITD;
    }
}

/*
 * Disable a voice slot.
 */
void hwOff(s32 slot)
{
    salDeactivateVoice(&dspVoice[slot]);
}

/*
 * Set the four AUX-mix DSP processing callbacks for a studio.
 */
void hwSetAUXProcessingCallbacks(u8 studio, SynthAuxCallback cb0, void* cb1, SynthAuxCallback cb2, void* cb3)
{
    DSPstudioinfo* entry = &dspStudio[studio];
    entry->auxAHandler = cb0;
    entry->auxAUser = cb1;
    entry->auxBHandler = cb2;
    entry->auxBUser = cb3;
}

/*
 * Activate the audio "studio" effect chain - thin wrapper.
 */
void hwActivateStudio(u8 studio, bool isMaster, SND_STUDIO_TYPE type)
{
    salActivateStudio(studio, isMaster, type);
}

/*
 * Deactivate the audio "studio" effect chain - thin wrapper.
 */
void hwDeactivateStudio(u8 studio)
{
    salDeactivateStudio(studio);
}
