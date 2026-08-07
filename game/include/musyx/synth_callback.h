#ifndef MUSYX_SYNTH_CALLBACK_H_
#define MUSYX_SYNTH_CALLBACK_H_

#include "types.h"

typedef struct SynthCallbackLink SynthCallbackLink;
typedef struct SynthVoice SynthVoice;
typedef void (*SynthMessageCallback)(u32 voiceId, s32 message);

extern SynthMessageCallback synthMessageCallback;

void ResetNotes(SynthVoice *voice);
SynthCallbackLink *AllocateNote(s32 triggerValue, u8 controllerIndex);
s32 HandleNotes(void);
void KeyOffNotes(void);
void seqFreeKeyOffNote(SynthCallbackLink *callback);
u32 GetPublicId(s32 voiceIndex);
u32 seqGetPrivateId(u32 seqId);

#endif /* MUSYX_SYNTH_CALLBACK_H_ */
