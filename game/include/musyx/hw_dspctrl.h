#ifndef MUSYX_HW_DSPCTRL_H_
#define MUSYX_HW_DSPCTRL_H_

#include "types.h"
#include "musyx/dsp_voice.h"

void salBuildCommandList(s16 *dest, u32 nsDelay);
void salHandleAuxProcessing(void);
void salActivateVoice(DSPvoice *voice, u8 studio);
void salDeactivateVoice(DSPvoice *voice);
u32 salAddStudioInput(DSPstudioinfo *studio, SND_STUDIO_INPUT *input);
u32 salRemoveStudioInput(DSPstudioinfo *studio, SND_STUDIO_INPUT *input);

extern DSPstudioinfo dspStudio[8];

#endif /* MUSYX_HW_DSPCTRL_H_ */
