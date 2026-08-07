#ifndef FOXHOLLOW_SHIM_GXFIFO_H
#define FOXHOLLOW_SHIM_GXFIFO_H

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>
#include_next <dolphin/gx/GXFifo.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*GXBreakPtCallback)(void);
GXBreakPtCallback GXSetBreakPtCallback(GXBreakPtCallback cb);
void GXEnableBreakPt(void* break_pt);
void GXDisableBreakPt(void);
u16 GXReadDrawSync(void);
void GXSetDrawSync(u16 token);
void GXResetWriteGatherPipe(void);
#ifdef __cplusplus
}
#endif

#endif
