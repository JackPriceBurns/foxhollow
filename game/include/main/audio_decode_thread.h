#ifndef MAIN_AUDIO_DECODE_THREAD_H_
#define MAIN_AUDIO_DECODE_THREAD_H_

#include "types.h"
#include "dolphin/os.h"

BOOL CreateAudioDecodeThread(OSPriority priority, void *param);

#endif /* MAIN_AUDIO_DECODE_THREAD_H_ */
