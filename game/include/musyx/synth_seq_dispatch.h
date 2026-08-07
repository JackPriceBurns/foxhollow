#ifndef MUSYX_SYNTH_SEQ_DISPATCH_H_
#define MUSYX_SYNTH_SEQ_DISPATCH_H_

#include "types.h"

typedef struct SynthSequenceEvent SynthSequenceEvent;

SynthSequenceEvent* HandleEvent(SynthSequenceEvent* event, u8 groupIndex, u32* loopFlag);
void InitTrackEvents(void);
u32 HandleTrackEvents(u8 groupIndex, u32 deltaTime);

#endif /* MUSYX_SYNTH_SEQ_DISPATCH_H_ */
