#ifndef MAIN_AUDIO_SFX_H_
#define MAIN_AUDIO_SFX_H_

#include "global.h"
#include "dolphin/mtx/vec_types.h"
#include "game/objects/object.h"

int Sfx_IsPlayingFromObjectChannel(GameObject* obj, int channel);
void Sfx_SetObjectChannelVolume(GameObject* obj, u32 channel, u8 volume, f32 volumeScale);
void Sfx_KeepAliveLoopedObjectSound(GameObject* obj, u16 sfxId);
u32 Sfx_PlayFromObjectLimited(GameObject* obj, u16 sfxId, int limit);
void Sfx_KeepAliveLoopedObjectSoundLimited(GameObject* obj, u16 sfxId, u16 limit);
void Sfx_AddLoopedObjectSound(GameObject* obj, u16 sfxId);
void Sfx_RemoveLoopedObjectSound(GameObject* obj, u16 sfxId);
void Sfx_RemoveLoopedObjectSoundForObject(GameObject* obj);
s32 Sfx_IsPlayingFromObject(GameObject* obj, u16 sfxId);
void Sfx_ClearLoopedObjectSounds(void);
void Sfx_UpdateLoopedObjectSounds(void);
void Sfx_SetObjectSoundsPaused(s32 paused);
void Sfx_InitObjectChannels(void);
void Sfx_SetObjectSfxVolume(GameObject* obj, u16 sfxId, u8 volume, f32 volumeScale);
void Sfx_PlayFromObject(GameObject* obj, u16 sfxId);
void Sfx_PlayFromObjectChannel(GameObject* obj, u32 channel, u16 sfxId);
void Sfx_PlayFromObjectEx(GameObject* obj, Vec* pos, u32 channel, u16 sfxId);
void Sfx_PlayAtPositionFromObject(GameObject* obj, f32 x, f32 y, f32 z, u16 sfxId);
int Sfx_ResolveObjectSfxId(GameObject** object, u16* sfxId);
void Sfx_StopObjectChannel(GameObject* obj, int channel);
void Sfx_StopFromObject(GameObject* obj, u16 sfxId);

#endif /* MAIN_AUDIO_SFX_H_ */
