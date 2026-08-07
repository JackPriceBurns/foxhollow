#ifndef FOXHOLLOW_SHIM_GXTEXTURE_H
#define FOXHOLLOW_SHIM_GXTEXTURE_H

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>
#include_next <dolphin/gx/GXTexture.h>

#ifdef __cplusplus
extern "C" {
#endif
void GXLoadTexObjPreLoaded(GXTexObj* obj, GXTexRegion* region, GXTexMapID id);
void GXPreLoadEntireTexture(const GXTexObj* obj, const GXTexRegion* region);
#ifdef __cplusplus
}
#endif

#endif
