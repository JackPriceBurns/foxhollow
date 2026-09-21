#ifndef MUSYX_SYNTH_SEQ_EVENTS_H_
#define MUSYX_SYNTH_SEQ_EVENTS_H_

#include "src/musyx/runtime/synth_internal.h"

SynthSequenceEvent* GenerateNextTrackEvent(u8 channel);
void InsertGlobalEvent(SynthSequenceQueue* queue, SynthSequenceEvent* event);

#endif /* MUSYX_SYNTH_SEQ_EVENTS_H_ */
