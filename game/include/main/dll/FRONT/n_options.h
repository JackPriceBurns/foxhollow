#ifndef MAIN_DLL_FRONT_N_OPTIONS_H_
#define MAIN_DLL_FRONT_N_OPTIONS_H_

#include "types.h"
#include "main/attract_movie_api.h"
#include "dolphin/os/OSMessage.h"

void THPPlayerDrawCurrentFrame(void *yTexture,void *uTexture,void *vTexture,u32 width,u32 height);
void AttractMovieAudio_DmaCallback(void);
void THPPlayerPostDrawDone(void);
BOOL THPPlayerGetVideoInfo(void *dst);
void AttractMovie_AddVideoTevStages(void);
BOOL AttractMovie_DrawTextureCallback(int unused, u32* modelPtr, u32 renderOpIdx);
int ProperTimingForGettingNextFrame(void);

extern char gPicMenuDvdReadBuffer[0x40];
extern OSMessageQueue gAttractMovieSpentTextureSetQueue;

#endif /* MAIN_DLL_FRONT_N_OPTIONS_H_ */
