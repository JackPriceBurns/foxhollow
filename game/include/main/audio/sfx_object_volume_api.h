#ifndef MAIN_AUDIO_SFX_OBJECT_VOLUME_API_H_
#define MAIN_AUDIO_SFX_OBJECT_VOLUME_API_H_

#include "types.h"
#include "game/objects/object.h"

void Sfx_SetObjectSfxVolume(GameObject* obj, u16 sfxId, u8 volume, f32 volumeScale);

#endif /* MAIN_AUDIO_SFX_OBJECT_VOLUME_API_H_ */
