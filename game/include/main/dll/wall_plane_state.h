#ifndef MAIN_DLL_WALL_PLANE_STATE_H_
#define MAIN_DLL_WALL_PLANE_STATE_H_

#include "global.h"

typedef struct WallPlaneState {
    f32 normal[3];
    f32 normalW;
    f32 axisLimit;
    f32 anchorY;
    f32 boundMin;
    f32 anchorX;
    f32 anchorZ;
} WallPlaneState;

STATIC_ASSERT(sizeof(WallPlaneState) == 0x24);

#endif
