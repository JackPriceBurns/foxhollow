#ifndef FOXHOLLOW_SHIM_GXTEV_H
#define FOXHOLLOW_SHIM_GXTEV_H

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>
#include_next <dolphin/gx/GXTev.h>

#ifdef __cplusplus
extern "C" {
#endif
void GXSetTevIndRepeat(GXTevStageID tev_stage);
#ifdef __cplusplus
}
#endif

#endif
