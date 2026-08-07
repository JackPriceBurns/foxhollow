#ifndef MAIN_AUDIO_SFX_CHANNEL_VOLUME_API_H_
#define MAIN_AUDIO_SFX_CHANNEL_VOLUME_API_H_

#include "game/objects/object.h"

void Sfx_SetObjectChannelVolume(GameObject* obj, u32 channel, u8 volume, f32 volumeScale);

#endif /* MAIN_AUDIO_SFX_CHANNEL_VOLUME_API_H_ */
