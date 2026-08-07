#ifndef MAIN_MAP_TEXSCROLL_H_
#define MAIN_MAP_TEXSCROLL_H_

#include "types.h"

#define MAP_TEXTURE_SCROLL_SLOT_UNALLOCATED 0xFF

void mapTextureScrollSetStep(int idx, int xStep, int yStep, int texWidthFixed, int texHeightFixed,
                             int secondaryXStep, int secondaryYStep, int texWidthFixed2, int texHeightFixed2);
int mapTextureScrollAcquire(int xStep, int yStep, int texWidthFixed, int texHeightFixed,
                            int secondaryXStep, int secondaryYStep, int texWidthFixed2, int texHeightFixed2);
void mapTextureScrollGetOffset(int idx, f32* outX, f32* outY);

#endif /* MAIN_MAP_TEXSCROLL_H_ */
