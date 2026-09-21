#ifndef FOXHOLLOW_SHIM_GXGET_H
#define FOXHOLLOW_SHIM_GXGET_H

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>
#include_next <dolphin/gx/GXGet.h>

#ifdef __cplusplus
extern "C" {
#endif
void GXGetScissor(u32* left, u32* top, u32* width, u32* height);
#ifdef __cplusplus
}
#endif

#endif
