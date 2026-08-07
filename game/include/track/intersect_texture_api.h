#ifndef TRACK_INTERSECT_TEXTURE_API_H_
#define TRACK_INTERSECT_TEXTURE_API_H_

#include "types.h"

int objFrozenRenderCb(void* object, void** model, int slot);
/*
 * Three-tex-coord-gen ind+direct TEV setup. Loads the active env-mtx
 * (gCameraLightPerspectiveFlipYMatrix) for tex0, scales tex1 by lbl_803DEF2C through a 3x4
 * matrix from PSMTXScale, and stamps an indirect tex matrix from local
 * stack data. Two TEV stages: stage 0 K-modulates the texture by alpha,
 * stage 1 modulates by the second texture. Uses ind tex stage 0 to warp
 * tex coord 0 by tex1.
 */
void setupQuakeSpellRingGxState(u8 alpha);

#endif /* TRACK_INTERSECT_TEXTURE_API_H_ */
