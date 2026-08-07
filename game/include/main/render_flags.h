#ifndef MAIN_RENDER_FLAGS_H_
#define MAIN_RENDER_FLAGS_H_

#include "types.h"

extern u32 renderFlags;
/* Global renderFlags bits (decoded by the accessor fns below: shouldDrawShadows,
 * shouldDrawClouds, setDisableAntiAlias, getDrawDistanceFlag, isOvercast,
 * setPendingMapLoad, setStarsHidden). */
#define RENDERFLAG_WIDESCREEN      0x8
#define RENDERFLAG_DRAW_CLOUDS     0x10
#define RENDERFLAG_DISABLE_ANTI_ALIAS 0x20
#define RENDERFLAG_DRAW_SHADOWS    0x80
#define RENDERFLAG_PENDING_MAP_LOAD 0x1000
#define RENDERFLAG_DRAW_DISTANCE   0x10000
#define RENDERFLAG_20000           0x20000
#define RENDERFLAG_OVERCAST        0x40000
#define RENDERFLAG_HIDE_STARS      0x80000

#endif /* MAIN_RENDER_FLAGS_H_ */
