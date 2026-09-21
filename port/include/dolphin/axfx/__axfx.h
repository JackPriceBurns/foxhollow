#ifndef _DOLPHIN_AX_INTERNAL_H_
#define _DOLPHIN_AX_INTERNAL_H_

#include <dolphin/axfx.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __AXFXAlloc(size) OSAlloc(size)
#define __AXFXFree(ptr)   OSFree(ptr)

extern s32 sReverbStdDelayLengths[];

#ifdef __cplusplus
}
#endif

#endif
