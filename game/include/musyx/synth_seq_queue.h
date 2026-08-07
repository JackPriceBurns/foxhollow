#ifndef MUSYX_SYNTH_SEQ_QUEUE_H_
#define MUSYX_SYNTH_SEQ_QUEUE_H_

#include "types.h"

void synthInitChannelEventQueues(void);
void synthRefreshChannelEventQueue(u8 groupIndex);

#endif /* MUSYX_SYNTH_SEQ_QUEUE_H_ */
