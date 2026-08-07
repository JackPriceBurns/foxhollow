#ifndef MAIN_OBJPRINT_ANIM_API_H_
#define MAIN_OBJPRINT_ANIM_API_H_

#include "global.h"
#include "game/objects/object.h"
#include "main/objprint_sound_api.h"

void objSoundUpdateMouth(GameObject* obj, ObjSoundState* state);
void characterHeadLookCalm(GameObject* obj, s16* state, f32 value);

#endif /* MAIN_OBJPRINT_ANIM_API_H_ */
