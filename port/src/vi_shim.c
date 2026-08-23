#include <dolphin/types.h>
#include <dolphin/vi.h>
#include <aurora/aurora.h>
#include <aurora/dvd.h>
#include <aurora/event.h>
#include <SDL3/SDL_timer.h>
#include <stdlib.h>
#include "foxhollow_compat.h"
#include "foxhollow_config.h"
#include "foxhollow_mods.h"
#include "foxhollow_quit.h"
#include "shim_log.h"

typedef void (*VIRetraceCallback)(u32 retraceCount);

#if defined(FOXHOLLOW_DEBUG_SHORTCUTS)
void fhDebugOverlayDraw(void);
#endif

static VIRetraceCallback sPreRetraceCallback;
static VIRetraceCallback sPostRetraceCallback;
static void* sNextFrameBuffer;
static u32 sRetraceCount;
static int sFrameOpen;
static Uint64 sNextRetraceNs;

void fhGXCompleteFrame(void);

enum { VI_RETRACE_DEFAULT_HZ = 60 };

static void wait_for_retrace_deadline(void) {
  const int limit = fhConfigFrameLimit();
  Uint64 periodNs;
  Uint64 now;

  if (limit == 0) {
    sNextRetraceNs = 0;
    return;
  }

  periodNs = SDL_NS_PER_SECOND / (Uint64)(limit > 0 ? limit : VI_RETRACE_DEFAULT_HZ);
  now = SDL_GetTicksNS();

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

static void shutdown_and_exit(void) {
  fhModsShutdown();
  aurora_dvd_close();
  aurora_shutdown();
  exit(0);
}

static void pump_events(void) {
  const AuroraEvent* event = aurora_update();
  int quitting = fhQuitRequested();

  while (event != NULL && event->type != AURORA_NONE) {
    if (event->type == AURORA_EXIT) {
      quitting = 1;
    }
    ++event;
  }
  if (quitting) {
    shutdown_and_exit();
  }
}

void foxhollowFramePumpInit(void) {
  pump_events();
  sFrameOpen = aurora_begin_frame();
}

void VIWaitForRetrace(void) {
  if (sFrameOpen) {
#if defined(FOXHOLLOW_DEBUG_SHORTCUTS)
    fhDebugOverlayDraw();
#endif
    fhGXCompleteFrame();
    aurora_end_frame();
    sFrameOpen = 0;
  }
  fhModsUpdate();
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
u32 VIGetRetraceCount(void) { return sRetraceCount; }
u32 VIGetNextField(void) { return 0; }
u32 VIGetDTVStatus(void) { return 1; }
