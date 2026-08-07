#include <dolphin/types.h>
#include <dolphin/vi.h>
#include "shim_log.h"

typedef void (*VIRetraceCallback)(u32 retraceCount);

static VIRetraceCallback sPreRetraceCallback;
static VIRetraceCallback sPostRetraceCallback;
static void* sNextFrameBuffer;
static u32 sRetraceCount;

void VIWaitForRetrace(void) {
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
