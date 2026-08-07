#ifndef MUSYX_VOICE_ID_H_
#define MUSYX_VOICE_ID_H_

#include "types.h"
#include "musyx/mcmd.h"

void vidRemoveVoiceReferences(McmdVoiceState* state);
u32 vidMakeRoot(McmdVoiceState* voice);
u32 vidMakeNew(McmdVoiceState* state, int returnNewId);
int vidGetInternalId(u32 id);
void voiceRemovePriority(McmdVoiceState *voice);

#endif /* MUSYX_VOICE_ID_H_ */
