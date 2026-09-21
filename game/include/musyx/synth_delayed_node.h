#ifndef MUSYX_SYNTH_DELAYED_NODE_H_
#define MUSYX_SYNTH_DELAYED_NODE_H_

#include "types.h"

typedef struct SynthDelayedNode {
    struct SynthDelayedNode* next;
    struct SynthDelayedNode* prev;
    u8 voiceIndex;
    u8 jobTabIndex;
    u8 pad[2];
} SynthDelayedNode;

#endif
