#ifndef MAIN_NEWSHADOWS_AUDIO_API_H_
#define MAIN_NEWSHADOWS_AUDIO_API_H_

#include "global.h"
#include "game/objects/object.h"
#include "main/objanim.h"

extern u8 gSurfaceSfxTable[];

int surfaceSfxSelectTrigger(u8 surfaceType, u8 soundId);
void objAudioDispatchEventMask(GameObject* obj, int eventMask, u8 type, void* points, void* state, f32 unused,
                               f32 scale);
void objAudioDispatchAnimEvents(GameObject* obj, ObjAnimEventList* events, u8 type, void* points, void* state,
                                f32 unused, f32 scale);

#endif /* MAIN_NEWSHADOWS_AUDIO_API_H_ */
