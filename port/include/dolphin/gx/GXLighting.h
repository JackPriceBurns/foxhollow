#ifndef FOXHOLLOW_SHIM_GXLIGHTING_H
#define FOXHOLLOW_SHIM_GXLIGHTING_H

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>
#include_next <dolphin/gx/GXLighting.h>

#ifdef __cplusplus
extern "C" {
#endif
void GXGetLightAttnK(GXLightObj* obj, f32* k0, f32* k1, f32* k2);
#ifdef __cplusplus
}
#endif

#endif
