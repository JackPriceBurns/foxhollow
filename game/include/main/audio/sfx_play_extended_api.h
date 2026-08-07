#ifndef MAIN_AUDIO_SFX_PLAY_EXTENDED_API_H_
#define MAIN_AUDIO_SFX_PLAY_EXTENDED_API_H_

#include "types.h"
#include "dolphin/mtx/vec_types.h"
#include "game/objects/object_fwd.h"

void Sfx_PlayFromObjectEx(GameObject* obj, Vec* pos, u32 channel, u16 sfxId);

#endif /* MAIN_AUDIO_SFX_PLAY_EXTENDED_API_H_ */
