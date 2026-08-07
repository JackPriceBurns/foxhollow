#ifndef FOXHOLLOW_SHIM_GXMISC_H
#define FOXHOLLOW_SHIM_GXMISC_H

#include <dolphin/gx.h>

#ifdef __cplusplus
extern "C" {
#endif
void GXEnableBreakPt(void* break_pt);
void GXDisableBreakPt(void);
u16 GXReadDrawSync(void);
void GXSetDrawSync(u16 token);
void GXResetWriteGatherPipe(void);
void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1);
#ifdef __cplusplus
}
#endif

#endif
