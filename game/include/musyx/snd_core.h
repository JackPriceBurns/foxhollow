#ifndef MUSYX_SND_CORE_H_
#define MUSYX_SND_CORE_H_

#include "types.h"

extern u8 sndActive;
extern u8 synthIdleWaitActive;

void sndQuit(void);
void sndSetMaxVoices(u8 valueA, u8 valueB);
u32 sndIsInstalled(void);
void salApplyMatrix(f32 *matrix, f32 *vec, f32 *out);
f32 salNormalizeVector(f32 *v);
void inpSetGlobalMIDIDirtyFlag(u8 index, u8 group, u32 flags);

extern u64 synthRealTime;

#endif /* MUSYX_SND_CORE_H_ */
