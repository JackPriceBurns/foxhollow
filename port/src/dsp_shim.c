#include <dolphin/types.h>
#include <dolphin/dsp.h>
#include "shim_log.h"

static BOOL sDspInited;

void DSPInit(void) { sDspInited = 1; }
BOOL DSPCheckInit(void) { return sDspInited; }

DSPTaskInfo* DSPAddTask(DSPTaskInfo* task) {
  FH_STUB_ONCE("DSPAddTask (DSP tasks not executed; audio comes in Phase 5)");
  return task;
}

u32 DSPCheckMailToDSP(void) { return 0; }
u32 DSPCheckMailFromDSP(void) { return 0; }
u32 DSPReadMailFromDSP(void) { return 0; }
void DSPSendMailToDSP(u32 mail) { (void)mail; }
void DSPAssertInt(void) {}
void DSPHalt(void) {}
u32 DSPGetDMAStatus(void) { return 0; }
