#ifndef MAIN_PROJECTED_SHADOW_H_
#define MAIN_PROJECTED_SHADOW_H_

#include "global.h"
#include "main/texture.h"

typedef struct ProjectedShadowTexture
{
    f32 textureMtx[3][4];
    f32 depthMtx[3][4];
    Texture* texture;
    u8 alpha;
    u8 mode;
    u8 pad66[2];
} ProjectedShadowTexture;

STATIC_ASSERT(sizeof(ProjectedShadowTexture) == 0x68);
STATIC_ASSERT(offsetof(ProjectedShadowTexture, texture) == 0x60);
STATIC_ASSERT(offsetof(ProjectedShadowTexture, alpha) == 0x64);
STATIC_ASSERT(offsetof(ProjectedShadowTexture, mode) == 0x65);

#endif /* MAIN_PROJECTED_SHADOW_H_ */
