#ifndef MAIN_AUDIO_SFX_POSITION_API_H_
#define MAIN_AUDIO_SFX_POSITION_API_H_

#include "types.h"
#include "game/objects/object_fwd.h"

void Sfx_PlayAtPositionFromObject(GameObject* obj, f32 x, f32 y, f32 z, u16 sfxId);

#endif /* MAIN_AUDIO_SFX_POSITION_API_H_ */
