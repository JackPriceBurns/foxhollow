#ifndef MAIN_MAKETEX_TIMER_H_
#define MAIN_MAKETEX_TIMER_H_

#include "types.h"

int timerIsActive(const f32* value);
int timerCountDown(f32* timer);
void storeZeroToFloatParam(f32* timer);
void s16toFloat(f32* timer, s16 duration);

#endif /* MAIN_MAKETEX_TIMER_H_ */
