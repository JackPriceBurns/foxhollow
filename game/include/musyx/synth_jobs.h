#ifndef MUSYX_SYNTH_JOBS_H_
#define MUSYX_SYNTH_JOBS_H_

#include "types.h"

void streamHandle(void);
void streamCorrectLoops(void);
void streamKill(u32 voice);
void streamOutputModeChanged(void);
int dataInsertKeymap(u16 keymapId, void *data);
int dataRemoveKeymap(u16 keymapId);

#endif /* MUSYX_SYNTH_JOBS_H_ */
