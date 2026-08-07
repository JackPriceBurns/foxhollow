#ifndef MUSYX_MCMD_VOLUME_H_
#define MUSYX_MCMD_VOLUME_H_

#include "types.h"
#include "musyx/mcmd.h"

u32 TranslateVolume(u32 value, u16 keyId);
void mcmdScaleVolume(McmdVoiceState *state, McmdCommandArgs *params, s32 envCurrent);

#endif /* MUSYX_MCMD_VOLUME_H_ */
