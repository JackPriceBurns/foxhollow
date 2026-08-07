#ifndef FOXHOLLOW_SHIM_GXMANAGE_H
#define FOXHOLLOW_SHIM_GXMANAGE_H

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>
#include_next <dolphin/gx/GXManage.h>

#ifdef __cplusplus
extern "C" {
#endif
void __GXAbortWaitPECopyDone(void);
#ifdef __cplusplus
}
#endif

#endif
