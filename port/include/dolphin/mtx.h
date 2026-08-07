#ifndef FOXHOLLOW_SHIM_MTX_H
#define FOXHOLLOW_SHIM_MTX_H

#include_next <dolphin/mtx.h>

typedef f32 (*MtxP)[4];
typedef const f32 (*CMtxP)[4];

#endif
