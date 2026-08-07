#include <dolphin/gx.h>
#include "shim_log.h"

static u16 sDrawSyncToken;
static GXDrawSyncCallback sDrawSyncCallback;
static GXBreakPtCallback sBreakPtCallback;

void GXSetDrawSync(u16 token) {
  sDrawSyncToken = token;
  if (sDrawSyncCallback) {
    sDrawSyncCallback(token);
  }
}

u16 GXReadDrawSync(void) { return sDrawSyncToken; }

GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb) {
  GXBreakPtCallback prev = sBreakPtCallback;
  sBreakPtCallback = cb;
  return prev;
}

void GXEnableBreakPt(void* break_pt) { (void)break_pt; }
void GXDisableBreakPt(void) {}
void GXLoadTexObjPreLoaded(GXTexObj* obj, GXTexRegion* region, GXTexMapID id) {
  (void)region;
  GXLoadTexObj(obj, id);
}
void GXPreLoadEntireTexture(const GXTexObj* obj, const GXTexRegion* region) {
  (void)obj; (void)region;
}
void GXInitFifoLimits(GXFifoObj* fifo, u32 hiWatermark, u32 loWatermark) {
  (void)fifo; (void)hiWatermark; (void)loWatermark;
}
void GXSetMisc(u32 token, u32 value) { (void)token; (void)value; }
void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1) { (void)perf0; (void)perf1; }
void GXReadXfRasMetric(u32* xf_wait_in, u32* xf_wait_out, u32* ras_busy, u32* clocks) {
  if (xf_wait_in) { *xf_wait_in = 0; }
  if (xf_wait_out) { *xf_wait_out = 0; }
  if (ras_busy) { *ras_busy = 0; }
  if (clocks) { *clocks = 0; }
}
void GXResetWriteGatherPipe(void) {}
void __GXAbortWaitPECopyDone(void) {}

GXRenderModeObj GXNtsc480Prog = {
    VI_TVMODE_NTSC_PROG, 640, 480, 480, 40, 0, 640, 480, VI_XFBMODE_SF, 0, 0,
    {{6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6},
     {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}},
    {0, 0, 21, 22, 21, 0, 0}};
