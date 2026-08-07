#ifndef MAIN_DLL_DLL_02C0_FRONT_API_H_
#define MAIN_DLL_DLL_02C0_FRONT_API_H_

#include "types.h"

void titleScreenPositionElements(f32 x, f32 y);
void titleScreenShowCopyright(u8 enabled);
void titleScreenDisableActors(void);
void titleScreenTextDrawFunc(int x0, int y0, int x1, int y1,
                             f32 u0, f32 v0, f32 u1, f32 v1);
void nameEntryTextDrawFunc(int x0, int y0, int x1, int y1,
                           f32 u0, f32 v0, f32 u1, f32 v1);
void creditsStart_(void);
int isFrontEndUiActive(void);
u8 shouldShowCredits(void);

void titleScreenSetMenuActive(s8 arg);
void nameEntrySetScroll(u32 a, u32 b);

#endif /* MAIN_DLL_DLL_02C0_FRONT_API_H_ */
