#ifndef MAIN_ATTRACT_MOVIE_API_H_
#define MAIN_ATTRACT_MOVIE_API_H_

#include "types.h"
#include "dolphin/ai.h"

extern s32 gAttractMovieState;
extern s32 gAttractMovieIdleFrameCount;
extern u8 gAttractMoviePreparePending;
extern u8 gAttractMovieRetraceCountdown;
extern u8 gAttractMovieLoopCompleted;
extern void* gAttractMovieBuffer0;
extern void* gAttractMovieBuffer1;
extern void* gAttractMovieBuffer2;
extern void* gAttractMovieBuffer3;
extern void* gAttractMovieOptionalBuffer;
extern void* gAttractMovieWorkBuffer;
extern void* gAttractMovieScratchBuffer;
extern int gAttractMovieOffsetX;
extern int gAttractMovieOffsetY;

BOOL Movie_SetVolumeFade(int volume, int fadeFrames);

extern s32 gAttractMovieAudioMode;
extern u32 gAttractMovieAudioMixSourceAddr;
extern u32 gAttractMovieAudioPendingSourceAddr;
extern u32 gAttractMovieAudioDmaBufferIndex;
extern char gAttractMovieAudioDmaBuffer[];
extern AIDCallback gAttractMovieAudioPrevDmaCallback;

#endif /* MAIN_ATTRACT_MOVIE_API_H_ */
