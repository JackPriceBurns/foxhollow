#ifndef MUSYX_SND_SERVICE_H_
#define MUSYX_SND_SERVICE_H_

#include "types.h"
#include "musyx/mcmd.h"

u16 sndRand(void);
s16 sndSin(u16 angle);
void* sndBSearch(void* key, void* base, int count, u32 stride, s32 (*cmp)(void*, void*));
void sndConvertMs(u32* value);
void sndConvertTicks(u32* value, McmdVoiceState* state);
u32 sndConvert2Ms(u32 value);

#endif /* MUSYX_SND_SERVICE_H_ */
