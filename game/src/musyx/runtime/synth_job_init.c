#include "musyx/synth_job.h"
#include "musyx/synth_config.h"
#include "musyx/synth_job_init.h"

u32 lbl_803DE284;
u8 streamCallDelay;
u8 streamCallCnt;
SynthJob streamInfo[64];

void streamInit(void) {
    streamCallCnt = 0;
    streamCallDelay = 3;
    for (s32 i = 0; i < synthInfo.voiceCount; ++i) {
        streamInfo[i].state = SYNTH_JOB_STATE_FREE;
    }
    lbl_803DE284 = 0;
}
