#include <dolphin/types.h>
#include <dolphin/vi.h>
#include <aurora/aurora.h>
#include <aurora/event.h>
#include <SDL3/SDL_timer.h>
#include <stdlib.h>
#include "foxhollow_compat.h"
#include "shim_log.h"

typedef void (*VIRetraceCallback)(u32 retraceCount);

static VIRetraceCallback sPreRetraceCallback;
static VIRetraceCallback sPostRetraceCallback;
static void* sNextFrameBuffer;
static u32 sRetraceCount;
static int sFrameOpen;
static Uint64 sNextRetraceNs;

enum { VI_RETRACE_HZ = 60 };

static void wait_for_retrace_deadline(void) {
  const Uint64 periodNs = SDL_NS_PER_SECOND / VI_RETRACE_HZ;
  const Uint64 now = SDL_GetTicksNS();

  if (sNextRetraceNs == 0) {
    sNextRetraceNs = now + periodNs;
  } else if (now >= sNextRetraceNs + periodNs) {
    /* Do not run several retraces back-to-back after a long host stall. */
    sNextRetraceNs = now + periodNs;
  }

  const Uint64 deadline = sNextRetraceNs;
  sNextRetraceNs += periodNs;
  if (now < deadline) {
    SDL_DelayPrecise(deadline - now);
  }
}

static void pump_events(void) {
  const AuroraEvent* event = aurora_update();
  while (event != NULL && event->type != AURORA_NONE) {
    if (event->type == AURORA_EXIT) {
      exit(0);
    }
    ++event;
  }
}

void foxhollowFramePumpInit(void) {
  pump_events();
  sFrameOpen = aurora_begin_frame();
}

void VIWaitForRetrace(void) {
  if (sFrameOpen) {
    aurora_end_frame();
    sFrameOpen = 0;
  }
  pump_events();
  wait_for_retrace_deadline();
  while (!aurora_begin_frame()) {
    pump_events();
  }
  sFrameOpen = 1;
  fhAIPump();

  sRetraceCount++;
  if (sPreRetraceCallback) {
    sPreRetraceCallback(sRetraceCount);
  }
  if (sPostRetraceCallback) {
    sPostRetraceCallback(sRetraceCount);
  }
}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb) {
  VIRetraceCallback prev = sPreRetraceCallback;
  sPreRetraceCallback = cb;
  return prev;
}

VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback cb) {
  VIRetraceCallback prev = sPostRetraceCallback;
  sPostRetraceCallback = cb;
  return prev;
}

void VISetNextFrameBuffer(void* fb) { sNextFrameBuffer = fb; }
void* VIGetNextFrameBuffer(void) { return sNextFrameBuffer; }
void VISetBlack(BOOL black) { (void)black; }
u32 VIGetNextField(void) { return 0; }
u32 VIGetDTVStatus(void) { return 1; }
