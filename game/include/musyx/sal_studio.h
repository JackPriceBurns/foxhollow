#ifndef MUSYX_SAL_STUDIO_H_
#define MUSYX_SAL_STUDIO_H_

#include "types.h"
#include "musyx/dsp_voice.h"

u32 salInitDspCtrl(u8 numVoices, u8 numStudios, u32 defaultStudioDPL2);
void salInitHRTFBuffer(void);
u32 salExitDspCtrl(void);
void salActivateStudio(u8 studio, u32 isMaster, SND_STUDIO_TYPE type);
void salDeactivateStudio(u8 studio);
u32 salCheckVolErrorAndResetDelta(u16 *dspVolume, u16 *dspDelta, u16 *lastVolume,
                                  u16 targetVolume, u16 *resetFlags, u16 resetMask);
void HandleDepopVoice(DSPstudioinfo *studio, DSPvoice *voice);
void SortVoices(DSPvoice **voices, int left, int right);

extern s32* dspSurround;
extern u32 dspHRTFOn;
extern u16* dspCmdPtr;
extern u16* dspCmdMaxPtr;
extern u16* dspCmdCurBase;
extern u16 dspCmdLastSize;
extern u16* dspCmdLastBase;
extern u16* dspCmdLastLoad;
extern u32 dspARAMZeroBuffer;

#endif /* MUSYX_SAL_STUDIO_H_ */
